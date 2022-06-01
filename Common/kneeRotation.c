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
#include <math.h>
#include <sys/attribs.h>
#include "kneeRotation.h"
#include "AD.h"
#include "timers.h"


/******************
 VARIABLES
 ******************/
double bend; // initial reading of the flex sensor converted to degrees
double prevAng = 0; // the previous angle of the sensor if no init, set to 0
double diff; // the difference between current and previous bends
//double sum; // accumulated sum of the difference

/*bendKnee
 * (timers done in state machine)
 * read flex sensor every 50 hz 
 * take the abs of the change between current value and previous value
 * convert to angles from resistance
 */
void bendKnee(double *sum) {
    // reading AD pin 0 for the flex sensor and convert to degrees
    bend = (((double)AD_ReadADPin(AD_A0) * 1.8) - 1110);
    // taking the difference and adding to the sum over the step
    diff = (abs(bend - prevAng));
    *sum += diff;
    prevAng = bend;

}

