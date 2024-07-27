
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include "morse.hpp"

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
  #include <windows.h>
  inline void delay( unsigned long ms ) {    Sleep( ms );    }
  u64 time_ms(void){ return (u64)timeGetTime();}

#else  /* presume POSIX */
  #include <unistd.h>
  inline void delay( unsigned long ms )    {    usleep( ms * 1000 );    }
  u64 time_ms(void){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (u64)tv.tv_sec * 1000 + tv.tv_usec / 1000;
  }
#endif
struct morse_node{
   const char * const letter;
   const morse_node * const dit;
   const morse_node * const dah;
};
const struct morse_node root_letter = {.letter = NULL, .dit = &e_letter, .dah = &t_letter};
const struct morse_node e_letter = {.letter ="E", .dit = &i_letter, .dah = &a_letter};
const struct morse_node i_letter = {.letter ="I", .dit = &s_letter, .dah = &u_letter};
const struct morse_node s_letter = {.letter ="S", .dit = &h_letter, .dah = &v_letter};
const struct morse_node h_letter = {.letter ="H", .dit = &d5_digit, .dah = &d4_digit};
const struct morse_node d5_digit = {.letter ="5", .dit = NULL, .dah = NULL};
const struct morse_node d4_digit = {.letter ="4", .dit = NULL, .dah = NULL};
const struct morse_node v_letter = {.letter ="V", .dit = NULL, .dah = &d3_digit};
const struct morse_node d3_digit = {.letter ="3", .dit = NULL, .dah = NULL};
const struct morse_node u_letter = {.letter ="U", .dit = &f_letter, .dah = &utta_letter};
const struct morse_node f_letter = {.letter ="F", .dit = NULL, .dah = NULL};
const struct morse_node utta_letter = {.letter ="Ut", .dit = &detta_letter, .dah = &d2_digit};
const struct morse_node detta_letter = {.letter ="Dt", .dit = &question_sign, .dah = &underscore_sign};
const struct morse_node question_sign= {.letter ="?", .dit = NULL, .dah = NULL};
const struct morse_node underscore_sign= {.letter ="_", .dit = NULL, .dah = NULL};
const struct morse_node d2_digit = {.letter ="2", .dit = NULL, .dah = NULL};
const struct morse_node a_letter = {.letter ="A", .dit = &r_letter, .dah = &w_letter};
const struct morse_node r_letter = {.letter ="R", .dit = &l_letter, .dah = &atte_letter}; 
const struct morse_node l_letter = {.letter ="L", .dit = NULL, .dah = &etta_letter};
const struct morse_node etta_letter = {.letter ="E", .dit =&quotes_signs, .dah = NULL};
const struct morse_node quotes_signs = {.letter ="\'\'", .dit =NULL, .dah = NULL};
const struct morse_node atte_letter = {.letter ="At", .dit =&plus_sign, .dah = NULL};
const struct morse_node plus_sign = {.letter ="+", .dit =NULL, .dah = &dot_sign};
const struct morse_node dot_sign = {.letter =".", .dit =NULL, .dah = NULL};
const struct morse_node w_letter = {.letter ="W", .dit = &p_letter, .dah = &j_letter};
const struct morse_node p_letter = {.letter ="P", .dit = NULL, .dah = &amba_letter};
const struct morse_node amba_letter = {.letter ="Ab", .dit = &dog_sign, .dah = NULL};
const struct morse_node dog_sign = {.letter ="@", .dit = NULL, .dah = NULL};
const struct morse_node j_letter = {.letter ="J", .dit = &jetta_letter, .dah = &d1_digit};
const struct morse_node jetta_letter = {.letter ="Jt", .dit = NULL, .dah = NULL};

