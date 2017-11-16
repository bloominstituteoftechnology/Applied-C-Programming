#include <stdio.h>
#include "lastmessage.h"

/**
 * Save data to a file
 */
void save_last_message(char *s)
{
  FILE *fp;

  fp = fopen("server_data.txt", "w");
  fputs(s, fp);
  fclose(fp);
}

/**
 * Get the last message
 */
char *load_last_message(char *buffer, size_t size)
{
  FILE *fp;

  fp = fopen("server_data.txt", "r");

  if (fp == NULL) {
    return NULL;
  }

  int num_bytes = fread(buffer, sizeof(char), size - 1, fp);
  buffer[num_bytes] = '\0';

  fclose(fp);

  return buffer;
}


