/* 
 * File:   testFlex.c
 * Author: taltaffe
 *
 * Created on May 31, 2022, 6:17 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"
#include "AD.h"
#include "timers.h"
#include "serial.h"
#include "kneeRotation.h"

/*
 * 
 */
double yeet = 0;

int main(void) {
    BOARD_Init();
    AD_Init();
    TIMERS_Init();
    AD_AddPins(AD_A0);
    while (1) {
        if (TIMERS_GetMilliSeconds() % 20 == 0) {
            //double bend = (90/(670-572))*((double)AD_ReadADPin(AD_A0) - 572);
            int read = (AD_ReadADPin(AD_A0) - 572);
            if (read < 0) {
                read = 0;
            }
            int bend = ((read) * 90) / (670-572);
            
            printf("theta = %d\n\r",bend);
        }
    }

    while (1);
}
