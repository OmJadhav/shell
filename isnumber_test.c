#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
  int a[10];
  memset(a, -1, sizeof(a));
 char str[3] = "12";
  int b = (int) str;
  
 int c =  isdigit(str[0]);  
 printf("%d \n", c);
 return 0;
}
