#include "bell202data.h"
#include "stdio.h"
#include "stdlib.h"
uc16 MASS_12_SIN_BELL[]={
	2048,2148,2248,2344,2440,2529,2617,2694,2772,2836,2899,2946,2994,3023,3052,3062,
	3072,3062,3052,3023,2994,2946,2899,2836,2772,2694,2617,2529,2440,2344,2248,2148,
	2048,1948,1848,1752,1656,1568,1479,1402,1324,1261,1197,1150,1102,1073,1044,1033,
	1024,1033,1044,1073,1102,1150,1197,1261,1324,1402,1479,1568,1656,1752,1848,1948
};
uc16 MASS_22_SIN_BELL[]={
	2048,2248,2440,2617,2772,2836,2899,2946,3023,3072,3052,2994,2899,2772,2617,2440,
  	2248,2048,1848,1656,1479,1324,1197,1102,1044,1024,1044,1102,1197,1324,1402,1568,
  	1752,1948,2048,2248,2440,2617,2772,2836,2899,2946,3023,3072,3052,2994,2899,2772,
  	2617,2440,2248,2048,1848,1656,1479,1324,1197,1102,1044,1024,1044,1102,1197,1324,
};
uc16 MASS_LOW = 1024;
uc16 MASS_HIGH = 3063;

uc16 MASS_MIDDLE = 2048;

static void set_parity (uint16_t *code, int p) {
   int parity = 0;
   for (int bit = 1; bit <= 12; ++bit) {
         if (bit == p) continue;
         if ((bit & p) != 0) {
            parity ^= ((*code >> (bit - 1)) & 0x1u);
         }
   }
   if (parity) {
         *code |= (1u << (p - 1));
   }
};

static uint16_t hamming_encode_byte(uint8_t data){
   uint16_t code = 0;
   int d = 0;
   // Put the 8 data bits into positions 3,5,6,7,9,10,11,12
   for (int bit = 1; bit <= 12; ++bit) {
      if (bit == 1 || bit == 2 || bit == 4 || bit == 8) {
            continue;
      }
      code |= ((data >> d) & 0x1u) << (bit - 1);
      ++d;
   }
   set_parity(&code, 1);
   set_parity(&code, 2);
   set_parity(&code, 4);
   set_parity(&code, 8);
   return code;
}

static size_t generate_bell202_data(struct bell202_data *data,  u8 * packet, u16 packet_size){
   size_t index = 0;
   u16 encoded_packet[packet_size];
   for(int i = 0; i < packet_size; ++i){
      encoded_packet[i] = hamming_encode_byte(packet[i]);
   }
   int shift = rand() % 512;
   int sign = rand() % 2;
   if(sign == 0){
      shift = -shift;
   }
   if(data->p_data != NULL && data->size > 0){
      /*noise lets keep it stable to middle*/
      for(int i = 0; i < 640; ++i){
         if(i%2 == 0){
            data->p_data[index++] = MASS_MIDDLE - i%100;
         }else{
            data->p_data[index++] = MASS_MIDDLE + i%100;
         }
      }
      /*capture line signal*/
      for(int i = 0; i < 64; ++i){
         data->p_data[index++] = MASS_12_SIN_BELL[i] + shift;
      }
      for(int i = 0; i < packet_size; ++i){
         u16 word = encoded_packet[i];
         /*start bit*/
         for(int j = 0; j < 64; ++j){
            data->p_data[index++] = MASS_22_SIN_BELL[j] + shift;
         }
         int x = rand() % 12;  // 0..11
         word = word ^ (1 << x);  // Flip the bit at position x
         for(int bit = 0; bit < 12; ++bit){
            u16 bit_value = (word << bit) & 0x800;
            if(bit_value == 0){
               for(int j = 0; j < 64; ++j){
                  data->p_data[index++] = MASS_22_SIN_BELL[j] + shift;
               }
            }else{
               for(int j = 0; j < 64; ++j){
                  data->p_data[index++] = MASS_12_SIN_BELL[j] + shift;
               }
            }
         }
         /*stop bit*/
         for(int j = 0; j < 64; ++j){
            data->p_data[index++] = MASS_12_SIN_BELL[j] + shift;
         }
      }
      /*noise lets keep it stable to middle*/
      for(int j = 0; j < 64; ++j){
         data->p_data[index++] = MASS_12_SIN_BELL[j] + shift;
      }
      for(int j = 0; j < 64; ++j){
         data->p_data[index++] = MASS_12_SIN_BELL[j] + shift;
      }
      for(int j = 0; j < 64; ++j){
         data->p_data[index++] = MASS_12_SIN_BELL[j] + shift;
      }
   }
   return index;
}

