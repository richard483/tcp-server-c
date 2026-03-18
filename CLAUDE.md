# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Goal

This is a learning project. The user is building a TCP webserver in C from scratch. The goal is to **guide, not write code for them** — explain concepts, point to relevant syscalls/man pages, and review their code when asked.

## Build & Run (once files exist)

```bash
# Compile
gcc -Wall -Wextra -o server server.c

# Run
./server

# With address sanitizer (useful for catching memory bugs)
gcc -Wall -Wextra -fsanitize=address -o server server.c && ./server
```

## Teaching Approach

- Explain concepts first, then let the user implement.
- When reviewing code, point out issues rather than rewriting.
- Reference relevant man pages (e.g., `man 2 socket`, `man 7 ip`, `man 2 accept`).
- Key syscalls to introduce in order: `socket()` → `bind()` → `listen()` → `accept()` → `read()`/`write()` → `close()`.

## Key Concepts for This Project

- **Socket**: `socket(AF_INET, SOCK_STREAM, 0)` — creates a TCP socket file descriptor.
- **`SO_REUSEADDR`**: Set via `setsockopt()` to avoid "Address already in use" on restart.
- **`struct sockaddr_in`**: Used to specify IP/port for `bind()`.
- **Blocking vs non-blocking I/O**: Start blocking; introduce `select()`/`poll()` later for multiple clients.
- **HTTP basics**: Request line (`GET / HTTP/1.1`), headers, blank line, body — all over plain TCP.

## Editor

User uses `nvim` with minimal extensions — avoid suggesting IDE-specific workflows.
