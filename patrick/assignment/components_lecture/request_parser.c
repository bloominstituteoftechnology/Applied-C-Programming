#include <stdio.h>
#include <string.h>
#include <time.h>

#define GET_ROOT "GET / H"
#define GET_INFO "GET /info"
#define POST_INFO "POST /info"
#define GET_RESPONSE_HEAD "<html><head></head><body>Welcome to Patrick's AWESOME Page</body></html>"
#define GET_RESPONSE_TAIL "\r\n\r\n"

/******************************************************************************
 * parse_client_request takes a single HTTP request as a STRING
 * and PROCESSES it.
 * NOTE: The 'request' char* could contain binary data, which we
 * are not handling. If it did get binary data, it would crash (or worse).
 ******************************************************************************/

char* parse_client_request(const char* request, int size) {
    printf("Parsing data of size: %d\n", size);
    printf("Client opened socket and sent: \n\n%s\n", request);
    // int double_newline = memchr(buffer, "\n", 1024);
    // printf("read_result: %d\n", double_newline);

// LS: loop above until \n\n is sent, signaling the end of an HTTP request

// LS: parse the input and determine what result to send

// 1: must handle GET/
// look for 'GET/' inside request
    char* find_GET = strnstr(request, GET_ROOT, size);
    if (find_GET != NULL) {
        puts("I found GET!");
        time_t the_time = time(NULL);
        printf("The current UNIX time: %ld", the_time);
        return GET_RESPONSE_HEAD;
    }

// 2: must handle GET/info
// look for 'GET/info' inside request
    char* find_GET_info = strnstr(request, GET_INFO, size);
    if (find_GET_info != NULL) {
        puts("I found GET /info!");
    }

// 3: must handle POST/info
    char* find_POST = strnstr(request, POST_INFO, size);
    if (find_POST != NULL) {
        puts("I found POST /info!");
    }

    return " ";
}
