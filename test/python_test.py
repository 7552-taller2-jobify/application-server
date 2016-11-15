#!/usr/bin/env python
from subprocess import call
from multiprocessing import Process
from time import sleep
import os, signal
import requests
import unittest

class testApplicationServer(unittest.TestCase):
    def test_01_RegisterEmptyFields(self):
        sleep(5)
        body = {"email": "test@yahoo.com", "password": "admin", "device_id": "123", "first_name": "", "last_name": "T", "gender": "M",	"birthday": "01/01/2000",
                "address": { "lat": "123456789", "lon": "12345678" }, "city": "lalala" }
        reply = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("There are empty fields.", reply.json()["message"])

    def test_02_RegisterSuccessfully(self):
        body = {"email": "test@yahoo.com", "password": "admin", "device_id": "123", "first_name": "test", "last_name": "T", "gender": "M",	"birthday": "01/01/2000",
                "address": { "lat": "123456789", "lon": "12345678" }, "city": "lalala" }
        reply = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(201, reply.status_code)
    
    def test_03_RegisterClienExists(self):
        body = {"email": "test@yahoo.com", "password": "admin", "device_id": "123", "first_name": "test", "last_name": "T", "gender": "M",	"birthday": "01/01/2000",
                "address": { "lat": "123456789", "lon": "12345678" }, "city": "lalala" }
        reply = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Client already exists.", reply.json()["message"])

    def test_04_LoginSucces(self):
        body = {"email": "test@yahoo.com", "password": "admin"}
        reply = requests.post('http://localhost:8000/users/login', json=body)
        self.assertEqual(200, reply.status_code)

    def test_05_LoginInvalidCredentials(self):
        body = {"email": "test@yahoo.com", "password": "a"}
        reply = requests.post('http://localhost:8000/users/login', json=body)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])
    
    def test_06_ModifyPersonalDataSuccessfully(self):
        body = {"first_name": "Donnal", "last_name": "Trump", "birthday": "23/05/1960", "gender": "M", "address": {"lat": "9999", "lon": "9999"}, "city": "New York"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/perfil/personal', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_07_ModifyPersonalDataUnsuccsessfully(self):
        body = {"first_name": "Donald", "last_name": "Trump", "birthday": "14/06/1946", "gender": "M", "address": {"lat": "9999", "lon": "9999"}, "city": "New York"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/perfil/personal', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Don't upload.", reply.json()["message"])

    def test_08_GetPersonalDataSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/perfil/personal', params=params)
        self.assertEqual(200, reply.status_code)

    def test_09_GetPersonalDataUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/perfil/personal', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_10_ModifySummarySuccessfully(self):
        body = {"summary": "Future president of the USA"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/perfil/summary', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_11_ModifySummaryUnsuccsessfully(self):
        body = {"summary": "Future president of the usa"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/perfil/summary', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Don't upload.", reply.json()["message"])

    def test_12_GetSummarySuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/perfil/summary', params=params)
        self.assertEqual(200, reply.status_code)

    def test_13_GetSummaryUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/perfil/summary', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_14_LogoutUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/logout', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_15_LogoutSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/logout', params=params)
        self.assertEqual(200, reply.status_code)


suite = unittest.TestLoader().loadTestsFromTestCase(testApplicationServer)
unittest.TextTestRunner(verbosity=2).run(suite)
