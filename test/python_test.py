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
    
    def test_03_RegisterClientExists(self):
        body = {"email": "test@yahoo.com", "password": "admin", "device_id": "123", "first_name": "test", "last_name": "T", "gender": "M",	"birthday": "01/01/2000",
                "address": { "lat": "123456789", "lon": "12345678" }}
        reply = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Client already exists.", reply.json()["message"])

    def test_04_LoginSuccess(self):
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

    def test_09_GetPersonalDataOfOtherUserSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/a@yahoo.com/profile/personal', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("", reply.json()["first_name"])

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

    def test_14_GetSummaryOfOtherUserSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/a@yahoo.com/profile/summary', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("", reply.json()["summary"])

    def test_15_GetEmptyExpertiseSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual(0, len(reply.json()["expertises"]))

    def test_16_CreateExpertisesUnsuccessfully(self):
        body = {"expertises":[{"company": "LalalaDOS","position": "Lider tecnico","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "software"},{"company": "LololoDOS","position": "Especialista tecnico","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "hardware"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXz"}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not upload.", reply.json()["message"])

    def test_17_CreateExpertisesSuccessfully(self):
        body = {"expertises":[{"company": "LalalaDOS","position": "Lider tecnico","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "software"},{"company": "LololoDOS","position": "Especialista tecnico","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "hardware"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params, json=body)
        self.assertEqual(201, reply.status_code)

    def test_18_ModifyExpertiseSuccessfully(self):
        body = {"expertises":[{"company": "Lalala","position": "Lider tecnico","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "software"},{"company": "Lololo","position": "Especialista tecnico","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "hardware"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_19_ModifyExpertiseUnsuccsessfully(self):
        body = {"expertises":[{"company": "Lalala","position": "Lider tecnico","from": "01/01/2010","to": "26/09/2016","expertise": "aaer","category": "software"},{"company": "Lololo","position": "Especialista tecnico","from": "02/02/2012","to": "02/02/2016","expertise": "aaer","category": "hardware"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not upload.", reply.json()["message"])

    def test_20_GetExpertiseSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/expertise', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("software", reply.json()["expertises"][0]["category"])
        self.assertEqual("01/01/2010", reply.json()["expertises"][0]["from"])
        self.assertEqual("Lalala", reply.json()["expertises"][0]["company"])
        self.assertEqual("26/09/2016", reply.json()["expertises"][0]["to"])
        self.assertEqual("aaer", reply.json()["expertises"][0]["expertise"])
        self.assertEqual("Lider tecnico", reply.json()["expertises"][0]["position"])
        self.assertEqual("hardware", reply.json()["expertises"][1]["category"])
        self.assertEqual("02/02/2012", reply.json()["expertises"][1]["from"])
        self.assertEqual("Lololo", reply.json()["expertises"][1]["company"])
        self.assertEqual("02/02/2016", reply.json()["expertises"][1]["to"])
        self.assertEqual("aaer", reply.json()["expertises"][1]["expertise"])
        self.assertEqual("Especialista tecnico", reply.json()["expertises"][1]["position"])

    def test_21_GetExpertiseOfOtherUserSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/a@yahoo.com/profile/expertise', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual([], reply.json()["expertises"])

    def test_22_GetEmptySkillsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/skills', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual(0, len(reply.json()["every_skill"]))

    def test_23_ModifySkillsSuccessfully(self):
        body = {"every_skill":[{"skills": ["java","c","UML"],"category": "software"},{"skills": ["moto","auto"],"category": "licencia_manejo"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/skills/category', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_24_ModifySkillsUnsuccsessfully(self):
        body = {"every_skill":[{"skills": ["java","c","UML"],"category": "software"},{"skills": ["moto","auto"],"category": "licencia_manejo"}]}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/skills/category', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not upload.", reply.json()["message"])

    def test_25_GetSkillsSuccessfully(self):
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

    def test_26_GetSkillsOfOtherUserSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/a@yahoo.com/profile/skills', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual([], reply.json()["every_skill"])

    def test_27_GetEmptyPictureSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/picture', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("", reply.json()["picture"])

    def test_28_ModifyPictureSuccessfully(self):
        body = {"picture": "asdf1234asdf"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/picture', params=params, json=body)
        self.assertEqual(200, reply.status_code)

    def test_29_ModifyPictureUnsuccsessfully(self):
        body = {"picture": "asdf1234asdf"}
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.put('http://localhost:8000/users/test@yahoo.com/profile/picture', params=params, json=body)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not upload.", reply.json()["message"])

    def test_30_GetPictureSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/picture', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("asdf1234asdf", reply.json()["picture"])

    def test_31_GetPictureOfOtherUserSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/a@yahoo.com/profile/picture', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("", reply.json()["picture"])

    def test_32_GetContactVacioSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual(0, len(reply.json()["solicitudes"]))

    def test_33_PostContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(201, reply.status_code)
        self.assertEqual("La solicitud de amistad ha sido enviada a contact@gmail.com con exito", reply.json()["message"])

    def test_34_PostContactUnsuccsessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"),("date","03-01-2010T12:34:00.000Z"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("Could not post.", reply.json()["message"])

    def test_35_GetContactSuccessfully(self):
        body = {"email": "contact@gmail.com", "password": "admin", "device_id": "321", "first_name": "cont", "last_name": "act", "gender": "M","birthday": "01/01/2000", "address": { "lat": "123456789", "lon": "12345678" }}
        reply = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(201, reply.status_code)
        self.assertEqual("OK", json.loads(reply.content)["registration"])


        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6ImNvbnRhY3RAZ21haWwuY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.5a4NlJZMTFESSA3QWnX2Mwjt2vA2LFsB2wa6W2bc3zA"}
        reply = requests.get('http://localhost:8000/users/contact@gmail.com/contact', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual(1, len(reply.json()["solicitudes"]))
        self.assertEqual("03-01-2010T12:34:00.000Z", reply.json()["solicitudes"][0]["date"])
        self.assertEqual("test@yahoo.com", reply.json()["solicitudes"][0]["email"])
        self.assertEqual("Donnal", reply.json()["solicitudes"][0]["first_name"])
        self.assertEqual("Trump", reply.json()["solicitudes"][0]["last_name"])
        self.assertEqual(0, reply.json()["solicitudes"][0]["votes"])
        self.assertEqual("asdf1234asdf", reply.json()["solicitudes"][0]["thumbnail"])

    def test_36_GetContactUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_37_PostAcceptContactSuccessfully(self):
        params = (("date","03-01-2010T12:34:00.000Z"),("email", "test@yahoo.com"),("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6ImNvbnRhY3RAZ21haWwuY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.5a4NlJZMTFESSA3QWnX2Mwjt2vA2LFsB2wa6W2bc3zA"))
        reply = requests.post('http://localhost:8000/users/contact@gmail.com/accept', params=params)
        self.assertEqual(201, reply.status_code)

    def test_38_PostAcceptContactWithoutSolicitude(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6ImNvbnRhY3RAZ21haWwuY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.5a4NlJZMTFESSA3QWnX2Mwjt2vA2LFsB2wa6W2bc3zA"}
        reply = requests.post('http://localhost:8000/users/contact@gmail.com/accept', params=params)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("User did not send solicitude.", reply.json()["message"])

    def test_39_PostAcceptContactUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.post('http://localhost:8000/users/contact@gmail.com/accept', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_40_DeleteRejectContactSuccessfully(self):
        body = {"email": "a@a.com", "password": "admin", "device_id": "222", "first_name": "A", "last_name": "A", "gender": "M","birthday": "01/01/2000","address": { "lat": "123456789", "lon": "12345678" }}
        reply = requests.post('http://localhost:8000/users/register', json=body)

        params = "date=03-01-2010T12:34:00.000Z&email=a@a.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/contact', params=params)
        params = "date=03-01-2010T12:34:00.000Z&email=test@yahoo.com&token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6ImFAYS5jb20iLCJpbmNyZW1lbnRhbF9udW1iZXIiOjAsInBhc3N3b3JkIjoiYWRtaW4ifQ._q7591YbCv2c-Rm_6qQfEbHwCMD_qlAWNhqRWwPvxRM"
        reply_reject = requests.delete('http://localhost:8000/users/a@a.com/reject', params=params)
        self.assertEqual(204, reply_reject.status_code)
#        self.assertEqual("La solicitud de amistad de test@yahoo.com ha sido rechazada con exito", reply_reject.json()["message"])

    def test_41_DeleteRejectContactWithoutSolicitude(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/reject', params=params)
        self.assertEqual(500, reply.status_code)
        self.assertEqual("User did not send solicitude.", reply.json()["message"])

    def test_42_DeleteRejectContactUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/reject', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_43_GetFriendsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/contacts', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("contact@gmail.com", reply.json()["friends"][0]["email"])

    def test_44_GetFriendsUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/profile/contacts', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_45_PutVoteContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(201, reply.status_code)

    def test_46_PutUnVoteContactSuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"),("email", "contact@gmail.com")}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(204, reply.status_code)

    def test_47_PutVoteContactUnsuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"),("email", "contact@gmail.com")}
        reply = requests.post('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_48_PutUnvoteContactUnsuccessfully(self):
        params = {("token","eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"),("email", "contact@gmail.com")}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/vote', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_49_GetOwnRecommendationsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/own_recommendations', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual([], reply.json()["own_recommendations"])

    def test_50_GetOwnRecommendationsUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/own_recommendations', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_51_GetOwnRecommendationsSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/others_recommendations', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual([], reply.json()["others_recommendations"])

    def test_52_GetOwnRecommendationsUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.get('http://localhost:8000/users/test@yahoo.com/others_recommendations', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_53_GetMostPopularUsersSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.get('http://localhost:8000/users/pop', params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual(0, reply.json()["most_popular_users"][0]["votes"])
        self.assertEqual("contact@gmail.com", reply.json()["most_popular_users"][0]["email"])
        self.assertEqual(0, reply.json()["most_popular_users"][1]["votes"])
        self.assertEqual("test@yahoo.com", reply.json()["most_popular_users"][1]["email"])

    def test_54_GetMostPopularUsersAddingVotesSuccessfully(self):
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

    def test_55_SearchByPosition(self):
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
        reply_fulano = requests.put('http://localhost:8000/users/fulano@yahoo.com/profile/expertise', params=params, json=body)
        self.assertEqual(200, reply_fulano.status_code)

        # Distance is in Km
        params = (('distance', ''),('lat', ''),('lon', ''),("position", "Lider tecnico"),("skills", ""),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        #self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual([], reply.json()["results"])

    def test_56_SearchBySkills(self):
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
        #self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual([], reply.json()["results"])

    def test_57_SearchByDistance(self):
        body = {"email": "mengano@yahoo.com", "password": "admin", "device_id": "123", "first_name": "test", "last_name": "T", "gender": "M",	"birthday": "01/01/2000",
                "address": { "lat": "-11.9302235", "lon": "-77.0846415" }}
        reply_pepito_register = requests.post('http://localhost:8000/users/register', json=body)
        self.assertEqual(201, reply_pepito_register.status_code)
        self.assertEqual("OK", json.loads(reply_pepito_register.content)["registration"])
        # Distance is in Km
        params = (('distance', '4000.0'),('lat', '-34.3971898'),('lon', '-58.7568274'),("position", ""),("skills", ""),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        #self.assertEqual("test@yahoo.com", reply.json()["results"][1]["email"])
        self.assertEqual("mengano@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual("3110.75", reply.json()["results"][0]["distance"])
        self.assertEqual(0, reply.json()["results"][0]["votes"])
        self.assertEqual("test", reply.json()["results"][0]["first_name"])
        self.assertEqual("T", reply.json()["results"][0]["last_name"])
        self.assertEqual("", reply.json()["results"][0]["thumbnail"])

    def test_58_SearchByDistanceByDefault(self):
        # Distance is in Km
        params = (('distance', ''),('lat', '-11.9302'),('lon', '-77.0856'),("position", ""),("skills", ""),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        #self.assertEqual("test@yahoo.com", reply.json()["results"][1]["email"])
        #self.assertEqual("mengano@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual("mengano@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual("0.104311", reply.json()["results"][0]["distance"])
        self.assertEqual(0, reply.json()["results"][0]["votes"])
        self.assertEqual("test", reply.json()["results"][0]["first_name"])
        self.assertEqual("T", reply.json()["results"][0]["last_name"])
        self.assertEqual("", reply.json()["results"][0]["thumbnail"])

    def test_59_SearchByPositionAndSkills(self):
        params = (('distance', ''),('lat', ''),('lon', ''),("position", "Lider tecnico"),("skills", "UML,c"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        #self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual([], reply.json()["results"])

    def test_60_SearchByPositionAndDistance(self):
        params = (('distance', '4000.0'),('lat', '-34.3971898'),('lon', '-58.7568274'),("position", "Lider tecnico"),("skills", ""),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        #self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual([], reply.json()["results"])

    def test_61_SearchBySkillsAndDistance(self):
        params = (('distance', '4000.0'),('lat', '-34.3971898'),('lon', '-58.7568274'),("position", ""),("skills", "UML,c"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        #self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual([], reply.json()["results"])

    def test_62_SearchByPositionAndSkillsAndDistance(self):
        params = (('distance', '4000.0'),('lat', '-34.3971898'),('lon', '-58.7568274'),("position", "Lider tecnico"),("skills", "UML,c"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        #self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])
        #print(reply.json()["results"][0])
        self.assertEqual([], reply.json()["results"])

    def test_63_SearchByPositionAndSkillsAndDistanceUnsuccesfully(self):
        params = (('distance', '4000.0'),('lat', '-34.3971898'),('lon', '-58.7568274'),("position", "Lider tecnico"),("skills", "UML,c"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0B23'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_64_SearchAllUsers(self):
        params = (('distance', ''),('lat', ''),('lon', ''),("position", ""),("skills", ""),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        #self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual("contact@gmail.com", reply.json()["results"][0]["email"])
        self.assertEqual("a@a.com", reply.json()["results"][1]["email"])
        self.assertEqual("fulano@yahoo.com", reply.json()["results"][2]["email"])

    def test_65_SearchWithOffset0AndLimit2(self):
        params = (('distance', ''),('lat', ''),('lon', ''),("position", ""),("skills", ""),("offset", "0"),("limit", "2"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        #self.assertEqual("test@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual("contact@gmail.com", reply.json()["results"][0]["email"])
        self.assertEqual(5, reply.json()["paging"]["total"])
        self.assertEqual(0, reply.json()["paging"]["offset"])

    def test_66_SearchWithOffset2AndLimit2(self):
        params = (('distance', ''),('lat', ''),('lon', ''),("position", ""),("skills", ""),("offset", "2"),("limit", "2"),('token','eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4'))
        reply = requests.get('http://localhost:8000/users', params=params)
        self.assertEqual(200, reply.status_code)
        #self.assertEqual("a@a.com", reply.json()["results"][0]["email"])
        self.assertEqual("fulano@yahoo.com", reply.json()["results"][0]["email"])
        self.assertEqual(5, reply.json()["paging"]["total"])
        self.assertEqual(2, reply.json()["paging"]["offset"])

    def test_67_DeleteEveryExpertisesUnsuccessfully(self):
        params = {"token":"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InBlcGl0b0B5YWhvby5jb20iLCJpbmNyZW1lbnRhbF9udW1iZXIiOjAsInBhc3N3b3JkIjoiYWRtaW4ifQ.SlwDCMaJfRHKF-WYUDgucr5reR6nemsgg7oYntyk_OZ"}
        reply_delete_expertises = requests.delete('http://localhost:8000/users/pepito@yahoo.com/profile/expertise', params=params)
        self.assertEqual(401, reply_delete_expertises.status_code)
        self.assertEqual("Invalid credentials.", reply_delete_expertises.json()["message"])

    def test_68_DeleteEveryExpertiseSuccessfully(self):
        params = {"token":"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InBlcGl0b0B5YWhvby5jb20iLCJpbmNyZW1lbnRhbF9udW1iZXIiOjAsInBhc3N3b3JkIjoiYWRtaW4ifQ.SlwDCMaJfRHKF-WYUDgucr5reR6nemsgg7oYntyk_Ok"}        
        reply = requests.delete('http://localhost:8000/users/pepito@yahoo.com/profile/expertise', params=params)
        self.assertEqual(204, reply.status_code)

    def test_69_DeleteEverySkillsUnsuccessfully(self):
        params = {"token":"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InBlcGl0b0B5YWhvby5jb20iLCJpbmNyZW1lbnRhbF9udW1iZXIiOjAsInBhc3N3b3JkIjoiYWRtaW4ifQ.SlwDCMaJfRHKF-WYUDgucr5reR6nemsgg7oYntyk_OZ"}
        reply_delete_skills = requests.delete('http://localhost:8000/users/pepito@yahoo.com/profile/skills', params=params)
        self.assertEqual(401, reply_delete_skills.status_code)
        self.assertEqual("Invalid credentials.", reply_delete_skills.json()["message"])

    def test_70_DeleteEverySkillsSuccessfully(self):
        params = {"token":"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InBlcGl0b0B5YWhvby5jb20iLCJpbmNyZW1lbnRhbF9udW1iZXIiOjAsInBhc3N3b3JkIjoiYWRtaW4ifQ.SlwDCMaJfRHKF-WYUDgucr5reR6nemsgg7oYntyk_Ok"}        
        reply_delete_skills = requests.delete('http://localhost:8000/users/pepito@yahoo.com/profile/skills', params=params)
        self.assertEqual(204, reply_delete_skills.status_code)

    def test_71_DeleteSummaryUnsuccessfully(self):
        params = {"token":"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InBlcGl0b0B5YWhvby5jb20iLCJpbmNyZW1lbnRhbF9udW1iZXIiOjAsInBhc3N3b3JkIjoiYWRtaW4ifQ.SlwDCMaJfRHKF-WYUDgucr5reR6nemsgg7oYntyk_OZ"}
        reply_delete_skills = requests.delete('http://localhost:8000/users/pepito@yahoo.com/profile/summary', params=params)
        self.assertEqual(401, reply_delete_skills.status_code)
        self.assertEqual("Invalid credentials.", reply_delete_skills.json()["message"])

    def test_72_DeleteSummarySuccessfully(self):
        params = {"token":"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InBlcGl0b0B5YWhvby5jb20iLCJpbmNyZW1lbnRhbF9udW1iZXIiOjAsInBhc3N3b3JkIjoiYWRtaW4ifQ.SlwDCMaJfRHKF-WYUDgucr5reR6nemsgg7oYntyk_Ok"}        
        reply_delete_skills = requests.delete('http://localhost:8000/users/pepito@yahoo.com/profile/summary', params=params)
        self.assertEqual(204, reply_delete_skills.status_code)

    def test_73_DeletePictureUnsuccessfully(self):
        params = {"token":"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InBlcGl0b0B5YWhvby5jb20iLCJpbmNyZW1lbnRhbF9udW1iZXIiOjAsInBhc3N3b3JkIjoiYWRtaW4ifQ.SlwDCMaJfRHKF-WYUDgucr5reR6nemsgg7oYntyk_OZ"}
        reply_delete_skills = requests.delete('http://localhost:8000/users/pepito@yahoo.com/profile/picture', params=params)
        self.assertEqual(401, reply_delete_skills.status_code)
        self.assertEqual("Invalid credentials.", reply_delete_skills.json()["message"])

    def test_74_DeletePictureSuccessfully(self):
        params = {"token":"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InBlcGl0b0B5YWhvby5jb20iLCJpbmNyZW1lbnRhbF9udW1iZXIiOjAsInBhc3N3b3JkIjoiYWRtaW4ifQ.SlwDCMaJfRHKF-WYUDgucr5reR6nemsgg7oYntyk_Ok"}        
        reply_delete_skills = requests.delete('http://localhost:8000/users/pepito@yahoo.com/profile/picture', params=params)
        self.assertEqual(204, reply_delete_skills.status_code)

    def test_75_LogoutUnsuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BXZ"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/logout', params=params)
        self.assertEqual(401, reply.status_code)
        self.assertEqual("Invalid credentials.", reply.json()["message"])

    def test_76_LogoutSuccessfully(self):
        params = {"token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InRlc3RAeWFob28uY29tIiwiaW5jcmVtZW50YWxfbnVtYmVyIjowLCJwYXNzd29yZCI6ImFkbWluIn0.dNn-xtRfvbN27cD1X7sE_m-RGLgPQ5p9ilHYyjL0BX4"}
        reply = requests.delete('http://localhost:8000/users/test@yahoo.com/logout', params=params)
        self.assertEqual(200, reply.status_code)

    def test_77_RecoveryPassUnsuccessfully(self):
        reply_recovery_pass = requests.get('http://localhost:8000/users/noExist@yahoo.com/recovery_pass')
        self.assertEqual(500, reply_recovery_pass.status_code)
        self.assertEqual("Client not exists.", reply_recovery_pass.json()["message"])
        self.assertEqual(6, reply_recovery_pass.json()["code"])        

    def test_78_RecoveryPassSuccessfully(self):
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

    def test_79_RegisterFacebookSuccessfully(self):
        body = {"email": "facebook@yahoo.com", "password": "", "device_id": "123", "first_name": "test", "last_name": "T", "gender": "",	"birthday": "01/01/2000",
                "address": { "lat": "123456789", "lon": "12345678" }}
        params = {"app":"facebook"}
        reply = requests.post('http://localhost:8000/users/register', json=body, params=params)
        self.assertEqual(201, reply.status_code)
        self.assertEqual("OK", json.loads(reply.content)["registration"])

    def test_80_RegisterFacebookAlreadyRegistered(self):
        body = {"email": "facebook@yahoo.com", "password": "", "device_id": "123", "first_name": "test", "last_name": "T", "gender": "",	"birthday": "01/01/2000",
                "address": { "lat": "123456789", "lon": "12345678" }}
        params = {"app":"facebook"}
        reply = requests.post('http://localhost:8000/users/register', json=body, params=params)
        self.assertEqual(201, reply.status_code)
        self.assertEqual("OK", json.loads(reply.content)["registration"])

    def test_81_LoginFacebookSuccess(self):
        body = {"email": "facebook@yahoo.com", "password": ""}
        params = {"app":"facebook"}
        reply = requests.post('http://localhost:8000/users/login', json=body, params=params)
        self.assertEqual(200, reply.status_code)
        self.assertEqual("", reply.json()["profile"]["picture"])
        self.assertEqual("test", reply.json()["profile"]["first_name"])
        self.assertEqual("T", reply.json()["profile"]["last_name"])
        self.assertEqual("facebook@yahoo.com", reply.json()["profile"]["email"])
        self.assertEqual("eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6ImZhY2Vib29rQHlhaG9vLmNvbSIsImluY3JlbWVudGFsX251bWJlciI6MCwicGFzc3dvcmQiOiIifQ.Yw39CN31RFTTTmzbEgL5avJKpOAL6pGD5Zs8kV5z1XI", reply.json()["metadata"]["token"])


    def test_82_LoginFacebookUnsuccess(self):
        body = {"email": "facebook@yahoo.com", "password": "admin"}
        reply = requests.post('http://localhost:8000/users/login', json=body,)
        self.assertEqual(400, reply.status_code)
        self.assertEqual("Client has Facebook login.", json.loads(reply.content)["message"])

    def test_83_RequestIsNotSupport(self):
        reply = requests.get('http://localhost:8000/BLA')
        self.assertEqual(404, reply.status_code)

    def test_84_RequestIsNotSupport(self):
        reply = requests.delete('http://localhost:8000/users/BLA')
        self.assertEqual(404, reply.status_code)


suite = unittest.TestLoader().loadTestsFromTestCase(testApplicationServer)
def StartServer():
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
