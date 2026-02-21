> God bless I don't have to write LLDB myself...

# qbdebug

`qbdebug` is a simple Linux debugger written in C utilizing `ptrace` API. The sole idea of this project is to create a CLI debugger as a way of learning about debuggers, processors, signals, memory, breakpoints and process introspection and debugger.

## Architecture of the debugger

- After issuing `run` command, the debugger uses `fork()` to spawn a child process.
- Child process calls `ptrace(PTRACE_TRACEME)` to allow the parent (debugger) process to trace its execution

# Features

- [ ] Spawning and attaching to a child processes
- [ ] Support for multiple commands with abbreviations
- [ ] Setting software breakpoints
- [ ] Reading and writing CPU registers
- [ ] Handling multithreaded processes

# Prerequisites

- Linux (requires `sys/ptrace.h` and ELF binaries)
- `gcc` or `clang` compiler
- `cmake` (3.13+)

# Build

To build the project you can use the `Makefile` provided in the root of the directory to build the debugger

```bash
make
```

Alternatively you can build it manually directly with `cmake`

```bash
cmake -S . -B build
cmake --build build
```

# Usage

To run the debugger run the compiled binary in `/build` directory of the project

```bash
./build/qbdbg
```

Once inside the debugger you should see the debugger CLI interface starting with `qbdbg>` marker before entering commands.

## List of commands

The debugger supports basic commands

- `run <path_to_binary>` - Launches a new process to be debugged (does not handle passing arguments yet)
- `q, quit, exit` - Exits the debugger 

# Roadmap

- [ ] Improve the architecture to support stateless command handling
- [ ] `--help` flag
- [ ] `b, bp, break, breakpoint <address/function_name>` - Sets a breakpoint on an address or a function
- [ ] `c, continue` - Continues the control flow of the debugged process. If breakpoints are set it pauses on the first breakpoint hit stopping the execution of the program.
- [ ] `si, step_into` - Steps into a function
- [ ] `so, step_out` - Steps out of current scope
- [ ] `sov, step_over` - Steps over current scope
- [ ] `read <register/address>` - Read  

