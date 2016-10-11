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
  int is_bg = 0;
  int i, rc, len;

  while(1) {
     
    //printf("parent:  pid: %d\n", (int) getpid()); 
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
        //puts(ch_ptr); 
        // tokenise the string - remove extra spaces, 
        i = 0;
        tokens[i]  = strtok(input_buffer, delimit);
        while(tokens[i]) {
            //printf("token : ");
            //puts(tokens[i]);
           i++;
           tokens[i] = strtok(NULL, delimit);
        } // end while loop for tokens
        // else - if it's one of the shell command act accordingly
        // tokens j = 0  to j < i -- as last one is null
        if(tokens[0]) {

          if ((strcmp(tokens[0], "exit") == 0) && (tokens[1] == NULL)) {                                
            // exit is occured - terminate
            // printf("in exit\n");                                   
            // strcpy(msg_buffer, "exit is called");                            
            //write(STDOUT_FILENO, msg_buffer,
            //    sizeof(char)*strlen(msg_buffer));
            exit(0);  
            printf("after exit call\n");                                                       
          }     

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

            // used by both parent and children
            
            is_bg = 0;
            // bg check                                                        
            if(strcmp(tokens[i-1], "&") == 0)                                  
              is_bg = 1;
            
            printf("call fork \n");
            rc = fork();


            // else call fork 
            // check for bg

            // in child
            if (rc == 0) {
              //printf("in child\n pid :%d\n", (int)getpid());
              len = i; 
              
              // foreground 
              if(is_bg == 0) {
                //printf("foreground job\n");
                // int execvp(const char *file, char *const argv[]);
                /*char **argv_c = malloc(sizeof(char*) * len);
                int j;
                for (j = 0; j < len; j++) {
                  argv_c[j] = tokens[j];
                } 
                argv_c[j] = NULL;
                */
                
                execvp(tokens[0], tokens);
                perror("exec failed\n");
                printf("exiting child \n");
                exit(0);
                // free memory
                // free(argv_c);
                
              } else {
      
                // background
                //printf("background job]\n");
                /*char **argv_c = malloc(sizeof(char*) * (len - 1));                       
                int j;                                                           
                for (j = 0; j < (len - 1); j++) {                                      
                  argv_c[j] = tokens[j];                                           
                }                                                                
                argv_c[j] = NULL; */
                tokens[i - 1] = NULL;                                   
                execvp(tokens[0], tokens);                                           
                perror("exec failed\n");
                // free memory
                printf("exit from child");
                exit(0);
              }
            } else if(rc > 0) {
              
              // parent
              //printf("in parent again pid: %d and child :rc: %d\n", (int)getpid(), rc);  
              int status;
              if(!is_bg) {
                //  pid_t waitpid(pid_t pid, int *status, int options);
                int c_pid = waitpid(rc, &status, 0);
              }
            } else {
                
                // fork error
                printf("error in calling fork()");
            }

            // setup environmemt for exec 
            // - setup argv with last arg as NULL

            // call exec - deped 
            //check for is_bg

            // in parent 
            // wait for child - void wait() if is_bg = 0

          }
        }

    }  

 } // end of while loop

  return 0;
}
