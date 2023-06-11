#include "GPIODUtils.h"
#include <cstdlib>


// GPIODUtils::GPIODUtils(const char* name) {
// }

int GPIODUtils::get_value(unsigned int gpio){
    return 1;
}
int GPIODUtils::set_value(unsigned int gpio, unsigned int *value) {
    return 1;
}
void GPIODUtils::close() {
    chip.close();
}