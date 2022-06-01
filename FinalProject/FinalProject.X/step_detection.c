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

/*
 * -------------------- Helper functions -------------------------------
 */

// this function iterates a running average and returns the running average                    v
int iterate_running_average(int sample_size, int running_avg_arr[], int new_value) {
    // create a temporary sum
    int average_sum = 0;
    
    // shift our sample array by 1 dropping the least recent value
    for (int k = sample_size; k > 0; k--){        
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

// takes the magnitude of a 3x1 vector
double magnitude(double x, double y, double z) {
    return sqrt(x*x + y*y + z*z);
}

/*
 * -------------------- END Helper functions -------------------------------
 */



/*
 * -------------------- Init Variables -------------------------------
 */

/*
 * below are some variables that will hold values used for the running averages
 */
static int sample_size_acc = 25;    // sample size of the running averages
static int sample_size_gyro = 25;

int running_avg_acc[25];        // arrays to hold running averages
int running_avg_gyro[25];       // BE SURE ARRAY SIZE = SAMPLE SIZE

static int acc_threshold_offset = 20;   // offset to calibrated values so noise
static int gyro_threshold_offset = 20;  // does not indicate moving

/*
 * -------------------- END Init Variables -------------------------------
 */




/*
 * -------------------- step detection functions -------------------------------
 */

/*
 * function detects motion by comparing a running average of acc and gyro
 * values to 
 */
int detect_motion(int acc_mag_calibrated, int gyro_mag_calibrated) {
    // firstly, measure the xyz of the acc and gyro
    int acc_x = BNO055_ReadAccelX();
    int acc_y = BNO055_ReadAccelY();
    int acc_z = BNO055_ReadAccelZ();
    
    int gyro_x = BNO055_ReadGyroX();
    int gyro_y = BNO055_ReadGyroY();
    int gyro_z = BNO055_ReadGyroZ();
    
    // calculate the magnitude of each vector
    int acc_mag = magnitude(acc_x, acc_y, acc_z);
    int gyro_mag = magnitude(gyro_x, gyro_y, gyro_z);
    
    // plug these values into the running average functions and return the running average
    int acc_avg = iterate_running_average(sample_size_acc, running_avg_acc, acc_mag);
    int gyro_avg = iterate_running_average(sample_size_gyro, running_avg_gyro, gyro_mag);
    printf("%d\n", gyro_avg);
    
    // check values against calibrated
    int moving;
    if(acc_avg > (acc_mag_calibrated + acc_threshold_offset) && \
            gyro_avg > (gyro_mag_calibrated + gyro_threshold_offset)) {
        
        moving = 1;
    } else {
        moving = 0;
    }
    
    return moving;
}

/*
 * -------------------- END step detection AVG functions -------------------------------
 */

