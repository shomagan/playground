#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>   
#define unreal_value_for_seat 101
static int find_min(int* array_of_values, int array_size);
static int minMovesToSeat(int* seats, int seatsSize, int* students, int studentsSize);
int seats[] = {3,1,5};
int students[] = {2,7,4};
int main(int argc, const char * argv[]) {
   int moves = minMovesToSeat(seats, sizeof(seats)/sizeof(int), students, sizeof(students)/sizeof(int));
   printf("moves - %i ", moves); 
   return 0;
}

static int minMovesToSeat(int* seats, int seatsSize, int* students, int studentsSize) {
    int moves = 0;
    for(int i=0;i<studentsSize;i++){
        int min_seat_number = find_min(seats, seatsSize);
        int min_student_number = find_min(students, studentsSize);
        if (min_seat_number > min_student_number){
            moves += min_seat_number - min_student_number;
        }else{
            moves += min_student_number - min_seat_number;
        }
    }
    return moves;
}
static int find_min(int* array_of_values, int array_size){
    int index = unreal_value_for_seat;
    int min_value = unreal_value_for_seat;
    for (int i=0;i<array_size;i++){
        if (array_of_values[i]!= unreal_value_for_seat){
            if ( array_of_values[i] < min_value){
                min_value = array_of_values[i];
                index = i;
            }
        }
    }
    if (index != unreal_value_for_seat){
        array_of_values[index] = unreal_value_for_seat;
    }
    return min_value;
}