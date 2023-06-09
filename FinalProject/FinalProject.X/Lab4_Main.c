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
#include "Part1_DCM2Euler.h"


#define PART3


/******************
 Variables
 ******************/
int delT = 1; // time step in seconds
double omega[3] = {0, 0, 0}; // matrix that will be updated by p,q,r
double matrix_exp[3][3] = {// matrix holder for R_exp
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};
double prevMatrix[3][3] = {// matrix holder of the previous position R matrix
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
};
double newMatrix[3][3] = {// matrix holder of the new position R matrix
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
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
    int a = 0;
    int tally = 0;
    int biasp = 0;
    int biasq = 0;
    int biasr = 0;
    printf("collecting biases\n");
    while (1) {
        // sum the biases over 10 seconds
        if (TIMERS_GetMilliSeconds() < 10000 && a == 0) {
            tally += 1;
            
            biasp += BNO055_ReadGyroZ();
            biasq += BNO055_ReadGyroX();
            biasr += BNO055_ReadGyroY();
            
        }
        // take the average of the biases
        if (TIMERS_GetMilliSeconds() > 10000 && a == 0) {
            biasp = biasp / tally;
            biasp = biasp / tally;
            biasp = biasp / tally;
            a = 1;
        }
        if (TIMERS_GetMilliSeconds() % 1000 <= 3 && a == 1) {
            Integrate_R(omega, delT, matrix_exp, prevMatrix, newMatrix, biasp, biasq, biasr);
            //printf("R with Rexp: \n\r");
            //printf("[%lf, %lf, %lf]\n\r", newMatrix[0][0], newMatrix[0][1], newMatrix[0][2]);
            //printf("[%lf, %lf, %lf]\n\r", newMatrix[1][0], newMatrix[1][1], newMatrix[1][2]);
            //printf("[%lf, %lf, %lf]\n\r", newMatrix[2][0], newMatrix[2][1], newMatrix[2][2]);
            prevMatrix[0][0] = newMatrix[0][0];
            prevMatrix[0][1] = newMatrix[0][1];
            prevMatrix[0][2] = newMatrix[0][2];
            prevMatrix[1][0] = newMatrix[1][0];
            prevMatrix[1][1] = newMatrix[1][1];
            prevMatrix[1][2] = newMatrix[1][2];
            prevMatrix[2][0] = newMatrix[2][0];
            prevMatrix[2][1] = newMatrix[2][1];
            prevMatrix[2][2] = newMatrix[2][2];
            double Eul[3][1];
            DCM2Euler(newMatrix, Eul);
            printf("yaw %f    pitch %f    roll %f   time %d\n", Eul[0][0], Eul[1][0], Eul[2][0], TIMERS_GetMilliSeconds()/1000);

        }
    }

#endif


}