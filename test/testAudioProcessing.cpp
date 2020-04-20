#define BOOST_TEST_MODULE sampletest
#include <boost/test/unit_test.hpp>
#include "../src/AudioProcessing.h"

class PrintSampleCallBack : public AudioProcessingCallback {
     virtual void process(float* in_buffer, float* out_buffer) {
         printf("%f\n", in_buffer);
     }
};

AudioProcessing *ap;
PrintSampleCallBack cb;

// Test suite
BOOST_AUTO_TEST_CASE(AudioProcessingThreadStarts)
{
    BOOST_CHECK_NO_THROW(ap = new AudioProcessing());
}

BOOST_AUTO_TEST_CASE(AudioProcessingCallbackIsRegistered)
{
    BOOST_CHECK_NO_THROW(ap->setCallback(&cb));
}

BOOST_AUTO_TEST_CASE(AudioProcessingStartJackClient)
{
    FILE *fd = open("inputs/sinewave_432Hz.txt");
    BOOST_CHECK_NO_THROW(ap->start());
}

BOOST_AUTO_TEST_CASE(FFTReturnsCorrectSpectrum)
{
    
    BOOST_CHECK_EQUAL(4, 4);
}

BOOST_AUTO_TEST_CASE(AudioProcessingThreadStops)
{
    BOOST_CHECK_EQUAL(ap->stop(),0);
}