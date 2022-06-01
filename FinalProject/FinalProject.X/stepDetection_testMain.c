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
#include "step_detection.h"
#include "running_avg.h"

int t = 0;
int a = 0;
int acc_x_sum = 0;
int acc_y_sum = 0;
int acc_z_sum = 0;
int gyro_x_sum = 0;
int gyro_y_sum = 0;
int gyro_z_sum = 0;
int acc_cal;
int gyro_cal;
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

void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void) {
    IFS0bits.T4IF = 0;
    //Anything else that needs to occur goes here
    a++;
    if (a < 500) {
        acc_x_sum += BNO055_ReadAccelX();
        acc_y_sum += BNO055_ReadAccelY();
        acc_z_sum += BNO055_ReadAccelZ();
        
        gyro_x_sum += BNO055_ReadGyroX();
        gyro_y_sum += BNO055_ReadGyroY();
        gyro_z_sum += BNO055_ReadGyroZ();
        
        detect_motion(0, 0);
    }
    if (a == 500) {
        acc_x_sum = acc_x_sum / a;
        acc_y_sum = acc_y_sum / a;
        acc_z_sum = acc_z_sum / a;
        
        gyro_x_sum = gyro_x_sum / a;
        gyro_y_sum = gyro_y_sum / a;
        gyro_z_sum = gyro_z_sum / a;
        
        int acc_cal = sqrt(acc_x_sum*acc_x_sum + acc_y_sum*acc_y_sum + acc_z_sum*acc_z_sum);
        int gyro_cal = sqrt(gyro_x_sum*gyro_x_sum + gyro_y_sum*gyro_y_sum + gyro_z_sum*gyro_z_sum);
    }  
    if (a > 500) {
        if(detect_motion(acc_cal, gyro_cal)) {
            //printf("up\n");
        } else {
            //printf("down\n");
        }
        t+=1;
    }
    if(t==10){
        t = 0;
        //printf("%f\n", ACC_INTEGRATOR_GETVELOCITY());
        //printf("%d    %d    %d\n", BNO055_ReadAccelX(), BNO055_ReadAccelY(), BNO055_ReadAccelZ());
    }
    
}

