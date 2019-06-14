#!/usr/bin/env python3
import os
import sys
simgui_dir = os.path.dirname(os.path.realpath(__file__))
sys.path.append(simgui_dir)

import threading

import server
import model
import ui

large = True

field = model.Field()
field.create((1, 1), model.test_id(1))
field.create((2, 1), model.test_id(2))
field.create((3, 1), model.test_id(3))
if large:
    field.create((2, 2), model.gen_id())
    field.create((3, 2), model.gen_id())
    field.create((3, 3), model.gen_id())
    field.create((3, 4), model.gen_id())
    field.create((2, 4), model.gen_id())
    field.create((1, 4), model.gen_id())
    field.create((1, 3), model.gen_id())
    field.create((1, 2), model.gen_id())
    #field.create((2, 3), model.gen_id())

server = server.CellServer(('localhost', 6788), field)
thread = threading.Thread(target = server.serve_forever)
thread.start()


ui.run(sys.argv, field)

print("Application has closed")
os._exit(0)