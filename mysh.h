#define CLENGTH 513
#define MAXJOBS 1000

typedef struct _shell_job {
  int jid; 
  int pid;
  char name[CLENGTH];
  int is_terminated;
} shell_job;
