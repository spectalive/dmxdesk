import socketserver


class QtMockServer(socketserver.ThreadingTCPServer):
    allow_reuse_address = True
    daemon_threads = True
