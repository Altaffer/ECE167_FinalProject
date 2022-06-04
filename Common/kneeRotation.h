/* 
 * File:   main.c
 * Author: Luca Altaffer and Richard Owens
 *
 * Created on May 31, 2022, 1:57 PM
 */

#ifndef kneeRotation
#define	kneeRotation

/**
 * @Function bendKnee

 * @return none
 * @brief  sums the changes of angle for the flex sensor
 * @param sum; change in angle return
 * @author Luca Altaffer */
void bendKnee(int *sum, int maxread, int minread);

int iterate_running_average_rot(int sample_size, int running_avg_arr[], int new_value);

#endif