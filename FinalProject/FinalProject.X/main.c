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
#include "kneeRotation.h"

/*
 * This main function runs the data collection of our project
 */

/******************
 * VARIABLES
 ******************/
// variables for time
double curtime = TIMERS_GetMilliSeconds(); // current time
double prevtime = TIMERS_GetMilliSeconds(); // previous time
double steptime = TIMERS_GetMilliSeconds(); // time of step

// holder values for data
double delbend; // accumulated angle of the bend change of the angle 
double vel; // place holder for the integral of the acceleration

/******************
 * HELPER FUNCTIONS
 ******************/
// have this here to add something to the SM

void readZeroAccel(void) {
    // could do this using the velocity (integration)
    // might want to add a threshold for what is considered static
    if ((BNO055_ReadAccelX() == 0) && (BNO055_ReadAccelY() == 0) && (BNO055_ReadAccelZ() == 0)) {
        return TRUE;
    } else {
        return FALSE;
    }

}

/******************
 * STATE MACHINE
 ******************/
typedef enum {
    Plant,
    Step,
} State;

static State curState = Plant;

void runSM(void) {
    switch (curState) {
        case(Plant):
            // wait until there is a nonzero read to indicate movement
            if (!readZeroAccel) {
                // updating the previous time
                prevtime = TIMERS_GetMilliSeconds();
                // reseting delbend
                delbend = 0;
                // reset velocity
                vel = 0;
                // transition to the next state
                curState = Step;
            }
            break;
        case(Step):
            if (TIMERS_GetMilliSeconds() % 50 <= 3) { // sampling every 50 Hz
                // calling the bendknee function
                bendKnee(delbend);
                // calling the integrate accel function

            }
            if (readZeroAccel) {
                // calculating current time 
                curtime = TIMERS_GetMilliSeconds();
                // calculating the time of step
                steptime = curtime - prevtime;
                // printing the data
                printf("delta theta: %lf | foot velocity: %lf", delbend, vel);
                // transition to the next state
                curState = Plant;
            }
            break;
    }
}

int main(void) {
    BOARD_Init();
    BNO055_Init();


    while (1);
}

