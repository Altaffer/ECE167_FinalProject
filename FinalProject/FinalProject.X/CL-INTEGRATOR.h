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
void CL_Integrate(double* R_plus[3][3], double* b_plus[3][1], double R_minus[3][3], double b_minus, \
                int gyro_meas[3][1], int acc_meas[3][1], int mag_meas[3][1]);

/**
 * @function    INTEGRATOR_GetEuler()
 * @brief       gets the current euler angles
 * @return      the euler angles in degrees
 */
void INTEGRATOR_GetEuler(double Euler[3][1]);

/**
 * @function    INTEGRATOR_TimeElapsed()
 * @brief       gets the current time elapsed
 * @return      the time elapsed in seconds
 */
double INTEGRATOR_TimeElapsed(void);



/**
 * @function    normalize()
 * @brief       helper function used to normalize a matrix using the A and B tilde matrices
 * @return      the normalization hat
 */
void normalize(double A[3][3], int meas[3][1], double B[3][1], double* hat[3][1]);

/**
 * @function    R_OnebyThree()
 * @brief       multiplies a 3x3 matrix by a 3x1
 * @return      void, returns hat as the resulting 3x1
 */
void R_OnebyThree(double A[3][3], double meas[3][1], double* hat[3][1]);

/**
 * @function    skew(void)
 * @brief       returns the skew symmetric matrix of a given 3x1
 * @return      void, result
 */
void skew(double A[3][1], double* result[3][3]);

/**
 * @function    Integrate_R
 * @brief       integrates the R matrix
 * @return      newR
 */
void Integrate_R(double w[3][1], double dT, double R_exp[3][3], double PrevR[3][3], double newR[3][3]);

/**
 * @function    Rexp
 * @brief       calculates the Rexp matrix
 * @return      Rexp
 */
void Rexp(double w[3][1], double dT, double R_exp[3][3]);

#endif