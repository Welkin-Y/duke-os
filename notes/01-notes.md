# Intro

tty: Telescope TYpewriter

## OS Platform model

- OS - (API) - Libraries/Frameworks - (API) - Applications/Services

- Isolated environment for programs

### Processes and virtualization

- process: running program instance
- process has at least one thread to run code
- each process has a private VAS
- OS controls everything, creates/destroys processes

### VAS (Virtual Address Space)

- early DOS does not have VAS -> vulnerable to attacks

## Virtual machines

- Run a hypervisor on the bare metal physical machine
  - introduce a new trusted layer

## p-p-paradigm

- Monitor/controller - machine
  - monitor configure/launch machine
  - machine exception notify monitor

## OS rols

- serving requests (e.g. I/O)
- directing traffic (e.g. manage resources)
- guard private property (VAS)

## Memory/storage is an abstraction

- Storage: an array of locations

## Code

- Code has many representations
- each language has tools and steps to translate and combine
subprograms and prepare them for execution

### instructions in memory

- depending on the machine ISA
- an opcode and zero or more operands

### registers

- fast memory inside the CPU
- speed: access at CPU clock speed
- naming/layout/size is processor-specific

## Code stream

- cpu executes instruction in sequence
- stream runs with a stack
  - RAM region
  - SP (stack pointer) register: address of stack top

## Threads and contexts

- contexts are data objects
- thread context switch:
  - take snapshot of current register values
  - save into memory
  - load register with another context
  - switch to another code stream

### 2 threads sharing a CPU core

- contexts switch between threads as if they are running in parallel

## modern computers are multi-core

- a core with R registers sets can run R threads in parallel
- core has R slots -> logical processors or hardware threads
- for simplicity, we assume R == 1, tread each slot as a separate core

## Concurrency

- interleaving matters
  - how to avoid racing

## Protection

- one bit in a protected register controls mode at which CPU is running
- Trusted OS code executes in kernel mode

### CPU mode

- kernel mode
  - control entire machine
- user mode

## the kernel

- a program in a file: the firmware loads it into memory and starts it (boot)
- once running, it inis the machine and starts launching user programs
- user code can invoke the kernel only at well-defined entry points (syscall)
- once booted, the kernel acts as one big event handler

### kernal mode

- What turns it on
  - Trap
  - Fault
  - Interrupt
  - Boot
- What turns it off
  - kernel code returns to user context

### entering kernel

- exception: trap, fault, interrupt
- kernel sets up vector tables for exception handler when booting

#### Trap

- application makes a system call
- actively call system call

#### Fault

- Current instruction causes a fault
- hardware cannot execute the instruction

#### Interrupt

- Interrupt pin goes high during execution of current instruction

