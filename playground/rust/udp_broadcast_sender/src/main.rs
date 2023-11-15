use tokio::net::UdpSocket;
use std::{io};
use std::net::{SocketAddr,SocketAddrV4};

const IP_ALL: [u8; 4] = [255, 255, 255, 255];

#[tokio::main]
async fn main() -> io::Result<()> {
    let sock = UdpSocket::bind("0.0.0.0:65501").await?;
    let remote_address = SocketAddrV4::new(IP_ALL.into(), 65500);
    sock.set_broadcast(true).expect("Failed to set broadcast mode");
    println!("udp broadcast");
    let buf: [u8; 5] = [1, 2, 3, 4, 5];
    let len = sock.send_to(&buf[..5],remote_address).await?;
    println!("{:?} bytes sent", len);
    Ok(())
}