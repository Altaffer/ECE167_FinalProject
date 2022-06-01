/* 
 * File:   acc_Integrator.c
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
double mG_to_SI(int value) {
    return (double)value * 0.001 * 9.80665;
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
 * below are some variables that will hold values stored in the integrator
 */
double vel_mag = 0;                 // the absolute magnitude of velocity in m/s^2 
double vel[3][1] = {{0},            // the velocity vector in the IMU body frame in m/s
                    {0},
                    {0}};

/*
 * -------------------- END Init Variables -------------------------------
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
        int acc_cal_x, int acc_cal_y, int acc_cal_z, double dT) {
    
    // start by converting units and removing bias
    double acc_x = mG_to_SI(acc_meas_x - acc_cal_x);
    double acc_y = mG_to_SI(acc_meas_y - acc_cal_y);
    double acc_z = mG_to_SI(acc_meas_z - acc_cal_z);
    
    // integrate over each axis using forward integration
    vel[0][0] = vel[0][0] + acc_x * dT;
    vel[1][0] = vel[1][0] + acc_y * dT;
    vel[2][0] = vel[2][0] + acc_z * dT;
    
    // update the velocity magnitude value
    vel_mag = magnitude(vel[0][0], vel[1][0], vel[2][0]);
    
    return;
    
}

/*
 * function resets the integrator
 */
void ACC_INTEGRATOR_RESET() {
    vel_mag = 0;
    vel[0][0] = 0;
    vel[1][0] = 0;
    vel[2][0] = 0;
    
    return;
}

/*
 * function returns the magnitude of the velocity
 */
double ACC_INTEGRATOR_GETVELOCITY() {
    return vel_mag;
}

/*
 * -------------------- END Integrator functions -------------------------------
 */

