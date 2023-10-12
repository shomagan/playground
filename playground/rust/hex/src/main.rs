use std::env;
/*get values form argv and 
  print out hex values in decimal format
  conditions:
    if starts from 0x then handle as hex else as decimal
  printouts :
    1 print dec_value in hex format dec_value could be bigger then long int
    2 print dec_value byte by byte intepretation adaptive to value size
      */
fn main() {
    let args: Vec<String> = env::args().collect();
    println!("rust impl");
    for arg in args.iter().skip(1) {
        let mut radix = 10;
        let value_str: &str;
     
        if arg.starts_with("0x") {
            radix = 16;
            value_str = &arg[2..];
        }else{
            value_str = &arg;
        }
        let value = i128::from_str_radix(value_str, radix).unwrap();
        /*print decimal value*/
        println!("Decimal: {}", value);
        /*print hex value*/
        println!("Hex: 0x{:X}", value);
        let mut bytes: Vec<u8> = Vec::new();
        let mut remaining = value;
        while remaining > 0 {
            bytes.push((remaining & 0xFF) as u8);
            remaining >>= 8;
        }
        /*print bytes hex and decimal values in big-endian*/
        bytes.reverse();
        print!("[");
        for byte in bytes.iter() {
            print!(" 0x{:X} - {}; ", byte, byte);
        }
        println!("]")
    }
}

