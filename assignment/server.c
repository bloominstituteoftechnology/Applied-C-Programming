/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

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

void sigchld_handler(int s)
{
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while(waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
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

      // LS: parse the input and determine what result to send
      close(sockfd); // child doesn't need the listener
      // LS: Send the correct response in JSON format
      if (send(new_fd, "Hello, world!", 13, 0) == -1)
        perror("send");
      close(new_fd);
      exit(0);
    }
    close(new_fd);  // parent doesn't need this
  }

  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "lastmessage.h"

#define PORT "7080"  // the port users will be connecting to

#define GET_ROOT "GET / HTTP/1.1"
#define GET_INFO "GET /info HTTP/1.1"
#define POST_INFO "POST /info HTTP/1.1"
#define DATE "Date:"
#define SERVER "Server:"
#define CONTENT_LENGTH "Content-Length:"
#define CONNECTION "Connection: close"
#define CONTENT_TYPE "Content-Type: text/html"

#define BACKLOG 10   // how many pending connections queue will hold

void sigchld_handler()
{
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while(waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/**
 * Set up the listening socket
 */
int setup_socket(void) {
  int sockfd;  // listen on sock_fd
  struct addrinfo hints, *servinfo, *p;
  struct sigaction sa;
  int yes=1;
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

  return sockfd;
}

/**
 * Create 404 response
 */
int create_404_resp(char *result_buffer, char *headers)
{
  char content[8192];

  sprintf(content, "<h1>404: not found</h1>");

  int content_length = strlen(content);

  int num_bytes = sprintf(result_buffer, "HTTP/1.1 404 Not Found\n"
    "%s"
    "Content-Length: %d\n"
    "Content-Type: text/html\n"
    "\n"
    "%s", headers, content_length, content);

  return num_bytes;
}

/**
 * Create response for GET /
 */
int create_root_response(char *result_buffer, char *headers, char *student_name)
{
  char content[8192];

  sprintf(content, "<html><head></head><body>Welcome to %s</body></html>", student_name);

  int content_length = strlen(content);

  int num_bytes = sprintf(result_buffer, "HTTP/1.1 200 OK\n"
    "%s"
    "Content-Length: %d\n"
    "Connection: close\n"
    "Content-Type: text/html\n"
    "\n"
    "%s", headers, content_length, content);

  return num_bytes;
}

/**
 * Create info get response
 */
int create_info_get_resp(char *result_buffer, char *headers, char *student_name)
{
  printf("Detected GET /info\n");

  char message_buffer[8192];
  char *last_message = load_last_message(message_buffer, 8192);

  char content[8192];

  sprintf(content, "{"
    "\"info\": {\"name\": \"%s\", "
    "\"url_request\": \"/info\", "
    "\"last_message\": \"%s\"}}",
      student_name,
      last_message == NULL? "undefined": last_message);

  int content_length = strlen(content);

  int num_bytes = sprintf(result_buffer, "HTTP/1.1 200 OK\n"
    "%s"
    "Content-Length: %d\n"
    "Content-Type: application/json\n"
    "\n"
    "%s", headers, content_length, content);

  return num_bytes;
}

/**
 * Create info post reponse
 */
int create_info_post_resp(char *req_buffer, char *result_buffer, char *headers)
{
  int num_bytes;

  printf("Detected POST /info\n");

  char *data_start = strstr(req_buffer, "\r\n\r\n");

  if (data_start == NULL) {
    printf(">> Couldn't find end of header\n");
  } else {
    // Jump past the header
    data_start += 4;
    printf("Body is: %s\n", data_start);
    save_last_message(data_start);

    char content[8192];

    sprintf(content, "{\"status\": \"ok\"}");
    int content_length = strlen(content);

    num_bytes = sprintf(result_buffer, "HTTP/1.1 200 OK\n"
      "%s"
      "Content-Length: %d\n"
      "Content-Type: application/json\n"
      "\n"
      "%s", headers, content_length, content);
  }

  return num_bytes;
}

/**
 * Main
 */
int main(void)
{
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  char s[INET6_ADDRSTRLEN];
  int sockfd, new_fd;

  char headers[8192];

  
  char *student_name = getenv("STUDENT_NAME");
  printf("Student name: %s\n", student_name);

  // Get a listening socket
  sockfd = setup_socket();

  printf("server: waiting for connections...\n");

  while(1) {  // main accept() loop
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }

    // Compute time
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char time_buffer[80];
    strftime(time_buffer, 80, "%Y-%m-%d %H:%M:%S", local);

    // Create common header field
    sprintf(headers,
      "Date: %s\n"
      "Server: %s\n"
      "Connection: close\n",
      time_buffer, student_name);
      
    // Print out connector's address
    inet_ntop(their_addr.ss_family,
      get_in_addr((struct sockaddr *)&their_addr),
      s, sizeof s);
    printf("server: got connection from %s\n", s);

    if (fork() == 0) { // this is the child process
      // LS: read from client input
      const int READ_BUFFER_SIZE = 1024;
      char buffer[READ_BUFFER_SIZE+1];
      int read_result = read(new_fd, &buffer, READ_BUFFER_SIZE);

      const int WRITE_BUFFER_SIZE = 8192;
      char result_buffer[WRITE_BUFFER_SIZE];
      int num_bytes;

      printf("read_result: %d\n", read_result);
      buffer[read_result] = '\0';
      printf("%s\n", buffer);

      if (memcmp(POST_INFO, buffer, strlen(POST_INFO)) == 0) {
        num_bytes = create_info_post_resp(buffer, result_buffer, headers);
      }

      else if (memcmp(GET_INFO, buffer, strlen(GET_INFO)) == 0) {
        num_bytes = create_info_get_resp(result_buffer, headers, student_name);
      }

      else if (memcmp(GET_ROOT, buffer, strlen(GET_ROOT)) == 0) {
        num_bytes = create_root_response(result_buffer, headers, student_name);
      }

      else {
        num_bytes = create_404_resp(result_buffer, headers);
      }

      close(sockfd); 

      if (send(new_fd, result_buffer, num_bytes, 0) == -1)
        perror("send");

      close(new_fd); 
      exit(0);
    }
    close(new_fd); 
  }

  return 0;
}
