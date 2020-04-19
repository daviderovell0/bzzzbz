#define BOOST_TEST_MODULE sampletest
#include <boost/test/unit_test.hpp>
#include "../src/AudioProcessing.h"

class PrintSampleCallBack : public AudioProcessingCallback {
     virtual void process(float* in_buffer, float* out_buffer) {
         printf("%f\n", in_buffer);
     }
};

AudioProcessing *ap = new AudioProcessing();

// Test suite
BOOST_AUTO_TEST_CASE(FailTest)
{
    
    BOOST_CHECK_EQUAL(5, 5);
}

BOOST_AUTO_TEST_CASE(PassTest)
{
    BOOST_CHECK_EQUAL(4, 5);
}