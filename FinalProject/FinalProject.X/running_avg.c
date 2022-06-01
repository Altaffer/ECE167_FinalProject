#include "BOARD.h"
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
#include "BNO055.h"
#include <xc.h>
#include <sys/attribs.h>
#include "step_detection.h"

int iterate_running_average(int sample_size, int running_avg_arr[], int new_value) {
    // create a temporary sum
    int average_sum = 0;
    
    // shift our sample array by 1 dropping the least recent value
    for (int k = sample_size; k > 0; k--){        
        running_avg_arr[k]= running_avg_arr[k-1];

        // while iterating, start summing all elements in the array
        // to take an average
        average_sum += running_avg_arr[k];
    }
    // add in the newest sample to the array and add it to the average sum
    running_avg_arr[0] = new_value;
    average_sum += running_avg_arr[0];

    // take the average over the sample size and reset the average sum
    return (average_sum / sample_size);
}