#define CLENGTH 512
#define MAXJOBS 32

typedef struct _shell_job {
  int jid; 
  char *name;
  int is_bg;
} shell_job;
