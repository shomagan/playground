#include <stdio.h>
#include <string.h>
#include <math.h>
class foo{
   int x_;
   public:
      foo(int const x = 0):x_{x}{}
      int& get(){return x_;}
};
decltype(auto) proxy_get(foo& f) {return f.get();}
int main(int argc, const char * argv[]) {
   auto f = foo{42};
   decltype(auto) x= proxy_get(f); 
}
