import random
import binascii
import re
import threading
import subprocess
from PySide2.QtCore import QObject, Signal, Property, Slot

id_byte_len = 12
display_size = 8

def hexify(bytes):
    return binascii.b2a_hex(bytes).decode('ascii')

def gen_id():
    return hexify(random.getrandbits(id_byte_len*8).to_bytes(id_byte_len, 'big'))

def test_id(n: int):
    return hexify(n.to_bytes(id_byte_len, 'big'))


re_hex_string = re.compile('^[A-Za-z0-9]*$')

def is_valid_id_string(str: str):
    return ((len(str) == id_byte_len * 2) and re_hex_string.fullmatch(str))

class CellNotEmpty(Exception):
    def __init__(self, at: (int, int)):
        super("Cell at {} is not empty".format(at))
        self.at = at

class Field(QObject):
    """Represents a "playing field", where DotMatrix cells are placed"""
    changed = Signal()
    def __init__(self):
        super().__init__()
        self.cells = list()
        self.cell_by_position = dict()
        self.cell_by_id = dict()
        self.lock = threading.Lock()

    def checkEmpty(self, at: (int, int)):
        if at in self.cell_by_position:
            raise CellNotEmpty(at)

    @Slot()
    def restartAll(self):
        for c in self.cells:
            c.restart()

    @Slot()
    def killAll(self):
        for c in self.cells:
            c.kill()

    def create(self, at: (int, int), id: str = None):
        if id == None:
            id = gen_id()
        assert is_valid_id_string(id)
        id = id.upper()
        cell = Cell(self, at, id)
        self.cells.append(cell)
        self.cell_by_position[at] = cell
        self.cell_by_id[id] = cell

    @Property('QVariantList', notify = changed)
    def qCells(self):
        return self.cells

class Cell(QObject):
    changed = Signal()
    def __init__(self, field, position: (int, int), id):
        super().__init__()
        self.field = field
        self.position = position
        self.id = id
        self._display = [Pixel((x % display_size, x / display_size)) for x in range(display_size * display_size)]
        self._led_state = False
        self._client = None
        self._target = None

    @Slot()
    def restart(self):
        if self._target is not None:
            self._target.terminate()
            self._target = None

        self._target = subprocess.Popen(['../GameOfLife/build_sim/main', '-i', self.id])
        self.changed.emit()

    @Slot()
    def kill(self):
        if self._target is not None:
            self._target.terminate()
            self._target = None
            self.changed.emit()

    @Property(bool, notify = changed)
    def ledState(self):
        return self._led_state

    @Property('QVariantList', constant = True)
    def display(self):
        return self._display

    @Property(int, notify = changed)
    def x(self):
        return self.position[0]

    @Property(int, notify = changed)
    def y(self):
        return self.position[1]

    def update_led(self, new_state):
        self._led_state = new_state
        self.changed.emit()

    def update_display_row(self, row, row_data):
        for i, v in enumerate(row_data):
            self._display[row * display_size + i].update(v)

    def client_connected(self, client):
        assert self._client is None
        self._client = client
        self.changed.emit()

    def client_disconnected(self):
        self._client = None
        self.changed.emit()

    @Property(bool, notify = changed)
    def connected(self):
        return self._client is not None

    @Property(bool, notify=changed)
    def targetRunning(self):
        return self._target is not None


    def __str__(self):
        return 'Cell[{}]'.format(self.id)

class Pixel(QObject):
    changed = Signal()
    def __init__(self, position: (int, int)):
        super().__init__()
        self.position = position
        self._value = 0

    def update(self, value):
        if self._value == value:
            return
        self._value = value
        self.changed.emit()

    @Property(int, notify = changed)
    def value(self):
        return self._value * 8

    @Property(int, constant = True)
    def x(self):
        return self.position[0]

    @Property(int, constant = True)
    def y(self):
        return self.position[1]