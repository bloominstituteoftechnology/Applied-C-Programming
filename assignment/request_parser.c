#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define GET_ROOT "GET / H"
#define GET_INFO "GET /info"
#define POST_INFO "POST /info"
#define GET_RESPONSE_HEAD "<html><head></head><body>Welcome to Manisha's Page</body></html>"
#define HEADER_BODY_SEPERATOR "\r\n\r\n"
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_HEADER_LENGTH 16


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
        time_t the_time = time(NULL);
        printf("Current UNIX TIME %ld", the_time);
        struct tm * ptr_time;

        time ( &the_time );
        ptr_time = localtime ( &the_time );
        printf ("\nCurrent local date and time: %s", asctime(ptr_time));
    
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
    char* find_Content_Length = strnstr(request, CONTENT_LENGTH, size);
    char* find_len_cont_len = strnstr(find_Content_Length, "\n", size-strlen(find_Content_Length));
    puts("Did we find Content-Length: here?");
    printf("%s\n", find_Content_Length);
    printf("%s\n", find_len_cont_len);
    puts("Did we find next line here?");
    printf("%p\n", find_Content_Length);
    printf("%p\n", find_len_cont_len);

    int len_con_len_line = find_len_cont_len - find_Content_Length-1;
    char len_str[1024];
    strncpy(len_str, find_Content_Length + CONTENT_LENGTH_HEADER_LENGTH, len_con_len_line - CONTENT_LENGTH_HEADER_LENGTH);
    int body_length = atoi(len_str);
    printf("Content Length? %d\n", body_length);
    printf("Difference between two pointers: %ld\n", find_len_cont_len - find_Content_Length);

    char* body = strnstr(request, HEADER_BODY_SEPERATOR, size);
    printf("BODY! %s\n", body);

    char the_body[1024];
    memset(the_body, 0, sizeof(the_body));
    strncpy(the_body, body+4, body_length);
    printf("THE BODY: %s\n", the_body);

    return " ";
}
