// Экспертная система. Управление простым меню.
// Тестирование классов.

#include <iostream.h>
#include <string.h>
#include <stdlib.h>

#include "es_1.h"
#include "es_2.h"
#include "error.h"

char buf[20];  // буфер входной строки
char kb_name[20];
char* ed_name = "ne ";
char ob[20];
char va[20];
char i='1';    // пункт меню

int main()
{
  KNOWN_BASE kb;  // база знаний
  OBJECT* p;
  OBJECT* po;
  VALUE*  pv;

  while (i!='0') {
     cout << "\n1 - Добавление объекта (объект=значение[,кд=ЦЕЛОЕ])\n";
     cout << "2 - Просмотр объектов\n";
     cout << "3 - Очистка значений объктов\n";
     cout << "4 - Чтение файла правил базы знаний\n";
     cout << "5 - Редактирование правил\n";
     cout << "6 - Удаление правил\n";
     cout << "7 - Консультация\n";
     cout << "\nВаш выбор? ";
     cin >> i;
     switch (i) {

       case '1':cout << "Добавление факта.\n";
		cin >> buf;
		split(buf,ob,va);
		int cf=get_cf(buf);
		kb.ol.add_object(ob,va,cf);
		break;

       case '2':
		cout << "Факты базы знаний.\n";
		kb.ol.print();
		break;

       case '3' :
		kb.ol.clear_value();
		break;

       case '4' :
		cout << "Имя файла ->";
		cin >> kb_name;
		kb.read_file(kb_name);
		p=kb.ol.find("цель");
		if (p) {
		   pv = p->get_value(1);
		   while(pv) {
		     kb.pursue(pv->name);
		     pv=pv->next;
		   }
                   cout << "\n*** РЕЗУЛЬТАТ КОНСУЛЬТАЦИИ ***\n";
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
		cout << "Имя объекта:";
		cin >> buf;
		kb.pursue(buf);
                cout << "\n*** РЕЗУЛЬТАТ КОНСУЛЬТАЦИИ ***\n";
                po=kb.ol.find(buf);
                (*po).print();
		break;

     }
   }
   return 0;
}