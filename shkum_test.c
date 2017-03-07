#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
//#include "fb00072.h"
#define MAX_NEIGHBORS 121
typedef  unsigned char uint8_t ;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t ;
typedef  unsigned char u8 ;
typedef  unsigned short u16;
typedef  unsigned int u32;
typedef  signed char s8 ;
typedef  signed short s16;
typedef  signed int s32;

//---------- Функция вычисления плотности нефти в текущих условиях по МИ 2632 --------------
float calc_ro_oil(float ro_in/*кг/куб.м*/, float temp, u32 press /*МПа*1000*/)
{
	float ro;
	float Gt;
	float A15;

	if ((ro_in > 500.0) && (ro_in < 2000.0))
		Gt = exp(-1.6208 + 0.00021592 * temp + (870960.0 / (ro_in * ro_in)) + (4.2092 * temp * 1000 / (ro_in * ro_in))) / 1000;
  else Gt = 0;
	A15 = 613.97226 / (ro_in * ro_in);
	ro = ro_in * exp((-1.0) * A15 * (temp - 15.0) * (1.0 + 0.8 * A15 * (temp - 15.0)));
	ro = ro / (1.0 - ((Gt * ((float)press)) / 1000.0));

	return ro;
}
//---------- Функция вычисления коэффициента плотности воды в текущих условиях при темп. 0 - 100 --------------
float calc_ro_water(float ro_in/*кг/куб.м*/, float temp, u16 press /*МПа*1000*/) {
    float ro;

    if (temp < 5) {
        ro=1.00176;
    } else {
        if ((temp >= 5.0) && (temp <= 90.0)) {
            ro = ((-0.000013)*(temp*temp*temp*temp) + 0.004*(temp*temp*temp) - 0.7394*(temp*temp) + 4.6*temp + 100173.0) / 100000.0;
        } else {
            ro=0.96706;
        }
    }

    ro = (ro_in * ro) / (1.0-(0.00046*((float)press)/1000.0));

    return ro;
}
//---------------------------------------------------------------------------------------------------------

//---------- Функция вычисления плотности нефти в текущих условиях при темп. -50, 0, 50, 100, 150 --------------
float calc_ro_tabl(float ro_in, s16 temp) {
    float ro;

    if (temp > 150) temp = 150;

    switch (temp) {
        case -50: ro = (6.42913E-05*ro_in+0.83378703)*ro_in+1.42633E+02; break;
        case -40: ro = (5.55416E-05*ro_in+0.8562389 )*ro_in+1.23195E+02; break;
        case -30: ro = (4.64214E-05*ro_in+0.87944813)*ro_in+1.03308E+02; break;
        case -20: ro = (3.70353E-05*ro_in+0.90324762)*ro_in+8.30808E+01; break;
        case -10: ro = (2.95668E-05*ro_in+0.92420167)*ro_in+6.38398E+01; break;
        case   0: ro = (1.88157E-05*ro_in+0.95065442)*ro_in+4.22670E+01; break;
        case  10: ro = (9.33093E-06*ro_in+0.97539572)*ro_in+2.11520E+01; break;
        case  20: ro = ro_in; break;
        case  30: ro = (-1.01056E-05*ro_in+1.0260019)*ro_in-2.18031E+01; break;
        case  40: ro = (-1.80674E-05*ro_in+1.0484841)*ro_in-4.22355E+01; break;
        case  50: ro = (-2.67470E-05*ro_in+1.0722282)*ro_in-6.32402E+01; break;
        case  60: ro = (-3.51667E-05*ro_in+1.0955748)*ro_in-8.41279E+01; break;
        case  70: ro = (-4.33278E-05*ro_in+1.118512 )*ro_in-1.04885E+02; break;
        case  80: ro = (-5.11979E-05*ro_in+1.1409644)*ro_in-1.25469E+02; break;
        case  90: ro = (-5.87847E-05*ro_in+1.1629332)*ro_in-1.45872E+02; break;
        case 100: ro = (-6.60873E-05*ro_in+1.1844035)*ro_in-1.66080E+02; break;
        case 110: ro = (-7.30935E-05*ro_in+1.2053401)*ro_in-1.86070E+02; break;
        case 120: ro = (-7.98327E-05*ro_in+1.2257879)*ro_in-2.05856E+02; break;
        case 130: ro = (-8.62953E-05*ro_in+1.2457182)*ro_in-2.25419E+02; break;
        case 140: ro = (-9.00849E-05*ro_in+1.2602269)*ro_in-2.42221E+02; break;
        case 150: ro = (-9.58898E-05*ro_in+1.278646 )*ro_in-2.60933E+02; break;
        default:  ro = (-2.67470E-05*ro_in+1.0722282)*ro_in-6.32402E+01; break; // считаем как будто temp=+50 гр.
    }
    return ro;
}
//---------------------------------------------------------------------------------------------------------

