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
#include <time.h>
#include <pthread.h>


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


pthread_mutex_t myMutex;

void write_lock(char **data)
{
  pthread_mutex_lock(&myMutex);
  FILE *fp;
  fp = fopen("server_data.txt", "w");
  fputs(*data, fp);
  fclose(fp);
  pthread_mutex_unlock(&myMutex);
}

void read_lock(char **message)
{
  pthread_mutex_lock(&myMutex);
  FILE *fp;
  fp = fopen("server_data.txt", "r");
  fseek (fp , 0 , SEEK_END);
  long lSize = ftell(fp);
  rewind(fp);
  *message = (char*) malloc (sizeof(char)*lSize);
  fread(*message, 1, lSize, fp);
  fclose(fp);
  pthread_mutex_unlock(&myMutex);
}

void sigchld_handler(int s)
{
  // waitpid() might overwrite errno, so we save and restore it:
  // errno is the number of the last error
  int saved_errno = errno;
  //wait for child process
  while(waitpid(-1, NULL, WNOHANG) > 0);
  // set errno to saved errno because wait will mess with it
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
  char *studentName = getenv("STUDENT_NAME");
  //set memory for hints
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  //if cant get addrinfo
  //getaddrinfo contains addrinfo structure
  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and bind to the first we can
  // set p equal to servinfo
  // loop through all in the structures
  // socket creates an endpoint for communication 
    //takes in domain, type, and protocol
    //if == -1 then theres an error
  
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }
    //if error when setting option exit;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
        sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }
    //binds a name to a socket
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo); // all done with this structure

  if (p == NULL)  { //means we got to the end without binding a socket
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }

  if (listen(sockfd, BACKLOG) == -1) {//listens for socket connection
    perror("listen");
    exit(1);
  }
  //sahandler specieifes the action associated with signum
  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask); //intitalizes signal set given to empty
  sa.sa_flags = SA_RESTART; //modifies behavior of signal
  if (sigaction(SIGCHLD, &sa, NULL) == -1) { //examines and changes signal action
    perror("sigaction");
    exit(1);
  }
//if we make it this far server loaded correctly
  printf("server: waiting for connections...\n");

  while(1) {  // main accept() loop
    sin_size = sizeof their_addr;
    //accepts new connection
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }
    //converts ip address from binary to text
    inet_ntop(their_addr.ss_family,
      get_in_addr((struct sockaddr *)&their_addr),
      s, sizeof s);
    printf("server: got connection from %s\n", s);

    if (!fork()) { // this is the child process
      // LS: read from client input
      char *token, *userAgent, *contentLength, *host, *tim, *request, *data, *message;
      char response[2048], content[1024];
      const char s[2] = "\n";
      const int READ_BUFFER_SIZE = 1024;
      char buffer[READ_BUFFER_SIZE];
      int read_result = read(new_fd, &buffer, READ_BUFFER_SIZE);
      printf("read_result: %d\n", read_result);
      printf("%s\n", buffer);
      token = strtok(buffer, s);
      request = token;

      while( token != NULL ) {
      if (strstr(token, "User-Agent")) userAgent = token;
      if (strstr(token, CONTENT_LENGTH)) contentLength = token;
      if (strstr(token, "Host")) host = token;
      data = token;
      token = strtok(NULL, s);
      }
    
      if (memcmp(request, GET_ROOT, 14) == 0) strcpy(content, "<html><head></head><body>Welcome to :::student name:::</body></html>");
      if (memcmp(request, GET_INFO, strlen(GET_INFO)) == 0) {
        read_lock(&message);
        strcpy(content, "{\"info\": {\"name\": \"");
        strcat(content, studentName);
        strcat(content, "\", \"url_request\": \"/info\", \"last_message\": \"");
        strcat(content, message);
        strcat(content, "\"}}");
        free(message);
      }
      if (memcmp(request, POST_INFO, 14) == 0) write_lock(&data), strcpy(content, "Info has been posted");

      time_t t = time(0);
      char* date = ctime(&t);

      sprintf(response, "HTTP/1.1 200 OK\n%s %s%s %s\n%s %lu\n%s\n%s\n\n%s\n", 
        DATE, date, SERVER, studentName, CONTENT_LENGTH, strlen(content), CONNECTION, CONTENT_TYPE, content);
      printf("%s", response);
      // LS: loop above until \n\n is sent, signaling the end of an HTTP request
      // LS: parse the input and determine what result to send
      close(sockfd); // child doesn't need the listener
      // LS: Send the correct response in JSON format
      if (send(new_fd, response, strlen(response), 0) == -1)
        perror("send");
      close(new_fd);
      exit(0);
    }
    close(new_fd);  // parent doesn't need this
  }
  return 0;
}
