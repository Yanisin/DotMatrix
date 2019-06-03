#!/usr/bin/env python3
import pyudev

context = pyudev.Context()
monitor = pyudev.Monitor.from_netlink(context)
monitor.filter_by(subsystem='usb')
monitor.start()

for (action, device) in monitor:
    if action == 'add':
        if device.get('ID_VENDOR_ID') == 'dead' and device.get('ID_MODEL_ID') == 'ca5d':
            print(device)
            break

