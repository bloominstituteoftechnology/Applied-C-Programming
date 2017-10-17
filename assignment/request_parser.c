#include <stdio.h>
#include <string.h>
#include <time.h>

#define GET_ROOT "GET / H"
#define GET_INFO "GET /info"
#define POST_INFO "POST /info"
#define GET_RESPONSE_HEAD "<html><head></head><body>Welcome to Manisha's Page</body></html>"
#define HEADER_BODY_SEPERATOR "\r\n\r\n"


char* parse_client_request(const char* request, int size) {
    printf("Parsing data of size: %d\n", size);
    printf("Client opened socket and sent: \n\n%s\n", request);
// LS: parse the input and determine what result to send

// 1: must handle GET/
// look for 'GET/' inside request

    char* find_GET = strnstr(request, GET_ROOT, size);
    if (find_GET != NULL) {
        puts("I found Get!");
    // get time 
        // time_t the_time = time(NULL);
        // printf("Current UNIX TIME %ld", the_time);
        time_t time_raw_format;
        struct tm * ptr_time;

        time ( &time_raw_format );
        ptr_time = localtime ( &time_raw_format );
        printf ("Current local date and time: %s", asctime(ptr_time));
    
        return GET_RESPONSE_HEAD;
    }

// 1: must handle GET/info
// look for 'GET/info' inside request
    char* find_GET_info = strnstr(request, GET_INFO, size);
    if (find_GET_info != NULL) {
        puts("I found Get info!");
    }

// 1: must handle POST/info

    char* find_POST = strnstr(request, POST_INFO, size);
    if (find_POST != NULL) {
        puts("I found Post info!");
    }

    return " ";
}
