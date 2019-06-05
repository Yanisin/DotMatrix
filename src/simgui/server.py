import socketserver
import socket
import struct
import proto_defs
import binascii

class CellServer(socketserver.ThreadingTCPServer):
    allow_reuse_address = True
    def __init__(self, addr, field):
        super().__init__(addr, ClientHandler)
        self.field = field

class CommException(Exception):
    pass

class Message:
    def __init__(self, len: int, msgid: int, data: bytes):
        self.len = len
        self.msgid = msgid
        self.data = data

    def __str__(self):
        return 'Message({}, len={})'.format(self.msgid, self.len)

class ClientHandler(socketserver.BaseRequestHandler):
    def handle(self):
        self.field = self.server.field
        self.cell_id = None

        router = {
            proto_defs.MSGID_CLIENT_HELLO: self.msg_hello,
            proto_defs.MSGID_LED_STATE: self.msg_led,
            proto_defs.MSGID_UART_TX: self.msg_uart_tx,
            proto_defs.MSGID_UPDATE_DISP: self.msg_disp,
            proto_defs.MSGID_GPIO_STATE: self.msg_gpio_state,
        }
        while True:
            header = self.request.recv(4, socket.MSG_WAITALL)
            if len(header) != 4:
                break
            (msglen, msgid) = struct.unpack('!HH', header)
            data = self.request.recv(msglen, socket.MSG_WAITALL)
            if len(data) != msglen:
                break
            msg = Message(msglen, msgid, data)
            if msg.msgid in router:
                with self.field.lock:
                    router[msg.msgid](msg)
            else:
                raise CommException('Unknown message ID {} received'.format(msg.msgid))

        if self.cell is not None:
            with self.field.lock:
                self.cell.client_disconnected()
        print('Disconnected')

    def send(self, msgid, data: bytes):
        msg = struct.pack('!HH', len(data), msgid) + data
        # print(binascii.b2a_hex(msg)
        self.request.send(msg, 0)

    def msg_hello(self, msg: Message):
        assert self.cell_id is None
        self.cell_id = binascii.b2a_hex(msg.data).decode('ascii').upper()
        if self.cell_id not in self.field.cell_by_id:
            raise CommException('Cell with unknown ID {} tried to connected'.format(self.cell_id))
        self.cell = self.field.cell_by_id[self.cell_id]
        self.cell.client_connected(self)

    def msg_gpio_state(self, msg: Message):
        self.cell.update_gpio(msg.data[0] != 0)

    def msg_disp(self, msg: Message):
        self.cell.update_display_row(msg.data[0], msg.data[1:])

    def msg_led(self, msg: Message):
        self.cell.update_led(msg.data[0] != 0)

    def msg_uart_tx(self, msg: Message):
        dir = msg.data[0]
        data = msg.data[1:]
        self.cell.uart_tx(dir, data)
