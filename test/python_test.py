#!/usr/bin/env python
from subprocess import call
from multiprocessing import Process
from time import sleep
import os, signal
import requests
import unittest
import urllib

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

    def test_14_ModifyExpertiseSuccessfully(self):
        body = {"expertises":[{"company": "Lalala","position": "Lider tecnico","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "software"},{"company": "Lololo","position": "Especialista tecnico","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "hardware"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/perfil/expertise/position', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_15_ModifyExpertiseUnsuccsessfully(self):
        body = {"expertises":[{"company": "Lalala","position": "Lider tecnico","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "software"},{"company": "Lololo","position": "Especialista tecnico","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "hardware"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/perfil/expertise/position', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Don't upload.", reply.json()["message"])

    def test_16_GetExpertiseSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/perfil/expertise', params=params)
        self.assertEqual(200, reply.status_code)

    def test_17_GetExpertiseUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/perfil/expertise', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_18_ModifySkillsSuccessfully(self):
        body = {"every_skill":[{"skills": ["java","c","UML"],"category": "software"},{"skills": ["moto","auto"],"category": "licencia_manejo"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/perfil/skills/category', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_19_ModifySkillsUnsuccsessfully(self):
        body = {"every_skill":[{"skills": ["java","c","UML"],"category": "software"},{"skills": ["moto","auto"],"category": "licencia_manejo"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/perfil/skills/category', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Don't upload.", reply.json()["message"])

    def test_20_GetSkillsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/perfil/skills', params=params)
        self.assertEqual(200, reply.status_code)

    def test_21_GetSkillsUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/perfil/skills', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_22_ModifyPictureSuccessfully(self):
        body = {"picture": "asdf1234asdf"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/perfil/picture', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_23_ModifyPictureUnsuccsessfully(self):
        body = {"picture": "asdf1234asdf"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/perfil/picture', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Don't upload.", reply.json()["message"])

    def test_24_GetPictureSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/perfil/picture', params=params)
        self.assertEqual(200, reply.status_code)

    def test_25_GetPictureUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/perfil/picture', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_26_PostContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(201, reply.status_code)

    def test_27_PostContactUnsuccsessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"),("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Don't posted.", reply.json()["message"])

    def test_28_GetContactSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(200, reply.status_code)

    def test_29_GetContactUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_30_PostAcceptContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/accept', params=params)
        self.assertEqual(201, reply.status_code)

    def test_31_PostAcceptContactWithoutSolicitude(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/accept', params=params)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("User did not send solicitude.", reply.json()["message"])

    def test_32_PostAcceptContactUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/accept', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_33_DeleteRejectContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com")}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/reject', params=params)
        self.assertEqual(204, reply.status_code)

    def test_34_DeleteRejectContactWithoutSolicitude(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/reject', params=params)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("User did not send solicitude.", reply.json()["message"])

    def test_35_DeleteRejectContactUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/reject', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_36_GetFriendsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/contacts', params=params)
        self.assertEqual(200, reply.status_code)

    def test_37_GetFriendsUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/contacts', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_38_PutVoteContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(201, reply.status_code)

    def test_39_PutUnVoteContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("email", "contact@gmail.com")}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(204, reply.status_code)

    def test_40_PutVoteContactUnsuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_40_PutUnvoteContactUnsuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"),("email", "contact@gmail.com")}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_41_GetOwnRecommendationsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/own_recommendations', params=params)
        self.assertEqual(200, reply.status_code)

    def test_42_GetOwnRecommendationsUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/own_recommendations', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_43_GetOwnRecommendationsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/others_recommendations', params=params)
        self.assertEqual(200, reply.status_code)

    def test_44_GetOwnRecommendationsUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/others_recommendations', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_45_GetMostPopularUsersSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/pop', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual([], reply.json()["most_popular_users"])

    def test_46_GetMostPopularUsersAddingVotesSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/pop', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual(["{\"email\":\"contact@gmail.com\",\"votes\":1}"], reply.json()["most_popular_users"])

#    def test_47_GetMostPopularUsersUnsuccessfully(self):
#        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
#        reply = requests.get('http://localhost:8000/users/pop', params=params)
#        self.assertEqual(401, reply.status_code)
#        self.assertEqual("Invalid credentials.", reply.json()["message"])



    def test_48_LogoutUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/logout', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_49_LogoutSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/logout', params=params)
        self.assertEqual(200, reply.status_code)


suite = unittest.TestLoader().loadTestsFromTestCase(testApplicationServer)
unittest.TextTestRunner(verbosity=2).run(suite)
