/* 
 * File:   Lab4_Main.c
 * Author: Richard Owens, Luca Altaffer
 *
 * Created on May 23, 2022, 3:26 PM
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "BOARD.h"
#include "serial.h"
#include <xc.h>
#include <I2C.h>
#include "BNO055.h"
#include "MatrixMath.h"
#include "Part3_Rexp.h"
#include "timers.h"


#define PART3


/******************
 Variables
 ******************/
int delT = 1; // time step in seconds
double omega[3] = {0,0,0};     // matrix that will be updated by p,q,r
double matrix_exp[3][3] = {     // matrix holder for R_exp
    {0,0,0},
    {0,0,0},
    {0,0,0} 
};
double newMatrix[3][3] = {       // matrix holder of the new position R matrix
    {0,0,0},
    {0,0,0},
    {0,0,0} 
}; 

int main(void) {
    // initializing libraries
    BOARD_Init();
    printf("test\n\r");
    BNO055_Init();
    printf("test2\n\r");
    TIMERS_Init();
     /*****
     Part3
     *****/
#ifdef PART3
    // forward integration with Rexp
    while(1){
        if (TIMERS_GetMilliSeconds() % 1000 >= 3) {
            Integrate_R(omega,delT,matrix_exp, newMatrix);
            printf("R with Rexp: %d\n\r", newMatrix);
//            printf("simple R: %d\n\r", ).
            
            
        }
    }
    
#endif


}