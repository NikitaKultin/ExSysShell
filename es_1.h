// es_1.h : определения для экспертной системы
// Культин Н.Б. (c) 1992-1994

#ifndef ES_1
#define ES_1

#define DEFINITE 100
#define BUF_SIZE 100
#define OBJECTLEN 30
#define VALUELEN 30

#define C_MULTIVALD "multivald"
#define C_QUESTION  "question"
#define C_LEGAL     "legal"
#define C_RULE      "rule"
#define C_THEN      "then"
#define C_OBJECT    "object"

struct VALUE {         // значение  объекта
   char*  name;        // имя
   char*  setby;
   int    cert;        // коэф доверия
   VALUE* next;
  public:
   VALUE(const char*);       // значение
   ~VALUE();
   void add_cf(int);   // добавить(изменить) коэф доверия
   void print();
};

class VALUE_LIST {     // список значений имеет только многозначный объект
  public:
   VALUE* head;
   VALUE_LIST() {head = 0;}
   VALUE* add_value(const char*);     // добавить значение
   VALUE* test(const char*);          // проверить значение
   void   clear_value();
   void   print();
};

struct L_VALUE {       // разрешенное значение для объекта
   char*  name;
   L_VALUE* next;
  public:
   L_VALUE(char*);
};

class LEGAL_LIST {      // список разрешенных значений для объекта
  public:
   L_VALUE* head;
  public:
   LEGAL_LIST() {head = 0;}
   L_VALUE* add(char*);       // добавить к списку
   L_VALUE* get_l_value(int); // вернуть разрешенное значение
};

struct OBJECT {              // объект
    char* name;              // имя
    char* question;          // вопрос выводимый при определении значения
    int   multivald;         // признак многозначности
    int   sought;            // признак установленного значения
    LEGAL_LIST  legal_list;  // список разрешеных значений
    VALUE_LIST  value_list;  // список значений, если многозначный
    OBJECT* next;
  public:
    OBJECT(char*);	          // конструктор
    VALUE*   add_value(char*);    // добавить значение
    L_VALUE* add_l_value(char*);  // добавить разрешенное значение
    VALUE*   test_value(char*);   // проверить значение
    VALUE*   get_value(int);      // получить значение с порядковым номером в списке
    void     set_multivald();     // объявить многозначным
    void     add_cf(char*,int);	  // установить(изменить)коэф достоверности
    void     add_question(char*); // добавить вопрос об объекте
    void     ask();               // установить значения мспользуя legal_list
    void     clear_value();
    void     print();
};

// список объектов
class OBJECT_LIST {
  public:
    OBJECT* head;
  public:
    OBJECT_LIST() { head = 0;};          // конструктор
    int     ok_add(char*, const int);    // проверить право добавления значения объекта
    OBJECT* add_object(char*,const char*,int); // добавить: объект,значение,коэф уверенности
    OBJECT* find(const char*);                 // найти  объект в списке
    VALUE*  test_value(char*, char*);    // проверить объект-значение
    OBJECT* add_question(char*,char*);
    OBJECT* set_multivald(char*);        // объявить многозначным
    void    clear_value();                     // очистить значения
    void    print();
};

struct PREM {		// предпосылка правила
    char* object;       // объект
    char* value;        // значение
    PREM* next;
   public:
    PREM(char*,char*);
    ~PREM();
    void print();
};

class PREM_LIST {       // список предпосылок
  public:
    PREM* head;
  public:
    PREM_LIST() { head = 0;}
    ~PREM_LIST();
    void add_prem(char*,char*);
    void print();
};

struct CON {		// заключение
   char*  object;       // объект
   char*  value;        // значение
   int    cert;         // коэф. доверия
   CON*   next;
  public:
   CON(char*, char*,int);
   ~CON();
   void print();
};

class CON_LIST {        // список заключений
  public:
   CON* head;
  public:
   CON_LIST() { head = 0;}
   ~CON_LIST();
   int find_con(char*); // найти предпосылку
   void add_con(char*, char*,int);
   void print();
};

struct RULE {		// правило
   char* name;          // имя
   PREM_LIST prem;      // список предпосылок
   CON_LIST  con;       // список заключений
   RULE*     next;
 public:
   RULE(char*);
   ~RULE();
   void add_con(char*,char*,int);  // добавить заключение
   void add_prem(char*,char*);     // добавить предпосылку
   RULE* find_rule(char*);         // найти правило с заданным заключением
   void print();
};

class RULE_LIST {       // список правил
  public:
   RULE* head;
  public:
   RULE_LIST() { head = 0;}
   RULE* add_rule(char*);
   void  clear();
   void  print();
};

class KNOWN_BASE {   // база знаний
    int trass;             // трассировка машины вывода
  public:
    OBJECT_LIST ol;        // список объектов
    RULE_LIST   rl;        // список правил
    //KNOWN_BASE(){trass=1;};
    KNOWN_BASE(){trass=0;};
    int  read_file(char*); // чтение файла базы знаний

    // Рекурсивная !!
    void pursue(char*); // достижение цели - консультация
    // Рекурсивная !!
};
#endif