/** @file TestLatency.h
 *
 * @brief Class for latency test using an oscilloscope and a dedicated gpio pin.
 * 
 * @author Marcell Illyes (marcellillyes)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "gpio-sysfs.h"

/**
 * \class TestLatency
 * \brief Accesses the state of a dedicated GPIO pin
 **/

class TestLatency{
public:
    /**
	 * \brief Constructor to initialize a GPIO pin with a given state
	 * \param unsigned int pin - Choose the GPIO pin to test with
     * \param unsigned int state - initialize state with 0 or 1
	 **/
    TestLatency(unsigned int pin , unsigned int state);

    /**
     * Changes the state of the test pin
     */
    void change_state(unsigned int pin);

private:
    unsigned int state_flag;
};
