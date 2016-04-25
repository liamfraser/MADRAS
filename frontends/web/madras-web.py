#!/usr/bin/env python2

from flask import Flask, render_template, request
from flask_bootstrap import Bootstrap
import sys
sys.path.append("../lib")
from madras import MADRASClient

# Needed because of unicode strings
import locale
locale.setlocale(locale.LC_ALL, '')

# Uses flask-bootstrap:
# https://pythonhosted.org/Flask-Bootstrap/basic-usage.html
# sudo pip2 install flask-bootstrap

app = Flask(__name__)
mclient = MADRASClient("http://localhost:8080")

@app.route('/')
def index(editing=None, settings=None):
    # Get a list of plugins we can add
    new_plugins = [str(x) for x in mclient.listPlugins().keys()]
    new_plugins = sorted(new_plugins, key=str.lower)
    
    plugin_chain = mclient.viewChain()
    if plugin_chain == None: plugin_chain = {}
    # Plugin chain is a dict which maps an index to a plugin obj
    # Want to have it as 0, Chorus as a tuple
    pchain_lst = []
    for k in plugin_chain.keys():
        pchain_lst.append((k, plugin_chain[k]['type']))
    pchain_lst = sorted(pchain_lst, key=lambda x: x[0])

    return render_template('index.html',
                           plugins = new_plugins,
                           plugin_chain = pchain_lst,
                           editing = editing,
                           settings = settings)

@app.route('/add/<plugin>')
def add(plugin):
    mclient.addPlugin(plugin)
    return index()

@app.route('/edit/<id>')
def edit(id):
    settings = mclient.getPlugin(id)

    # Get rid of trailing zeros on floats
    for p in settings['parameters']:
        val = str(settings['parameters'][p]['value'])
        val = val.rstrip('0').rstrip('.')
        settings['parameters'][p]['value'] = val

    return index(editing = id, settings = settings)

@app.route('/delete/<id>')
def delete(id):
    mclient.removePlugin(id)
    return index()

@app.route('/update', methods=['POST'])
def update():
    id = request.form['_plugin_id']
    newSettings = {}
    for k, v in request.form.iteritems():
        if k == "_plugin_id":
            continue
        else:
            # Update each value with the API
            mclient.updatePlugin(id, k, v)
    
    return edit(id)

def bootstrap_app():
    Bootstrap(app)
    return app

if __name__ == '__main__':
    bootstrap_app().run(debug=True, host='0.0.0.0')
