/* 
 * File:   main.c
 * Author: Luca Altaffer and Richard Owens
 *
 * Created on May 31, 2022, 1:57 PM
 */

#include "BOARD.h"
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
#include "BNO055.h"
#include <xc.h>
#include <sys/attribs.h>
#include "acc_Integrator.h"

int t = 0;
int a = 0;
int acc_x_sum = 0;
int acc_y_sum = 0;
int acc_z_sum = 0;
double freq = 50;

/*
 * This main function runs the data collection of our project
 */
int main(void) {
    BOARD_Init();
    BNO055_Init();
    
    // following block inits the timer
    T4CON = 0;
    T4CONbits.TCKPS = 0b110;
    PR4 = 12500;                // ____ initialize to dT - 20ms -> 50Hz ______
    T4CONbits.ON = 1;
    IFS0bits.T4IF = 0;
    IPC4bits.T4IP = 3;
    IEC0bits.T4IE = 1;
    // other necessary inits below
    printf("calibrating...\n");
    
    while(1);
}
int sample_size = 25;
int running_avg_arr[25];
double average_sum = 0;

int lee_set = 1100;

void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void) {
    IFS0bits.T4IF = 0;
    //Anything else that needs to occur goes here
    int x = BNO055_ReadAccelX(); 
    int y = BNO055_ReadAccelY(); 
    int z = BNO055_ReadAccelZ();
    double lee = sqrt((double)x*x + (double)y*y + (double)z*z);
    
    // shift our sample array by 1 dropping the least recent value
    for (int k = sample_size; k > 0; k--){        
        running_avg_arr[k]=running_avg_arr[k-1];

        // while iterating, start summing all elements in the array
        // to take an average
        average_sum += running_avg_arr[k];
    }
    // add in the newest sample to the array and add it to the average sum
    running_avg_arr[0] = lee;
    average_sum += running_avg_arr[0];

    // take the average over the sample size and reset the average sum
    double average = (average_sum / sample_size);
    //printf("%f\n", average);
    average_sum = 0;
    
    if(average > lee_set) {
        printf("up\n");
    }
    if(average < lee_set) {
        printf("down\n");
    }
    //printf("%d    %d    %d\n", BNO055_ReadAccelX(), BNO055_ReadAccelY(), BNO055_ReadAccelZ());
    a++;
    if (a < 500) {
        acc_x_sum += BNO055_ReadAccelX();
        acc_y_sum += BNO055_ReadAccelY();
        acc_z_sum += BNO055_ReadAccelZ();
    }
    if (a == 500) {
        acc_x_sum = acc_x_sum / a;
        acc_y_sum = acc_y_sum / a;
        acc_z_sum = acc_z_sum / a;
    }  
    if (a > 500) {
        ACC_INTEGRATE(BNO055_ReadAccelX(), BNO055_ReadAccelY(), BNO055_ReadAccelZ(), \
                acc_x_sum, acc_y_sum, acc_z_sum, 1/freq);
        t+=1;
    }
    if(t==10){
        t = 0;
        //printf("%f\n", ACC_INTEGRATOR_GETVELOCITY());
        //printf("%d    %d    %d\n", BNO055_ReadAccelX(), BNO055_ReadAccelY(), BNO055_ReadAccelZ());
    }
    
}

