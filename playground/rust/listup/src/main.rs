use std::fs::{self, File, OpenOptions};
use std::io::{self, BufRead, BufReader, Write};
use std::net::{SocketAddr, UdpSocket};
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::{Duration, SystemTime};
use serde_json::{self, Value};
use reqwest::blocking::Client;
use log::{info, debug, error, LevelFilter};
use simplelog::{Config, SimpleLogger};
use crossterm::event::{self, Event, KeyCode};
use regex::Regex;

const UDP_BROADCAST_PORT: u16 = 65500;
const UDP_BROADCAST_REQUEST_MESSAGE: &str = "UDP_REQUEST";
const LOG_DIR: &str = "data_log";
const DESCRIPTION_FILE_NAME: &str = "devices_description.json";

#[derive(Debug, Clone, PartialEq)]
struct Description {
   devices: Vec<Device>,
   running: bool,
}

#[derive(Debug, Clone, PartialEq)]
struct Device {
   device_name: String,
   mdb_address: u16, // Assuming mdb_address is a u16
   spaces: Vec<Space>,
}

#[derive(Debug, Clone, PartialEq)]
struct Space {
   function: String,
   var_name: String,
   value: Option<f64>,
   interval_sec: Option<f64>,
}

fn main() {
   SimpleLogger::init(LevelFilter::Debug, Config::default()).unwrap();

   let description = Arc::new(Mutex::new(load_description(DESCRIPTION_FILE_NAME)));
   let description_clone = Arc::clone(&description);

   if !std::path::Path::new(LOG_DIR).exists() {
      fs::create_dir(LOG_DIR).unwrap();
   }

   let udp_thread = thread::spawn(move || udp_list(description_clone));

   let mut last_time = SystemTime::now();
   loop {
      // Check for user input
      if let Some(command) = get_user_input() {
         info!("User input received: {}", command);
         if command == "q" {
               info!("Exiting...");
               let mut desc = description.lock().unwrap();
               desc.running = false;
               break;
         }
      }

      // Periodically reload the description file
      if last_time.elapsed().unwrap().as_secs() >= 5 {
         info!("Reloading description file...");
         let mut desc = description.lock().unwrap();
         let new_desc = load_description(DESCRIPTION_FILE_NAME);
         if *desc != new_desc {
               info!("Description file updated.");
               *desc = new_desc;
         }
         last_time = SystemTime::now();
      }
      thread::sleep(Duration::from_millis(100)); // Prevent busy-waiting
   }

   udp_thread.join().unwrap();
}

fn get_user_input() -> Option<String> {
   if event::poll(Duration::from_millis(100)).unwrap() {
      if let Event::Key(key_event) = event::read().unwrap() {
         if let KeyCode::Char(c) = key_event.code {
               return Some(c.to_string());
         }
      }
   }
   None
}

fn extract_json(data: &[u8]) -> Option<String> {
   // Convert the raw bytes to a string, replacing invalid UTF-8 characters
   let text = String::from_utf8_lossy(data);

   // Use a regular expression to extract the content inside `{}`
   let re = Regex::new(r"\{.*?\}").unwrap();
   if let Some(captures) = re.find(&text) {
      return Some(captures.as_str().to_string());
   }

   None
}

fn udp_list(description: Arc<Mutex<Description>>) {
   let socket = UdpSocket::bind(("0.0.0.0", UDP_BROADCAST_PORT)).expect("Failed to bind UDP socket");
   info!("UDP listener started on port {}", UDP_BROADCAST_PORT);
   socket.set_read_timeout(Some(Duration::from_secs(3))).expect("Failed to set read timeout");
   loop {
      let mut buffer = [0; 1024];
      match socket.recv_from(&mut buffer) {
         Ok((size, addr)) => {
               let data = &buffer[..size];
               if let Some(json_content_from_udp) = extract_json(data) {
                  debug!("Extracted JSON: {} from {}", json_content_from_udp, addr);
                  if let Ok(data_json) = serde_json::from_str::<Value>(&json_content_from_udp) {
                     let desc = description.lock().unwrap();
                     for device in &desc.devices {
                        if device.device_name == data_json["device_name"] {
                           let mdb_addr = device.mdb_address as u16;
                           if check_mdb_addr(&addr, mdb_addr){
                              update_vars(&addr, device.clone());
                           }
                        }
                     }
                  }
               } else {
                  debug!("No valid JSON found in data from {}: {:?}", addr, data);
               }
         }
         Err(ref e) if e.kind() == io::ErrorKind::WouldBlock => {
               socket
                  .send_to(UDP_BROADCAST_REQUEST_MESSAGE.as_bytes(), ("255.255.255.255", UDP_BROADCAST_PORT))
                  .unwrap();
         }
         Err(e) => {
               error!("UDP error: {}", e);
         }
      }

      let desc = description.lock().unwrap();
      if !desc.running {
         break;
      }
   }
}

