import re

IPV4 = re.compile(r"\b\d{1,3}(?:\.\d{1,3}){3}\b")


def backend_diagnostics(stderr_path, host, lines=20):
    """The backend's last stderr lines, with the master's address removed.

    The command may reach a device over SSH, so what it prints can name the
    host it was pointed at; a failure report keeps the reason and drops that.
    """
    with open(stderr_path, errors="replace") as stream:
        tail = stream.read().splitlines()[-lines:]
    text = "\n".join(tail).replace(host, "<host>")
    return IPV4.sub("<ip>", text) or "(backend wrote nothing to stderr)"
