/* ** server.c -- a stream socket server demo */ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> // (http://pubs.opengroup.org/onlinepubs/009696699/basedefs/sys/socket.h.html)
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define PORT "7080"  // the port users will be connecting to

#define GET_ROOT "GET / HTTP/1.0"
#define GET_INFO "GET /info HTTP/1.0"
#define POST_INFO "POST /info HTTP/1.0"
#define DATE "Date:"
#define SERVER "Server:"
#define CONTENT_LENGTH "Content-Length:"
#define CONNECTION "Connection: close"
#define CONTENT_TYPE "Content-Type: text/html"

#define BACKLOG 10   // how many pending connections queue will hold

// takes in int s, not sure what it does with it or where it got it --> socket maybe???
// invokes waitpid which waits for state changes in the child of the calling process - terminated, stopped, resumed
// the if statement wants the waitpid to be child process ID equal to value of pid ( https://linux.die.net/man/2/waitpid)
// WNOHANG - returns immediately if no child existed
// don't know where we got errno either
void sigchld_handler(int s)
{
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while(waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}


// // takes in a pointer to the sockaddr struct and pending the data in it returns a reference to another ostruct
// casts the struct to the correct data structure after checking the sa_family
// sa_family represents the address family (https://www.tutorialspoint.com/unix_sockets/socket_structures.htm)
//https://stackoverflow.com/questions/18609397/whats-the-difference-between-sockaddr-sockaddr-in-and-sockaddr-in6/ get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
  int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  // Possible instantiation of hints which is the struct addrinfo, memset fills a block of memory that is the size of hints, and then fills the struct with variables specified in getaddrinfo() docs
  // hints arg points to addrinfo structure that specifies criteria for selecting socket addr structures (https://linux.die.net/man/3/getaddrinfo)
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;  // getaddrinfo() should return socket addresses for any address family (4, 6 etc)
  hints.ai_socktype = SOCK_STREAM; // preferred socket type
  hints.ai_flags = AI_PASSIVE; // use my IP

  // sets rv equal to the results of getaddrinfo
  // prints the results if there are any
  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and bind to the first we can --> assign local protocol address to socket
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    // If the socket options are not set, it will throw an error and exit
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
        sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    // the actual binding function
    // sockfd = socket descriptor returned by the socket function
    // if successful, returns 0
    // this conditional sends error message if bind fails
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo); // all done with this structure, just freeing the addrinfo struct

  // p was set to serverinfo
  // if no serverinfo, send error
  if (p == NULL)  {
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }

  // listen turns unconneccted socket into passive listening socket, and sets it up with a max number of connections the kernel can queue
  // returns 0 on success, -1 on failure
  // sending error message if failure
  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  // empty the memory for the signal set pointed to by set
  // returns 0 on success, -1 on failure
  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("server: waiting for connections...\n"); // indicator that socket is successfully listening

  while(1) {  // main accept() loop
    sin_size = sizeof their_addr;
    // accepting the connection by making a new socket that is not listening but did not affect the original socket
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }

    // converts numeric address into a text string suitable for presentation
    inet_ntop(their_addr.ss_family,
      get_in_addr((struct sockaddr *)&their_addr),
      s, sizeof s);
    printf("server: got connection from %s\n", s);

    if (!fork()) { // this is the child process
      // LS: read from client input
      // in the child process, reads the incoming info
      // read_result is the first integer of the buffer string
      const int READ_BUFFER_SIZE = 1024;
      char buffer[READ_BUFFER_SIZE]; // set up array for a string
      int read_result = read(new_fd, &buffer, READ_BUFFER_SIZE); // number of bytes actually read and placed in buffer (https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.2.0/com.ibm.zos.v2r2.bpxbd00/rtrea.htm)
      printf("read_result: %d\n", read_result);
      printf("buffer: %s\n", buffer);
// **********************************************************************************************
// The part I am changing - for reading
// **********************************************************************************************
      char postInfo[7] = {'P', 'O', 'S', 'T', ' ', '/', };
      char getInfo[10] = {'G', 'E', 'T', ' ', '/', 'i', 'n', 'f', 'o', '\0'};
      char getSlash[6] = {'G', 'E', 'T', ' ', '/', '\0'};
      // char line1[16] = {"HTTP/1.0 200 OK\n"};
      // char line2[6] = {"Date: "}; // find a way to stick a date in there
      time_t date = time(NULL); // c string with \n at the end
      // char newLine[1] = {"\n"};
      // char line3[14] = {"Server: Sarah\n"};
      // char line4[16] = {"Content-Length: "}; // add the read_result
      // char line5[18] = {"Connection: close\n"};
      // char line6[24] = {"Content-Type: text/html\n"};
      char getSlashResponse[64] = {"<html><head></head><body>Welcome to Sarah's server</body></html>"};
      // char getInfoResponse[] = {"info": {"name: Sarah", "url_request": "/info", "last_message": };
      char noPrevPost[9] = {"undefined"};
      // char postInfoResponse[] = ; // put in the message they sent
      // char* thisResponse = calloc(2048 * sizeof(char));
      char buff[2048];
      int len;
      char lastMessage[1025];
      int lastMessageLen;
      
      if (strstr(buffer, getInfo) && lastMessageLen == 0)
      {
        len = snprintf(buff, 2048, "<html><head>HTTP/1.0 OK\nDate: %dServer: Sarah\nContent-Length: %d\nConnection: Close\nContent-Type: text\html\n<head><body>{\"info\": {\"name\": \"Sarah\", \"url_request\": \"/info\", \"last_message\": \"undefined\"}}</body></html>",date, read_result);
        printf("\nGet info, no previous message\n");
      }
      else if (strstr(buffer, getInfo) && lastMessageLen > 0)
      {
        len = snprintf(buff, 2048, "HTTP/1.0 OK\nDate: %dServer: Sarah\nContent-Length: %d\nConnection: Close\nContent-Type: text\html\n<html><head></head><body>{\"info\": {\"name\": \"Sarah\", \"url_request\": \"/info\", \"last_message\": \"%s\"}}</body></html>",date, read_result, lastMessage);
      }
      else if (strstr(buffer, getSlash))
      {
        len = snprintf(buff, 2048, "<html><head>HTTP/1.0 OK\nDate: %dServer: Sarah\nContent-Length: %d\nConnection: Close\nContent-Type: text/html\n</head><body>Welcome To Sarah's Server</body></html>\n\n",date, read_result);
        printf("\nFound\n");
      }
      // else if (strstr(buffer, postInfo))
      // {
      //   lastMessageLen = sprintf(lastMessage, // all the things in the message section);
      //   len = snprintf(buff, 2048, "HTTP/1.0 OK\nDate: %dServer: Sarah\nContent-Length: %d\nConnection: Close\nContent-Type: text\html\n<html><head></head><body>{"info": {"name": "Sarah", "url_request": "/info", "last_message": "%s"}}</body></html>",date, read_result, lastMessage);
      // }

// **********************************************************************************************
      // LS: loop above until \n\n is sent, signaling the end of an HTTP request

      // LS: parse the input and determine what result to send
      close(sockfd); // child doesn't need the listener
      // LS: Send the correct response in JSON format
      // use the send function to send the response
      // if (send(new_fd, "Hello, world!", 13, 0) == -1)
// **********************************************************************************************
// The part I am changing - for sending 
// **********************************************************************************************
      // if (send(new_fd, "HTTP/1.0 200 OK\n\n<html></html>", 31, 0) == -1) // thanks Patrick!
      if (send(new_fd, buff, len, 0) == -1)
// **********************************************************************************************
        perror("send");
      close(new_fd);
      exit(0);
    }
    close(new_fd);  // parent doesn't need this
  }

  return 0;
}

void clear(int number)
{
  for (int i = 0; i < number; ++i)
  {
    
  }
}
