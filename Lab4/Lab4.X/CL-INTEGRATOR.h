/* 
 * File:   Part6.h
 * Author: richd
 *
 * Created on May 23, 2022, 10:12 PM
 */

#include "BOARD.h"
#include <xc.h>
#include <sys/attribs.h>

#ifndef PART6_H
#define PART6_H

/**
 * @function    INTEGRATOR_Init()
 * @brief       runs the closed loop integrator off of a timer
 * @return      void, returns pointers R_plus and b_plus
 */
char INTEGRATOR_Init(void);

/**
 * @function    CL_Integrate()
 * @brief       does a one step closed loop integration of R and b
 * @return      void, returns pointers R_plus and b_plus
 */
void CL_Integrate(float* R_plus[3][3], float* b_plus[3][1], float R_minus[3][3], float b_minus, \
                int gyro_meas[3][1], int acc_meas[3][1], int mag_meas[3][1]);

/**
 * @function    INTEGRATOR_GetEuler()
 * @brief       gets the current euler angles
 * @return      the euler angles in degrees
 */
float INTEGRATOR_GetEuler(void);

/**
 * @function    INTEGRATOR_TimeElapsed()
 * @brief       gets the current time elapsed
 * @return      the time elapsed in seconds
 */
float INTEGRATOR_TimeElapsed(void);



/**
 * @function    normalize()
 * @brief       helper function used to normalize a matrix using the A and B tilde matrices
 * @return      the normalization hat
 */
void normalize(float A[3][3], int meas[3][1], float B[3][1], float* hat[3][1]);

/**
 * @function    R_OnebyThree()
 * @brief       multiplies a 3x3 matrix by a 3x1
 * @return      void, returns hat as the resulting 3x1
 */
void R_OnebyThree(float A[3][3], float meas[3][1], float* hat[3][1]);

/**
 * @function    skew(void)
 * @brief       returns the skew symmetric matrix of a given 3x1
 * @return      void, result
 */
void skew(float A[3][1], float* result[3][3]);

#endif