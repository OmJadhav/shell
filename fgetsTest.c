#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main()
{ 
  //char chArr[5] = "hell";
  char buffer[513];
  char *t;
  while(strlen(t = fgets(buffer, 513, stdin)) > 1) {
     write(STDOUT_FILENO, buffer, sizeof(char) * strlen(t));
  }
  //puts(t);
 // write(STDOUT_FILENO, buffer, sizeof(char) * strlen(t));  
  return 0;
}
