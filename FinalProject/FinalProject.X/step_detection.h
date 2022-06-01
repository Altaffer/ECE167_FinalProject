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

// this function iterates a running average and returns the running average
int iterate_running_average(int sample_size, int running_avg_arr[], int new_value);

// takes the magnitude of a 3x1 vector
double magnitude(double x, double y, double z);

/*
 * -------------------- END Helper functions -------------------------------
 */

/*
 * -------------------- step detection functions -------------------------------
 */

/*
 * function detects motion by comparing a running average of acc and gyro
 * values to 
 */
int detect_motion(int acc_mag_calibrated, int gyro_mag_calibrated);

/*
 * -------------------- END step detection AVG functions -------------------------------
 */