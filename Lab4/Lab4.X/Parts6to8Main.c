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
    
    while(1) {
        
    }
    
    while(1);
}
