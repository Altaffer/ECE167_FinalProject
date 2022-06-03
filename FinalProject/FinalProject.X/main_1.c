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
int iterator = 0;       // an iterator for taking averages

// holder values for data
double delbend; // accumulated angle of the bend change of the angle 

// biases for detecting motion
double gyrobias = 0;    // bias of the gyro
double accbias = 0;     // bias of the accelerometer


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

State curState = Calibrate;

// at 50 Hz, run sm
void runSM(void) {
    switch (curState) {
        case(Calibrate):    // calibrating the gyro and flex sensor to get bias for 10s
            printf("ur mom\n");
            }
            break;
    }
}

int main(void) {
    BOARD_Init();
    BNO055_Init();


    while (1) {
        
        printf("ur dad\n");
        runSM();
    }
    
}

