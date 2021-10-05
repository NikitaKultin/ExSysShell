#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "error.h"
#include "es_1.h"

// Расщепление пары ОБЪЕКТ=ЗНАЧЕНИЕ
// строка1[)]=строка2[,|.] OБЪЕКТ=строка1, ЗНАЧЕНИЕ=строка2
void split(char* line,   // входная строка - не содержит пробелов
	   char* ob,     // объект
	   char* va)     // значение
{
     char* p1 = line;
     int   n = 0;
     while ((*p1 != ')')&&(*p1 != '=')&&(*p1))
     {
	 p1++; n++;
     }
     strncpy_s(ob,OBJECTLEN,line,n); 
     ob[n]=0;
     if (*p1 == ')') p1++;
     if (*p1 == '=') p1++;
     char* p2 = p1;
     n=0;
     while ((*p1 != ',')&&(*p1))
     {
	 p1++; n++;
     }
     /* удаление точки в конце правила */
     p1--;
     if (*p1 == '.')
	   n--;
     strncpy_s(va,VALUELEN, p2,n); va[n]=0;
}

// Поиск в входной строке коэффициента доверия
// строка может содержать подстроку ",кд="
int get_cf(char* line)
{
  char* p = strstr(line,"cf="); // strstr(s1,s2) - найти подстроку s2 в строке s1
  if (p)
      { p++;p++;p++;
	return atoi(p);
      }
  else return 100; // коэф доверия по умолчанию
}

// определение позиции символа в строке
int strpos(char* st,char c)
{
  char* p = st;
  int n = 0;
  while ((*p != c)&&(*p))
  {
    p++;n++;
  }
  if (!*p) n=0;
  return n;
}

