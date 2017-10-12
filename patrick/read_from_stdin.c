
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
  char input[128];
  int returnVal;
  while((returnVal = scanf(stdin, "%s", &input)) != EOF) {
    printf("%s",input);
    printf("%d",strlen(input));
    char c = getc(stdin);
    if(strcmp(input,"exit") == 0) {
      printf("exit time! %d\n",returnVal);
      exit(0);
    }
  }
}

