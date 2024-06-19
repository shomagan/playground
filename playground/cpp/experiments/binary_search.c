#include <stdio.h>
#include <string.h>
#include <math.h>
static int binary_search(int* test_array, int test_array_size,int value);
int array_to_search[] = {1,2,3,12,81,90,111,122,990,11000};
int array_to_search_1[] = {1,2,3,12,81,90,111,122,990,11000,11222};

int main(int argc, const char * argv[]) {
   int array_size = sizeof(array_to_search)/sizeof(int);
   int array_size_1 = sizeof(array_to_search_1)/sizeof(int);

   for (int i =0;i < array_size;i++){
      printf("%i,",array_to_search[i]);
   }
   printf("\n");

   int index =binary_search(array_to_search, array_size,111);
   printf ("index 111 : %i\n",index);
   index =binary_search(array_to_search, array_size,1);
   printf ("index 1 : %i\n",index);
   index =binary_search(array_to_search, array_size,11000);
   printf ("index 11000 : %i\n",index);
   index =binary_search(array_to_search, array_size,4);
   printf ("index 4 : %i\n",index);
   index =binary_search(array_to_search, array_size,990);
   printf ("index 990 : %i\n",index);
   index =binary_search(array_to_search, array_size,-4);
   printf ("index -4 : %i\n",index);
   index =binary_search(array_to_search, array_size,100000);
   printf ("index 100000 : %i\n",index);

   index =binary_search(array_to_search_1, array_size_1,111);
   printf ("index 111 : %i\n",index);
   index =binary_search(array_to_search_1, array_size_1,1);
   printf ("index 1 : %i\n",index);
   index =binary_search(array_to_search_1, array_size_1,11000);
   printf ("index 11000 : %i\n",index);
   index =binary_search(array_to_search_1, array_size_1,4);
   printf ("index 4 : %i\n",index);
   index =binary_search(array_to_search_1, array_size_1,990);
   printf ("index 990 : %i\n",index);
   index =binary_search(array_to_search_1, array_size_1,11222);
   printf ("index 11222 : %i\n",index);
   index =binary_search(array_to_search_1, array_size_1,-4);
   printf ("index -4 : %i\n",index);
   index =binary_search(array_to_search_1, array_size_1,100000);
   printf ("index 100000 : %i\n",index);

   return 0;
}

static int binary_search(int* test_array, int test_array_size,int value){
   int result = test_array_size;
   int left_element = 0;
   int right_element = test_array_size-1;
   int current_element = left_element + (right_element-left_element)/2; 
   for(int i =1;test_array_size>i;i<<1){
      if (test_array[current_element]==value){
         result = current_element;
         break;
      }else if (test_array[current_element]>value){
         if(current_element <= left_element){
            break;
         }else{
            right_element = current_element;
         }
         
      }else if (test_array[current_element]<value){
         if(current_element >= right_element){
            break;
         }else{
            left_element = current_element;
            if ((right_element-left_element)==1){
               if (test_array[right_element]==value){
                  result = right_element;
               }
               break;
            }
         }
      }
      current_element = left_element + (right_element-left_element)/2;
   };
   return result;
}
