# C Programming: Level Two

# Fundamentals of C

- Programming details
- Building and contributing to large projects
- Interprocess communication

# Programming details

    * const int five = 5;
    * const struct myStruct;
    * const int* six = 6;
    * const int* const six = 6;
    * int* const six = 6;

    * reading stdin and files
    #include <stdio.h>
    #include <string.h>
    int main(int argc, char** argv) {
      char input[128];
      int returnVal;
      while((returnVal = scanf(stdin, "%s", &input)) != EOF) {
        printf("%s",input);
        printf("%d",strlen(input));
        char c = getc(stdin);
        if(strcmp(input,"exit") == 0) {
          printf("exit time! %d\n",returnVal);
          exit(0);
        }
      }
    }

    * meta programming (#ifdef)
    #ifndef __THIS_HEADER_H
    #define __THIS_HEADER_H
    #endif __THIS_HEADER_H

    More often:
    #ifdef __DARWIN__
     // custom DARWIN code
    #elseif __LINUX__
     // custom LINUX code
    #elseif __WIN__
     // custom WINDOWS code
    #endif 

    or:
    #ifdef _SPECIAL_LIBRARY_FLAG_
     // load code made available by a loaded library
    #endif


    * declaration
    * definition
    * initialization
    int x; //declaration
    x = 6; // initialization (if not done previously)
    int y = 5; // declaration and definition
    void function(); // declaration (in header)
    void function() { // definition (in header or source)
    }

    * faults - SIGSEGV, SIG, et
    Delivered to your app as signals and can be overridden
    Extra credit - use the included sigusr.c or sigint.c code
    to prevent your application from crashing when it accesses 
    illegal memory

    * file scope
    * static
    * extern
    Variables that are declared inside of your .c file are limited to that
    unless you "hoist them" into global scope with the static keyword. Other
    files can now access them with the extern keyword. Avoid global scope.
    int x = 5; // file scope
    static int y = 6; // global scope
    extern int z; // global scope variable defined elsewhere
    int main() {...}

    * includes
    * libraries
    * objects
    * files within directories, linked and built
    * linking
    * compilation arguments
    * compiler optimizations
    * make and CMake
see [Building External Libraries](#imagemagick)

    * bit shifting >> <<
    * binary operators &, |, ~
    Binary operations are amazingly powerful! This is the only place you
    can directly manipulate wire values from a programming language other than
    ASM.
See bits.c and [Bitwise operations in C](https://en.wikipedia.org/wiki/Bitwise_operations_in_C#Right_shift_operator_usage)

    * function pointers
    int deliver_fruit(char *address, float speed)
    {
      printf("Delivering fruit to %s at speed %.2f\n", address, speed);
      return 3490;
    }
    int explode_fruit(char* address, float velocity)
    {
      printf("Detonating all fruit at %s with fragment velocity %.2fm/s\n", address, speed);
      return (int)velocity;
    }
    int main(void)
    {
      int (*p)(char*,float);  // declare a function pointer variable
      p = deliver_fruit; // p now points to the deliver_fruit() function
      deliver_fruit("My house", 5280.0); // a normal call
      p("My house", 5280.0); // the same call, but using the pointer
      return 0;
    }

# Building libraries

## ImageMagick

    https://github.com/ImageMagick/ImageMagick

ImageMagick is configured using `make` via a `Makefile`. This is historically the most popular way to build C programs, but large Makefiles get unbearably contorted, so many people have moved toward `CMake`.

    ./configure
    make
    make install

## curl

    https://github.com/curl/curl

Curl is configured using `CMake`, the most popular alternative to `make`.

    cmake ./

## Building

Whenever you install an application on your computer, somebody had to build a binary for that application and distribute it to you. When you install something from the AppStore, that app was built on a machine owned by the application writer, codesigned according to Apple's requirements, and uploaded to the store.

When you download a .dmg file, or a .zip file, those files were also built by someone at the vendor company using a similar method.

Open source software is quite different - as long as all of the components of an open source project are freely available, you can build the project on your own machine, making .zip or .dmg or AppStores unnecessary. `apt-get` and `brew install` use a combination of pre-built binaries and building automatically for you from source. The purpose of all these tools is: dependency management.

## Dependency management

When we build an application from source, we have to ensure that all of its dependencies are available to the build script. This can be hard.

# Interprocess Communication (IPC)

Pipes, Streams, Sockets, Signals, Shmem, and Synchronization

Now you know how to execute a process and `fork` it, or to `exec` an all new process directly within your C program. Now you have two programs running simultaneously.  By scheduling processes with the operating system, it is possible to gain the "appearance of simultaneous execution". Not only can we have simultaneous execution, but we can use different processes to handle different jobs.

This serves a multitude of purposes: You can create library processes (of which the operating system is built from) that can be used by other applications to accomplish specific tasks. You can link two processes together in order to combine their results intelligently. Most importantly, you can take advantage of the automatic process and core scheduling routines of the operating system to gain true simultaneous operation, by executing different processes on separate cores.

This is real asynchronous execution, commonly called threading, and it is fraught with dangers.

# Mini sprint

Run

```
    make
```

in this repository and play with all of the compiled c programs. They are explained in part below.

# Synchronization

When two processes attempt to use the same piece of data or communicate at the same time period, there is a high probability that the operating system will interrupt one mid-stride, messing up its expected inputs or outputs.

[Synchronization](https://en.wikipedia.org/wiki/Synchronization_(computer_science))

[Dining Philosophers](https://en.wikipedia.org/wiki/Dining_philosophers_problem)

[Deadlock](https://en.wikipedia.org/wiki/Deadlock)

[Race Condition](https://en.wikipedia.org/wiki/Race_condition)

Semaphors and mutexes allow processes to place a "lock", which then control the behavior of other processes that observe that lock. Fraught with difficulty, guaranteed to cause your computer to freeze. Stick with Node.js.

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

## Pipes and streams

Pipes enable a one-way communication between two processes.

    pipe1.c
    pipe2.c
    pipe3.c

### named pipe (FIFO) file
### Pipes as file descriptors, connecting processes

Named pipes allow two or more processes to share messages.

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

## Semaphores

Same as file locks, except managed directly by the operating system without an intermediate file.

## Sockets

socketpair - Unix Domainm Socket (SOL_STREAM, SOCK_DGRAM, SOCK_SEQPACKET)

## Signals

Integer values and system calls

## Shmem

shmget - two processes can have access to the same memory

## Memory Mapped Files

Map a file directly to memory for easy read/write

#  Assignment

Your objective is to refactor the following simple UNIX net sockets server `server.c` to respond to a few specific requests, including storing a message sent by a previous uploader.

The HTTP protocol is a simple text protocol that is parsed line by line by C servers around the world. It is much easier to write an HTTP server using Node.js, and much more informative to create one from scratch with C. For our benefit we can use the example socket server code to protect us from the primarily hassle of writing a simple C server. Real HTTP servers need to be fault tolerant (able to return an ISE 500 when something goes wrong), multi threaded, and efficient.

A simple HTTP request to / works as follows at the protocol level:
    
```
    GET / HTTP/1.1
    Host: localhost:7080
    Connection: keep-alive
    Cache-Control: max-age=0
    Upgrade-Insecure-Requests: 1
    User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36
    Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8
    Accept-Encoding: gzip, deflate, br
    Accept-Language: en-US,en;q=0.8
    Cookie: _ga=GA1.1.1267589271.1490801020; intercom-id-vaspqzqc=b356e5a1-3e1a-4d0d-9efa-0e07e98df7c4; olfsk=olfsk5267054354755929; hblid=jH6UDoDmWj5Pvq6g3m39N0V8REA6rT2B
```

^^^ Generated via Chrome ^^^

The top portion is called the header, and the lower portion is the body. They are always separated by a single line of whitespace. A GET request can contain a body, but generally does not, and is not required to in this assignment. Chrome sends complicated and long headers in a basic request, which you are not required to support.

![Chrome doesn't like it](https://www.dropbox.com/s/ijw44ez3l9h1ioa/Screenshot%202017-10-12%2011.14.26.png?raw=1)

Your goal is to make Chrome happy when you return /, by including the proper headers and BODY.

A simple HTTP POST looks like this:

```
    POST /info HTTP/1.1
    User-Agent: curl/7.27.0
    Host: localhost:7080
    Accept: */*
    Content-Length: 10
    Content-Type: application/x-www-form-urlencoded

    my message
```


Curl generates the simple request above when trying to save "my message". Your app must handle this request, and save the result.

### Starter application and basic steps

    assignment/server.c

1. Read this file and write comments before each block explaining what you think its purpose is and what it does. After doing this, create a summary file server_summary.md that explains the operation of the file based on your analysis.

2. Modify this file to become a simple HTTP server. It should accept `/` and `/info` requests.

  * Your server must support the following requests:

```
    GET / HTTP/1.0
    GET /info HTTP/1.0
    POST /info HTTP/1.0
```

  * Your server must support the following headers:

```
    Content-Length:
    Host:
    User-Agent:
```

3. Your server response must match the HTTP response header protocol:

```
    HTTP/1.0 200 OK
    Date: xxxx (formatted time or Unix time)
    Server: Name of Student
    Content-Length: yy
    Connection: close
    Content-Type: text/html
```

  * Your server must return the following results:

```
    GET /
    <html><head></head><body>Welcome to :::student name:::</body></html>

    GET /info
    {"info": {"name":":::student name:::", "url_request": "/info", "last_message": ":::undefined::: OR :::POST message:::"}}

    POST /info

    ::: any message less than 1024 bytes :::
```

Parameters

```
    Server/:::student name::: - stored on your local environment (ENV) as 
    STUDENT_NAME and read at run time by your server with the `getenv` 
    function call.

    Date - calculated at the time of request using the `time()` function and the
    `time_t` structure loaded from `#include <time.h>`

    Content-Length - The length of the BODY of the message that is sent.

    message - Either return "undefined" if no message has been sent, or return
    the message that was uploaded via the POST /info request previously.
```

# Resources

http://beej.us/guide/bgipc/output/html/singlepage/bgipc.html