fn load_description(file_name: &str) -> Description {
   let file = File::open(file_name).expect("Failed to open description file");
   let reader = BufReader::new(file);
   let json: Value = serde_json::from_reader(reader).expect("Failed to parse JSON");

   let devices = json["devices"]
      .as_array()
      .unwrap()
      .iter()
      .map(|device| Device {
         device_name: device["device_name"].as_str().unwrap().to_string(),
         mdb_address: device["mdb_address"].as_u64().unwrap() as u16,
         spaces: device["spaces"]
               .as_array()
               .unwrap()
               .iter()
               .map(|space| Space {
                  function: space["function"].as_str().unwrap().to_string(),
                  var_name: space["var_name"].as_str().unwrap().to_string(),
                  value: space["value"].as_f64(),
                  interval_sec: space["interval_sec"].as_f64(),
               })
               .collect(),
      })
      .collect();

   Description {
      devices,
      running: true,
   }
}

fn update_vars(addr: &SocketAddr, device: Device) {
   let client = Client::builder()
   .timeout(Duration::from_secs(5)) // Increase timeout to 10 seconds
   .build()
   .unwrap();
   for space in device.spaces {
      if space.function == "write" {
         let read_request = format!(
               r#"{{"request":"regs","reg_num":1,"get_type":"only_value","name":"{}"}}"#,
               space.var_name
         );
         if let Some(result) = send_json(&client, addr, &read_request) {
               if let Some(value) = result.get("value").and_then(|v| v.as_f64()) {
                  if Some(value) != space.value {
                     let write_request = format!(
                           r#"{{"request":"regs_set","reg_num":1,"name":"{}","value":{}}}"#,
                           space.var_name,
                           space.value.unwrap_or(0.0)
                     );
                     send_json(&client, addr, &write_request);
                  }
               }
         }
      } else if space.function == "read" {
         let file_name = format!("{}/{}_{}.csv", LOG_DIR, device.device_name, space.var_name);
         let update = should_update(&file_name, space.interval_sec.unwrap_or(0.0));

         if update {
               let read_request = format!(
                  r#"{{"request":"regs","reg_num":1,"get_type":"only_value","name":"{}"}}"#,
                  space.var_name
               );
               if let Some(result) = send_json(&client, addr, &read_request) {
                  if let Some(value) = result.get("value").and_then(|v| v.as_f64()) {
                     let mut file = OpenOptions::new().append(true).create(true).open(&file_name).unwrap();
                     let time_str = chrono::Local::now().format("%Y-%m-%d %H:%M:%S").to_string();
                     writeln!(file, "{}; {}; {}", chrono::Local::now().timestamp(), value, time_str).unwrap();
                  }
               }
         }
      }
   }
}
/*function check mdb_addr via http request and return true if match */
fn check_mdb_addr(addr: &SocketAddr, mdb_addr: u16) -> bool {
   let client = Client::builder()
      .timeout(Duration::from_secs(5)) // Increase timeout to 5 seconds
      .build()
      .unwrap();
   let read_request = format!(
      r#"{{"request":"regs","reg_num":1,"get_type":"only_value","name":"{}"}}"#,
      "mdb_addr"
   );   
   if let Some(result) = send_json(&client, addr, &read_request) {
      if let Some(value) = result.get("value").and_then(|v| v.as_u64()) {
         debug!("MDB address from device: {}", value);
         debug!("MDB address to check: {}", mdb_addr);
         if Some(value) != Some(mdb_addr.into()) {
            return false;
         }else {
            return true;
         }
      }
   }
   false
}

fn should_update(file_name: &str, interval_sec: f64) -> bool {
   if let Ok(file) = File::open(file_name) {
      let reader = BufReader::new(file);
      if let Some(last_line) = reader.lines().last() {
         if let Ok(last_line) = last_line {
               let last_time: f64 = last_line.split(';').next().unwrap().parse().unwrap();
               return (chrono::Local::now().timestamp() as f64 - last_time) > interval_sec;
         }
      }
   }
   true
}

fn send_json(client: &Client, addr: &SocketAddr, json_data: &str) -> Option<Value> {
   let url = format!("http://{}:80/get_json.cgi", addr.ip());
   debug!("Sending JSON to {}: {}", url, json_data);
   match client.get(&url).header("Json", json_data).send() {
      Ok(response) => {
         if let Ok(json) = response.json::<Value>() {
               debug!("Response JSON: {:?}", json);
               return Some(json);
         }
      }
      Err(e) => error!("HTTP error: {}", e),
   }
   //delay 10 ms
   thread::sleep(Duration::from_millis(10));
   None
}