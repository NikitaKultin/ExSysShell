// A rule based expert system shell

#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "es_1.h"
#include "es_2.h"
#include "error.h"




char buf[BUF_SIZE];  // буфер входной строки
char kb_name[128];
char ed_name[4] = "ne ";
char ob[OBJECTLEN];
char va[VALUELEN];
char i = '1';    // пункт меню



int main()
{
    std::cout << "A rule based expert system shell\n(c) Nikita Kultin, 2021\n\n";
	KNOWN_BASE kb;  // база знаний
	OBJECT* p;
	OBJECT* po;
	VALUE* pv;

	int cf;

	while (i != '0') {
		std::cout << "\n1 - Add object (object=value[,cf=integer])\n";
		std::cout << "2 - Show objects\n";
		std::cout << "3 - Clear objects values\n";
		std::cout << "4 - Load knowledge base from file\n";
		std::cout << "5 - Show rules\n";
		std::cout << "6 - Delete rulles\n";
		std::cout << "7 - Consultation\n";
		std::cout << "0 - Exit\n";
		std::cout << "\nYour chose? ";
		std::cin >> i;
		switch (i) {

		case '1':std::cout << "Adding a fact\n";
			std::cin >> buf;
			split(buf, ob, va);
			cf = get_cf(buf);
			kb.ol.add_object(ob, va, cf);
			break;

		case '2':
			std::cout << "Knowledge base facts\n";
			kb.ol.print();
			break;

		case '3':
			kb.ol.clear_value();
			break;

		case '4':
			std::cout << "Knowledge base full file name ->";
			std::cin >> kb_name;
			kb.read_file(kb_name);
			p = kb.ol.find("goal");
			if (p) {
				pv = p->get_value(1);
				while (pv) {
					kb.pursue(pv->name);
					pv = pv->next;
				}
				std::cout << "\n*** CONSULTATION RESULT ***\n";
				pv = p->get_value(1);
				while (pv) {
					po = kb.ol.find(pv->name);
					(*po).print();
					pv = pv->next;
				}
			}

			break;
		case '5':
			kb.rl.print();
			/*
			kb.rl.clear();
			strcpy_s(buf, strlen(buf), ed_name);
			strcat_s(buf, kb_name);
			if (!system(buf)) kb.read_file(kb_name);
			*/
			break;
		case '6':
			kb.rl.clear();
			break;
		case '7':
			std::cout << "Object name:";
			std::cin >> buf;
			kb.pursue(buf);
			std::cout << "\n*** CONSULTATION RESULT ***\n";
			po = kb.ol.find(buf);
			(*po).print();
			break;

		}
	}

	//std::cout << "\nA rule based expert system shell\n(c) Nikita Kultin, 2021\n\nPress <Enter>";
	//std::cin >> i;
	return 0;

}

