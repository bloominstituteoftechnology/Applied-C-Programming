#include <stdio.h>
#include <sys/types.h>

int main(void)
{
  unsigned int x = 1;
  uint y = 65535;
  printf("0x%08x\n", x);
  printf("0x%08x\n", (x&4));
  printf("0x%08x\n", (x|4));
  printf("0x%08x\n", (x|15));
  printf("0x%08x\n", (x<<4));
  printf("0x%08x\n", (x<<8));
  printf("0x%08x\n", (x<<12));
  printf("0x%08x\n", (x<<12>>8));

  printf("0x%08x\n", (y^4));
  printf("0x%08x\n", (y^0xffff));
  printf("0x%08x\n", (y^0xff00ff00));
  printf("0x%08x\n", (y));
  printf("0x%08x\n", (~y));
}
