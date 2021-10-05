// es_1.h : ����������� ��� ���������� �������
// ������� �.�. (c) 1992-1994

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

struct VALUE {         // ��������  �������
   char*  name;        // ���
   char*  setby;
   int    cert;        // ���� �������
   VALUE* next;
  public:
   VALUE(const char*);       // ��������
   ~VALUE();
   void add_cf(int);   // ��������(��������) ���� �������
   void print();
};

class VALUE_LIST {     // ������ �������� ����� ������ ������������ ������
  public:
   VALUE* head;
   VALUE_LIST() {head = 0;}
   VALUE* add_value(const char*);     // �������� ��������
   VALUE* test(const char*);          // ��������� ��������
   void   clear_value();
   void   print();
};

struct L_VALUE {       // ����������� �������� ��� �������
   char*  name;
   L_VALUE* next;
  public:
   L_VALUE(char*);
};

class LEGAL_LIST {      // ������ ����������� �������� ��� �������
  public:
   L_VALUE* head;
  public:
   LEGAL_LIST() {head = 0;}
   L_VALUE* add(char*);       // �������� � ������
   L_VALUE* get_l_value(int); // ������� ����������� ��������
};

struct OBJECT {              // ������
    char* name;              // ���
    char* question;          // ������ ��������� ��� ����������� ��������
    int   multivald;         // ������� ��������������
    int   sought;            // ������� �������������� ��������
    LEGAL_LIST  legal_list;  // ������ ���������� ��������
    VALUE_LIST  value_list;  // ������ ��������, ���� ������������
    OBJECT* next;
  public:
    OBJECT(char*);	          // �����������
    VALUE*   add_value(char*);    // �������� ��������
    L_VALUE* add_l_value(char*);  // �������� ����������� ��������
    VALUE*   test_value(char*);   // ��������� ��������
    VALUE*   get_value(int);      // �������� �������� � ���������� ������� � ������
    void     set_multivald();     // �������� ������������
    void     add_cf(char*,int);	  // ����������(��������)���� �������������
    void     add_question(char*); // �������� ������ �� �������
    void     ask();               // ���������� �������� ��������� legal_list
    void     clear_value();
    void     print();
};

// ������ ��������
class OBJECT_LIST {
  public:
    OBJECT* head;
  public:
    OBJECT_LIST() { head = 0;};          // �����������
    int     ok_add(char*, const int);    // ��������� ����� ���������� �������� �������
    OBJECT* add_object(char*,const char*,int); // ��������: ������,��������,���� �����������
    OBJECT* find(const char*);                 // �����  ������ � ������
    VALUE*  test_value(char*, char*);    // ��������� ������-��������
    OBJECT* add_question(char*,char*);
    OBJECT* set_multivald(char*);        // �������� ������������
    void    clear_value();                     // �������� ��������
    void    print();
};

struct PREM {		// ����������� �������
    char* object;       // ������
    char* value;        // ��������
    PREM* next;
   public:
    PREM(char*,char*);
    ~PREM();
    void print();
};

class PREM_LIST {       // ������ �����������
  public:
    PREM* head;
  public:
    PREM_LIST() { head = 0;}
    ~PREM_LIST();
    void add_prem(char*,char*);
    void print();
};

struct CON {		// ����������
   char*  object;       // ������
   char*  value;        // ��������
   int    cert;         // ����. �������
   CON*   next;
  public:
   CON(char*, char*,int);
   ~CON();
   void print();
};

class CON_LIST {        // ������ ����������
  public:
   CON* head;
  public:
   CON_LIST() { head = 0;}
   ~CON_LIST();
   int find_con(char*); // ����� �����������
   void add_con(char*, char*,int);
   void print();
};

struct RULE {		// �������
   char* name;          // ���
   PREM_LIST prem;      // ������ �����������
   CON_LIST  con;       // ������ ����������
   RULE*     next;
 public:
   RULE(char*);
   ~RULE();
   void add_con(char*,char*,int);  // �������� ����������
   void add_prem(char*,char*);     // �������� �����������
   RULE* find_rule(char*);         // ����� ������� � �������� �����������
   void print();
};

class RULE_LIST {       // ������ ������
  public:
   RULE* head;
  public:
   RULE_LIST() { head = 0;}
   RULE* add_rule(char*);
   void  clear();
   void  print();
};

class KNOWN_BASE {   // ���� ������
    int trass;             // ����������� ������ ������
  public:
    OBJECT_LIST ol;        // ������ ��������
    RULE_LIST   rl;        // ������ ������
    //KNOWN_BASE(){trass=1;};
    KNOWN_BASE(){trass=0;};
    int  read_file(char*); // ������ ����� ���� ������

    // ����������� !!
    void pursue(char*); // ���������� ���� - ������������
    // ����������� !!
};
#endif