#include "TestLatency.h"

TestLatency::TestLatency(unsigned int pin,unsigned int state){
    
    gpio_export(pin);
    gpio_set_dir(pin,1);
    gpio_set_value(pin,state);
    state_flag = state;
}

void TestLatency::change_state(unsigned int pin){
    if(state_flag){state_flag=0;}
    else{state_flag=1;}
    gpio_set_value(pin,state_flag);

}