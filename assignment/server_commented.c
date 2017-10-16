/*
** server.c -- a stream socket server demo
*/

/*********************************************************************
 * COMMENTS FOR LambdaSchool/Applied-C-Programming Assignment        *
 * By Wesley Harvey                                                  *
 * 2017-10-13                                                        *
 * Version 0.1_a                                                     *
 *                                                                   *
 *********************************************************************
 *                               ASSIGNENT                           *
 ********************************************************************* 
 *
 * Your objective is to refactor the following simple UNIX net sockets
 * server server.c  to respond to  a few specific  requests, including
 * storing a message sent by a previous uploader.
 * 
 * Your goal is  to make Chrome happy when you  return /, by including
 * the proper  headers and BODY.   Your app must handle  this request,
 * and save the result.
 *
 * 1. Read this  file and write comments before  each block explaining
 * what you think  its purpose is and what it  does. After doing this,
 * create a summary file server_summary.md that explains the operation
 * of the file based on your analysis.
 * 
 * 2.  Modify this  file to  become a  simple HTTP  server. It  should
 * accept / and /info requests.
 *
 *     -- Your server must support the following requests:
 *        GET / HTTP/1.0
 *        GET /info HTTP/1.0
 *        POST /info HTTP/1.0
 *
 *     -- Your server must support the following headers:
 *        Content-Length:
 *        Host:
 *        User-Agent:
 *
 * 3.  Your  server  response  must match  the  HTTP  response  header
 * protocol:
 * 
 *  HTTP/1.0 200 OK
 *  Date: xxxx (formatted time or Unix time)
 *  Server: Name of Student
 *  Content-Length: yy
 *  Connection: close
 *  Content-Type: text/html
 *
 * Your server must return the following results:
 * 
 * GET /
 * <html><head></head><body>Welcome to ::student name::</body></html>
 *
 *  GET /info
 *  {"info": {"name":":::student name:::", "url_request": "/info", \
 *         "last_message": ":::undefined::: OR :::POST message:::"}}
 *
 *  POST /info
 *
 *  ::: any message less than 1024 bytes :::
 *
 * Parameters
 * 
 * Server/:::student name::: - stored  on your local environment (ENV)
 * as  STUDENT_NAME and  read  at run  time by  your  server with  the
 * `getenv` function call.
 *
 * Date  -  calculated at  the  time  of  request using  the  `time()`
 * function and the `time_t` structure loaded from `#include <time.h>`
 *
 * Content-Length -  The length  of the  BODY of  the message  that is
 * sent.
 *
 * message - Either return "undefined" if no message has been sent, or
 * return the  message that  was uploaded via  the POST  /info request
 * previously.
 *
 ********************************************************************/
/* POSIX            http://pubs.opengroup.org/onlinepubs/9699919799 */
/*------------------------------------------------------------------*/
#include <stdio.h>        // standard input/output library functions
#include <stdlib.h>	  // standard library definitions 
#include <unistd.h>	  // standard symbolic constants and types
#include <errno.h>	  // system error numbers
#include <string.h>	  // string operations (see also strings.h)
#include <sys/types.h>	  // data types
#include <sys/socket.h>   // main sockets header--
                          //     create an endpoint for comm
#include <netinet/in.h>   // Internet address family
#include <netdb.h>	  // defn's for network database operations
#include <arpa/inet.h>	  // defn's for internet operations
#include <sys/wait.h>	  // declarations for waiting
#include <signal.h>	  // signals

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





/* TWO FUNCTIONS */
/********************************************************************/

/* First Function: wait for a child process to terminate            *
/*==================================================================*/

