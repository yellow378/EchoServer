# AGENTS.md

This document provides guidelines for AI agents working on the EchoServer C project.

## Project Overview

EchoServer is a learning project implementing a TCP echo server using POSIX socket APIs with multi-process architecture (forking per connection). The server listens on port 6789 and echoes back any received data.

## Build Commands

```bash
# Build all targets
make

# Build specific target
make hello       # Builds hello.c (simple test program)
make tcp         # Builds tcp.c (main echo server)

# Run targets
make run         # Runs ./hello

# Clean build artifacts
make clean       # Removes *.o and binaries
```

**Single file compilation:**
```bash
gcc hello.c -o hello
gcc tcp.c -o tcp
```

**Compiler:** Uses GCC detected via `$(which gcc)`.

## Linting & Formatting

**Formatting with clang-format (LLVM configuration):**
```bash
# Format a single file
clang-format -i tcp.c

# Check formatting (dry-run)
clang-format --dry-run tcp.c

# Format all C files
clang-format -i *.c *.h
```

**Key formatting rules:**
- Column limit: 80 characters
- Indent width: 4 spaces, Tab width: 8 spaces
- Use spaces, never tabs
- Braces attach (K&R style): `if (cond) {`
- Pointer alignment: Right (`char *ptr`)
- Max empty lines: 1
- `BreakBeforeBraces: Attach`

## Code Style Guidelines

### Naming Conventions

- **Functions:** `snake_case` (e.g., `create_server`, `handle_client`)
- **Variables:** `snake_case` (e.g., `client_socket`, `recv_size`)
- **Constants:** `UPPER_SNAKE_CASE` (e.g., `BUFFER_SIZE`, `DEFAULT_PORT`)
- **Structs:** Use standard `struct` types (e.g., `struct sockaddr_in`)
- **Macros:** `UPPER_SNAKE_CASE`

### Headers & Imports

- System headers use `< >` (e.g., `<stdio.h>`, `<sys/socket.h>`)
- Local headers use `" "` (e.g., `"tcp.h"`)
- Include order: system headers first, then local headers
- Required headers: `<stdio.h>`, `<string.h>`, `<unistd.h>`, `<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`, `<sys/wait.h>`
- Header guards required in all `.h` files

### Types

- `int` for file descriptors (sockets)
- `size_t` for sizes, `ssize_t` for signed sizes (recv/send return values)
- `socklen_t` for socket address lengths
- `struct sockaddr_in` for IPv4 addresses

### Error Handling

- Check all system calls that can fail: `socket()`, `bind()`, `listen()`, `accept()`, `recv()`, `send()`, `fork()`
- Return `-1` on error, valid fd/0 on success where appropriate
- Print errors with `perror()` or `printf()` before returning
- Use `MSG_NOSIGNAL` with `send()`/`recv()` to prevent SIGPIPE
- Handle partial reads/writes with loops

### Control Flow

- Use braces for ALL blocks, even single statements
- Use `if (condition) { ... } else { ... }` style
- Prefer explicit comparisons (`==`, `!=`) over implicit truthiness
- Avoid nested deep conditionals; use early returns

### Buffer Safety

- Define buffers with explicit sizes: `char buffer[101]` (not magic numbers)
- Account for null terminator when sizing: `recv(buffer, 100, ...)` + room for `\0`
- Never write: `buffer[recv_size] = '\0'` without bounds checking

### Multi-Process Pattern

```c
int pid = fork();
if (pid == 0) {
    // Child: close server, handle client
    close(server);
    // ... client handling ...
    return 0;
} else {
    // Parent: close client, reap children
    close(client);
    while (waitpid(-1, NULL, WNOHANG) > 0) { }
}
```

## Project Structure

- `*.c` files contain implementations
- `*.h` files contain declarations
- Functions should be declared before use or in header files
- Header guards required in all `.h` files

## Testing

Manual testing only:

```bash
# Build
make tcp

# Run server
./tcp

# Connect with netcat
nc localhost 6789
```

## Key Files

- `tcp.c` - Main echo server implementation (115 lines)
- `tcp.h` - Header with required includes
- `hello.c` - Simple test program
- `Makefile` - Build configuration
- `.clang-format` - Code formatting rules (LLVM-based)
- `TODO.md` - Task tracking

## Important TODOs

1. **[ ] Buffer overflow risk** - `tcp.c:94-95`: `recv_size` can be 100, but `buffer[100] = '\0'` overflows. Fix: use `char buffer[101]` or limit `recv()` to 99 bytes.
