CC=gcc

all: fork1 sigusr sigint pipe1 pipe2 pipe3 speak tick lockdemo kirk spock semrm semdemo shmdemo mmapdemo echos echoc spair bits showip read_from_stdin pointer_crazy
.PHONY : all

clean:
	rm fork1 sigusr sigint pipe1 pipe2 pipe3 speak tick lockdemo kirk spock semrm semdemo shmdemo mmapdemo echos echoc spair bits showip read_from_stdin pointer_crazy
.PHONY : clean

# fork and exec (remember me?)
fork1:
	$(CC) fork1.c -o fork1

# Signals
sigusr:
	$(CC) sigusr.c -o sigusr
sigint:
	$(CC) sigint.c -o sigint

# One way pipes
pipe1:
	$(CC) pipe1.c -o pipe1

pipe2:
	$(CC) pipe2.c -o pipe2

pipe3:
	$(CC) pipe3.c -o pipe3

# Two way pipes - named pipes
speak:
	$(CC) speak.c -o speak

tick: speak
	$(CC) tick.c -o tick

# File locks for asynchronous comms
lockdemo:
	$(CC) lockdemo.c -o lockdemo

# Message queues
kirk:
	$(CC) kirk.c -o kirk

spock: kirk
	$(CC) spock.c -o spock

# Semaphores
semrm:
	$(CC) semrm.c -o semrm
semdemo: semrm
	$(CC) semdemo.c -o semdemo

# Shared memory
shmdemo:
	$(CC) shmdemo.c -o shmdemo

# Memory mapped files
mmapdemo:
	$(CC) mmapdemo.c -o mmapdemo

# Unix Sockets
# Echo server
echos:
	$(CC) echos.c -o echos
echoc: echos
	$(CC) echoc.c -o echoc
# two directional communication
# Update spair to be a unix level chat room
spair:
	$(CC) spair.c -o spair

bits:
	$(CC) bits.c -o bits

showip:
	$(CC) showip.c -o showip

read_from_stdin:
	$(CC) read_from_stdin.c -o read_from_stdin

pointer_crazy:
	$(CC) pointer_crazy.c -o pointer_crazy
