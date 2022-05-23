/* 
 * File:   Part2_DCM2Euler
 * Author: richard owens, Luca Altaffer
 *
 * Created on May 5, 2022, 3:19 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float DCM2Euler(float DCM[3][3]) {
    
    // create a vector of euler angles to return
    float euler[3]; // [yaw, pitch, roll]
    
    // solve for the yaw value in radians
    float yaw_radians = atan2f(DCM[0][1], DCM[0][0]);
    
    // solve for pitch in radians
    float pitch_radians = -1*asinf(DCM[0][2]);
    
    // solve for roll in radians
    float roll_radians = atan2(DCM[1][2], DCM[2][2]);
    
    // return the values in degrees
    euler[0] = yaw_radians * 57.2958;
    euler[1] = pitch_radians * 57.2958;
    euler[2] = roll_radians * 57.2958;
    
    return *euler;
    
}