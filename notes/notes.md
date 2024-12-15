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
