#ifndef UTILS_H
#define UTILS_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define LEN(arr) ((int)(sizeof(arr) / sizeof(arr)[0]))

/**
 * @brief Delay execution with millisecond precision.
 *
 * @param tms Milliseconds to sleep for.
 */
void msleep(int tms);

/**
 * @brief Converts a char* to a number. Serves as a wrapper function for
 * strtol().
 *
 * @param str String to convert.
 * @param numbase Base of number (10 is decimal, 16 is hexadecimal.)
 * @return int
 */
int chartoint(char *str, int numbase);

/**
 * @brief Get the time.
 *
 * @return struct tm The time.
 */
struct tm get_time();

/**
 * @brief Repeatedly print a single character `times` times.
 *
 * @param ch Character to repeat.
 * @param times How many times to print character.
 */
void repeat(char ch, int times);
#endif /* UTILS_H */
