import time
import socket
import random

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 8091))


max_count=10000

min_user=1000
num_users=1000
max_user=min_user+num_users

min_retailer=1000
num_retailers=1000
max_retailer=min_retailer+num_retailers

def test():
    count=0
    start=time.time_ns()

    while count < max_count:
        random_user=random.randint(min_user, max_user)
        random_retailer=random.randint(min_retailer, max_retailer)
        msg=f"set {random_user} {random_retailer}"
        s.send(bytes(msg, "utf8"))
        reply = s.recv(1024).decode()
        # print(count, reply)
        count+=1

    end=time.time_ns()
    diff = end - start
    time_each = diff / max_count
    time_each_μs = round(time_each / 1000, 2)
    qps = round(1e9 / time_each, 2)
    print(f"Compleated {max_count} queries in {diff}ns {time_each_μs}μs/q {qps}qps")

for i in range(10):
    test()