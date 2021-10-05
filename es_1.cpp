// es_1.cpp : Описания классов для экспертной системы
// Объявления в файле es_1.h

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "es_1.h"
#include "es_2.h"
#include "error.h"

#define  DOS

VALUE::VALUE(const char* s)
{
   name = new char[strlen(s)+1];
   strcpy_s(name, strlen(s) + 1, s);
   setby=0;
   cert=0;
   next=0;
}

VALUE::~VALUE() {
   delete name;
}

void VALUE::add_cf(int cf) // добавить(изменить) коэф доверия
{
  cert=((100*cert)+(100*cf)-(cert*cf))/100;
}

void VALUE::print()
{
  #ifdef DOS
    std::cout << '\t' << name << " cf="<< cert  << '\n';
  #endif
}

void VALUE_LIST::clear_value(){
    while(head) {
      VALUE* p = head;
      head = p->next;
      delete p;
    }
}

VALUE* VALUE_LIST::add_value(const char* va) {
      if (!strlen(va)) return 0;
      if (!head) return head = new VALUE(va);
      VALUE* p = head;
      while (p) {
	 if (!strcmp(p->name,va)) { p=0; break; }
	 if (!p->next) break;
	 p=p->next;
      }
      if (!p) return 0;
      p->next=new VALUE(va);
      return p->next;


}

VALUE* VALUE_LIST::test(const char*va) // проверить значение
{
   VALUE* p = head;
   while (p) {
      if (strcmp(p->name,va)) p = p->next;
	    else break;
   }
   return p;
}
void VALUE_LIST::print()
{
   VALUE* p = head;
   if (p) while(p) {
	      p->print();
	      p=p->next;
	   }
    #ifdef DOS
      else std::cout<<"\tundefined\n";
    #endif
}

L_VALUE::L_VALUE(char* v)
{
   name = new char[strlen(v)+1];
   strcpy_s(name, strlen(v) + 1, v);
   next=0;
}

L_VALUE* LEGAL_LIST::add(char* va) // добавить в список, если в списке нет
{
  L_VALUE* p = head;
  while (p)
      if (!strcmp(p->name,va)) break;
	  else p=p->next;
  if (!p) {
       p = new L_VALUE(va);
       p->next = head;
       head = p;
    }
  return p;
}

L_VALUE* LEGAL_LIST::get_l_value(int n)
{
      L_VALUE* p = head;
      n--;
      while(n) {
	 p=p->next;
	 if (!p) break;
	 n--;
      }
      return p;
}

OBJECT::OBJECT(char* ob)
{
   name = new char[strlen(ob)+1];
   strcpy_s(name, strlen(ob) + 1, ob);
   question=0;
   multivald=sought=0;
   // голова списка значений инициализируется при создании
   // объекта VALUE_LIST
   next=0;
}

VALUE* OBJECT::add_value(char* va) // добавить значение
{

    VALUE* p = value_list.add_value(va);
    return p;
}

VALUE* OBJECT::get_value(int n) {
   int i = 1;
   VALUE* p = value_list.head;
   while(i<n) {
     p=p->next;
     i++; if(!p) break;
   }
   return p;
}

void OBJECT::set_multivald() // объявить многозначным
{
    multivald=1;
}
void OBJECT::add_cf(char* va, int cf)
{
  VALUE* p=value_list.test(va);
  if(p)
     p->add_cf(cf);
}
void OBJECT::add_question(char* q)
{
   question = new char[strlen(q)+1];
   strcpy_s(question, strlen(q) + 1, q);
}

L_VALUE* OBJECT::add_l_value(char* va) // добавить разрешенное значение
{
    L_VALUE* p = legal_list.add(va);
    return p;
}
VALUE* OBJECT::test_value(char* va) // проверить значение
{
    VALUE* p = value_list.test(va);
    return p;
}

