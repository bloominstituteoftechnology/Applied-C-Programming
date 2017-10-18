/*
** server.c -- a stream socket server demo
*/

/* C libraries <> */
#include <stdio.h>      // core input and output
#include <stdlib.h>     // numeric conversion functions, pseudo-random numbers generation functions, memory allocation, process control functions
#include <unistd.h>     // close() http://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html
#include <errno.h>      // For testing error codes reported by library functions.
#include <string.h>     // string handling functions
#include <sys/types.h>  // http://pubs.opengroup.org/onlinepubs/7908799/xsh/systypes.h.html
#include <sys/socket.h> // socklen_t http://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html
#include <netinet/in.h> // http://pubs.opengroup.org/onlinepubs/7908799/xns/netinetin.h.html
#include <netdb.h>      // http://pubs.opengroup.org/onlinepubs/7908799/xns/netdb.h.html
#include <arpa/inet.h>  // makes stuff available from netinet/in.h http://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html
#include <sys/wait.h>   // defines symbolic constants for use with waitpid(): http://pubs.opengroup.org/onlinepubs/7908799/xsh/syswait.h.html
#include <signal.h>     // signal handling functions, expanded types: http://pubs.opengroup.org/onlinepubs/7908799/xsh/signal.h.html

// #include "index.html" // HTML stuff?

/* Macros */
#define PORT "7080"  // the port users will be connecting to

#define GET_ROOT "GET / HTTP/1.1"
#define ROOT_INDEX_HTTP_HTML "HTTP/1.1 200 OK\n\n<html><head></head><body>Hello World!</body></html>"
#define ROOT_INFO "HTTP/1.1 200 OK\n\n{\"info\": {\"name\":\"Jake, Antonio & Patrick\", \"url_request\": \"/info\", \"last_message\": \":::undefined::: OR :::POST message:::\"}}"
#define POST_INFO_HEADER "HTTP/1.1 200 OK\n\n"
#define GET_INFO "GET /info HTTP/1.1"
#define POST_INFO "POST /info HTTP/1.1"
#define DATE "Date:"
#define SERVER "Server:"
#define CONTENT_LENGTH "Content-Length:"
#define CONNECTION "Connection: close"
#define CONTENT_TYPE "Content-Type: text/html"

#define BACKLOG 10   // how many pending connections queue will hold

/* https://www.gnu.org/software/libc/manual/html_node/Process-Completion.html
 * WNOHANG - This flag specifies that waitpid should return immediately instead of waiting, if there is no child process ready to be noticed.
 * deals with signals from child processes?
 */
void sigchld_handler(int s)
{
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while(waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}

// I'm happy with this explanation, discerns IPv4 and 6
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


/* Whoa. As I understand, the code prior to the while loop
 * is setting up the server components
 * and once the while loop engages, that's where the server is listening
 */

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

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
        sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo); // all done with this structure

  if (p == NULL)  {
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }

  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("server: waiting for connections...\n");

  while(1) {  // main accept() loop
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }

    inet_ntop(their_addr.ss_family,
      get_in_addr((struct sockaddr *)&their_addr),
      s, sizeof s);
    printf("server: got connection from %s\n", s);

    if (!fork()) { // this is the child process
      // LS: read from client input
      const int READ_BUFFER_SIZE = 1024;
      char buffer[READ_BUFFER_SIZE];
      int read_result = read(new_fd, &buffer, READ_BUFFER_SIZE);
      printf("read_result: %d\n", read_result);
      printf("buffer: %s\n", buffer);

      // LS: loop above until \n\n is sent, signaling the end of an HTTP request
      /* substring - looks for a string within a string
        or char8 within a char*
        split a string
        HTTP protocol
        deserialize the string from and putting it in a serial fomat
        based on what is sent in, determine the kind of response (GET, POST /info, GET /info)
        "split" baed on \n

        measure the data, parse it out, to discern /GET or /info GET or POST
        https://linux.die.net/man/3/strstr
      */
      int option;
      /* GET */
      if (strstr(buffer, GET_INFO)) {
        option = 2;
        puts("Found GET /info\n");
      } else if (strstr(buffer, GET_ROOT)) {
        option = 1;
        puts("Found GET /\n");
      } else if (strstr(buffer, "GET /favicon.ico HTTP/1.1")) {
        puts("wOt?!?!");
        option = 5;
      }

      /* POST */
      // char searchPOST[5] = {'P', 'O', 'S', 'T', '\0'};
      // if (strstr(buffer, searchPOST)) {
      //   option = 3;
      //   puts("Found POST /, what about posting to /info???\n");
      // } else

      if (strstr(buffer, POST_INFO)) {
        option = 3;
        puts("Found POST /info\n");
      }

      // LS: parse the input and determine what result to send
      close(sockfd); // child doesn't need the listener

      // if option = 1; or SWITCH
      char postmanHEADER[1024] = {'H', 'T', 'T', 'P', '/', '1', '.', '1', ' ', '2', '0', '0', ' ', 'O', 'K', '\n', '\n', '\0'};
      // TODO: ^^^^^^ ACTUALLY PARSE OUT BODY AND SEND IT OR WRITE IT TO A FILE; MAYBE JUST SEND A CONFIRMATION OF RECEIPT?
      // printf("Wait a minute Mr. Postman: %s", postmanHEADER);
      // printf("the buff'n'wax: %s", buffer);
      // strcat(postmanHEADER, buffer);
      // printf("stir Kitten? %s", postmanHEADER);
      switch(option) {
      case 1:
        if (send(new_fd, ROOT_INDEX_HTTP_HTML, strlen(ROOT_INDEX_HTTP_HTML), 0) == -1)
          perror("send");
        break;
      case 2:
        if (send(new_fd, ROOT_INFO, strlen(ROOT_INFO), 0) == -1)
          perror("send");
        break;
      // case 3:
      //   if (send(new_fd, "HTTP/1.1 200 OK\n\n<html><head></head><body>The PUT msg</body></html>", 69, 0) == -1)
      //     perror("send");
      //   break;
      case 3:
        // check buffer for /r/n/r/n and grab string after that
        if (send(new_fd, postmanHEADER, strlen(postmanHEADER), 0) == -1)
          perror("send");
        printf("What's in the buffer: %s\n", buffer);
        break;
      // case 4:
      //   if (send(new_fd, "HTTP/1.1 200 OK\n\n<html><head></head><body>Hello World!</body></html>", 69, 0) == -1)
      //     perror("send");
      //   break;
      // case 5:
      //   send binary data
      //   break;
      default:
        break;
      }
      // LS: Send the correct response in JSON format
      // if (send(new_fd, "Hello, world!", 13, 0) == -1)
      // if (send(new_fd, "HTTP/1.1 200 OK\n\n<html><head></head><body>Hello World!</body></html>", 69, 0) == -1)
      //   perror("send");
      close(new_fd);
      exit(0);
    }
    close(new_fd);  // parent doesn't need this
  }

  return 0;
}
