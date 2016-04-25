#!/usr/bin/env python

import simplejson
import requests

class MADRASClient:
    def __init__(self, url):
        self.url = url

    def _apicall(self, path, method = "GET", data = None):
        fullpath = "{0}/{1}".format(self.url, path)

        if method == "GET":
            r = requests.get(fullpath, data)
        elif method == "POST":
            r = requests.post(fullpath, data)

        if not r.status_code == 200:
            print("API Response not 200")
            print("{}: {}".format(r.status_code, r.text))

        try:
            return r.json()
        except simplejson.scanner.JSONDecodeError:
            pass

    def listPlugins(self):
        return self._apicall("plugins/all")["plugins"]
    
    def viewChain(self):
        return self._apicall("plugins/list")["plugins"]

    def addPlugin(self, name):
        self._apicall("plugins/add", "POST", {"name" : name})

    def getPlugin(self, id):
        return self._apicall("plugins/get", "GET", {"id" : id})["plugin"]

    def removePlugin(self, id):
        self._apicall("plugins/delete", "POST", {"id" : id})
    
    def updatePlugin(self, id, parameter, value):
        self._apicall("plugins/update", "POST", {"id" : id,
                                                 "parameter" : parameter,
                                                 "value" : value})
