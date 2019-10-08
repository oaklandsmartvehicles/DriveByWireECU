import socket
import time

UDP_IP = "192.168.2.100"
UDP_PORT = 12090
example_integer = 25
BYTES = example_integer.to_bytes(4, byteorder='little')

sock = socket.socket(socket.AF_INET, # Internet
             socket.SOCK_DGRAM) # UDP
while True:
    print(sock.sendto(BYTES, (UDP_IP, UDP_PORT)))
    print("sent")
    time.sleep(1)
