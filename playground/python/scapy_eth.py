from scapy.all import *
g = Ether(src=get_if_hwaddr("eth0.103"), dst="aa:aa:aa:aa:aa:aa")\
    /IPv6(src="fe80::da3a:ddff:fe88:7bbb", dst="fe80::a8aa:aaff:feaa:5555")/TCP(sport=12345, dport=80)\
    /"GET / HTTP/1.0\r\n\r\n"
g.show()
sendp(g, iface="eth0.1")
packet_wake_on_lan = Ether(src=get_if_hwaddr("eth0.1"), dst="55:45:55:05:a5:25", type=0x842)/"I'm travelling on Ethernet"
sendp(packet_wake_on_lan, iface="eth0.1", loop=1, inter=0.2)
