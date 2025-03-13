import sys
import os
import _thread as thread
import threading
import socket
import atexit
import time
try:
    import msvcrt
    PLATFORM = "win"
except ImportError:
    PLATFORM = "unix"
    import tty
    import termios
    from select import select

def get_ch():
    if PLATFORM == "win":
        ch = msvcrt.getch()
        return ch
    elif PLATFORM == "unix":
        fd = sys.stdin.fileno()
        old_setting = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            i, o, e = select([sys.stdin.fileno()], [], [], 5)
            if i:
                ch = sys.stdin.read(1)
            else:
                ch = ""
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_setting)
        return ch
    else:
        return ""



def udp_list(sock):
    print("start listening")
    packet_number = 0    
    while 1:
        data, address = sock.recvfrom(1024)
        packet_number += 1
        print(f"time - {time.time()}")
        print(address,packet_number)
        print(data, len(data))
        data_s = []
        for i in range(0, len(data)):
            data_s.append(data[i])


def main():
    udp_port_port = 69
    buffer_size = 1512
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # Enable broadcasting mode
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    sock.bind(("192.168.203.12", udp_port_port))
    thread.start_new_thread(udp_list, (sock,))
    message = b"UDP_REQUEST"
    while 1:
        q = get_ch()
        print(q)
        if isinstance(q, str) and len(q):
            if ord(q) == 113: #q
                sock.close()
                sys.exit(1)
            elif ord(q) == 117: #u
                print("send message",message)
                sock.sendto(message, ('255.255.255.255', 70))


if __name__ == "__main__":
    main()
