/* 
 * File:   acc_Integrator.h
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
#include "math.h"
#include <sys/attribs.h>


/*
 * -------------------- Helper functions -------------------------------
 */

// converts a value in milli-G to m/s^2
double mG_to_SI(double value);

// takes the magnitude of a 3x1 vector
double magnitude(double x, double y, double z);

/*
 * -------------------- END Helper functions -------------------------------
 */



/*
 * -------------------- Integrator functions -------------------------------
 */

/*
 * function integrates the acc and keeps it in vel_mag
 * 
 * inputs:
 * acc_meas...: the x,y,or z values measured from the accelerometer, in mG
 * dT: the timestep in seconds
 */
void ACC_INTEGRATE(int acc_meas_x, int acc_meas_y, int acc_meas_z, \
        int acc_cal_x, int acc_cal_y, int acc_cal_z, double dT);

/*
 * function returns the magnitude of the velocity
 */
double ACC_INTEGRATOR_GETVELOCITY();