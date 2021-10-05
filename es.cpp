// ���������� �������. ���������� ������� ����.
// ������������ �������.

#include <iostream.h>
#include <string.h>
#include <stdlib.h>

#include "es_1.h"
#include "es_2.h"
#include "error.h"

char buf[20];  // ����� ������� ������
char kb_name[20];
char* ed_name = "ne ";
char ob[20];
char va[20];
char i='1';    // ����� ����

int main()
{
  KNOWN_BASE kb;  // ���� ������
  OBJECT* p;
  OBJECT* po;
  VALUE*  pv;

  while (i!='0') {
     cout << "\n1 - ���������� ������� (������=��������[,��=�����])\n";
     cout << "2 - �������� ��������\n";
     cout << "3 - ������� �������� �������\n";
     cout << "4 - ������ ����� ������ ���� ������\n";
     cout << "5 - �������������� ������\n";
     cout << "6 - �������� ������\n";
     cout << "7 - ������������\n";
     cout << "\n��� �����? ";
     cin >> i;
     switch (i) {

       case '1':cout << "���������� �����.\n";
		cin >> buf;
		split(buf,ob,va);
		int cf=get_cf(buf);
		kb.ol.add_object(ob,va,cf);
		break;

       case '2':
		cout << "����� ���� ������.\n";
		kb.ol.print();
		break;

       case '3' :
		kb.ol.clear_value();
		break;

       case '4' :
		cout << "��� ����� ->";
		cin >> kb_name;
		kb.read_file(kb_name);
		p=kb.ol.find("����");
		if (p) {
		   pv = p->get_value(1);
		   while(pv) {
		     kb.pursue(pv->name);
		     pv=pv->next;
		   }
                   cout << "\n*** ��������� ������������ ***\n";
                   pv = p->get_value(1);
                   while (pv) {
                     po=kb.ol.find(pv->name);
                     (*po).print();
                     pv=pv->next;
                   }
		}

		break;
       case '5' :
		//kb.rl.print();
		kb.rl.clear();
		strcpy(buf,ed_name);
		strcat(buf,kb_name);
		if (! system(buf)) kb.read_file(kb_name);
		break;
       case '6' :
		kb.rl.clear();
		break;
       case '7' :
		cout << "��� �������:";
		cin >> buf;
		kb.pursue(buf);
                cout << "\n*** ��������� ������������ ***\n";
                po=kb.ol.find(buf);
                (*po).print();
		break;

     }
   }
   return 0;
}