const struct morse_node d1_digit = {.letter ="1", .dit = &quote_sign, .dah = NULL};
const struct morse_node quote_sign = {.letter ="\'", .dit = NULL, .dah = NULL};
const struct morse_node t_letter = {.letter ="T", .dit = &n_letter, .dah = &m_letter};
const struct morse_node n_letter = {.letter ="N", .dit = &d_letter, .dah = &k_letter};
const struct morse_node d_letter = {.letter ="D", .dit = &b_letter, .dah = &x_letter};
const struct morse_node b_letter = {.letter ="B", .dit = &d6_digit, .dah = &equal_sign};
const struct morse_node d6_digit = {.letter ="6", .dit = NULL, .dah = &minus_sign};
const struct morse_node minus_sign = {.letter ="-", .dit = NULL, .dah = NULL};
const struct morse_node equal_sign = {.letter ="=", .dit = NULL, .dah = NULL};
const struct morse_node x_letter = {.letter ="X", .dit = &slash_sign, .dah = NULL};
const struct morse_node slash_sign = {.letter ="/", .dit = NULL, .dah = NULL};
const struct morse_node k_letter = {.letter ="K", .dit = &c_letter, .dah = &y_letter};
const struct morse_node c_letter = {.letter ="C", .dit = NULL, .dah = &new_line};
const struct morse_node new_line = {.letter ="\n", .dit = &semicolon, .dah = &exclamation};
const struct morse_node semicolon = {.letter =";", .dit = NULL, .dah = NULL};
const struct morse_node exclamation = {.letter ="!", .dit = NULL, .dah = NULL};
const struct morse_node y_letter = {.letter ="Y", .dit = &open_bracket, .dah = NULL};
const struct morse_node open_bracket = {.letter ="(", .dit = NULL, .dah = &close_bracket};
const struct morse_node close_bracket = {.letter =")", .dit = NULL, .dah = NULL};
const struct morse_node m_letter = {.letter ="M", .dit = &g_letter, .dah = &o_letter};
const struct morse_node g_letter = {.letter ="G", .dit = &z_letter, .dah = &q_letter};
const struct morse_node z_letter = {.letter ="Z", .dit = &d7_digit, .dah = &zetta_letter};
const struct morse_node d7_digit = {.letter ="7", .dit = NULL, .dah = NULL};
const struct morse_node zetta_letter = {.letter ="Zt", .dit = NULL, .dah = &comma};
const struct morse_node comma = {.letter =",", .dit = NULL, .dah = NULL};
const struct morse_node q_letter = {.letter ="Q", .dit = NULL, .dah = NULL};
const struct morse_node o_letter = {.letter ="O", .dit = &otto_letter, .dah = &chapter};
const struct morse_node otto_letter = {.letter ="Ot", .dit = &d8_digit, .dah = NULL};
const struct morse_node d8_digit = {.letter ="8", .dit = &colon, .dah = NULL};
const struct morse_node colon = {.letter =":", .dit = NULL, .dah = NULL};
const struct morse_node chapter = {.letter ="   ", .dit = &d9_digit, .dah = &d0_digit};
const struct morse_node d9_digit = {.letter ="9", .dit = NULL, .dah = NULL};
const struct morse_node d0_digit = {.letter ="0", .dit = NULL, .dah = NULL};
int morse_unit_time = 100; 
#define MORSE_UNIT_TIME_MS morse_unit_time
#define DOT_LENGTH MORSE_UNIT_TIME_MS
#define DASH_LENGTH (3u*MORSE_UNIT_TIME_MS)
#define SPACE_SIGNALS MORSE_UNIT_TIME_MS
#define SPACE_LETTERS (3u*MORSE_UNIT_TIME_MS)
#define SPACE_WORDS (7u*MORSE_UNIT_TIME_MS)


u64 time_state(int key,short state_start){
   u64 start_time = time_ms();
   state_start &= 0x8000;
   while(1){
      short state = GetKeyState(key) & 0x8000;
      u64 diff = time_ms() - start_time;
      if((state != state_start) || (diff>SPACE_WORDS && !state)){
         return diff;
      }
   }
}

char input_string[48];

int main(int argc, const char * argv[]) {
   int increment =0;
   if (argc == 2) {
        strcpy(input_string, argv[1]);
        /*if starts from 0x then handle as hex else as decimal*/
        if (input_string[0] == '0' && input_string[1] == 'x') {
           morse_unit_time = strtoll(input_string, NULL, 16);
        } else {
           morse_unit_time = strtoll(input_string, NULL, 10);
        }
   }   else {
       printf("Usage: morse <unit time ms - eg 100> \n");
   }

   while(1){
      short current_state = GetKeyState(int('M')) & 0x8000;
      if(current_state){
         const struct morse_node * node = &root_letter; 
         int word_end = 0;
         while(node){
            u64 diff = time_state(int('M'),0x8000);
            if(diff > DASH_LENGTH){
               node = node->dah;
            }else{
               node = node->dit;
            }
            diff = time_state(int('M'),0);
            if(diff > SPACE_WORDS){
               word_end = 1;
               break;
            }else if (diff > SPACE_LETTERS){
               break;
            }
         }
         if(node!= NULL && node->letter != NULL){
            printf("%s", node->letter);
            if(word_end){
               printf(" ");
            }
         }
      }
      if(GetKeyState(int('Q')) & 0x8000){
         break;
      }
      increment++;
   }   
   return 0;
}

























































