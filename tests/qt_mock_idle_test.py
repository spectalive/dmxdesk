"""Verify the mock QLC+ master keeps a quiet websocket open.

2026-11-04: on the tablet the mock dropped the link about 150 ms after every
link-up, because a 0.5 s receive timeout ended the handler whenever the desk
said nothing. The text read "Enlazado" while the dot never left red, and the
earlier frame-time runs saw the same closes.
"""

import base64
import os
import queue
import socket
import sys
import threading
import time
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from qt_mock_handler import QtMockHandler  # noqa: E402
from qt_mock_server import QtMockServer  # noqa: E402

REQUEST = "QLC+API|isProjectLoaded"


class QtMockIdleTest(unittest.TestCase):
    def test_quiet_socket_still_answers(self):
        with QtMockServer(("127.0.0.1", 0), QtMockHandler) as server:
            server.fixture = b"{}"
            server.frames = queue.Queue()
            threading.Thread(target=server.serve_forever, daemon=True).start()
            try:
                with socket.create_connection(server.server_address, timeout=3) as client:
                    key = base64.b64encode(os.urandom(16))
                    client.sendall(b"GET /qlcplusWS HTTP/1.1\r\nHost: mock\r\nUpgrade: websocket\r\n"
                                   b"Connection: Upgrade\r\nSec-WebSocket-Key: " + key
                                   + b"\r\nSec-WebSocket-Version: 13\r\n\r\n")
                    header = b""
                    while b"\r\n\r\n" not in header:
                        header += client.recv(4096)
                    self.assertIn(b" 101 ", header)
                    time.sleep(1.2)
                    mask = os.urandom(4)
                    body = bytes(byte ^ mask[index % 4] for index, byte in enumerate(REQUEST.encode()))
                    client.sendall(bytes([0x81, 0x80 | len(body)]) + mask + body)
                    reply = client.recv(4096)
                    self.assertTrue(reply.endswith(b"QLC+API|isProjectLoaded|1"), reply)
            finally:
                server.shutdown()


if __name__ == "__main__":
    unittest.main()
