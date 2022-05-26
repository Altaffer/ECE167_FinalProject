/* 
 * File:   Parts6to8Main.c
 * Author: richd
 *
 * Created on May 5, 2022, 3:19 PM
 * 
 * for this build, only include the following (along with common code):
 * Parts6to8Main.c
 * CL-INTEGRATOR.c
 * Part1_DCM2Euler.c
 */

#include "BOARD.h"
#include "CL-INTEGRATOR.h"
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
#include "BNO055.h"
#include <xc.h>
#include <sys/attribs.h>

/*
 * The main function simply starts the integrator
 */
int main(void) {
    BOARD_Init();
    BNO055_Init();
    
    // starts the timer for the integrator
    INTEGRATOR_Init();
    
    // for timer
    double last_time = 0;
    
    while(1) {
        if(INTEGRATOR_TimeElapsed() > (last_time + 0.25)) {
            last_time = INTEGRATOR_TimeElapsed();
            
            double Euler[3][1];
            INTEGRATOR_GetEuler(Euler);
            
            printf("y: %d,  p: %d,  R: %d,       t: %f\n", \
                    (int)Euler[0][0], (int)Euler[1][0], (int)Euler[2][0], last_time);
        }
    }
    
    while(1);
}