void OBJECT::ask()
{
  #ifdef DOS

    if (question != NULL) std::cout << question <<'\n';
	else std::cout << "Object value " << name << " ?\n";
    int n = 1;
    L_VALUE *p;
    while((p=legal_list.get_l_value(n))!=0) {
	std::cout << n <<". "<<p->name<<'\n';
	n++;
    }
    n--;
    char buf[20];

    if (n)
      {	std::cout <<"Your choice (1.."<<n<<") or 0  ";
	std::cin >> buf;
	n = atoi(buf);
	if (n) {
	   p=legal_list.get_l_value(n);
	   this->add_value(p->name);
	   this->add_cf(p->name,100);
	}
      }
    else { std::cout <<"Enter value ";
	   std::cin >> buf;
	   this->add_value(buf);
	   this->add_cf(buf,100);
    }
  #endif
}

void OBJECT::clear_value() {
   value_list.clear_value();
   sought = 0;
}

void OBJECT::print()
{
   std::cout << name;
   value_list.print();
}

int OBJECT_LIST::ok_add(char* ob, const int cf)
{
   int is_100 = 0;        // в списке значений есть объекты с кд = 100
   OBJECT* po =find(ob);
   if (po) {
      VALUE* pv = po->value_list.head;
      while (pv) {
	 if ( pv->cert == DEFINITE )
	    { is_100=1;break; }
	 pv = pv->next;
      }
   }
   if ((cf == DEFINITE)&&(is_100 == 1)&&(!po->multivald))
	return 0; // добавлять нельзя
   return 1;      // добавлять можно
}

VALUE*  OBJECT_LIST::test_value(char* ob, char* va)
{
   OBJECT* po = this->find(ob);
   VALUE* pv = NULL;
   if (po) pv = po->test_value(va);
   return pv;
}
OBJECT* OBJECT_LIST::add_object(char* ob, const char* va, int cf)
{
    OBJECT* po = this->find(ob);
    if (!po) 
    {
      if (!head) 
      {
	        po=new OBJECT(ob);
	        head = po;
      }
      else
      {
	        po = head;
	        while (po->next)
	            po=po->next;
	        po->next = new OBJECT(ob);
	        po = po->next;
      }
    }
    if (this->ok_add(ob, cf))
    {
        VALUE* pv = po->value_list.test(va);
        if (!pv)
             pv = po->value_list.add_value(va);
        if ( pv ) 
            pv->add_cf(cf);
        return po;
        
    }
    error("You cannot add an object");
    po = 0;
    return po;
}

OBJECT* OBJECT_LIST::find(const char* ob)
{
   OBJECT* p = head;
   while(p)  {
      if (strcmp(p->name,ob) == 0) break;
      p = p->next;
   }
   return p;
}

OBJECT* OBJECT_LIST::add_question(char* ob, char* q)
{
  OBJECT* p = this->find(ob);
  if (!p) p=this->add_object(ob,"",0);
  p->add_question(q);
  return p;
}

OBJECT* OBJECT_LIST::set_multivald(char* ob)
{
   OBJECT* p = this->find(ob);
   if (!p) p=this->add_object(ob,"",0);
   p->set_multivald();
   return p;
}

void OBJECT_LIST::clear_value() {
    OBJECT* p = head;
    while (p) {
       p->clear_value();
       p=p->next;
   }
}
void OBJECT_LIST::print() {
    OBJECT* p = head;
    while(p) {
       p->print();
       p=p->next;
    }
}

PREM::PREM(char* ob,char* va) {
   object = new char[strlen(ob)+1];
   strcpy_s(object, strlen(ob) + 1, ob);
   value = new char[strlen(va)+1];
   strcpy_s(value, strlen(va) + 1, va);
   next=0;
}
PREM::~PREM() {
    delete object;
    delete value;
}

void PREM::print() {
    std::cout<< object << "=" << value <<'\n';
}

PREM_LIST::~PREM_LIST() {
   while (head) {
     PREM* p = head;
     head=p->next;
     delete p;
   }
}

void PREM_LIST::add_prem(char* ob,char* va)
{
   if (!head)
      head = new PREM(ob,va);
   else {
     PREM* p = head;
     while (p->next) {
       p=p->next;
     }
     p->next = new PREM(ob,va);
   }
}

void PREM_LIST::print()
{
    PREM* p = head;
    while (p) {
       p->print();
       p=p->next;
    }
}

