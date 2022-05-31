/* 
 * File:   main.c
 * Author: Luca Altaffer and Richard Owens
 *
 * Created on May 31, 2022, 1:57 PM
 */

#include "BOARD.h"
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
#include "BNO055.h"
#include <xc.h>
#include <sys/attribs.h>

/*
 * This main function runs the data collection of our project
 */
int main(int argc, char** argv) {
    BOARD_Init();
    BNO055_Init();
    
    
    while(1);
}

