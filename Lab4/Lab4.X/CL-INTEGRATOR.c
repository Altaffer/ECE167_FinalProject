/* 
 * File:   Part6.c
 * Author: richd
 *
 * Created on May 23, 2022, 10:12 PM
 */

#include "BOARD.h"
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
#include "BNO055.h"
#include <xc.h>
#include <sys/attribs.h>
#include <math.h>
#include "Part1_DCM2Euler.h"

/*
 * ------------------ Block below are some MM helper functions -------------------------
 */
void normalize(float A[3][3], int meas[3][1], float B[3][1], float hat[3][1]) {
    
    hat[0][0] = A[0][0]*meas[0][0] + A[0][1]*meas[1][0] + A[0][2]*meas[2][0] + B[0][0];
    hat[1][0] = A[1][0]*meas[0][0] + A[1][1]*meas[1][0] + A[1][2]*meas[2][0] + B[1][0];
    hat[2][0] = A[2][0]*meas[0][0] + A[2][1]*meas[1][0] + A[2][2]*meas[2][0] + B[2][0];
    
    return;
}

void R_OnebyThree(float A[3][3], float meas[3][1], float hat[3][1]) {
    hat[0][0] = A[0][0]*meas[0][0] + A[0][1]*meas[1][0] + A[0][2]*meas[2][0];
    hat[1][0] = A[1][0]*meas[0][0] + A[1][1]*meas[1][0] + A[1][2]*meas[2][0];
    hat[2][0] = A[2][0]*meas[0][0] + A[2][1]*meas[1][0] + A[2][2]*meas[2][0];
    return;
}

void skew(float A[3][1], float result[3][3]) {
    result[0][0] = 0;
    result[0][1] = -A[2][0];
    result[0][2] = A[1][0];
    
    result[1][0] = A[2][0];
    result[1][1] = 0;
    result[1][2] = -A[0][0];
    
    result[2][0] = -A[1][0];
    result[2][1] = A[0][0];
    result[2][2] = 0;
    
    return;
}
/*
 * ------------------End helper function block, below is CL integration -----------------
 */


/*
 *
 * Below is the matrices from the tumble test calibration in Lab 3
 *
 */
static float Atilde_mag[3][3] = {{0.0012, 0, 0},
                                {0, 0.0013, 0},
                                {0, 0, 0.0013}};
static float Btilde_mag[3][1] = {{0.8127},
                                {-0.3073},
                                {0.5926}};
static float Atilde_accel[3][3] = {{0.9736e-03, -0.0110e-03, -0.0051e-03},
                                {-0.0045e-03, 0.9818e-03, 0.0350e-03},
                                {0.0009e-03, 0.0014e-03, 0.9732e-03}};
static float Btilde_accel[3][1] = {{0.0244},
                                {0.0269},
                                {0.0520}};

/*
 * Below are the inertial unit vectors for accelerometer and magnetometer
 */
static float a_i[3][1] = {{0},
                            {0},
                            {1}};
static float m_i[3][1] = {{0.4772},
                            {0.1127},
                            {0.8716}};

/*
 * Below is Rmis from batch misalignment
 */
static float Rmis[3][3] = {{1,0,0},
                        {0,1,0},
                        {0,0,1}};

/*
 * Below are the holders for current values of R and Bs, initted to 0
 */
float R_k[3][3] = {{1,0,0},
                    {0,1,0},
                    {0,0,1}};
float b_k[3][1] = {{0},
                    {0},
                    {0}}; 

/*
 * Below are the placeholder matrices for R_plus and B_plus
 */
float R_kPlus[3][3];
float b_kPlus[3][1];

/*
 * Below are some integration parameters
 */
float dT = 0.02;        // in seconds, 50Hz
float Kp_a = 1;         // accelerometer proportional gain
float Ki_a = 1/10;      // Kp_a/10;    // acc integral gain
float Kp_m = 0.5;       // mag prop gain
float Ki_m = 0.5/10;    // Kp_m/10;   // mag int gain

/*
 * This value holds a running timer value
 */
float time_elapsed = 0; // in seconds

/**
 * @function    CL_Integrate()
 * @brief       Integrates a DCM and Gyro bial
 * @parameters  R_minus
 *              b_minus
 *              gyro_meas - in Int16
 *              acc_meas - in Int16
 *              mag_meas - in Int16
 * @return      Returns R_plus and b_plus
 * 
 * NOTE: following lab appendix A with this integrator
 */
