
#include "BOARD.h"
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
#include "BNO055.h"
#include <xc.h>
#include <sys/attribs.h>
#include "step_detection.h"

int iterate_running_average_gyro(int gyro_sample_size, int gyro_running_avg_arr[], int gyro_new_value);