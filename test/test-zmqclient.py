#
#   Hello World server in Python
#   Binds REP socket to tcp://*:5555
#   Expects b"Hello" from client, replies with b"World"
#

import time
import zmq
import json

context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://127.0.0.1:6006")

while True:
    #  Wait for next request from client
    d_out ={  
        "r_out": 1.1,
        "i_out": 7,
        "b_out": True,
        "s_out": "send_sucess"
    }
    print(d_out)
    
    json_object = json.dumps(d_out)
    print(json_object)
    socket.send_string(json_object)

    #  Do some 'work'
    time.sleep(0.01)

    message = socket.recv()
    print("Received request: %s" % message)
    print("Received request: %s" % message.decode())

    json_dumped = message.decode()
    d = json.loads(json_dumped)
    if "terminate" in d:
        break
    #  Send reply back to client