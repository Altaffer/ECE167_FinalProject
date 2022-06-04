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
int bend_avg_arr[15];
//double sum; // accumulated sum of the difference
/*
 * -------------------- Helper functions -------------------------------
 */

/*****************************
 * helper functions
 *****************************/
// this function iterates a running average and returns the running average                    v
int iterate_running_average_rot(int sample_size, int running_avg_arr[], int new_value) {
    // create a temporary sum
    int average_sum = 0;
    
    // shift our sample array by 1 dropping the least recent value
    int k;
    for (k = sample_size; k > 0; k--){        
        running_avg_arr[k]= running_avg_arr[k-1];
        // while iterating, start summing all elements in the array
        // to take an average
        average_sum += running_avg_arr[k];
    }
    
    // add in the newest sample to the array and add it to the average sum
    running_avg_arr[0] = new_value;
    average_sum += running_avg_arr[0];

    // take the average over the sample size and reset the average sum
    return (average_sum / sample_size);
}


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
    
    // add the bend to a running average
    int average = iterate_running_average_rot(15, bend_avg_arr, bend);
    
    // taking the difference between the the current angle and previous angle
    diff = (abs(average - prevAng));
    
    // adding to the sum over the step
    *sum += diff;

    // update previous angle
    prevAng = average;
}

