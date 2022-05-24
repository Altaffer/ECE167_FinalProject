/* 
 * File:   Part6.c
 * Author: richd
 *
 * Created on May 23, 2022, 10:12 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 *
 * Below is the matrices from the tumble test calibration in Lab 3
 *
 */
static float Atilde_mag[] = {{0.0012, 0, 0},
                                {0, 0.0013, 0},
                                {0, 0, 0.0013}};
static float Btilde_mag[] = {{0.8127},
                                {-0.3073},
                                {0.5926}};
static float Atilde_accel[] = {{0.9736e-03, -0.0110e-03, -0.0051e-03},
                                {-0.0045e-03, 0.9818e-03, 0.0350e-03},
                                {0.0009e-03, 0.0014e-03, 0.9732e-03}};
static float Btilde_accel[] = {{0.0244},
                                {0.0269},
                                {0.0520}};

/*
 * Below is Rdis from batch misalignment
 */
static float Rdis[] = {{1,0,0},
                        {0,1,0},
                        {0,0,1}};

/*
 * Below are the initial R0 and Bhat, the initial DCM and Gyro Bias
 */
static float R0[] = {{1,0,0},
                    {0,1,0},
                    {0,0,1}};
static float bhat[] = {{0},
                        {0},
                        {0}};

/*
 * Below are some integration parameters
 */
int freq = 50;      // frequency in Hz
float dT = 1/freq;  // timestep in seconds

