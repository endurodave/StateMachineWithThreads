#ifndef _PRESSURE_TEST_H
#define _PRESSURE_TEST_H

#include "SelfTest.h"

/// @brief A state machine implementing the pressure test. SelfTest base class
/// provides the common states. 
class PressureTest : public SelfTest
{
public:
	PressureTest();

	virtual void Start();

private:
	// State enumeration order must match the order of state method entries
	// in the state map.
	enum States
	{
		// Continue state numbering using the last SelfTest::States enum value
		ST_START_TEST = SelfTest::ST_MAX_STATES,	
		ST_MAX_STATES
	};

	// Define the state machine state functions with event data type
	STATE_DECLARE(PressureTest, 	StartTest,				NoEventData)

	// State map to define state object order. Each state map entry defines a
	// state object.
	BEGIN_STATE_MAP
		STATE_MAP_ENTRY(&Idle)
		STATE_MAP_ENTRY(&Completed)
		STATE_MAP_ENTRY(&Failed)
		STATE_MAP_ENTRY(&StartTest)
	END_STATE_MAP	
};

#endif
