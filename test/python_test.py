#!/usr/bin/env python

from subprocess import call
from multiprocessing import Process
from time import sleep
import os, signal

def start_server():
    #os.chdir("..")
    call(["./Server"])

def skills():
    sleep(1)
    call(["curl", "-XPOST", "http://127.0.0.1:8000/users/test@gmail.com/perfil/skills", "-H", "'Content-Type: application/json'", "-d", "{\"every_skill\":[{\"skills\":[\"java\",\"c\",\"UML\"],\"category\":\"software\"}]}", "-v"])

def login():
    sleep(1)
    call(["curl", "-XPOST", "http://127.0.0.1:8000/users/login", "-H", "'Content-Type: application/json'", "-d", "{\"email\":\"test@gmail.com\",\"password\":\"test\"}", "-v"])

def register():
    sleep(1)
    call(["curl", "-XPOST", "http://127.0.0.1:8000/users/register", "-H", "'Content-Type: application/json'", "-d", "{\"email\":\"test@gmail.com\",\"password\":\"test\",\"device_id\":1234567890,\"first_name\":\"test\",\"last_name\":\"T\",\"gender\":\"M\",\"birthday\":\"01/01/2000\",\"address\":{\"lat\":\"123456789\",\"lon\":\"12345678\"},\"city\":\"lalala\"}", "-v"])
    login()
    skills()

def kill_process(name):
    for line in os.popen("ps ax | grep " + name + " | grep -v grep"):
        fields = line.split()
        pid = fields[0]
    os.kill(int(pid), signal.SIGKILL)

if __name__ == '__main__':
    t1 = Process(target = start_server, args = ())
    t2 = Process(target = register, args = ())
    t1.start()
    t2.start()
    t2.join()
    kill_process("Server")
    print "\nTest has finished"
