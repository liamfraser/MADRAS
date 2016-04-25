#!/usr/bin/env python

import simplejson
import requests
import sys
sys.path.append("../lib")
from madras import MADRASClient

class CLIOption:
    def __init__(self, value, text):
        self.value = value
        self.text = text
        self.number = None

class MADRASCli:
    def __init__(self, client):
        self.client = client

    def addPlugin(self):
        plugins = self.client.listPlugins()

        options = []
        for p in plugins:
            options.append(CLIOption(p, p))

        resp = self.askQuestion(options)

        if resp == None:
            return

        self.client.addPlugin(resp)

    def _pluginMenuItem(self, chain, p):
        t = chain[p]['type']
        param_list = []
        out = "[{}] {}".format(p, t)

        parameters = chain[p]['parameters']

        if not parameters:
            return out

        out += "("
        for param in chain[p]['parameters']:
            s = "{} = {}".format(param, chain[p]['parameters'][param]['value'])
            param_list.append(s)

        out += ", ".join(param_list)
        out += ")"

        return out

    def viewChain(self):
        print("")

        chain = self.client.viewChain()
        if not chain:
            print("The plugin chain is currently empty")
            return

        for p in chain:
            print(self._pluginMenuItem(chain, p))
    
    def askChainQuestion(self):
        chain = self.client.viewChain()
        options = []
        
        if not chain:
            print("\nThe plugin chain is currently empty")
            return

        for p in chain:
            pstr = self._pluginMenuItem(chain, p)
            options.append(CLIOption(p, pstr))

        result = self.askQuestion(options)
        return result

    def removePlugin(self):
        result = self.askChainQuestion()
        if result:
            self.client.removePlugin(result)

    def updatePlugin(self):
        id = self.askChainQuestion()
        if not id:
            return
    
        p = self.client.getPlugin(id)
        parameters = p['parameters']

        if not parameters:
            print("\nNo parameters to edit")
            return

        options = []
        for param in parameters:
            s = "{} = {}".format(param, parameters[param]['value'])
            options.append(CLIOption(param, s))

        targetParam = self.askQuestion(options)

        if not targetParam:
            return

        newval = input("Please enter a new value for {}: ".format(targetParam))
        self.client.updatePlugin(id, targetParam, newval)

    def doOptions(self):
        options = [CLIOption(self.addPlugin, "Add a plugin"),
                   CLIOption(self.updatePlugin, "Update a plugin"),
                   CLIOption(self.removePlugin, "Remove a plugin"),
                   CLIOption(self.viewChain, "View plugin chain"),
                   CLIOption(sys.exit, "Exit")]

        result = self.askQuestion(options)
    
        if result:
            result()

    def askQuestion(self, options):
        print("")

        count = 1
        for o in options:
            o.number = count
            print("{0}. {1}".format(o.number, o.text))
            count += 1
        
        resp = input("\nWhich option would you like? ")
        num = None

        try:
            num = int(resp)
        except ValueError:
            return None

        for o in options:
            if o.number == num:
                return o.value

    def loop(self):
        print("Welcome to MADRAS (My Arm Digital Real-time Audio System)")
        while True:
            self.doOptions()

if __name__ == "__main__":
    server = "http://madras.home.org:8080"
    
    if len(sys.argv) > 1:
        if sys.argv[1] == "--localhost":
             server = "http://localhost:8080"

    print("Using server {0}".format(server))
    client = MADRASClient(server)
    cli = MADRASCli(client)
    cli.loop()
