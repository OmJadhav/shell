#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

int global =10;

if(fork() == 0) {
global = 101; 
printf("glbal in child is %d\n", global);
} else {
int i=100000;
while(i) {
i--;
}
printf("in parent global : %d", global);
}

return 0;
}
