# Interprocess communication with Rust and Python

A basic example of a rust program that starts a Python program and sends messages and receives via named pipes (FIFOs in Linux speak).

Messages are encoded as 
```
message = size + payload
size = 8 bytes big endian unsigned int, size in bytes of payload
payload = aritrary bytes, utf-8 strings in this example
```