/* WAIT (2)                                              <SYS/WAIT.H>
/********************************************************************
 #include <sys/wait.h> | wait for process termination
 ----------------------|
 #include <sys/types.h>|
 ----------------------|

 --waitpid()--                       ==> process ID of the child | -1
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 pid_t
 waitpid(
   pid_t pid,                   -1 :=: wait for any child process
   int *stat_loc,               NULL
   int options                  WNOHANG :=: don't hang unnecessarily
 );
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 --wait()--
 __________
 The  --wait()-- function  suspends execution  of its  calling process
 until  <stat_loc> information  is  available for  a terminated  child
 process,  or a  signal  is  received.  On  return  from a  successful
 --wait()-- call, the <stat_loc> area contains termination information
 about the process that exited as defined below.

 --wait4()--
 ___________
 The --wait4()-- call  provides a more general  interface for programs
 that need  to wait  for certain child  processes, that  need resource
 utilization  statistics  accumulated  by  child  processes,  or  that
 require  options.  The  other  wait functions  are implemented  using
 --wait4()--.

 --waitpid()--
 _____________
 The --waitpid()--  call is  identical to  --wait4()-- with  an rusage
 value of zero.

 --wait3()--
 ___________
 The older  --wait3()-- call  is the  same as  --wait4()-- with  a pid
 value of -1.

 <pid>
 _____
 The <pid> parameter specifies the set of child processes for which to
 wait.

   -If pid is -1,  the call waits for any child process.   

   -If pid is  0, the call waits  for any child process  in the process
    group of the  caller.  

   -If pid  is greater than zero,  the call waits for  the process with
    process id  pid.  

   -If  pid is  less than  -1, the  call waits  for any  process whose
    process group id equals the absolute value of pid.

 <options>
 _________
 The options parameter contains the bitwise OR of any of the following
 options: 

   -The WNOHANG  option is used to  indicate that the call  should not
    block if there are no processes that wish to report status.

 Macros may  be used  to test the  manner of exit  of the  process.  A
 return status of 0 indicates normal termination.

 RETURN
 ______
 If --wait()-- returns  due to a stopped or  terminated child process,
 the  process ID  of the  child is  returned to  the calling  process.
 Otherwise, a value of -1 is returned and errno is set to indicate the
 error.

 If ...   --waitpid()-- returns due  to a stopped or  terminated child
 process,  the process  ID of  the child  is returned  to the  calling
 process.   If there  are no  children not  previously awaited,  -1 is
 returned  with  errno set  to  [ECHILD].   Otherwise, if  WNOHANG  is
 specified and there are no stopped or exited children, 0 is returned.
 If an error is  detected or a caught signal aborts  the call, a value
 of -1 is returned and errno is set to indicate the error.

*********************************************************************/

void sigchld_handler(int s) /* called after --listen()--
                               why is <s> never used? */
{
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while(waitpid(-1, NULL, WNOHANG) > 0);
                /* return should be pid of child process, i.e., > 0 */
  errno = saved_errno;
}

/* From SIGACTION(2) <signal.h>
 *********************************************************************
/* "Also, it  is good practice to  make a copy of  the global variable
   errno  and restore  it before  returning from  the signal  handler.
   This  protects  against the  side  effect  of  errno being  set  by
   functions called from inside the signal handler."                */






