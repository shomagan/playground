use tokio::net::UdpSocket;
use std::io;

#[tokio::main]
async fn main() -> io::Result<()> {
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