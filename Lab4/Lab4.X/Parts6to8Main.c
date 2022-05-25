/* 
 * File:   Parts6to8Main.c
 * Author: richd
 *
 * Created on May 5, 2022, 3:19 PM
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
    float last_time = 0;
    
    while(1) {
        if(INTEGRATOR_TimeElapsed() > (last_time + 1)) {
            last_time = INTEGRATOR_TimeElapsed();
            
            float Euler[3][1];
            INTEGRATOR_GetEuler(Euler);
            
            printf("yaw: %f  ,  pitch: %f  ,  roll: %f  ,  time: %f\n", \
                    Euler[0][0], Euler[1][0], Euler[2][0], last_time);
        }
    }
    
    while(1);
}
