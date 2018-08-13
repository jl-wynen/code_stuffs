"""
This context manager suppresses output to STDOUT.

Might not restore correctly with all modifiers but seems to work under normal circumstances.
"""

import sys
import os
import contextlib

@contextlib.contextmanager
def nostdout():
    fd = sys.stdout.fileno()

    def _redirect_stdout(to):
        sys.stdout.close() # implicitly flushes
        os.dup2(to.fileno(), fd) # now fd writes to 'to'
        sys.stdout = os.fdopen(fd, "w") # Python writes to fd

    with os.fdopen(os.dup(fd), "w") as old_stdout:
        with open(os.devnull, "w") as devnull:
            _redirect_stdout(devnull)  # swallow output
        try:
            yield
        finally:
            _redirect_stdout(old_stdout) # restore

print("0")
with nostdout():
    print("1")
print("2")
