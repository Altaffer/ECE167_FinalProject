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
#include "AD.h"
#include "kneeRotation.h"
#include "step_detection.h"

/*
 * This main function runs the data collection of our project
 */

/******************
 * VARIABLES
 ******************/
// variables for time
double freq = 50;       // Timestep of timer in Hz
double total_time = 0;  // total time elapsed in seconds
double step_timer = 0;  // timer for steps in seconds
double calibration_seconds = 3;  // time to calibrate
int iterator = 0;       // an iterator for taking averages

// holder values for data
int delbend; // accumulated angle of the bend change of the angle 

// biases for detecting motion
double gyrobias = 0;    // bias of the gyro
double accbias = 0;     // bias of the accelerometer
int rot_minread = 606;  // minimum analog read of flex sensor
int rot_maxread = 670;  // maximum read of flex sensor


/******************
 * HELPER FUNCTIONS
 ******************/


/******************
 * STATE MACHINE
 ******************/
typedef enum {
    Calibrate,
    Plant,
    Step
} State;

static State curState = Calibrate;

void runSM(void) {
    switch (curState) {
        case(Calibrate):    // calibrating the gyro and flex sensor to get bias for 10s
            // add time elapsed to total timer
            total_time += 1/freq;
            iterator += 1;          // will be used to take bias average
            
            //take the average of the gyro and acc
            accbias += magnitude((double)BNO055_ReadAccelX(), (double)BNO055_ReadAccelY(), (double)BNO055_ReadAccelZ());
            gyrobias += magnitude((double)BNO055_ReadGyroX(), (double)BNO055_ReadGyroY(), (double)BNO055_ReadGyroZ());
            
            // if 10 seconds has elapsed, enter plant
            if (total_time >= calibration_seconds) {
                // average the gyro and acc biases
                gyrobias = gyrobias / iterator; // average of gyro
                accbias = accbias / iterator;   // average of acc
                
                // change to next state
                curState = Plant;
                
                // signal ready
                printf("ready!\n");
                detect_motion(accbias, gyrobias);
            }
            
            break;
            
        case(Plant):
            // add time elapsed to total timer
            total_time += 1/freq;
            
            // this sums the change in rotation in the knee during the step every 50Hz
            bendKnee(&delbend, rot_maxread, rot_minread);
            
            // wait until there is a nonzero read to indicate movement
            if (detect_motion(accbias, gyrobias)) {
                
                // reset step timer
                step_timer = 0;
                
                // change to next state
                curState = Step;
            }
            
            break;
            
        case(Step):
            // add time elapsed to total timer
            total_time += 1/freq;
            
            // add time to step timer
            step_timer += 1/freq;
            
            // this sums the change in rotation in the knee during the step every 50Hz
            bendKnee(&delbend, rot_maxread, rot_minread);
            
            if (!detect_motion(accbias, gyrobias)) {
                // printing the data
                printf("%d, %f\n", delbend, step_timer);
                
                // transition to the next state
                curState = Plant;
                
                // reseting delbend
                delbend = 0;
            }
            break;
    }
}

void timer_init(void) {
    // following block inits the timer
    T4CON = 0;
    T4CONbits.TCKPS = 0b110;
    PR4 = 12500;                // ____ initialize to dT - 20ms -> 50Hz ______
    T4CONbits.ON = 1;
    IFS0bits.T4IF = 0;
    IPC4bits.T4IP = 3;
    IEC0bits.T4IE = 1;
    // other necessary inits below
    
    return;
    
}


int main(void) {
    BOARD_Init();
    BNO055_Init();
    AD_Init();
    AD_AddPins(AD_A0);
    timer_init();
    printf("\n\ncalibrating...\n");
    
    while (1);
}

// Timer interrupt
void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void) {
    IFS0bits.T4IF = 0;
    //Anything else that needs to occur goes here
    runSM();
}

