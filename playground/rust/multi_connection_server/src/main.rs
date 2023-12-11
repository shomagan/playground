use tokio::net::UdpSocket;
use std::io;
use std::env;
use std::fs::File;
use std::io::prelude::*;
use serde_json::{Value};

#[tokio::main]
async fn main() -> io::Result<()> {
    let args: Vec<String> = env::args().collect();
    let file_path = &args[1];
    println!("In file {}", file_path);
    let mut file = File::open(file_path)?;
    let mut contents = String::new();
    file.read_to_string(&mut contents)?;
    println!("{:?}", contents);
    let config: Value = serde_json::from_str(&contents)?;
    println!("address - {:?},", config["address"]);
    println!("udp broadcast listening");
    let sock = UdpSocket::bind("0.0.0.0:65500").await?;
    let mut buf = [0; 1024];
    println!("start listening");
    loop {
        let (len, addr) = sock.recv_from(&mut buf).await?;
        println!("{:?} bytes received from {:?}", len, addr);

        let len = sock.send_to(&buf[..len], addr).await?;
        println!("{:?} bytes sent", len);
    }
}