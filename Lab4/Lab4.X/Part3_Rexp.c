/* 
 * File:   Part2_DCM2Euler
 * Author: Richard Owens, Luca Altaffer
 *
 * Created on May 5, 2022, 3:39 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "BOARD.h"
#include "serial.h"
#include "BNO055.h"
#include <I2C.h>
#include "MatrixMath.h"

// Foward Init

char BNO055_Init(void);
double wx[3][3] = {// Initialization of the 3x3 matrix
    {},
    {},
    {}
};
double I[3][3] = {// Identity Matrix
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
};
double result1[3][3] = {// Initialization of the 3x3 matrix
    {},
    {},
    {}
};
double result2[3][3] = {// Initialization of the 3x3 matrix
    {},
    {},
    {}
};
double result3[3][3] = {// Initialization of the 3x3 matrix
    {},
    {},
    {}
};
double result4[3][3] = {// Initialization of the 3x3 matrix
    {},
    {},
    {}
};
double result5[3][3] = {// Initialization of the 3x3 matrix
    {},
    {},
    {}
};
double result6[3][3] = {// Initialization of the 3x3 matrix
    {},
    {},
    {}
};
// Other vaiables used
double magw; // mantitude of omegaw 
double mag_wxdT; // magnitude of omegax time time step
double s; // holer fpr math involving sine
double c; // holder for math involving cosine
//double newR;
//double w[3];
//int dT;
//double R_exp[3][3];

void Rexp(double w[3], int dT, double R_exp[3][3], int biasp, int biasq, int biasr) {
    // Setting the gyro values to be in the matrix 
    w[0] = ((250 * (BNO055_ReadGyroZ()-biasp)) / (32767)) * 0.0174533; // p 
    w[1] = ((250 * (BNO055_ReadGyroX()-biasq)) / 32767) * 0.0174533; // q
    w[2] = ((250 * (BNO055_ReadGyroY()-biasr)) / 32767) * 0.0174533; // r

    // finding the magnitude of w
    magw = sqrt((pow(w[0], 2))+(pow(w[1], 2))+(pow(w[2], 2)));

    // Creating the wx matrix
    // row 1
    wx[0][0] = 0;
    wx[0][1] = -w[2];
    wx[0][2] = w[1];
    // row 2
    wx[1][0] = w[2];
    wx[1][1] = 0;
    wx[1][2] = -w[0];
    //row 3
    wx[2][0] = -w[1];
    wx[2][1] = w[0];
    wx[2][2] = 0;

    // mag of omegax dT
    mag_wxdT = sqrt((2 * pow((w[0] * dT), 2)) + (2 * pow((w[1] * dT), 2)) + (2 * pow((w[2] * dT), 2)));

    // Returning R_exp if the magnitude is larger than the threshold
    if (mag_wxdT >= 0.02) {
        s = sin(magw * dT) / magw; // sin math
        c = (1 - cos(magw * dT)) / (pow(magw, 2)); // cos math
        MatrixMultiply(wx, wx, result1);
        MatrixScalarMultiply(c, result1, result2);
        MatrixScalarMultiply(s, wx, result3);
        MatrixSubtract(I, result3, result4);
        MatrixAdd(result4, result2, R_exp);
    } else {
        s = dT - (((pow(dT, 3))*(pow(magw, 2))) / 6)+(((pow(dT, 5))*(pow(magw, 4))) / 120);
        c = (((pow(dT, 2)) / 2)-(((pow(dT, 4))*(pow(magw, 2)) / 24)+(((pow(dT, 6))*(pow(magw, 4)) / 720))));
        MatrixMultiply(wx, wx, result1);
        MatrixScalarMultiply(c, result1, result2);
        MatrixScalarMultiply(s, wx, result3);
        MatrixSubtract(I, result3, result4);
        MatrixAdd(result4, result2, R_exp);
    }
    //    R_exp = result5;
}

void Integrate_R(double w[3], int dT, double R_exp[3][3], double PrevR[3][3], double newR[3][3], int biasp, int biasq, int biasr) {
    // FORWARD INTEGRATION
    Rexp(w, dT, R_exp, biasp, biasq, biasr); // calling Rexp func
    MatrixMultiply(R_exp, PrevR, newR); // multiplying the previous position by the Rexp matrix
}