/* MAN 2 SOCKET                                           SYS/SOCKET.H
/*********************************************************************                                                                      
 * #include <sys/socket.h> |
 *--------------------------
 * --socket()-- creates  an endpoint  for communication and  returns a
 *   descriptor.
 *
 * int socket:
 *   ARGS:
 *     int domain
 *     int type
 *     int protocol
 *
 *    The <domain> parameter specifies  a communications domain within
 *    which communication  will take place; this  selects the protocol
 *    family which should be used.
 *
 *    The  socket  has  the  indicated  <type>,  which  specifies  the
 *    semantics of communication.
 *        SOCK_STREAM, SOCK_DGRAM, SOCK_RAW
 *
 *    The <protocol> specifies  a particular protocol to  be used with
 *    the socket.
 *
 * A -1 is returned if an  error occurs, otherwise the return value is
 * a descriptor referencing the socket.
 *====================================================================/


 * POSIX SOCKET                                           SYS/SOCKET.H
 *********************************************************************
 * #include <sys/socket.h> |
 * -------------------------
 *
 *
 * =========================
 * --struct sockaddr--
 * =========================
 * The <sys/socket.h> header shall  define the --sockaddr-- structure,
 * which shall include at least the following members:
 *
 * ///////////////////////////////////////////////////////////////////
 * struct sockaddr {
 *   sa_family_t  sa_family  Address family. (sockaddr_storage)
 *   char         sa_data[]  Socket address (variable-length data). 
 * }
 * \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 *
 * The --sockaddr-- structure is used to define a socket address which
 * is used in  the --bind()-- (see below),  --connect()-- (see below),
 * getpeername(), getsockname(), recvfrom(), and sendto() functions.
 *
 *
 * ============================
 * --struct sockaddr_storage--
 * ============================
 * The  <sys/socket.h> header  shall  define the  --sockaddr_storage--
 * structure, which shall include at least the following members:
 *
 * ///////////////////////////////////////////////////////////////////
 * struct sockaddr_storage {
 *   sa_family_t   ss_family (used by --sockaddr-- structure, above)
 * }
 * \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 *
 *      sa_family_t  :=: unsigned integer type
 *
 * The --sockaddr_storage structure-- solves  the problem of declaring
 * storage  for automatic  variables which  is both  large enough  and
 * aligned enough for storing the socket address data structure of any
 * family. For  example, code with  a file descriptor and  without the
 * context of the  address family can pass a pointer  to a variable of
 * this  type,  where a  pointer  to  a  socket address  structure  is
 * expected in calls such as  getpeername(), and determine the address
 * family by accessing the received content after the call.
 *
 *
 * ===================
 * --struct msghdr--
 * ===================
 * The <sys/socket.h>  header shall  define the  --msghdr-- structure,
 * which shall include at least the following members:
 *
 * ///////////////////////////////////////////////////////////////////
 * struct msghdr {  
 *   void          *msg_name        Optional address. 
 *   socklen_t      msg_namelen     Size of address. 
 *   struct iovec  *msg_iov         Scatter/gather array. 
 *   int            msg_iovlen      Members in msg_iov. 
 *   void          *msg_control     Ancillary data; see below. 
 *   socklen_t      msg_controllen  Ancillary data buffer len. 
 *   int            msg_flags       Flags on received message. 
 * }
 * \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 
 *
 * The --msghdr-- structure is used to minimize the number of directly
 * supplied parameters to the recvmsg() and sendmsg() functions.
 *
 * ==============
 * --socklen_t--
 * ==============
 * The <sys/socket.h> header shall define the socklen_t type, which is
 * an integer type of width of at least 32 bits; size of address.
 *
 *********************************************************************/



/* POSIX                                                  NETINET/IN.H
/*********************************************************************
 * #include <netinet/in.h> | ==> Internet address family
 * ------------------------
 *
 * =======================
 * --struct sockaddr_in--
 * =======================
 * The   <netinet/in.h>  header   shall  define   the  --sockaddr_in--
 * structure, which shall include at least the following members:
 *
 * ///////////////////////////////////////////////////////////////////
 * struct sockaddr_in {
 *   sa_family_t     sin_family   AF_INET. 
 *   in_port_t       sin_port     Port number. 
 *   struct in_addr  sin_addr     IP address. 
 * }
 * \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 *
 * The --sockaddr_in--  structure is used  to store addresses  for the
 * Internet address  family. Pointers  to this type  shall be  cast by
 * applications  to   --struct  sockaddr  *--  for   use  with  socket
 * functions. */



/*********************************************************************
 * QUERY:  what  does  `pointers  to   this  type  shall  be  cast  by
 * applications to struct sockaddr *' mean?
 *********************************************************************/








/* Second Function: Store a socket address                            *
/*====================================================================*/

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
  
/* --sockaddr-- argument, returns --sockaddr_in*
 * a socket address in, stored in a sockaddr_in structure */

{ // --AF_INET-- and --sin_addr-- defined in --sockaddr_in-- struct
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr); // IPv4
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr); // IPv6
}











/********************************************************************
 * MAIN                                                             *
 *******************************************************************/
