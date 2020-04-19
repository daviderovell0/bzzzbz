/** @file TestLatency.h
 *
 * @brief Class for letency test using an oscilloscope and a dedicated gpio pin.
 * 
 * @author Marcell Illyes (marcellillyes)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "gpio-sysfs.h"

class TestLatency{
public:

    TestLatency(unsigned int pin , unsigned int state);

    /**
     * Change the state of the test pin
     */
    void change_state(unsigned int pin);

private:
    unsigned int state_flag;
};