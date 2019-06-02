#!/usr/bin/env python3
import os
import sys
simgui_dir = os.path.dirname(os.path.realpath(__file__))
sys.path.append(simgui_dir)

import threading

import server
import model
import ui

field = model.Field()
field.create((1, 1), model.test_id(1))
field.create((2, 1), model.gen_id())
field.create((3, 1), model.gen_id())

server = server.CellServer(('localhost', 6788), field)
thread = threading.Thread(target = server.serve_forever)
thread.start()


ui.run(sys.argv, field)

print("Application has closed")
os._exit(0)