CON::CON(char* ob, char* va, int cf) {
   object = new char[strlen(ob)+1];
   strcpy_s(object, strlen(ob) + 1, ob);
   value = new char[strlen(va)+1];
   strcpy_s(value, strlen(va) + 1, va);
   cert=cf;
   next=0;
}

CON::~CON() {
   delete object;
   delete value;
}

void CON::print() {
   #ifdef DOS
     std::cout<< object << "=" << value << ",cf="<< cert <<'\n';
   #endif
}

CON_LIST::~CON_LIST() {
   while(head) {
     CON* p =head;
     head=p->next;
     delete p;
   }
}

int  CON_LIST::find_con(char* f_con)
{
    int found = 0;
    CON* p = head;
    while (p) {
    if (!strcmp(f_con,p->object)) {
	found = 1;
	break;
      }
      p=p->next;
    }
    return found;
}

void CON_LIST::add_con(char* ob,char* va,int cf) {
   if (!head)
      head = new CON(ob,va,cf);
   else {
     CON* p = head;
     while (p->next) {
       p=p->next;
     }
     p->next = new CON(ob,va,cf);
  }
}

void CON_LIST::print() {
    CON* p = head;
    while (p) {
       p->print();
       p=p->next;
    }
}

RULE::RULE(char* n) {
   name = new char[strlen(n)+1];
   strcpy_s(name, strlen(n) + 1, n);
   next=NULL;
}

RULE::~RULE() {
  delete name;
}

RULE* RULE::find_rule(char* f_con)
// найти правило с заданной предпосылкой
// просматривать с этого(this) правила
{
   RULE* p = this;
   while (p) {
	int found = p->con.find_con(f_con);
	if (!found) p=p->next;
	    else break;
   }
   return p;
}

void RULE::add_con(char* ob, char* va,int cf)
{
   con.add_con(ob,va,cf);
}

void RULE::add_prem(char* ob, char* va)
{
   prem.add_prem(ob,va);
}

void RULE::print() {
  #ifdef DOS
   std::cout << name << "\n";
   prem.print();
   std::cout << "then\n";
   con.print();
  #endif
}

void RULE_LIST::clear() {
  while (head) {
    RULE* p = head;
    head = p->next;
    delete p;
  }
}

RULE* RULE_LIST::add_rule(char* name) {  // добавить в конец списка
   if (!head) {
      head = new RULE(name);
      return head;
   }
   RULE* p = head;
   while (p->next) {
     p=p->next;
   }
   p->next = new RULE(name);
   return p->next;
}

void RULE_LIST::print()
{
   RULE* p = head;
   while(p) {
      p->print();
      p=p->next;
   }
}

