#
#   Hello World server in Python
#   Binds REP socket to tcp://*:5555
#   Expects b"Hello" from client, replies with b"World"
#

import time
import zmq

context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://127.0.0.1:8000")

while True:
    #  Wait for next request from client
    socket.send(b"World")

    #  Do some 'work'
    time.sleep(1)

    message = socket.recv()
    print("Received request: %s" % message)
    print("Received request: %s" % message.decode())
    #  Send reply back to client