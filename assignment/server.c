/*
** server.c -- a stream socket server demo
** version 0.2_c
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
#include <sys/time.h>

#define PORT "7080"  // the port users will be connecting to

// some HTTP requests
#define GET_ROOT  "GET / HTTP/1.1"
#define GET_INFO  "GET /info HTTP/1.1"
#define POST_INFO "POST /info HTTP/1.1"
#define GET_FAVICON "GET /favicon.ico HTTP/1.1"

// HTTP response components
#define STATUS_OK      "200 OK"
#define RESPONSE_OK    "HTTP/1.1 "STATUS_OK"\r\n"
#define DATE           "Date: "
#define SERVER         "Server: "
#define CONTENT_LENGTH "Content-Length: %d\r\n"
#define CONNECTION     "Connection: close\r\n"
#define CONTENT_TYPE   "Content-Type: text/html\r\n"

#define READ_BUFFER_SIZE  1024
#define MAX_RESPONSE_SIZE 1024
#define MAX_STUDENT_LEN 24

#define BACKLOG 10   // how many pending connections queue will hold

#define BYTE sizeof(char)
#define CRLF "\r\n"
#define REQUEST_END "\r\n\r\n"

char* STUDENT; // to be filled-in by environment variable STUDENT_NAME
#define MSG_FILE "last_message.txt"





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





/*
  Finds a blank line in the request; returns pointer to the start of the message body
   or NULL if no blank line could be found
 */
char* get_request(char* buffer, int buffer_size, int max_buffer_size) {
  char* index = memmem(buffer, buffer_size, REQUEST_END, 4);
  if (index == NULL) return NULL;
  if ((index + 4) - buffer >= max_buffer_size) return NULL;
  return index + 4; // pointer to the message body
}

/* Determines what kind of request was made */
int get_resource(char* buffer, char* buffer_end) {
  if (memmem(buffer, buffer_end - buffer, GET_ROOT, sizeof(GET_ROOT) - 1)) return 0;
  if (memmem(buffer, buffer_end - buffer, GET_INFO, sizeof(GET_INFO) - 1)) return 1;
  if (memmem(buffer, buffer_end - buffer, POST_INFO, sizeof(POST_INFO) - 1)) return 2;
  if (memmem(buffer, buffer_end - buffer, GET_FAVICON, sizeof(GET_FAVICON) - 1)) return 3;
  return -1;
}

/* Returns the current date and time */
void dateIs(char* date_b) {
 time_t clock;
  clock = time(NULL);
  strcpy(date_b, ctime(&clock));
  int len = strlen(date_b) - 1;
  strcpy(date_b + len, "\r\n");
}

/* for debugging */
void printBuffer(char* buffer, int bufsize) {
  printf("\nPRINT BUFFER:\n");
  char c = '\0';
  for (int i = 0; i < bufsize; i++) {
    if (buffer[i] == '\r') {
      putchar('\\');
      putchar('r');
      putchar('\n');
    }
    else if (buffer[i] == '\n') {
      putchar('\\');
      putchar('n');
      putchar('\n');
    }
    else if (buffer[i] == '\0') {
      putchar('\\');
      putchar('0');
      putchar('\n');
    }
    else {
      putchar(buffer[i]);
    }
  }
}

/* Puts together a response with data as the message body */
void create_response(char* response, char* data) {
  char dateNow[100];
  dateIs(dateNow);

  *response = '\0';
  strcat(response, RESPONSE_OK);
  strcat(response, DATE);
  strcat(response, dateNow);
  strcat(response, SERVER);
  strcat(response, STUDENT);
  strcat(response, CRLF);
  strcat(response, CONTENT_LENGTH);
  char cl[1024] = ""; // content length
  int data_length = strnlen(data, 1024);
  sprintf(cl, response, data_length);
  strcpy(response, cl);
  strcat(response, CONNECTION);
  strcat(response, CONTENT_TYPE);
  strcat(response, CRLF);

  strlcat(response + strlen(response), data, MAX_RESPONSE_SIZE);
  /* for debugging */
  /* printf("in create response:\n%s\n", response); */
  /* int len = strlen(response); */
  /* printBuffer(response, len); */
}

/*
  Locates the Content-Length value; puts that many characters from the message
  body into a buffer; returns that value.
 */
int get_message(char* message, char* buf, char* buf_req) {
  char* content_length = "Content-Length: ";
  int content_length_s = strlen(content_length);
  char* cl = memmem(buf, READ_BUFFER_SIZE, content_length, content_length_s);
  if (!cl) {
    fprintf(stderr, "ERROR getting message\n");
    exit(1);
  }
  int l;
  if (sscanf(cl + content_length_s, "%d", &l) != 1) {
    fprintf(stderr, "ERROR scanning content-length number\n");
    exit(1);
  }
  memcpy(message, buf_req, l);
  return l;
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

  STUDENT = getenv("STUDENT_NAME");
  if (STUDENT == NULL) {
    fprintf(stderr, "ERROR: need to provide environment variable STUDENT_NAME\n");
    exit(1);
  }

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
      char buffer[READ_BUFFER_SIZE];
      int read_result = read(new_fd, &buffer, READ_BUFFER_SIZE);
      printf("\nread_result: %d bytes read\n", read_result);
      printf("buffer:\n%s\n", buffer);

      /*
        request() searches the GET or POST request for a blank line;
        if one is found, a pointer to the start of the message body
        is returned; if not, NULL is returned.
       */
      char* request = get_request(buffer, read_result, READ_BUFFER_SIZE);
      if (!request) {
        fprintf(stderr, "ERROR: failed to find a REQUEST_END\n");
        exit(1);
      }

      // LS: loop above until \n\n is sent, signaling the end of an HTTP request
      // LS: parse the input and determine what result to send

      char* response = malloc(READ_BUFFER_SIZE);
      char* root = malloc(READ_BUFFER_SIZE);
      char* template;
      char* info;
      char* message;
      FILE* fd;

      switch (get_resource(buffer, request)) {
      case 0: // GET /
        template = "<html><head></head><body>Welcome to %s</body></html>\n";
        sprintf(root, template, STUDENT);
        create_response(response, root);
        break;

      case 1: // GET /info
        fprintf(stderr, "found a proper get info request\n");
        char* info = malloc(1024);
        
        break;

      case 2: // POST /info
        message = malloc(READ_BUFFER_SIZE - read_result);
        int length = get_message(message, buffer, request);
        if (length == -1) {
          fprintf(stderr, "ERROR: no message found in a POST request\n");
          exit(1);
        }
        if ((fd = fopen(MSG_FILE, "w")) == NULL) {
          fprintf(stderr, "ERROR opening file for appending");
          exit(1);
        }
        if ((fprintf(fd, "%s\n", message) != length + 1)) {
          fprintf(stderr, "ERROR writing message to file\n");
          exit(1);
        }
        printf("POSTED: \n%s\n", message);
        fclose(fd);
        break;

      case 3: // favicon
        fprintf(stderr, "received a request for a favicon; ignoring\n");
        break;

      default:
        fprintf(stderr, "ERROR: failed to find a proper get request\n");
        exit(1);
        break;
      }

      close(sockfd); // child doesn't need the listener
      // LS: Send the correct response in JSON format
      int size = strnlen(response, READ_BUFFER_SIZE);
      if (send(new_fd, response, 1024, 0) == -1)
        perror("send");

      close(new_fd);
      exit(0);
    }
    close(new_fd);  // parent doesn't need this
  }

  return 0;
}
