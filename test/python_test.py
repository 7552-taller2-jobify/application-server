#!/usr/bin/env python
from subprocess import call
from multiprocessing import Process
from time import sleep
import os, signal
import requests
import unittest
import urllib
import json

class testApplicationServer(unittest.TestCase):
    def test_01_RegisterEmptyFields(self):
        sleep(1)
        body = {"email": "test@yahoo.com", "password": "admin", "device_id": "123", "first_name": "", "last_name": "T", "gender": "M",	"birthday": "01/01/2000",
                "address": { "lat": "123456789", "lon": "12345678" } }
        reply = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("There are empty fields.", reply.json()["message"])

    def test_02_RegisterSuccessfully(self):
        body = {"email": "test@yahoo.com", "password": "admin", "device_id": "123", "first_name": "test", "last_name": "T", "gender": "M",	"birthday": "01/01/2000",
                "address": { "lat": "123456789", "lon": "12345678" }}
        reply = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(201, reply.status_code)
        self.assertEqual("OK", json.loads(reply.content)["registration"])
    
    def test_03_RegisterClienExists(self):
        body = {"email": "test@yahoo.com", "password": "admin", "device_id": "123", "first_name": "test", "last_name": "T", "gender": "M",	"birthday": "01/01/2000",
                "address": { "lat": "123456789", "lon": "12345678" }}
        reply = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Client already exists.", reply.json()["message"])

    def test_04_LoginSucces(self):
        body = {"email": "test@yahoo.com", "password": "admin"}
        reply = requests.post('http://localhost:8000/users/login', json=body)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("", reply.json()["profile"]["picture"])
        self.assertEqual("test", reply.json()["profile"]["first_name"])
        self.assertEqual("T", reply.json()["profile"]["last_name"])
        self.assertEqual("test@yahoo.com", reply.json()["profile"]["email"])
        self.assertEqual("eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4", reply.json()["metadata"]["token"])

    def test_05_LoginInvalidCredentials(self):
        body = {"email": "test@yahoo.com", "password": "a"}
        reply = requests.post('http://localhost:8000/users/login', json=body)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])
    
    def test_06_ModifyPersonalDataSuccessfully(self):
        body = {"first_name": "Donnal", "last_name": "Trump", "birthday": "23/05/1960", "gender": "M", "address": {"lat": "-11.9302", "lon": "-77.0846"}}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/personal', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_07_ModifyPersonalDataUnsuccsessfully(self):
        body = {"first_name": "Donald", "last_name": "Trump", "birthday": "14/06/1946", "gender": "M", "address": {"lat": "9999", "lon": "9999"}}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/personal', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not upload.", reply.json()["message"])

    def test_08_GetPersonalDataSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/personal', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("Donnal", reply.json()["first_name"])
        self.assertEqual("Trump", reply.json()["last_name"])
        self.assertEqual("M", reply.json()["gender"])
        self.assertEqual("23/05/1960", reply.json()["birthday"])
        self.assertEqual("test@yahoo.com", reply.json()["email"])
        self.assertEqual("-11.9302", reply.json()["address"]["lat"])
        self.assertEqual("-77.0846", reply.json()["address"]["lon"])

    def test_09_GetPersonalDataUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/personal', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_10_GetEmptySummarySuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/summary', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("", reply.json()["summary"])

    def test_11_ModifySummarySuccessfully(self):
        body = {"summary": "Future president of the USA"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/summary', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_12_ModifySummaryUnsuccsessfully(self):
        body = {"summary": "Future president of the usa"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/summary', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not upload.", reply.json()["message"])

    def test_13_GetSummarySuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/summary', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("Future president of the USA", reply.json()["summary"])

    def test_14_GetSummaryUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/summary', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_15_GetEmptyExpertiseSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual(0, len(reply.json()["expertises"]))

    def test_16_ModifyExpertiseSuccessfully(self):
        body = {"expertises":[{"company": "Lalala","position": "Lider tecnico","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "software"},{"company": "Lololo","position": "Especialista tecnico","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "hardware"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_16_OtherModifyExpertiseSuccessfully(self):
        body = {"expertises":[{"company": "LalalaDOS","position": "Lider tecnico","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "software"},{"company": "LololoDOS","position": "Especialista tecnico","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "hardware"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_16_CreateExpertisesSuccessfully(self):
        body = {"expertises":[{"company": "LalalaDOS","position": "Lider tecnico","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "software"},{"company": "LololoDOS","position": "Especialista tecnico","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "hardware"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_16_CreateExpertisesUnsuccessfully(self):
        body = {"expertises":[{"company": "LalalaDOS","position": "Lider tecnico","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "software"},{"company": "LololoDOS","position": "Especialista tecnico","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "hardware"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXz"}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not upload.", reply.json()["message"])

    def test_17_ModifyExpertiseUnsuccsessfully(self):
        body = {"expertises":[{"company": "Lalala","position": "Lider tecnico","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "software"},{"company": "Lololo","position": "Especialista tecnico","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "hardware"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not upload.", reply.json()["message"])

    def test_18_GetExpertiseSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("software", reply.json()["expertises"][0]["category"])
        self.assertEqual("01/01/2010", reply.json()["expertises"][0]["from"])
        self.assertEqual("LalalaDOS", reply.json()["expertises"][0]["company"])
        self.assertEqual("26/09/2016", reply.json()["expertises"][0]["to"])
        self.assertEqual("aaer", reply.json()["expertises"][0]["expertise"])
        self.assertEqual("Lider tecnico", reply.json()["expertises"][0]["position"])
        self.assertEqual("hardware", reply.json()["expertises"][1]["category"])
        self.assertEqual("02/02/2012", reply.json()["expertises"][1]["from"])
        self.assertEqual("LololoDOS", reply.json()["expertises"][1]["company"])
        self.assertEqual("02/02/2016", reply.json()["expertises"][1]["to"])
        self.assertEqual("aaer", reply.json()["expertises"][1]["expertise"])
        self.assertEqual("Especialista tecnico", reply.json()["expertises"][1]["position"])

    def test_19_GetExpertiseUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_20_GetEmptySkillsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/skills', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual(0, len(reply.json()["every_skill"]))

    def test_21_ModifySkillsSuccessfully(self):
        body = {"every_skill":[{"skills": ["java","c","UML"],"category": "software"},{"skills": ["moto","auto"],"category": "licencia_manejo"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/skills/category', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_22_ModifySkillsUnsuccsessfully(self):
        body = {"every_skill":[{"skills": ["java","c","UML"],"category": "software"},{"skills": ["moto","auto"],"category": "licencia_manejo"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/skills/category', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not upload.", reply.json()["message"])

    def test_23_GetSkillsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/skills', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("java", reply.json()["every_skill"][0]["skills"][0])
        self.assertEqual("c", reply.json()["every_skill"][0]["skills"][1])
        self.assertEqual("UML", reply.json()["every_skill"][0]["skills"][2])
        self.assertEqual("software", reply.json()["every_skill"][0]["category"])
        self.assertEqual("moto", reply.json()["every_skill"][1]["skills"][0])
        self.assertEqual("auto", reply.json()["every_skill"][1]["skills"][1])
        self.assertEqual("licencia_manejo", reply.json()["every_skill"][1]["category"])

    def test_24_GetSkillsUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/skills', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_25_GetEmptyPictureSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/picture', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("", reply.json()["picture"])


    def test_26_ModifyPictureSuccessfully(self):
        body = {"picture": "asdf1234asdf"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/picture', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_27_ModifyPictureUnsuccsessfully(self):
        body = {"picture": "asdf1234asdf"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/picture', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not upload.", reply.json()["message"])

    def test_28_GetPictureSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/picture', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("asdf1234asdf", reply.json()["picture"])

    def test_29_GetPictureUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/picture', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_30_GetContactVacioSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual(0, len(reply.json()["solicitudes"]))

    def test_31_PostContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(201, reply.status_code)

    def test_32_PostContactUnsuccsessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"),("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not post.", reply.json()["message"])

    def test_33_GetContactSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual(1, len(reply.json()["solicitudes"]))
        self.assertEqual("03-01-2010T12:34:00.000Z", reply.json()["solicitudes"][0]["date"])
        self.assertEqual("contact@gmail.com", reply.json()["solicitudes"][0]["email"])

    def test_34_GetContactUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_35_PostAcceptContactSuccessfully(self):
        params = (("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com"),("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"))
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/accept', params=params)
        self.assertEqual(201, reply.status_code)

    def test_36_PostAcceptContactWithoutSolicitude(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/accept', params=params)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("User did not send solicitude.", reply.json()["message"])

    def test_37_PostAcceptContactUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/accept', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_38_DeleteRejectContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com")}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/reject', params=params)
        self.assertEqual(204, reply.status_code)

    def test_39_DeleteRejectContactWithoutSolicitude(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/reject', params=params)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("User did not send solicitude.", reply.json()["message"])

    def test_40_DeleteRejectContactUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/reject', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_41_GetFriendsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/contacts', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("contact@gmail.com", reply.json()["friends"][0]["email"])

    def test_42_GetFriendsUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/contacts', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_43_PutVoteContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(201, reply.status_code)

    def test_44_PutUnVoteContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("email", "contact@gmail.com")}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(204, reply.status_code)

    def test_45_PutVoteContactUnsuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_46_PutUnvoteContactUnsuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"),("email", "contact@gmail.com")}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_47_GetOwnRecommendationsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/own_recommendations', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual([], reply.json()["own_recommendations"])

    def test_48_GetOwnRecommendationsUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/own_recommendations', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_49_GetOwnRecommendationsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/others_recommendations', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual([], reply.json()["others_recommendations"])

    def test_50_GetOwnRecommendationsUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/others_recommendations', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_51_GetMostPopularUsersSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/pop', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual(0, reply.json()["most_popular_users"][0]["votes"])
        self.assertEqual("contact@gmail.com", reply.json()["most_popular_users"][0]["email"])
        self.assertEqual(0, reply.json()["most_popular_users"][1]["votes"])
        self.assertEqual("test@yahoo.com", reply.json()["most_popular_users"][1]["email"])

    def test_52_GetMostPopularUsersAddingVotesSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/pop', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("contact@gmail.com", reply.json()["most_popular_users"][0]["email"])
        self.assertEqual(0, reply.json()["most_popular_users"][0]["votes"])
        self.assertEqual("test@yahoo.com", reply.json()["most_popular_users"][1]["email"])
        self.assertEqual(0, reply.json()["most_popular_users"][1]["votes"])
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/pop', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("contact@gmail.com", reply.json()["most_popular_users"][0]["email"])
        self.assertEqual(1, reply.json()["most_popular_users"][0]["votes"])
        self.assertEqual("test@yahoo.com", reply.json()["most_popular_users"][1]["email"])
        self.assertEqual(0, reply.json()["most_popular_users"][1]["votes"])

    def test_53_SearchByPosition(self):
       
        body = {"email": "fulano@yahoo.com", "password": "admin", "device_id": "123", "first_name": "test", "last_name": "T", "gender": "M",	"birthday": "01/01/2000",
                "address": { "lat": "-180.9302235", "lon": "-190.0846415" }, "city": "lalala" }
        reply_fulano_register = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(201, reply_fulano_register.status_code)
        self.assertEqual("OK", json.loads(reply_fulano_register.content)["registration"])

        body = {"email": "fulano@yahoo.com", "password": "admin"}
        reply_fulano_login = requests.post('http://localhost:8000/users/login', json=body)
        self.assertEqual(200, reply_fulano_login.status_code)

        body = {"expertises":[{"company": "Lalala","position": "Desarrollador C++","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "Software"},{"company": "Lololo","position": "Desarrollador Java","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "Software"}]}
        params = {"token": reply_fulano_login.json()["metadata"]["token"]}
        reply_fulano = requests.put('http://localhost:8000/users/fulano@yahoo.com/profile/expertise/position', params=params, json=body)
        self.assertEqual(200, reply_fulano.status_code)

        # Distance is in Km
        params = (('distance', ''),('lat', ''),('lon', ''),("position", "Lider tecnico"),("skills", ""),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])

    def test_54_SearchBySkills(self):
        body = {"email": "pepito@yahoo.com", "password": "admin", "device_id": "123", "first_name": "test", "last_name": "T", "gender": "M",	"birthday": "01/01/2000",
                "address": { "lat": "180.0", "lon": "180.0" }}
        reply_pepito_register = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(201, reply_pepito_register.status_code)
        self.assertEqual("OK", json.loads(reply_pepito_register.content)["registration"])

        body = {"email": "pepito@yahoo.com", "password": "admin"}
        reply_pepito_login = requests.post('http://localhost:8000/users/login', json=body)
        self.assertEqual(200, reply_pepito_login.status_code)

        body = {"every_skill":[{"skills": ["c"],"category": "software"},{"skills": ["moto","auto"],"category": "licencia_manejo"}]}
        params = {"token": reply_pepito_login.json()["metadata"]["token"]}
        reply_pepito = requests.put('http://localhost:8000/users/pepito@yahoo.com/profile/skills/category', params=params, json=body)
        self.assertEqual(200, reply_pepito.status_code)

        # Distance is in Km
        params = (('distance', ''),('lat', ''),('lon', ''),("position", ""),("skills", "UML,c"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])

    def test_55_SearchByDistance(self):
        body = {"email": "mengano@yahoo.com", "password": "admin", "device_id": "123", "first_name": "test", "last_name": "T", "gender": "M",	"birthday": "01/01/2000",
                "address": { "lat": "-11.9302235", "lon": "-77.0846415" }}
        reply_pepito_register = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(201, reply_pepito_register.status_code)
        self.assertEqual("OK", json.loads(reply_pepito_register.content)["registration"])
        # Distance is in Km
        params = (('distance', '4000.0'),('lat', '-34.3971898'),('lon', '-58.7568274'),("position", ""),("skills", ""),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("test@yahoo.com", reply.json()["results"][1]["email"])
        self.assertEqual("mengano@yahoo.com", reply.json()["results"][0]["email"])

    def test_56_SearchByDistanceByDefault(self):
        # Distance is in Km
        params = (('distance', ''),('lat', '-11.9302'),('lon', '-77.0856'),("position", ""),("skills", ""),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("test@yahoo.com", reply.json()["results"][1]["email"])
        self.assertEqual("mengano@yahoo.com", reply.json()["results"][0]["email"])

    def test_57_SearchByPositionAndSkills(self):
        params = (('distance', ''),('lat', ''),('lon', ''),("position", "Lider tecnico"),("skills", "UML,c"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])

    def test_58_SearchByPositionAndDistance(self):
        params = (('distance', '4000.0'),('lat', '-34.3971898'),('lon', '-58.7568274'),("position", "Lider tecnico"),("skills", ""),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])

    def test_59_SearchBySkillsAndDistance(self):
        params = (('distance', '4000.0'),('lat', '-34.3971898'),('lon', '-58.7568274'),("position", ""),("skills", "UML,c"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])

    def test_60_SearchByPositionAndSkillsAndDistance(self):
        params = (('distance', '4000.0'),('lat', '-34.3971898'),('lon', '-58.7568274'),("position", "Lider tecnico"),("skills", "UML,c"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])
        print(reply.json()["results"][0])

    def test_61_SearchByPositionAndSkillsAndDistanceUnsuccesfully(self):
        params = (('distance', '4000.0'),('lat', '-34.3971898'),('lon', '-58.7568274'),("position", "Lider tecnico"),("skills", "UML,c"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0B23'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_62_SearchAllUsers(self):
        params = (('distance', ''),('lat', ''),('lon', ''),("position", ""),("skills", ""),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual("fulano@yahoo.com", reply.json()["results"][1]["email"])
        self.assertEqual("pepito@yahoo.com", reply.json()["results"][2]["email"])
        self.assertEqual("mengano@yahoo.com", reply.json()["results"][3]["email"])

    def test_63_SearchWithOffset0AndLimit2(self):
        params = (('distance', ''),('lat', ''),('lon', ''),("position", ""),("skills", ""),("offset", "0"),("limit", "2"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual("fulano@yahoo.com", reply.json()["results"][1]["email"])
        self.assertEqual(4, reply.json()["paging"]["total"])
        self.assertEqual(0, reply.json()["paging"]["offset"])

    def test_64_SearchWithOffset2AndLimit2(self):
        params = (('distance', ''),('lat', ''),('lon', ''),("position", ""),("skills", ""),("offset", "2"),("limit", "2"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("pepito@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual("mengano@yahoo.com", reply.json()["results"][1]["email"])
        self.assertEqual(4, reply.json()["paging"]["total"])
        self.assertEqual(2, reply.json()["paging"]["offset"])

    def test_65_LogoutUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/logout', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_66_LogoutSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/logout', params=params)
        self.assertEqual(200, reply.status_code)

    def test_67_RecoveryPassUnsuccessfully(self):
        reply_recovery_pass = requests.get('http://localhost:8000/users/noExist@yahoo.com/recovery_pass')
        self.assertEqual(500, reply_recovery_pass.status_code)
        self.assertEqual("Client not exists.", reply_recovery_pass.json()["message"])
        self.assertEqual(6, reply_recovery_pass.json()["code"])        

    def test_68_RecoveryPassSuccessfully(self):
        reply_recovery_pass = requests.get('http://localhost:8000/users/test@yahoo.com/recovery_pass')
        self.assertEqual(200, reply_recovery_pass.status_code)
        self.assertEqual("admin", reply_recovery_pass.json()["password"])        

        body = {"email": "test@yahoo.com", "password": "admin"}
        reply = requests.post('http://localhost:8000/users/login', json=body)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("asdf1234asdf", reply.json()["profile"]["picture"])
        self.assertEqual("Donnal", reply.json()["profile"]["first_name"])
        self.assertEqual("Trump", reply.json()["profile"]["last_name"])
        self.assertEqual("test@yahoo.com", reply.json()["profile"]["email"])
        self.assertEqual("eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4", reply.json()["metadata"]["token"])


suite = unittest.TestLoader().loadTestsFromTestCase(testApplicationServer)
def StartServer():
   #bashCommand = "rm -rf db log.txt"
   #process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
   #output, error = process.communicate()
   call(["./Server"])

def KillServer():
   for line in os.popen("ps ax | grep Server | grep -v grep"):
       fields = line.split()
       pid = fields[0]
   os.kill(int(pid), signal.SIGKILL)

def RunTests():
   unittest.TextTestRunner(verbosity=2).run(suite)


t1 = Process(target = StartServer, args = ())
t2 = Process(target = RunTests, args = ())
t1.start()
t2.start()
t2.join()
KillServer()
#unittest.TextTestRunner(verbosity=2).run(suite)
