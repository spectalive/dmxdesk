import base64
import hashlib
import socketserver


class QtMockHandler(socketserver.BaseRequestHandler):
    def handle(self):
        self.request.settimeout(5)
        header = b""
        while b"\r\n\r\n" not in header:
            header += self.request.recv(4096)
        path = header.split(b" ")[1]
        if path == b"/vc.json":
            fixture = self.server.fixture
            self.request.sendall(
                b"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: "
                + str(len(fixture)).encode()
                + b"\r\nConnection: close\r\n\r\n"
                + fixture
            )
            return
        key = next(
            line.split(b":", 1)[1].strip()
            for line in header.split(b"\r\n")
            if line.lower().startswith(b"sec-websocket-key:")
        )
        accept = base64.b64encode(
            hashlib.sha1(key + b"258EAFA5-E914-47DA-95CA-C5AB0DC85B11").digest()
        )
        self.request.sendall(
            b"HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\n"
            b"Connection: Upgrade\r\nSec-WebSocket-Accept: " + accept + b"\r\n\r\n"
        )
        self.request.settimeout(0.5)
        try:
            while True:
                prefix = self.receive(2)
                length = prefix[1] & 127
                if length == 126:
                    length = int.from_bytes(self.receive(2), "big")
                elif length == 127:
                    length = int.from_bytes(self.receive(8), "big")
                mask = self.receive(4)
                body = self.receive(length)
                frame = bytes(byte ^ mask[index % 4] for index, byte in enumerate(body)).decode()
                self.server.frames.put(frame)
                if frame == "QLC+API|isProjectLoaded":
                    reply = b"QLC+API|isProjectLoaded|1"
                    self.request.sendall(bytes([0x81, len(reply)]) + reply)
        except (OSError, EOFError):
            pass

    def receive(self, count):
        result = b""
        while len(result) < count:
            received = self.request.recv(count - len(result))
            if not received:
                raise EOFError()
            result += received
        return result