int bell202_get_data(struct bell202_data data[PACKET_NUMBER]){

   /*for all request packets len is 8 bytes - for bell202 means
   noise - of 640 words(16 bits) - 1280 bytes, plus
   capture line signal -  832 uS  64 words(16 bits)
   12 bytes with start and stop bits - 14 * 11 * 64 = 9856 words -> 11000 bytes
   640 + 64 + 9856 + 64 + 64 + 64= 10752 bytes */
   u8 packet_1_data[] = {3, 4, 0, 0, 0, 1, 48, 40};
   data[0].size = 11000;
   data[0].p_data = (u16*)malloc(data[0].size * sizeof(u16));
   data[0].size = generate_bell202_data(&data[0], packet_1_data, sizeof(packet_1_data));
   u8 packet_2_data[] = {3, 4, 0, 1, 0, 1, 97, 232};
   data[1].size = 11000;
   data[1].p_data = (u16*)malloc(data[1].size * sizeof(u16));
   data[1].size = generate_bell202_data(&data[1], packet_2_data, sizeof(packet_2_data));
   u8 packet_3_data[] = {3, 3, 0, 0, 0, 1, 133, 232};
   data[2].size = 11000;
   data[2].p_data = (u16*)malloc(data[2].size * sizeof(u16));
   data[2].size = generate_bell202_data(&data[2], packet_3_data, sizeof(packet_3_data));
   u8 packet_4_data[] = {3, 3, 0, 1, 0, 1, 212, 40};
   data[3].size = 11000;
   data[3].p_data = (u16*)malloc(data[3].size * sizeof(u16));
   data[3].size = generate_bell202_data(&data[3], packet_4_data, sizeof(packet_4_data));
   u8 packet_5_data[] = {3, 3, 0, 0, 0, 16, 69, 228};
   data[4].size = 11000;
   data[4].p_data = (u16*)malloc(data[4].size * sizeof(u16));
   data[4].size = generate_bell202_data(&data[4], packet_5_data, sizeof(packet_5_data));
   u8 packet_6_data[] = {3, 1, 0, 0, 0, 1, 252, 40};
   data[5].size = 11000;
   data[5].p_data = (u16*)malloc(data[5].size * sizeof(u16));
   data[5].size = generate_bell202_data(&data[5], packet_6_data, sizeof(packet_6_data));
   u8 packet_7_data[] = {3, 2, 0, 0, 0, 1, 184, 40};
   data[6].size = 11000;
   data[6].p_data = (u16*)malloc(data[6].size * sizeof(u16));
   data[6].size = generate_bell202_data(&data[6], packet_7_data, sizeof(packet_7_data));
   u8 packet_8_data[] = {3, 1, 0, 5, 0, 3, 109, 232};
   data[7].size = 11000;
   data[7].p_data = (u16*)malloc(data[7].size * sizeof(u16));
   data[7].size = generate_bell202_data(&data[7], packet_8_data, sizeof(packet_8_data));
   u8 packet_9_data[] = {3, 2, 0, 7, 0, 7, 137, 235};
   data[8].size = 11000;
   data[8].p_data = (u16*)malloc(data[8].size * sizeof(u16));
   data[8].size = generate_bell202_data(&data[8], packet_9_data, sizeof(packet_9_data));
   u8 packet_10_data[] = {3, 3, 0, 4, 0, 5, 197, 234};
   data[9].size = 11000;
   data[9].p_data = (u16*)malloc(data[9].size * sizeof(u16));
   data[9].size = generate_bell202_data(&data[9],        packet_10_data, sizeof(packet_10_data));
   u8 packet_11_data[] = {3, 3, 0, 0, 0, 17, 132, 36};
   data[10].size = 11000;
   data[10].p_data = (u16*)malloc(data[10].size * sizeof(u16));
   data[10].size = generate_bell202_data(&data[10],        packet_11_data, sizeof(packet_11_data));
   u8 packet_12_data[] = {3, 3, 0, 16, 0, 5, 133, 238};
   data[11].size = 11000;
   data[11].p_data = (u16*)malloc(data[11].size * sizeof(u16));
   data[11].size = generate_bell202_data(&data[11],        packet_12_data, sizeof(packet_12_data));
   u8 packet_13_data[] = {3, 3, 0, 16, 0, 2, 196, 44};
   data[12].size = 11000;
   data[12].p_data = (u16*)malloc(data[12].size * sizeof(u16));
   data[12].size = generate_bell202_data(&data[12],        packet_13_data, sizeof(packet_13_data));
   u8 packet_14_data[] = {3, 3, 0, 16, 0, 2, 196, 44};
   data[13].size = 11000;
   data[13].p_data = (u16*)malloc(data[13].size * sizeof(u16));
   data[13].size = generate_bell202_data(&data[13],        packet_14_data, sizeof(packet_14_data));
   u8 packet_15_data[] = {3, 1, 0, 0, 0, 1, 252, 40};
   data[14].size = 11000;
   data[14].p_data = (u16*)malloc(data[14].size * sizeof(u16));
   data[14].size = generate_bell202_data(&data[14],        packet_15_data, sizeof(packet_15_data));
   u8 packet_16_data[] = {3, 2, 0, 0, 0, 2, 248, 41};
   data[15].size = 11000;
   data[15].p_data = (u16*)malloc(data[15].size * sizeof(u16));
   data[15].size = generate_bell202_data(&data[15],        packet_16_data, sizeof(packet_16_data));
   u8 packet_17_data[] = {3, 1, 0, 0, 0, 8, 60, 46};
   data[16].size = 11000;
   data[16].p_data = (u16*)malloc(data[16].size * sizeof(u16));
   data[16].size = generate_bell202_data(&data[16],        packet_17_data, sizeof(packet_17_data));
   u8 packet_18_data[] = {3, 2, 0, 0, 0, 8, 120, 46};
   data[17].size = 11000;
   data[17].p_data = (u16*)malloc(data[17].size * sizeof(u16));
   data[17].size = generate_bell202_data(&data[17],        packet_18_data, sizeof(packet_18_data));
   u8 packet_19_data[] = {3, 1, 0, 1, 0, 8, 109, 238};
   data[18].size = 11000;
   data[18].p_data = (u16*)malloc(data[18].size * sizeof(u16));
   data[18].size = generate_bell202_data(&data[18],        packet_19_data, sizeof(packet_19_data));
   u8 packet_20_data[] = {3, 2, 0, 2, 0, 8, 217, 238};
   data[19].size = 11000;
   data[19].p_data = (u16*)malloc(data[19].size * sizeof(u16));
   data[19].size = generate_bell202_data(&data[19],        packet_20_data, sizeof(packet_20_data));
   u8 packet_21_data[] = {3, 1, 0, 24, 0, 8, 188, 41};
   data[20].size = 11000;
   data[20].p_data = (u16*)malloc(data[20].size * sizeof(u16));
   data[20].size = generate_bell202_data(&data[20],        packet_21_data, sizeof(packet_21_data));
   u8 packet_22_data[] = {3, 2, 0, 0, 0, 17, 185, 228};
   data[21].size = 11000;
   data[21].p_data = (u16*)malloc(data[21].size * sizeof(u16));
   data[21].size = generate_bell202_data(&data[21],        packet_22_data, sizeof(packet_22_data));
   u8 packet_23_data[] = {3, 15, 0, 0, 0, 16, 2, 255, 255, 250, 240};
   data[22].size = 11000;
   data[22].p_data = (u16*)malloc(data[22].size * sizeof(u16));
   data[22].size = generate_bell202_data(&data[22],        packet_23_data, sizeof(packet_23_data));
   u8 packet_24_data[] = {3, 1, 0, 0, 0, 16, 60, 36};
   data[23].size = 11000;
   data[23].p_data = (u16*)malloc(data[23].size * sizeof(u16));
   data[23].size = generate_bell202_data(&data[23],        packet_24_data, sizeof(packet_24_data));
   u8 packet_25_data[] = {3, 16, 0, 0, 0, 1, 2, 0, 238, 63, 124};
   data[24].size = 11000;
   data[24].p_data = (u16*)malloc(data[24].size * sizeof(u16));
   data[24].size = generate_bell202_data(&data[24],        packet_25_data, sizeof(packet_25_data));
   u8 packet_26_data[] = {3, 3, 0, 0, 0, 2, 197, 233};
   data[25].size = 11000;
   data[25].p_data = (u16*)malloc(data[25].size * sizeof(u16));
   data[25].size = generate_bell202_data(&data[25],        packet_26_data, sizeof(packet_26_data));
   u8 packet_27_data[] = {3, 5, 0, 0, 0, 0, 204, 40};
   data[26].size = 11000;
   data[26].p_data = (u16*)malloc(data[26].size * sizeof(u16));
   data[26].size = generate_bell202_data(&data[26],        packet_27_data, sizeof(packet_27_data));
   u8 packet_28_data[] = {3, 6, 0, 0, 0, 0, 136, 40};
   data[27].size = 11000;
   data[27].p_data = (u16*)malloc(data[27].size * sizeof(u16));
   data[27].size = generate_bell202_data(&data[27],        packet_28_data, sizeof(packet_28_data));
   u8 packet_29_data[] = {3, 16, 0, 0, 0, 1, 2, 238, 238, 114, 220};
   data[28].size = 11000;
   data[28].p_data = (u16*)malloc(data[28].size * sizeof(u16));
   data[28].size = generate_bell202_data(&data[28],        packet_29_data, sizeof(packet_29_data));
   u8 packet_30_data[] = {3, 3, 0, 32, 0, 1, 132, 34};
   data[29].size = 11000;
   data[29].p_data = (u16*)malloc(data[29].size * sizeof(u16));
   data[29].size = generate_bell202_data(&data[29],        packet_30_data, sizeof(packet_30_data));
   u8 packet_31_data[] = {3, 16, 0, 32, 0, 1, 2, 0, 4, 185, 147};
   data[30].size = 11000;
   data[30].p_data = (u16*)malloc(data[30].size * sizeof(u16));
   data[30].size = generate_bell202_data(&data[30],        packet_31_data, sizeof(packet_31_data));
   u8 packet_32_data[] = {4, 3, 0, 32, 0, 1, 133, 149};
   data[31].size = 11000;
   data[31].p_data = (u16*)malloc(data[31].size * sizeof(u16));
   data[31].size = generate_bell202_data(&data[31],        packet_32_data, sizeof(packet_32_data));
   u8 packet_33_data[] = {4, 2, 0, 0, 0, 16, 121, 147};
   data[32].size = 11000;
   data[32].p_data = (u16*)malloc(data[32].size * sizeof(u16));
   data[32].size = generate_bell202_data(&data[32],        packet_33_data, sizeof(packet_33_data));
   u8 packet_34_data[] = {4, 1, 0, 0, 0, 16, 61, 147};
   data[33].size = 11000;
   data[33].p_data = (u16*)malloc(data[33].size * sizeof(u16));
   data[33].size = generate_bell202_data(&data[33],        packet_34_data, sizeof(packet_34_data));

   return 0;
}
