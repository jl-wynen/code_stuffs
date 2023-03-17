from pathlib import Path
import argparse
import sys
import os


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--pipe_to_ai",
        type=Path,
        help="Pipe used to send data to this process from the game.",
    )
    parser.add_argument(
        "--pipe_from_ai",
        type=Path,
        help="Pipe used to send data from this process to the game.",
    )
    return parser.parse_args()


def send(f, data: bytes):
    f.write(len(data).to_bytes(8, byteorder="big", signed=False))
    f.write(data)


def receive(f) -> bytes:
    size = int.from_bytes(f.read(8), byteorder="big", signed=False)
    return f.read(size)


def main():
    print("Started snek")
    sys.stdout.flush()  # show print immediately

    args = parse_args()
    pipe_to_ai = args.pipe_to_ai
    pipe_from_ai = args.pipe_from_ai
    assert os.path.exists(pipe_to_ai)
    assert os.path.exists(pipe_from_ai)
    with open(pipe_from_ai, "wb") as f:
        send(f, b"Hellow from snek")

    with open(pipe_to_ai, "rb") as f:
        data = receive(f)
    print("Snek received: ", data)
    sys.stdout.flush()  # show print immediately


if __name__ == "__main__":
    main()
