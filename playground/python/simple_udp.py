import socket
udp_port_port = 34999
buffer_size = 1512
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
sock.bind(("", udp_port_port))
message = b"UDP_REQUEST"
sock.sendto(message, ('192.168.133.3', 34500))
sock.close()
