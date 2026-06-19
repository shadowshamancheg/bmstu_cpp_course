#include "str2int.h"
#include <assert.h>
#include "stdio.h"

int str2int(const char* str)
{
	if(str == 0){ return 0;}
  int znak = 1 , result = 0, i = 0;
  while (str[i]==' '){i++;}
      
  if( str[i]== '-') {znak = -1; i++; }
    while (str[i] != '\0')
    {
      result = result * 10 + (str[i] - '0');
      i++;
    }
    return result * znak;
}
 