//------------------ Функция вычисления плотности в текущих условиях между табл. знач. --------------------
float calc_ro(float ro_in, float temp) {
  // ----- объявление локальных переменных -----
    s16 range;  			  // табличная температура (-50,0,50,100,150)
    float koef_flo;     	    	  // коэффициент для нахождения внутридиапазонных значений
    float ro_out;

    if ((ro_in < 690) || (ro_in > 1075)) {// получили некорректную плотность нефти
        // тут можно поставить флаг некорректной плотности нефти
    }

    if ((temp<(-50)) || (temp>150)) {// получили некорректную температуру
        range=50;
        koef_flo=0;
        // тут можно поставить флаг некорректной температуры
    } else {
        if (temp < 0) {// определяем начало диапазона к которому относится temp
            temp *= (-1);
            range = (((u8)(temp/10)) * 10 + 10);
            range *= (-1);
            temp *= (-1);
            koef_flo=0;
        } else {
            range = (((u8)(temp/10))*10);	 //
            koef_flo = (temp-range)/10; 			 // получаем коэффициент для нахождения внутридиапазонных значений
        }
    }
    ro_out = calc_ro_tabl(ro_in, range) - (calc_ro_tabl(ro_in, range)-calc_ro_tabl(ro_in, range+10)) * koef_flo;
    return ro_out;
}
//---------------------------------------------------------------------------------------------------------
int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    uint8_t line[1336];
    uint16_t line_len;
    for (float i=-45;i<50;i+=5.0){
      float fl;
      fl = calc_ro_oil(8100.0, i, 2000.0);
      printf("density for temperatur %f and press%f - %f\n",i,2000.0,fl);

      fl = calc_ro_oil(8100.0, i, 4000.0);
      printf("density for temperatur %f and press%f - %f\n",i,4000.0,fl);
      fl = calc_ro_oil(8100.0, i, 6000.0);
      printf("density for temperatur %f and press%f - %f\n",i,6000.0,fl);
      fl = calc_ro_oil(8100.0, i, 8000.0);
      printf("density for temperatur %f and press%f - %f\n",i,8000.0,fl);


    }
/*    while (fgets(line, 1336, file)) {
        // Do something with the line
        const char start = 0x3b;
        char *p_labirint;
        id_t *node;
        uint16_t j,i,node_number;//input output and hole
        uint8_t side_width;
//        start = 0x3b;            //";"
        line_len = strlen(line);
        if (line_len>1){
          side_width = strtol(&line[0], NULL, 10);  //get squer side widht
          p_labirint = malloc(side_width*side_width*side_width*sizeof(char));//alloce array for labirint
          for (i=0;i<line_len;i++){
            if (line[i]==start){
              j=0;
              for (++i;i<line_len;i++){
                p_labirint[j++] = line[i];
              }
              break;
            }
          }
          node_number=calculate_node_number(p_labirint,side_width);
          node = malloc(node_number*sizeof(id_t));
          init_node(p_labirint,side_width,node);
          print_node(node,node_number);
//          make_graph(p_labirint,side_width);
          printf("node number on cube %i\n",node_number);
        }
        free(p_labirint);
    }*/
    return 0;
}
             