# C Programming: Level Two




# Synchronization

Now you know how to launch process in C. By scheduling processes with the operating system, it is possible to gain the "appearance of simultaneous execution". Not only can we have simultaneous execution, but we can use different processes to handle different jobs.

semaphors and mutexes allow processes to place a "lock", which then control the behavior of other processes that observe that lock. Fraught with difficulty, guaranteed to cause your computer to freeze. Stick with Node.js.

`ipcs` command shows what OS level IPC resources are available. Lets use it:
    MacBook-Pro-thomcom-2:System-Calls thomcom$ ipcs
    IPC status from <running system> as of Tue Oct 10 17:11:59 MDT 2017
    T     ID     KEY        MODE       OWNER    GROUP
    Message Queues:
    q 131072 0x6d04e882 --rw-rw----  thomcom    staff
    q  65537 0x42048c35 --rw-r--r--  thomcom    staff

    T     ID     KEY        MODE       OWNER    GROUP
    Shared Memory:
    m 131073 0x5304e882 --rw-rw-rw-  thomcom    staff

    T     ID     KEY        MODE       OWNER    GROUP
    Semaphores:

Destroying these resources may be a command we can call from `bash`, or they might only be destroyable from with a C program.


# Interprocess Communication (IPC)

Pipes, Streams, Sockets, Signals, Shmem, and Synchronization

## Pipes and streams

    pipe1.c
    pipe2.c
    pipe3.c

### named pipe (FIFO) file
### Pipes as file descriptors, connecting processes

    speak.c
    tick.c

Prevent speak from blocking on the open call with `O_NDELAY`, and the problems it causes.

    fd = open(FIFO_NAME, O_RDONLY | O_NDELAY);

Atomicity
Chunking and interleaved data

"Named pipes" - a specific file that any process can asynchronously put data int
mknod("/tmp/MYFIFO", S_IFIFO|0666, 0);

## File locks

For concurrency and asynchronous safety

## Sockets

socketpair - Unix Domainm Socket (SOL_STREAM, SOCK_DGRAM, SOCK_SEQPACKET)

## Signals

Integer values and system calls

## Shmem

shmget - two processes can have access to the same memory

## Memory Mapped Files



# Resources

http://beej.us/guide/bgipc/output/html/singlepage/bgipc.html
