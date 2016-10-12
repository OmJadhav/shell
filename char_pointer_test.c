#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  char *str1;
  char *strPtr[5];
  //char **str3;
  str1 = "first String";
  //*str3 = "third string";
  strPtr[0] = "second array string";
  printf("strings  %s %s ",str1, *strPtr);
  return 0;
}