// чтение файла правил
int KNOWN_BASE::read_file(char* f_name)
{
   std::ifstream from(f_name); // входной поток

   char buf[BUF_SIZE];
   char command[BUF_SIZE];
   char ob[BUF_SIZE];
   char va[BUF_SIZE];
   char c;
   int n;

   if (!from) {
       error("File not found");
       return 1;};
   while(from.get(buf,BUF_SIZE,'\n')) {
	  from.get(c); // чтение символа-разделителя
	  n=strpos(buf,'(');
	  if (!n) n=strpos(buf,':');
	  if (!n) { error("Expected '(' or ':'\n");continue;}
	  strncpy_s(command,BUF_SIZE,buf,n);
	  command[n]=0;
	  strcpy_s(buf, strlen(buf), &buf[n+1]);
	  split(buf,ob,va);
	  if (!strcmp(command,C_RULE))
      {
	       // get rule
	       RULE* pr = this->rl.add_rule(command);
	       while (!from.eof())
           {
		       from.get(buf,BUF_SIZE,'\n');        // читать строку из файла
		       from.get(c);                   // чтение символа-разделителя
		       if ((!strcmp(buf, C_THEN))&&( strlen(C_THEN) ==strlen(buf)))
			      break;
		       split(buf,ob,va);
		       pr->prem.add_prem(ob,va);       // добавить предпосылку
	       }
	       while (!from.eof())
           {
		       from.get(buf,BUF_SIZE,'\n');    // читать строку из файла
		       from.get(c);                    // чтение символа-разделителя
		       split(buf,ob,va);
		       int cf=get_cf(buf);
		       pr->con.add_con(ob,va,cf);       // добавить заключение
		       if(buf[strlen(buf)-1]=='.') break;
	       }
	       continue; 
      };
	  if (!strcmp(command,C_MULTIVALD))
      {
		ol.set_multivald(ob);
		continue;
      };
	  if (!strcmp(command,C_QUESTION))
      {
		ol.add_question(ob,va);
		continue;
      };
	  if (!strcmp(command,C_LEGAL))
      {
		/** добавление разрешенных значений       **/
		/** объект=разрзн1,разрзн2, ...,разрзнК   **/
		OBJECT* po =ol.add_object(ob,"",0);
		n=strpos(buf,'=');n++;
		strcpy_s(buf,BUF_SIZE, &buf[n]);      // содержит строку после '='
		while(strlen(buf)) {
		   n=strpos(buf,',');
		   if (!n) n=strlen(buf);
		   strncpy_s(va,BUF_SIZE, buf,n);
		   va[n]=0;
		   po->legal_list.add(va); // добавить разрешенное значение
		   if (buf[n]=='\0') break;
		   n++;
		   strcpy_s(buf,BUF_SIZE,&buf[n]);
		};
		continue;
      };
	  if (!strcmp(command,C_OBJECT))
      {
		int cf = get_cf(buf);
		ol.add_object(ob,va,cf);
		// добавить коэф доверия
		continue;
      };

	  error("wrong statement");
   }
   return 0;
}

// машина вывода
void KNOWN_BASE::pursue(char* f_object)   // pursue - преследовать
{
  if (trass)
    std::cout <<"goal:"<<f_object<<'\n';
  int solved = 0;        // solve - решать,разрешать (проблему,задачу)
  OBJECT* curr_object = ol.find(f_object);
  if (!curr_object) curr_object = ol.add_object(f_object,"",0);
  if (!(curr_object->sought)) { // значение определено во время конультации
			      // seek (sought) - искать, разыскивать
   // начало вывода

   curr_object->sought = 1;
   RULE* curr_rule = curr_rule = rl.head->find_rule(f_object); // найти правило с заключением f_object
   if (trass)
     if (curr_rule) {
	   std::cout <<"rule is found"<<'\n';
	   curr_rule->print();
     }
   while ((curr_rule)&&(ol.ok_add(f_object,DEFINITE))) {
      PREM* curr_prem = curr_rule->prem.head;
      int bad = 0;
      int lowest = DEFINITE;
      while ((curr_prem)&&(!bad)) {
	 this->pursue(curr_prem->object);  // согласовать предпосылку правила
	 VALUE* curr_value = ol.test_value(curr_prem->object,curr_prem->value);
	 if (!curr_value) // Предпосылка не согласована, т.е. правило не подошло
	      bad = 1;
	 else  // Предпосылка согласована. Продолжить рассматривать оставшиеся
	    if (curr_value->cert < lowest )
		 lowest = curr_value->cert;
	 curr_prem = curr_prem->next;
      }
      if (trass)
	std::cout <<"rule processed"<<'\n';
      if (!bad) {
	 // Предпосылки правила согласованы, т.е. правило подошло
	 // добавить в список объектов пару "объект-значение"
	 // соответствующую заключению подошедшего правила
	 CON* curr_con = curr_rule->con.head;
	 while (curr_con) {
	      int cert = (curr_con->cert*lowest)/100;
	      if (trass)
		std::cout <<"adding an object: "<<curr_con->object<<' '<<curr_con->value<<'\n';
	      ol.add_object(curr_con->object,curr_con->value,cert);
	      curr_con = curr_con->next;
	 }
	 solved = 1;
      }
      curr_rule=curr_rule->next;
      curr_rule=curr_rule->find_rule(f_object);
   }
  if (!solved)
    // значение объекта на основе правил БЗ определить не удалось
    {    if (ol.ok_add(curr_object->name,100))
	 curr_object->ask();  // запросить значение объекта у пользователя
    }
  }
}
