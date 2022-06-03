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
int bend; // initial reading of the flex sensor converted to degrees
int prevAng = 0; // the previous angle of the sensor if no init, set to 0
int diff; // the difference between current and previous bends
//double sum; // accumulated sum of the difference

/*bendKnee
 * (timers done in state machine)
 * read flex sensor every 50 hz 
 * take the abs of the change between current value and previous value
 * convert to angles from resistance
 */
void bendKnee(int *sum, int maxread, int minread) {
    // collect reading and perform offset
    int read = (AD_ReadADPin(AD_A0) - minread);
    
    // handle case where noise sets read to less than zero
    if (read < 0) {
        read = 0;
    }
    
    // correct bend from scale 
    bend = ((read) * 90) / (maxread-minread);
    // taking the difference between the the current angle and previous angle
    diff = (abs(bend - prevAng));
    //printf("%d\n\r", diff);
    
    // remove any tiny degree rotations in knee
    if (diff > 6 && diff < 22) {
        // adding to the sum over the step
        *sum += diff;
        
        // update previous angle
        prevAng = bend;
    }

}