void CL_Integrate(float R_plus[3][3], float b_plus[3][1], float R_minus[3][3], float b_minus[3][1], \
                    int gyro_meas[3][1], int acc_meas[3][1], int mag_meas[3][1]) {
    
    /*
     * convert data to unit norm and align mag and accel
     */
    float a_hat[3][1];  // unit norm of accel data
    float m_hat[3][1];  // unit norm of mag data
    normalize(Atilde_accel, acc_meas, Btilde_accel, a_hat);
    normalize(Atilde_mag, mag_meas, Btilde_mag, m_hat);
    
    float m_aligned[3][1]; // aligned m matrix
    R_OnebyThree(Rmis, m_hat, m_aligned);
    
    /*
     * convert gyro Int16 into rad/sec
     */
    float gyro_rps[3][1] = {{gyro_meas[0][0] * (1/((1/250)*(pow(2,15)-1)) * 0.0174533)},
                            {gyro_meas[1][0] * (1/((1/250)*(pow(2,15)-1)) * 0.0174533)},
                            {gyro_meas[2][0] * (1/((1/250)*(pow(2,15)-1)) * 0.0174533)}};  
    
    /*
     * solve for omegas using feedback (see lab appendx)
     */
    float w[3][1] = {{gyro_rps[0][0] - b_minus[0][0]},
                    {gyro_rps[1][0] - b_minus[1][0]},
                    {gyro_rps[2][0] - b_minus[2][0]}};
    
    // for:  w meas a = (ab_skew)(R*a_i)
    float Rai[3][1];
    R_OnebyThree(R_minus, a_i, Rai);
    float abcross[3][3];
    skew(a_hat, abcross);
    
    float w_measA[3][1];
    R_OnebyThree(abcross, Rai, w_measA);
    
    // for:  w meas m = (mb_skew)(R*m_i)
    float Rmi[3][1];
    R_OnebyThree(R_minus, m_i, Rmi);
    float mbcross[3][3];
    skew(m_aligned, mbcross);
    
    float w_measM[3][1];
    R_OnebyThree(mbcross, Rmi, w_measM);
    
    // for w total = w + Kpa*wmeasA + kpm*wmeasM
    float w_total[3][1] = {{w[0][0] + Kp_a*w_measA[0][0] + Kp_m*w_measM[0][0]},
                            {w[1][0] + Kp_a*w_measA[1][0] + Kp_m*w_measM[1][0]},
                            {w[2][0] + Kp_a*w_measA[2][0] + Kp_m*w_measM[2][0]}};
    
    /*
     * Integrate R using matrix exponential as in part 3 of the lab
     */
    
    // - - - - - - - - - - - - - - - - -  - - - 
    // - - - - - - -  - - - 
    
    /*
     * integrate the bias b
     */
    
    // the rate of change of the bias using the integral gains to scale
    float b_dot[3][1] = {{-Ki_a*w_measA[0][0] - Ki_m*w_measM[0][0]},
                        {-Ki_a*w_measA[0][0] - Ki_m*w_measM[0][0]},
                        {-Ki_a*w_measA[0][0] - Ki_m*w_measM[0][0]}};
    
    // forward integration of the bias
    b_plus[0][0] = b_minus[0][0] + b_dot[0][0]*dT;
    b_plus[1][0] = b_minus[1][0] + b_dot[1][0]*dT;
    b_plus[2][0] = b_minus[2][0] + b_dot[2][0]*dT;
    
    return;
}

/*
 * Below is the integrator, that operates through a timer.
 * The main function simply needs to INIT the integrator and can then call for
 * the R value
 */

char INTEGRATOR_Init(void) {
    // following block inits the timer
    T4CON = 0;
    T4CONbits.TCKPS = 0b110;
    PR4 = 12500;                // ____ initialize to dT - 20ms -> 50Hz ______
    T4CONbits.ON = 1;
    IFS0bits.T4IF = 0;
    IPC4bits.T4IP = 3;
    IEC0bits.T4IE = 1;
    // other necessary inits below
    
}

void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void) {
    IFS0bits.T4IF = 0;
    //Anything else that needs to occur goes here
    
    // add time elapsed to counter
    time_elapsed += dT;
    
    // collect data at each timestep
    int gyros[3][1] = {{BNO055_ReadGyroX()},
                        {BNO055_ReadGyroY()},
                        {BNO055_ReadGyroZ()}};
    
    int accels[3][1] = {{BNO055_ReadAccelX()},
                        {BNO055_ReadAccelY()},
                        {BNO055_ReadAccelZ()}};
    
    int mags[3][1] = {{BNO055_ReadMagX()},
                    {BNO055_ReadMagY()},
                    {BNO055_ReadMagZ()}};
    
    // run the integration, it will update R_kPlus and b_kPlus
    CL_Integrate(R_kPlus, b_kPlus, R_k, b_k, gyros, accels, mags);
    
    // iterate the R and b placeholders
    R_k[0][0] = R_kPlus[0][0];
    R_k[0][1] = R_kPlus[0][1];
    R_k[0][2] = R_kPlus[0][2];
    
    R_k[1][0] = R_kPlus[1][0];
    R_k[1][1] = R_kPlus[1][1];
    R_k[1][2] = R_kPlus[1][2];
    
    R_k[2][0] = R_kPlus[2][0];
    R_k[2][1] = R_kPlus[2][1];
    R_k[2][2] = R_kPlus[2][2];
    
    b_k[0][0] = b_kPlus[0][0];
    b_k[1][0] = b_kPlus[1][0];
    b_k[2][0] = b_kPlus[2][0];
}

/*
 * function here converts the DCM to euler angles and returns them
 */
float INTEGRATOR_GetEuler(void) {
    return DCM2Euler(R_k);
}

/*
 * this checks the current time on the timer in ms
 */
float INTEGRATOR_TimeElapsed(void) {
    return time_elapsed;
}


