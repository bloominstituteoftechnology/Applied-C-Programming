#include<stdio.h>

int main() {
  int num = 21;
  int* pointer;
  pointer = &num;
  int ampersand = &num;

  printf("the number: %d\n", num);
  printf("the pointer: %p\n", pointer);
  printf("the ampersand: %p\n", &num);

  return 0;
}
