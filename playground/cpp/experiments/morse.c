struct morse_node{
   char letter;
   morse_node * dit;
   morse_node * dah;
};
const struct morse_node root_letter = {.letter =' ', .dit = &e_letter, .dah = &t_letter};
const struct morse_node e_letter = {.letter ='E', .dit = &i_letter, .dah = &a_letter};
const struct morse_node i_letter = {.letter ='I', .dit = &s_letter, .dah = &u_letter};
const struct morse_node s_letter = {.letter ='S', .dit = &h_letter, .dah = &v_letter};
const struct morse_node h_letter = {.letter ='H', .dit = &d5_digit, .dah = &d4_digit};
const struct morse_node d5_digit = {.letter ='5', .dit = NULL, .dah = NULL};
const struct morse_node d4_digit = {.letter ='4', .dit = NULL, .dah = NULL};
const struct morse_node v_letter = {.letter ='V', .dit = NULL, .dah = &d3_digit};
const struct morse_node d3_digit = {.letter ='3', .dit = NULL, .dah = NULL};
const struct morse_node u_letter = {.letter ='U', .dit = &f_letter, .dah = &utta_letter};
const struct morse_node f_letter = {.letter ='F', .dit = NULL, .dah = NULL};
const struct morse_node utta_letter = {.letter ='Ut', .dit = &detta_letter, .dah = &d2_digit};
const struct morse_node detta_letter = {.letter ='Dt', .dit = &question_sign, .dah = NULL};
const struct morse_node question_sign= {.letter ='?', .dit = NULL, .dah = NULL};
const struct morse_node d2_letter = {.letter ='2', .dit = NULL, .dah = NULL};
const struct morse_node a_letter = {.letter ='A', .dit = &r_letter, .dah = &w_letter};
const struct morse_node r_letter = {.letter ='R', .dit = &l_letter, .dah = &atte_letter}; 
const struct morse_node l_letter = {.letter ='L', .dit = NULL, .dah = &etta_letter};
const struct morse_node etta_letter = {.letter ='E', .dit =&quotes_signs, .dah = NULL};
const struct morse_node quotes_signs = {.letter ='\'\'', .dit =NULL, .dah = NULL};
const struct morse_node atte_letter = {.letter ='At', .dit =&plus_sign, .dah = NULL};
const struct morse_node plus_sign = {.letter ='+', .dit =NULL, .dah = &dot_sign};
const struct morse_node dot_sign = {.letter ='.', .dit =NULL, .dah = NULL};
const struct morse_node w_letter = {.letter ='W', .dit = &p_letter, .dah = j_letter};
const struct morse_node amba_letter = {.letter ='Ab', .dit = &dog_sign, .dah = NULL};
const struct morse_node dog_sign = {.letter ='@', .dit = NULL, .dah = NULL};
const struct morse_node j_letter = {.letter ='J', .dit = &jetta_letter, .dah = &d1_digit};
const struct morse_node d1_digit = {.letter ='1', .dit = &quote_sign, .dah = NULL};


























