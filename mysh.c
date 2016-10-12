#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> // for strdup
#include <sys/wait.h>
#include <sys/wait.h>
#include "mysh.h"

// array of job structures
shell_job jobs[MAXJOBS];
int jid = 1;
int end = -1;
char job_details[10000];

void
append_job_to_str(int index) {
  // write(STDOUT_FILENO, promt_buffer, sizeof(char)*6);
  char temp_str[CLENGTH + 20];
  sprintf(temp_str, "%s %d\n", jobs[index].name, jobs[index].jid);
  strcat(job_details, temp_str);
  //strcpy(job_details, jobs[index].name);
  //strcat(job_details, " ");
  //strcat(job_details, jobs[index].jid);
  //strcat(job_details, "\n");
  //write(STDOUT_FILENO, job_details, sizeof(char)*strlen(job_details));
}

void
print_bg_jobs() {
  // loop through the array
  int i;
  strcpy(job_details, "");
  for (i = 0; i <= end; i++) {
    append_job_to_str(i);  
  }
  // write job_details
  write(STDOUT_FILENO, job_details, sizeof(char)*strlen(job_details));
}

void
print_bg_jobs2() {
    // loop through the array
    int i, status, ret;
    strcpy(job_details, "");
    for (i = 0; i <= end; i++) {
      // check if it still running
      ret = waitpid(jobs[i].pid, &status, WNOHANG);
      if (ret == 0) { 
        jobs[i].is_terminated = 0; // still running 
        append_job_to_str(i);  
      } else if (ret > 0) {
        jobs[i].is_terminated = 1; // terminated
        // swap with old job and end--
        jobs[i] = jobs[end];
        end--;
        // check this index again as new job is arrived at this 
        i--;
      } 
    }
    // write job_details
    write(STDOUT_FILENO, job_details, sizeof(char)*strlen(job_details));
}

void clean_up_bg_jobs() {
  int i, status, ret;
  for (i = 0; i <= end; i++) {
    // check if it still running
    ret = waitpid(jobs[i].pid, &status, WNOHANG);
    if (ret == 0) { 
      jobs[i].is_terminated = 0; // still running 
    } else if (ret > 0) {
      jobs[i].is_terminated = 1; // terminated
      // swap with old job and end--
      jobs[i] = jobs[end];
      end--;
      // check this index again as new job is arrived at this 
      i--;
    } 
  }
   
}

void
print_jobs_array() {
  
  printf("in printing end : %d\n", end);
  int i;
  for (i = 0; i <= end; i++) {
    printf("name : %s jid : %d pid : %d is_terminated : %d \n", jobs[i].name,
jobs[i].jid, jobs[i].pid, jobs[i].is_terminated);
  }
}



int
main(int argc, char *argv[])
{
  
  printf("1: %d\n", (int) getpid());
  // check for interactive mode or batch mode
  
  // if batch mode 
      // batch mode code
  // else interacctive mode 
  char promt_buffer[] = "mysh> ";
  char input_buffer[CLENGTH];
  char error_buffer[500];
  char msg_buffer[500];
  char *tokens[CLENGTH];
  char temp_job_name[CLENGTH];
  char delimit[]=" \t\r\n\v\f";
  char *ch_ptr;
  int is_bg = 0;
  int i, rc, len;

  memset(&jobs, 0, sizeof(jobs)); 

  while(1) {
     
    //printf("parent:  pid: %d\n", (int) getpid()); 
    // print promt
    //  write(STDOUT_FILENO, buffer, sizeof(char) * strlen(t));
    write(STDOUT_FILENO, promt_buffer, sizeof(char)*6);

    // check for input and tokenise it
    //t = fgets(buffer, 513, stdin)
    ch_ptr =  fgets(input_buffer, CLENGTH, stdin);    
    if (ch_ptr == NULL) {
        perror("error while reading from promt / terminated prog with ctrl+d");
        exit(0); // check with this later
    } else {
        //puts(input_buffer); 
        // tokenise the string - remove extra spaces, 
        strcpy(temp_job_name,"");
        i = 0;
        tokens[i]  = strtok(input_buffer, delimit);
        while(tokens[i]) {
            //printf("token : ");
            //puts(tokens[i]);
           if (i != 0)
              strcat(temp_job_name, " ");
           strcat(temp_job_name, tokens[i]);
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
            clean_up_bg_jobs();
            print_bg_jobs();
            printf("\nnow the array\n");
            print_jobs_array();

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
            
            //printf("call fork \n");
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
                printf("background job]\n");
                /*char **argv_c = malloc(sizeof(char*) * (len - 1));                       
                int j;                                                           
                for (j = 0; j < (len - 1); j++) {                                      
                  argv_c[j] = tokens[j];                                           
                }                                                                
                argv_c[j] = NULL; */
                // clean up jobs
                clean_up_bg_jobs();
                // initialize structure node and add to array
                printf("end < MAx end : %d\n", end);
                tokens[i - 1] = NULL;                                   
                // fill the tokens and call execvp
                execvp(tokens[0], tokens);                                           
                perror("exec failed\n");
                // free memory ?? do we need to - dint use malloc

               
                //printf("exit from child");
                exit(0);
              }
            } else if(rc > 0) {
              
              // parent
              //printf("in parent again pid: %d and child :rc: %d\n", (int)getpid(), rc);  
              int status;
              if(!is_bg) {
                //  pid_t waitpid(pid_t pid, int *status, int options);
                int c_pid = waitpid(rc, &status, 0);
                if (c_pid != rc)
                  perror("error while waiting in parent for foreground job");
              } else {
                  // background job
                  end++;
                  jobs[end].jid = jid;
                  int temp_len = strlen(temp_job_name);
                  temp_job_name[temp_len - 2] = '\0';
                  strcpy(jobs[end].name, temp_job_name);
                  jobs[end].is_terminated = 0; 
                  printf("increment end \n");
                  printf("now end is %d \n",end);
                  jobs[end].pid =  rc;
                  jid++;
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
