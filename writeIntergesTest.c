#include <stdio.h>
#include <stdlib.h>

int main()
{
  // both works 
  //int a = 50;
 // write(1, &a, 1);

  char a = '2';
  write(1, &a, 1);
 
  return 0;
}
