/* 
 * File:   Part1_DCM2Euler.h
 * Author: richard owens, Luca Altaffer
 *
 * Created on May 5, 2022, 3:19 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * @Function DCM2Euler

 * @return 0 if error, 1 if succeeded
 * @brief  returns yaw pict roll from DCM
 */
float DCM2Euler(float DCM[3][3]);