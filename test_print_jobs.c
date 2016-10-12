#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  char j[100]="";
  strcat(j, "name\n");
  strcat(j, \0);
  strcat(j, "name 123\n");
  puts(j);
  printf("len : %d\n", (int)strlen(j));
  return 0;
}
