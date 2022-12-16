import socket
import protocol_pb2

message = protocol_pb2.WrapperMessage()
message.request_for_fast_response.SetInParent()
# message.request_for_slow_response.time_in_seconds_to_sleep = 5

HOST = "127.0.0.1"
PORT = 8000

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b"0002")
    s.sendall(message.SerializeToString())
    data = s.recv(1024)

print(f"Received {data!r}")
