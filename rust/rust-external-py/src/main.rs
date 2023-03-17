use std::fs::{File, OpenOptions};
use std::io::Read;
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::{Child, Command};

use log::{info, trace};
use nix::sys::stat;
use nix::unistd;
use tempfile::{tempdir, TempDir};

struct AIPipes {
    _dir: TempDir,
    pub pipe_to_ai: PathBuf,
    pub pipe_from_ai: PathBuf,
}

impl AIPipes {
    pub fn open() -> Self {
        let dir = tempdir().expect("Failed to create directory for pipes");
        let pipe_to_ai = make_pipe(&dir, "to_ai");
        let pipe_from_ai = make_pipe(&dir, "from_ai");

        info!(
            "Created pipes for communicating with an AI in {}",
            dir.path().to_str().unwrap()
        );

        Self {
            _dir: dir,
            pipe_to_ai,
            pipe_from_ai,
        }
    }

    pub fn open_to_ai(&self) -> File {
        open_pipe(&self.pipe_to_ai, true)
    }

    pub fn open_from_ai(&self) -> File {
        open_pipe(&self.pipe_from_ai, false)
    }
}

fn make_pipe(dir: &TempDir, name: &str) -> PathBuf {
    let fname = dir.path().join(name);
    unistd::mkfifo(&fname, stat::Mode::S_IRWXU).expect("Failed to create pipe.");
    fname
}

fn open_pipe(path: &Path, write: bool) -> File {
    trace!("Opening pipe at {:?}", path);
    OpenOptions::new()
        .write(write)
        .read(!write)
        .open(path)
        .unwrap()
}

fn spawn_ai(cmd: &str, pipe_to_ai: &Path, pipe_from_ai: &Path) -> Child {
    Command::new("python")
        .arg(cmd)
        .arg("--pipe_to_ai")
        .arg(pipe_to_ai)
        .arg("--pipe_from_ai")
        .arg(pipe_from_ai)
        .spawn()
        .unwrap_or_else(|_| panic!("Failed to start Ai process at {cmd}"))
}

fn read_size(f: &mut File) -> usize {
    let mut message_size = [0; 8];
    f.read_exact(&mut message_size).unwrap();
    usize::from_be_bytes(message_size)
}

fn write_size(f: &mut File, size: usize) {
    // Sizes must always be encoded with 8 bytes.
    let size = size as u64;
    f.write_all(&size.to_be_bytes()).unwrap();
}

fn send(f: &mut File, message: &[u8]) {
    write_size(f, message.len());
    f.write_all(message).unwrap();
}

fn receive(f: &mut File) -> Vec<u8> {
    let size = read_size(f);
    let mut buffer = vec![0; size];
    buffer.resize(size, 0);
    f.read_exact(&mut buffer).unwrap();
    buffer
}

fn main() {
    let pipes = AIPipes::open();
    let cmd = spawn_ai("snek.py", &pipes.pipe_to_ai, &pipes.pipe_from_ai);
    println!("started python");

    println!("Listening for message");
    let mut f = pipes.open_from_ai();
    let message = receive(&mut f);
    println!("Received: {:?}", message);
    let s = String::from_utf8(message).unwrap();
    println!("As string: {}", s);

    println!("------------------");

    println!("Sending message");
    let mut f = pipes.open_to_ai();
    let s = "Nice to meet you";
    let message = s.as_bytes();
    send(&mut f, message);

    println!("Done, shutting down");
}
