#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main()
{
  char input_buffer[513];
  char *ch_ptr, *token;
  ch_ptr =  fgets(input_buffer, 512 + 1, stdin); 
  puts(ch_ptr);
  puts(input_buffer);
  token = strtok(input_buffer, " ");
  while(token) {
    puts(token); 
    if (strcmp(token, "exit") == 0) {
        puts("exit");
        
    }
    token = strtok(NULL, " ");
  }
  return 0;
}