int main(void)
{

  /* VARIABLE DECLARATIONS                                          */
  /******************************************************************/
  
  /*
    sockfd is used by --bind()--

    new_fd is used by --accept()--,  a non-negative integer that is a
    descriptor for the accepted socket.
  */
  
  int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd

  /* POSIX
  /******************************************************************
   * #include <netdb.h> | definitions for network database operations
   * --------------------
   *
   * ====================
   * --struct addrinfo--
   * ====================
   * Address Information Structure
   * -----------------------------
   * The  <netdb.h> header  shall define  the --addrinfo--  structure,
   * which shall  include at least  the following members:
   *
   * ////////////////////////////////////////////////////////////////
       struct addrinfo {
             int ai_flags;             /* input flags
             int ai_family;            /* protocol family for socket
             int ai_socktype;          /* socket type
             int ai_protocol;          /* protocol for socket
             socklen_t ai_addrlen;     /* length of socket-address
             struct sockaddr *ai_addr; /* socket-address for socket
             char *ai_canonname;       /* canonical name for service location
             struct addrinfo *ai_next; /* pointer to next in list
       }
   * \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 
  *******************************************************************/

  /* --hints-- is a --struct addrinfo--; more info above and below  */
  struct addrinfo hints, *servinfo, *p;

  /* <sys/socket.h> socket storage */
  struct sockaddr_storage their_addr; // connector's address information

  socklen_t sin_size;  /* <sys/socket.h> uint 32 */




  
  struct sigaction sa; /* this sigaction struct is used to get get
                          rid of child processes? */

  /* SIGACTION (2)                                         <SIGNAL.H>
  /******************************************************************
  * The system  defines a set  of signals that  may be delivered  to a
  * process.  Signal  delivery resembles the occurrence  of a hardware
  * interrupt: ...
  *
  * The  sigaction()  system  call  assigns an  action  for  a  signal
  * specified by sig. ...
  *
  * #define sa_handler      __sigaction_u.__sa_handler
  * #define sa_sigaction    __sigaction_u.__sa_sigaction
  *
  * SIGACTION()                                            ==> 0 | -1
  *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  * int 
  *   sigaction(
  *     int sig,
  *	const struct sigaction *restrict act,  handler routine
  *     struct sigaction *restrict oact        previous handling info
  * );
  *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  *
  * The --sigaction()--  system call  assigns an  action for  a signal
  * specified by <sig>.  If <act>  is non-zero, it specifies an action
  * (SIG_DFL, SIG_IGN, or a handler routine)  and mask to be used when
  * delivering  the  specified signal.   If  <oact>  is non-zero,  the
  * previous handling  information for the  signal is returned  to the
  * user.
  *
  * Options may be specified by setting sa_flags. 
  *
  * /////////////////////////////////////////////////////////////////
  * struct sigaction {
  *   union __sigaction_u __sigaction_u;  /* signal handler
  *   sigset_t sa_mask;                   /* signal mask to apply
  *   int     sa_flags;                   /* see signal options below 
  * }
  * \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
  *
  * //////////////////////////////////////////////////////////////////
  * union __sigaction_u {
  *          void    (*__sa_handler)(int);
  *          void    (*__sa_sigaction)(int, siginfo_t *,
  *                         void *);
  *  };
  *
  *\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
  
  


  
  int yes=1;

  char s[INET6_ADDRSTRLEN];

  int rv; /* return value of a function, i.e., getaddrinfo() */





  
  memset(&hints, 0, sizeof hints);
  /* memset -- fill a byte string with a byte value */

  /* This structure can  be used to provide hints  concerning the type
     of socket that the caller supports or wishes to use. */
  
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  /* man 3 getaddrinfo
   * =================
  /* getaddrinfo, freeaddrinfo -- socket address structure to host and service name
     #include <sys/types.h>
     #include <sys/socket.h>
     #include <netdb.h>

     ARGUMENTS:
       char *hostname
       char *servname
       struct addrinfo *hints
       addrinfo **res

     The  --getaddrinfo()-- function  is  used  to get  a  list of  IP
     addresses  and  port  numbers  for host  <hostname>  and  service
     <servname>.   It   is  a   replacement  for  and   provides  more
     flexibility  than   the  gethostbyname(3)   and  getservbyname(3)
     functions.

     The <hostname>  and <servname>  arguments are either  pointers to
     NUL-terminated strings or the  null pointer.  
        *An acceptable  value for  <hostname> is  either a  valid host
         name or a numeric host  address string consisting of a dotted
         decimal IPv4 address  or an IPv6 address.
        *The <servname>  is either  a decimal port  number or  a service
         name listed in services(5).
     At least one of <hostname> and <servname> must be non-null.

     <hints>  is an  optional  pointer to  a  --struct addrinfo--,  as
     defined by <netdb.h> (SEE ABOVE Address Information Structure)

       This structure can be used to provide hints concerning the type
       of socket that the caller supports or wishes to use.

    RETURN:
      --getaddrinfo()-- returns  zero on success  or one of  the error
      codes listed in gai_strerror(3) if an error occurs.

    Memory allocated for the  dynamically allocated structures created
    by  a successful  call  to --getaddrinfo()--  is  released by  the
    --freeaddrinfo()-- function.
  */

  /* POSIX
  /*********************************************************************
   * The  --getaddrinfo()-- function  shall  translate the  name of  a
   * service location (for example, a host name) and/or a service name
   * and  shall  return  a  set of  socket  addresses  and  associated
   * information to be used in creating a socket with which to address
   * the specified service.
   * -------------------------------------------------------------------
   * The   --freeaddrinfo()--  function   shall  free   one  or   more
   * --addrinfo-- structures returned by --getaddrinfo()--, along with
   * any additional storage associated  with those structures.  If the
   * <ai_next> field of the structure is  not null, the entire list of
   * structures shall be freed.
   * The  --freeaddrinfo()-- function  shall  support  the freeing  of
   * arbitrary sublists of an --addrinfo-- list originally returned by
   * --getaddrinfo()--.
   *********************************************************************/


  /* something went wrong if rv == 1 */
  /* returns a set of socket addresses and a service name on success */
  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    /* hostname is NULL; PORT = 7080; servinfo is an --addrinfo-- struct,
       and is filled in by the getaddrinfo function */
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  /* getaddrinfo()
   * --------------
   * After  a  successful call  to  --getaddrinfo()--,  --*res-- is  a
     pointer to a linked list  of one or more --addrinfo-- structures.
     The list can be traversed by following the --ai_next-- pointer in
     each --addrinfo-- structure until a null pointer is encountered.
  */

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    /* GETSOCKOPT (2)                         #include <sys/socket.h>
    /****************************************************************

     GETSOCKOPT  --  SETSOCKOPT --  get  and  set options  on  sockets
     Getsockopt() and  setsockopt() manipulate the  options associated
     with a socket.  ...  The following options are  recognized at the
     socket level. ...

     setsockopt()                                          ==> 0 | -1
     ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     int
       setsockopt(
         int socket,       (sockfd below)
         int level,
         int option_name,  (SO_REUSEADDR below)
         const void *option_value, (yes below)
         socklen_t option_len
       );
     ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

         The parameters  <option_value> and  <option_len> are  used to
         access option values for setsockopt().

         SO_REUSEADDR    enables local address reuse
    *****************************************************************/

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
        sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    /* BIND (2)                                #include <sys/socket.h>
    /*****************************************************************
     --bind()-- assigns a name to an unnamed socket.  When a socket is
     created with  --socket(2)-- it  exists in  a name  space (address
     family)  but  has no  name  assigned.   --bind()-- requests  that
     address be assigned to the socket.

     Binding a  name in the UNIX  domain creates a socket  in the file
     system that  must be deleted by  the caller when it  is no longer
     needed (using unlink(2)).

     bind()                                                ==> 0 | -1
     ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     int
       bind(
         int socket,                      (sockfd)
         const struct sockaddr *address,
         socklen_t address_len
       );
     ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

     RETURN:
     Upon successful completion, a value of 0 is returned.  Otherwise,
     a value of  -1 is returned and the global  integer variable errno
     is set to indicate the error.
    *****************************************************************/

    /* p   is   an   addrinfo   from   enclosing   loop   created   by
     * --getaddrinfo()-- 
       this call binds a name to a socket */
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  /* POSIX
  /*********************************************************************
   * The   --freeaddrinfo()--  function   shall  free   one  or   more
   * --addrinfo-- structures returned by --getaddrinfo()--, along with
   * any additional storage associated  with those structures.  If the
   * --ai_next-- field of  the structure is not null,  the entire list
   * of structures  shall be  freed.  The  --freeaddrinfo()-- function
   * shall   support  the   freeing  of   arbitrary  sublists   of  an
   * --addrinfo-- list originally returned by --getaddrinfo().--
   *********************************************************************/
  freeaddrinfo(servinfo); // all done with this structure

  /* p is an addrinfo from prior set of code */
  if (p == NULL)  {
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }






  /* NETWORK LISTENING BEING SET UP                                 */
  /******************************************************************/
  
  if (listen(sockfd, BACKLOG) == -1) {
    /* sockfd is a socket descriptor, from above                    */


    perror("listen");
    exit(1);
  }

  /* LISTEN (2)                               #include <sys/socket.h>
  /******************************************************************
   listen() -- listen for connections on a socket

   Creation of  socket-based connections requires  several operations.
   ------------------------------------------------------------------
     First, a socket  is created with --socket(2)--.
     --
     Next, a  willingness to accept  incoming connections and  a queue
     limit for incoming connections are specified with --listen()--.
     --
     Finally, the  connections are  accepted with  --accept(2)--.
     --
     The   --listen()--  call   applies  only   to  sockets   of  type
     SOCK_STREAM.

   listen()                                                ==> 0 | -1
   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   int
     listen(
       int socket,  (must be a valid file descriptor, among others)
       int backlog  (maximum length for queue of pending connections)
                    (limited to 128)
    );
   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   */





  /* SIGNAL HANDLING BEING SET UP                                   */
  /******************************************************************/

  /* --sigchld_handler()-- is defined above, and simply waits for a
     child process to terminate, and then returns; 
     <sa.sa_handler> holds the function address to the child handler
     and will be called when the right signal is received. */

  /* <sa> declared at top of program as a --struct sigaction--; here,
     it is being assigned a handler, --sigchld_handler--, to run when
     a child process's status changes.
  */
  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  /* sa: struct sigaction (see above) includes a member `signal handler'
       union __sigaction_u __sigaction_u;  signal handler 
     and a `signal mask'
       sigset_t sa_mask;                   signal mask to apply
     and some flags
       int     sa_flags;                   see signal options below 
    <sa> is one of the variables assigned at the beginning of `main'

    The  sigaction()  system  call  assigns an  action  for  a  signal
    (see above); <act> is &sa; <oact> is NULL

    here, --sigaction()-- is assigning the signal --SIGCHLD--, defined
    in <signal.h>, as:
    SIGCHLD         discard signal          child status has changed
  */

  /* So,  when child  process status  changes, perform  the action  of
     getting   rid   of   the   child   process   (sa.sa_handler   :=:
     sigchld_handler) 
  */

  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  /* SERVER IS RUNNING                                               */
  /*******************************************************************/

  printf("server: waiting for connections...\n");





  
  while(1) {  // main accept() loop
/*==================================================================*/

    sin_size = sizeof their_addr;

    /*
      two variables set up at the beginning of `main' (see above);
        socklen_t sin_size;               <sys/socket.h> uint 32
        struct sockaddr_storage their_addr; connector's address info
      --their_addr-- is an address storage struct; 
      it hasn't been initialized yet; it is initialized in the
      --accept()-- function, below                               
    */

    /* ACCEPT (2)                                       <SYS/SOCKET.H>
    /****************************************************************/
    /* accept a connection on a socket
       #include <sys/socket.h>
       
       --accept()--            ==> nonegative int as fd | -1 on error
       ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       int
       accept(
         int socket, 
         struct sockaddr *restrict address, 
         socklen_t *restrict address_len
       );
       ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

       <socket>
       --------
       The argument  <socket> is a  socket that has been  created with
       --socket(2)--,  bound to  an address  with --bind(2)--,  and is
       listening for connections  after a --listen(2)--.  

       --accept()-- extracts the first connection request on the queue
       of  pending connections,  creates a  new socket  with the  same
       properties of socket,  and allocates a new  file descriptor for
       the  socket.

       If no  pending connections  are present on  the queue,  and the
       socket is not marked as  non- blocking, --accept()-- blocks the
       caller until a connection is  present.  If the socket is marked
       non-blocking  and no  pending  connections are  present on  the
       queue, --accept()--  returns an error as  described below.  The
       accepted socket  may not  be used  to accept  more connections.
       The original socket <socket>, remains open.

       <address>
       ---------
       The argument <address> is a  result parameter that is filled in
       with  the address  of the  connecting entity,  as known  to the
       communications  layer.    The  exact  format  of   the  address
       parameter   is  determined   by   the  domain   in  which   the
       communication is occurring.  

       <address_len>
       -------------
       The  <address_len>  is  a  value-result  parameter;  it  should
       initially contain the amount of space pointed to by address; on
       return  it will  contain the  actual length  (in bytes)  of the
       address  returned.

       This call is used with connection-based socket types, currently
       with SOCK_STREAM.
    */

    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }

    /* INET (3)                                           <ARPA/INET>
    /****************************************************************
      #include <arpa/inet.h> | Internet address manipulation routines
      -----------------------|

      These include files are also necessary for all functions.
      ------------------------
      #include <sys/types.h> |
      #include <sys/socket.h>|
      #include <netinet/in.h>| -> --in_addr-- structure
      ------------------------

      --inet_ntop()--       ==> pointer to dst string | NULL on error
      +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      const char *
      inet_ntop(
        int                     af,  (sa_family_t, 
                                      their_addr.ss_family below;
                                      see sockaddr_storage structure,
                                      above)
        const void * restrict   src, (address in network format)
        char * restrict         dst, (destination buffer)
        socklen_t               size (size of dst buffer)
      );
      +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

      Produces an address in presentation format

      The  function --inet_ntop()--  converts an  address <*src>  from
      network  format  (usually a  --struct  in_addr--  or some  other
      binary  form,  in network  byte  order)  to presentation  format
      (suitable for external display purposes).

      The <size> argument specifies the  size, in bytes, of the buffer
      <*dst>.  INET_ADDRSTRLEN and INET6_ADDRSTRLEN define the maximum
      size required to convert an address of the respective type.

      It returns NULL  if a system error occurs (in  which case, errno
      will have been set), or it  returns a pointer to the destination
      string. 

      This function is presently valid for AF_INET and AF_INET6.

    */

    /* POSIX
    /****************************************************************
     #include <arpa/inet.h> | definitions for internet operations
     -----------------------|

     The <arpa/inet.h>  header shall define the  --in_addr-- structure
     as described in <netinet/in.h>.

     const char  *inet_ntop(
                     int,                  (sa_famly_t, unsigned int)
                     const void *restrict, (src)
                     char *restrict,       (dst buffer)
                     socklen_t             (dst buffer size)
                  );

    *****************************************************************/

    /* --inet_ntop()-- produces  a presentation format address  from a
         binary address */
    inet_ntop(
              their_addr.ss_family, /* their_addr    filled   in    by
                                     * --accept()--; this member is an
                                     * sa_family_t, unsigned int */
              get_in_addr((struct sockaddr *)&their_addr), /* <src>; 
                                       this fn defined above */
              s,         /* <dst buffer> */
              sizeof s); /* size of <dst> buffer */

    /* and here the address in <s> is printed for the user */
    printf("server: got connection from %s\n", s);



    /* FORK (2)                                            <UNISTD.H>
    /****************************************************************
     #include <unistd.h>    | create a new process
     -----------------------|
     #include <sys/types.h> |
     -----------------------|

     --fork()--                                            ==> 0 | -1
     ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     pid_t
       fork(void);
     ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

     --fork()-- causes  creation of  a new  process.  The  new process
     (child process) is  an exact copy of the  calling process (parent
     process) except for the following:

       -The child process has a unique process ID.

       -The child process has a different parent process ID (i.e., the
        process ID of the parent process).

       -The  child   process  has  its   own  copy  of   the  parent's
        descriptors.  These descriptors  reference the same underlying
        objects, so that, for instance,  file pointers in file objects
        are  shared between  the  child  and the  parent,  so that  an
        lseek(2) on  a descriptor  in the child  process can  affect a
        subsequent  read  or write  by  the  parent.  This  descriptor
        copying is also used by  the shell to establish standard input
        and output  for newly created processes  as well as to  set up
        pipes.

     ****************************************************************/
    
    if (!fork()) { // this is the child process

      // LS: read from client input
      const int READ_BUFFER_SIZE = 1024;
      char buffer[READ_BUFFER_SIZE]; /* used by --read()-- function */

      /* READ (2)                                          <UNISTD.H>
       **************************************************************
       #include <unistd.h>    | read input
       -----------------------|
       #include <sys/uio.h>   |
       #include <sys/types.h> |
       -----------------------|

         --read()-        ==> # bytes read | EOF :=: 0 | ERROR :=: -1
         ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
         ssize_t
         read(
           int fildes,             (new_fd; from --accept()--, above)
           void *buf, 
           size_t nbyte
         );
         ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

         --read()--  attempts to  read nbyte  bytes of  data from  the
         object referenced by the  descriptor <fildes> into the buffer
         pointed  to by  <buf>.  On  objects capable  of seeking,  the
         --read()--  starts  at  a   position  given  by  the  pointer
         associated with  <fildes> (see  lseek(2)).  Upon  return from
         --read()--, the pointer is incremented by the number of bytes
         actually  read.   Objects that  are  not  capable of  seeking
         always read from the current position.

      ***************************************************************/

      int read_result = read(new_fd, &buffer, READ_BUFFER_SIZE);

      /* read_result should be >0 and <= 1024                       */
      printf("read_result: %d\n", read_result);
      printf("buffer: %s\n", buffer);

      // LS: loop  above until \n\n is  sent, signaling the end  of an
      // HTTP request

      // LS: parse the input and determine what result to send
      close(sockfd); // child doesn't need the listener
      // LS: Send the correct response in JSON format
      /* NOTE: <sockfd> is from --bind()--, above */

      
      /* SEND (2)                                      <SYS/SOCKET.H>
      /**************************************************************
       #include <sys/socket.h> | send a message from a socket
       ------------------------|
       #include <sys/types.h>  |
       ------------------------|

       --send()--                              ==> # bytes sent | -1
       ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       ssize_t
         send(
           int socket, 
           const void *buffer, 
           size_t length, 
           int flags
         );
       ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

       --Send()--,  sendto(), and  sendmsg()  are used  to transmit  a
       message to  another socket.  --Send()--  may be used  only when
       the  socket  is  in  a  connected  state,  while  sendto()  and
       sendmsg() may be used at any time.

       The  address  of  the  target  is  given  by  <dest_addr>  with
       <dest_len> specifying its  size.  The length of  the message is
       given  by  <length>.   If  the  message is  too  long  to  pass
       atomically through the underlying  protocol, the error EMSGSIZE
       is returned, and the message is not transmitted.

       No  indication  of   failure  to  deliver  is   implicit  in  a
       --send()--.  Locally detected errors  are indicated by a return
       value of -1.

       If no  messages space is  available at  the socket to  hold the
       message  to be  transmitted, then  --send()-- normally  blocks,
       unless the  socket has  been placed  in non-blocking  I/O mode.
       The  --select(2)-- call  may be  used to  determine when  it is
       possible to send more data.

       The --sendmsg()--  system call  uses a --msghdr--  structure to
       minimize  the  number  of directly  supplied  arguments.   [See
       --sockaddr_storage--]   See   --recv(2)--    for   a   complete
       description of the --msghdr-- structure.

       **************************************************************/

      /* Transmit a message to another  socket; must be in a connected
       * state (so no address is needed) */

      if (send(new_fd, "Hello, world!", 13, 0) == -1)
        perror("send");
      close(new_fd);
      exit(0);

    } // END OF FORK()

    close(new_fd);  // parent doesn't need this

  } // end main accept() loop
/*==================================================================*/

  return 0;
}
