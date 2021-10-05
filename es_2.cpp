#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "error.h"
#include "es_1.h"

// ����������� ���� ������=��������
// ������1[)]=������2[,|.] O�����=������1, ��������=������2
void split(char* line,   // ������� ������ - �� �������� ��������
	   char* ob,     // ������
	   char* va)     // ��������
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
     /* �������� ����� � ����� ������� */
     p1--;
     if (*p1 == '.')
	   n--;
     strncpy_s(va,VALUELEN, p2,n); va[n]=0;
}

// ����� � ������� ������ ������������ �������
// ������ ����� ��������� ��������� ",��="
int get_cf(char* line)
{
  char* p = strstr(line,"cf="); // strstr(s1,s2) - ����� ��������� s2 � ������ s1
  if (p)
      { p++;p++;p++;
	return atoi(p);
      }
  else return 100; // ���� ������� �� ���������
}

// ����������� ������� ������� � ������
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

