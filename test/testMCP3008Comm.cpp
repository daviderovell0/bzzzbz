/** \file testMCP3008Comm.cpp
 * 
 *  \brief Unit test suite for the MCP3008Comm class
 * 
 *  \note Always run tests from the repository home "bzzzbz"
 * 
 *  \author Davide Rovelli (daviderovell0)
 * 
 */

#define BOOST_TEST_MODULE sampletest
#include <iostream>
#include <fstream>
#include <boost/test/unit_test.hpp>
#include "../src/MCP3008Comm.h"



int test_vector[2];


class MCP3008printSampleCallback : public MCP3008callback {
	virtual void hasSample(int value, int channel) {
		test_vector[0]=value;
        test_vector[1]=channel;
	}
};

MCP3008Comm* mcp;
MCP3008printSampleCallback cb;
/** 
 * \test MCP3008CommThreadStarts
 * 
 * \brief Check that the class constructor does not throw any exception
 * 
 */ 
BOOST_AUTO_TEST_CASE(MCP3008CommCreate)
{
    BOOST_CHECK_NO_THROW(mcp = new MCP3008Comm());
}

/** 
 * \test MCP3008CommCallbackIsRegistered
 * 
 * \brief Check that the callback registration does not throw any exception
 * 
 */ 
BOOST_AUTO_TEST_CASE(MCP3008CommCallbackIsRegistered)
{
    BOOST_CHECK_NO_THROW(mcp->setCallback(&cb));
}

/** 
 * \test MCP3008CommStart
 * 
 * \brief Check that the SPI data acquisition thread starts successfully
 */ 
BOOST_AUTO_TEST_CASE(MCP3008CommStart)
{
   
    BOOST_CHECK_NO_THROW(mcp->start());
}

/**
 * \test MCP3008CommSPIvaluesWithinRange
 * 
 * \brief Check if 3 samples values from the 3 channels (v1.0 bzzzbz prototype)
 * are within range: 0 means that the SPI is not communicating, vaules larger than 
 * 1024 are voltage spikes in the MISO.
 * 
 */ 
BOOST_AUTO_TEST_CASE(MCP3008CommSPIvaluesWithinRange)
{
    for(int i = 0; i < 3; i++){
        BOOST_CHECK_MESSAGE(test_vector[0] > 1 && test_vector[0] < 1025, "Value outside range in channel: " << test_vector[1]);
    }
    
}

/** 
 * \test MCP3008CommThreadStops
 * 
 * \brief Check that the class and SPI communication terminates correctly
 * 
 */ 
BOOST_AUTO_TEST_CASE(MCP3008CommThreadStops)
{
    BOOST_CHECK_NO_THROW(mcp->stop());
}