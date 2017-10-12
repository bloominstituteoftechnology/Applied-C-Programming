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
  printf("why:   %p & %d\n", why, **why);
  printf("norly: %p & %d\n", norly, ***norly);
  printf("seriz: %p & %d\n", seriz, ****seriz);
  printf("noooo: %p & %d\n", noooo, *****noooo);
  printf("lame:  %p & %d\n", lame, ******lame);
  printf("llame: %p & %d\n", lame, ******lame);
  
  printf("wth:   %p & %d\n", wth, *wth);
  printf("wth:   %p & %d\n", why+1, *(why+1));
  printf("wth:   %p & %d\n", norly+2, *(norly+2));
  printf("wth:   %p & %d\n", seriz+3, *(seriz+3));
  printf("wth:   %p & %d\n", noooo+4, *(noooo+4));
  printf("wth:   %p & %d\n", lame+5, *(lame+5));

  return 0;
}
