import socket
import protocol_pb2

import time

def make_message(x):
    print("You can choose time delay for request from 10 to 1000 ms")
    print("Choose the time delay for request in [ms]: ")
    while 1:
        msec = float(input())
        if msec > 1000 or msec < 10:
            print("Choose correct number!")
        else:
            break
    time.sleep(msec / 1000)
    message = protocol_pb2.WrapperMessage()
    if x == 1:
        print("Choose the time delay in [s] for slow_response: ")
        sec = int(input())
        message.request_for_slow_response.time_in_seconds_to_sleep = sec
        message.request_for_slow_response.SetInParent()
    if x == 2:
        message.request_for_fast_response.SetInParent()
    return message

HOST = "127.0.0.1"
PORT = 8000

while 1:
    print("0) Disconnect from server.")
    print("1) Send slow_response.")
    print("2) Send fast_response.")
    while 1:
        x = int(input())
        if x > 2 or x < 0:
            print("Choose correct number!")
        else:
            break
    match x:
        case 0:
            print("Disconnected")
            break
        case _:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((HOST, PORT))
                message = make_message(x)
                s.sendall(b"0002")
                s.sendall(message.SerializeToString())
                data = s.recv(1024)
                print(f"Received {data!r}")
