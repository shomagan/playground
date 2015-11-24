// Sample code to read in test cases:
#include <stdio.h>
#include <math.h>
//int count;
char* pArray;
char number;
char point;

//const char prime_table[] = {2,3,5,7,11,13,17,19,23,29,31};
static unsigned char is_prime(char variable){
  if (variable<4){
    if (variable > 1)
      return 1;
    else
      return 0;
  }
  for (int d = 2;d<(variable/2+1);d++)
    if (variable%d==0)
      return 0;
  return 1;
}
static int find(){
  point++;
  if (point == (number-1)){
    if(is_prime(pArray[point]+pArray[point-1])){
      if(is_prime(pArray[point]+pArray[0])){
        return 1;
      }
    }
    return 0;
  }else{
    int count = 0;
    if(is_prime(pArray[point]+pArray[point-1])){
      count += find();
      point--;
    }
    for (char i=(point+1);i<number;i++){
      {
        char temp = pArray[point];
        pArray[point] = pArray[i];
        pArray[i] = temp;
      }
      if(is_prime(pArray[point]+pArray[point-1])){
        count += find();
        point--;
      }
      {
        char temp = pArray[i];
        pArray[i] = pArray[point];
        pArray[point] = temp;
      }
    }
    return count;
  }
}
int main(int argc, const char * argv[]) {
  FILE *file = fopen(argv[1], "r");
  char line[12];
  while (fgets(line, 64, file)) {
      // Do something with the line
    int count = 0;
    point=0;
    char i;
    char n = sscanf(line, "%d", &number);
    if((number & 1)==0){
      pArray = malloc(number*sizeof(char));
      for (i=0;i<number;i++){
        pArray[i] = i+1;
      }
      count = find();
      free(pArray);
    }
    printf("%i\n",count);

    pArray =NULL;
  }
  return 0;
} 