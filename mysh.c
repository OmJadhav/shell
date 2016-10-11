#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> // for strdup
#include <sys/wait.h>
#include <sys/wait.h>
#include "mysh.h"

// array of job structures
shell_job job[MAXJOBS];
int global_jid = 0;

int
main(int argc, char *argv[])
{
  
  printf("1: %d\n", (int) getpid());
  // check for interactive mode or batch mode
  
  // if batch mode 
      // batch mode code
  // else interacctive mode 
  char promt_buffer[] = "mysh> ";
  char input_buffer[CLENGTH + 1];
  char error_buffer[500];
  char msg_buffer[500];
  char *tokens[512];
  char delimit[]=" \t\r\n\v\f";
  char *ch_ptr;
  

  while(1) {
      
    // print promt
    //  write(STDOUT_FILENO, buffer, sizeof(char) * strlen(t));
    write(STDOUT_FILENO, promt_buffer, sizeof(char)*6);

    // check for input and tokenise it
    //t = fgets(buffer, 513, stdin)
    ch_ptr =  fgets(input_buffer, CLENGTH + 1, stdin);    
    if (ch_ptr == NULL) {
        perror("error while reading from promt / terminated prog with ctrl+d");
        exit(0); // check with this later
    } else {
        puts(ch_ptr); 
        // tokenise the string - remove extra spaces, 
        int i = 0;
        tokens[i]  = strtok(input_buffer, delimit);
        while(tokens[i]) {
            printf("token : ");
            puts(tokens[i]);
            if (strcmp(tokens[i], "exit") == 0) {
                // exit is occured - terminate
                strcpy(msg_buffer, "exit is called");
                write(STDOUT_FILENO, msg_buffer, sizeof(char)*strlen(msg_buffer));
                exit(0);
            }
           i++;
           tokens[i] = strtok(NULL, delimit);
        } // end while loop for tokens
        // else - if it's one of the shell command act accordingly
        // tokens j = 0  to j < i -- as last one is null
        if (((strcmp(tokens[0], "j") == 0) && (i == 1)) ||
              ((strcmp(tokens[0], "j") == 0) && 
                  (strcmp(tokens[i - 1], "&" ) == 0) && (i == 2))) {
          // print all runnin
          printf("print all running jobs here\n");
          
        } else if (((strcmp(tokens[0], "myw") == 0) && (i == 2)) ||                          
                    ((strcmp(tokens[0], "myw") == 0) &&                                     
                      (strcmp(tokens[i - 1], "&") == 0) && (i == 3))) {                        
            // implementation of myw
            // check for second token it should be valid jid                                                    
            printf("myw callled with one more arg\n");                               
                                                                                 
        } else { 
            // else - call fork() 
            // take care of command myw only here 
            printf("call fork \n");
            int rc = fork();
    

            // else call fork 
            // check for bg
                       
            // in child
            if (rc == 0) {
              printf("in child\n");
              int is_bg = 0;
              int len = i; 
              if(strcmp(tokens[i-1], "&") == 0)
                is_bg = 1;

              if(is_bg == 0) {
                printf("foreground job\n");
                // int execvp(const char *file, char *const argv[]);
                char **argv = malloc(sizeof(char*) * len);
                int j;
                for (j = 0; j < len; j++) {
                  argv[j] = tokens[j];
                } 
                argv[j] = NULL;
                execvp(argv[0], argv);
                perror("exec failed\n");
              } else {
                printf("background job]\n");
                char **argv = malloc(sizeof(char*) * len);                       
                int j;                                                           
                for (j = 0; j < len - 1; j++) {                                      
                  argv[j] = tokens[j];                                           
                }                                                                
                argv[j - 1] = NULL;                                                  
                execvp(argv[0], argv);                                           
                perror("exec failed\n");
              }
            } // end of child
            
            // setup environmemt for exec 
                  // - setup argv with last arg as NULL

            // call exec - deped 
            //check for is_bg

        // in parent 
            // wait for child - void wait() if is_bg = 0

        }

    }  

/*    int rc = fork();
    printf("2: %d [rc:%d]\n", (int) getpid(), rc);

    if (rc == 0) {
      // child
      // int execvp(const char *file, char *const argv[]);

      char *argv[4]; //stack allocated
      argv[0] = strdup("/bin/ls");
      argv[1] = strdup("-l");
      argv[2] = NULL;

      execvp(argv[0], argv);

      // when exec failed
      perror("exec failed");

      printf("child");
    } else if (rc > 0) {
      // parent

    } else {
      perror("fork");
    }
*/
  } // end of while loop

  return 0;
}
