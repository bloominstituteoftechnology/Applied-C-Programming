#include <stdio.h>
int main() {
  int five = 5;
  int* wth;
  int** why = &wth;
  int*** norly = &why;
  int**** seriz = &norly;
  int***** noooo = &seriz;
  int****** lame = &noooo;
  wth = &five;
  printf("five:  %p & %d\n", &five, five);
  printf("wth:   %p & %d\n", wth, *wth);
  printf("why:   %p & %d\n", why, *(*why));
  printf("norly: %p & %d\n", norly, *(*(*norly)));
  printf("seriz: %p & %d\n", seriz, *(*(*(*seriz))));
  printf("noooo: %p & %d\n", noooo, *(*(*(*(*noooo)))));
  printf("lame:  %p & %d\n", lame,  *(*(*(*(*(*lame))))));
  return 0;
}
