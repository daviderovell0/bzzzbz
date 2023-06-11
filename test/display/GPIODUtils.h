 /** 
 * @brief libgpiod wrapper class to simplify standard GPIO operations
 * @author daviderovell0
 */

#ifndef __GPIODUTILS_H
#define __GPIODUTILS_H

#include <gpiod.hpp>


class GPIODUtils {
    public:
        /**
         * \brief Constructor with the chardev chip. The default on Raspberry Pi 4
         * is gpiochip0
         **/
	     //GPIODUtils(const char* chip_name = "gpiochip0");
         int get_value(unsigned int gpio);
         int set_value(unsigned int gpio, unsigned int *value);
         void close();

         

    private:

        ::gpiod::chip chip;
};

#endif