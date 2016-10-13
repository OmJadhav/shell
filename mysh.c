#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/time.h>
#include "mysh.h"

// array of job structures
shell_job jobs[MAXJOBS];
int jid = 1;
int end = -1;
char job_details[10000];

// hastable for jid and pid mapping
int hash_jid_pid[1000000];

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
  //write(STDOUT_FILENO, job_details, sizeof(char) * strlen(job_details));
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
  write(STDOUT_FILENO, job_details, sizeof(char) * strlen(job_details));
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
  write(STDOUT_FILENO, job_details, sizeof(char) * strlen(job_details));
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

  int i;
  for (i = 0; i <= end; i++) {
    printf("name : %s jid : %d pid : %d is_terminated : %d \n", jobs[i].name,
        jobs[i].jid, jobs[i].pid, jobs[i].is_terminated);
  }
}


  void
usage()
{
  char error_buffer[500];
  sprintf(error_buffer, "Usage: mysh [batchfile]\n");
  write(STDERR_FILENO, error_buffer, sizeof(char) * strlen(error_buffer));
  exit(1);
}

  int
main(int argc, char *argv[])
{
  char promt_buffer[] = "mysh> ";
  char input_buffer[100000];
  char error_buffer[100000];
  char msg_buffer[100000];
  char *tokens[CLENGTH];
  char temp_job_name[CLENGTH];
  char delimit[]=" \t\r\n\v\f";
  char *ch_ptr;
  int is_bg = 0;
  int batch_mode = 0;
  int i, rc;
  FILE *fp;
  // argmunets
  if (argc > 2)
    usage();

  // open input file
  if (argc == 2) {
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
      sprintf(error_buffer, "Error: Cannot open file %s\n", argv[1]);
      write(STDERR_FILENO, error_buffer, sizeof(char) * strlen(error_buffer));
      exit(1);
    } else {
      batch_mode = 1;
    }
  }

  // initailize jobs
  memset(&jobs, 0, sizeof(jobs));

  // initializing hashmap for jid to pid
  memset(hash_jid_pid, -1, sizeof(hash_jid_pid));

  while(1) {

    //printf("parent:  pid: %d\n", (int) getpid());
    // print promt
    //  write(STDOUT_FILENO, buffer, sizeof(char) * strlen(t));
    if (batch_mode == 0)
      write(STDOUT_FILENO, promt_buffer, sizeof(char) * strlen(promt_buffer));

    // check for input and tokenise it
    //t = fgets(buffer, 513, stdin)
    if (batch_mode == 0)
      ch_ptr =  fgets(input_buffer, 100000, stdin);
    else
      ch_ptr =  fgets(input_buffer, 100000, fp);
    if (ch_ptr == NULL) {
      exit(0); // check with this later
    } else {
      // tokenise the string - remove extra spaces,

      if (batch_mode == 1) {
        write(STDOUT_FILENO, input_buffer, sizeof(char) * strlen(input_buffer));
      }
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
          exit(0);
          printf("after exit call\n");
        }

        if (((strcmp(tokens[0], "j") == 0) && (i == 1)) ||
            ((strcmp(tokens[0], "j") == 0) &&
             (strcmp(tokens[i - 1], "&" ) == 0) && (i == 2))) {
          // print all runnin
          //printf("print all running jobs here\n");
          clean_up_bg_jobs();
          print_bg_jobs();
          //printf("\nnow the array\n");
          //print_jobs_array();

        } else if (((strcmp(tokens[0], "myw") == 0) && (i == 2)) ||
            ((strcmp(tokens[0], "myw") == 0) &&
             (strcmp(tokens[i - 1], "&") == 0) && (i == 3))) {
          // implementation of myw
          // check for second token it should be valid jid
          // check if the jid is valid
          long ret_val;
          long temp_pid;
          ret_val = strtol(tokens[1], NULL, 10);
          //printf("ret value : %ld\n", ret_val);
          if (ret_val < 1000000) {
            if (hash_jid_pid[ret_val] == -1) {
              sprintf(error_buffer, "invalid jid %s\n", tokens[1]);
              write(STDERR_FILENO, error_buffer,
                  sizeof(char) * strlen(error_buffer));
            }
            else if (hash_jid_pid[ret_val] > 0) {
              // valid jid
              struct timeval tv;
              suseconds_t time1;
              suseconds_t time2;
              suseconds_t time_diff;
              gettimeofday(&tv, NULL);
              time1 = 1000000 * tv.tv_sec + tv.tv_usec;
              int status;
              temp_pid = hash_jid_pid[ret_val];
              int c_pid = waitpid(temp_pid, &status, 0);
              if (c_pid == temp_pid) {
                // still running
                gettimeofday(&tv, NULL);
                time2 = 1000000 * tv.tv_sec + tv.tv_usec;
                time_diff = time2 - time1;
                sprintf(msg_buffer, "%ld : Job %ld terminated\n", time_diff,
                    ret_val);
                write(STDOUT_FILENO, msg_buffer,
                    sizeof(char) * strlen(msg_buffer));
              } else {
                sprintf(msg_buffer, "0 : Job %ld terminated\n", ret_val);
                write(STDOUT_FILENO, msg_buffer,
                    sizeof(char) * strlen(msg_buffer));
              }
            }
          } else { // invalid jid
            sprintf(error_buffer, "invalid jid %s\n", tokens[1]);
            write(STDOUT_FILENO, error_buffer,
                sizeof(char) * strlen(error_buffer));
          }
        } else {
          // else - call fork()
          // take care of command myw only here

          // used by both parent and children

          is_bg = 0;
          // bg check
          if(strcmp(tokens[i-1], "&") == 0)
            is_bg = 1;

          rc = fork();

          // child
          if (rc == 0) {

            // foreground
            if(is_bg == 0) {
              execvp(tokens[0], tokens);
              char err_temp[] = "";
              strcpy(err_temp, temp_job_name);
              strcat(err_temp, ": command not found\n");
              write(STDERR_FILENO, err_temp,
                  sizeof(char) * strlen(err_temp));

              exit(0);

            } else {

              // background

              // clean up jobs
              clean_up_bg_jobs();

              // initialize structure node and add to array
              tokens[i - 1] = NULL;

              // fill the tokens and call execvp
              execvp(tokens[0], tokens);
              char err_temp[] = "";
              strcpy(err_temp, temp_job_name);
              strcat(err_temp, ": command not found\n");
              write(STDERR_FILENO, err_temp,
                  sizeof(char) * strlen(err_temp));
              exit(0);
            }
          } else if(rc > 0) {

            // parent
            int status;

            // foreground job
            if(!is_bg) {

              hash_jid_pid[jid] = rc;
              jid++;
              // wait
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
              jobs[end].pid =  rc;

              // add this to hashmap
              hash_jid_pid[jid] = rc;

              // increment jid for next process
              jid++;
            }
          } else {

            // fork error
            printf("error in calling fork()");
          }
        }
      }

    }

  } // end of while loop

  return 0;
}
