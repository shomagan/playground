#include <stdio.h>
#define f(a,b) a##b
#define g(a)   #a
#define h(a) g(a)
#define s(a) (a+a)
#define cast(a,b) ((a)(b))


int main(){
    int test=3;
    printf("sum %i\n",s(2));
    printf("g %s\n",g(test));
    int value = cast(char,0xf0f);
    printf("type cast %i \n",value);

  printf("%s\n",h(f(1,2)));
  printf("%s\n",g(f(1,2)));
  return 0;
}
