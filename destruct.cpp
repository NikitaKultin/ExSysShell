#include <iostream>
#include <string.h>
class v {
  char* name;
  int   cert;
 public:
  v*    next;
  v(char*,int);
  ~v();
};
v::v(char* n, int c) {
  cert = c;
  name = new char[strlen(n)+1];
  strcpy_s(name, strlen(n) + 1, n);
  next = 0;
}
v::~v() {
  delete name;
  std::cout << "deleted\n";
}

int main() {
 v* head = 0;
 for (int i = 0; i<5; i++) {
   v* p = new v("hello",i);
   p->next = head;
   head = p;
 }
 while(head) {
   v* p = head;
   head = p->next;
   delete p;
 }
}