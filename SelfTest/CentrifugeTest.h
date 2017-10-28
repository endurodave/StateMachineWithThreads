#ifndef _CENTRIFUGE_TEST_H
#define _CENTRIFUGE_TEST_H

#include "SelfTest.h"
#include "Timer.h"

// @brief CentrifugeTest shows StateMachine features including state machine
// inheritance, state function override, and guard/entry/exit actions. SelfTest
// provides common states shared with CentrifugeTest. 
class CentrifugeTest : public SelfTest
{
public:
	CentrifugeTest();

	virtual void Start();
	void Poll();

private:
	INT m_speed; 

	// Timer used to generate periodic callbacks to the Poll() event.
	Timer m_pollTimer;

	// State enumeration order must match the order of state method entries
	// in the state map.
	enum States
	{
		// Continue state numbering using the last SelfTest::States enum value
		ST_START_TEST = SelfTest::ST_MAX_STATES,	
		ST_ACCELERATION,
		ST_WAIT_FOR_ACCELERATION,
		ST_DECELERATION,
		ST_WAIT_FOR_DECELERATION,
		ST_MAX_STATES
	};

	// Define the state machine state functions with event data type
	STATE_DECLARE(CentrifugeTest, 	Idle,						NoEventData)
	STATE_DECLARE(CentrifugeTest, 	StartTest,					NoEventData)
	GUARD_DECLARE(CentrifugeTest, 	GuardStartTest,				NoEventData)
	STATE_DECLARE(CentrifugeTest, 	Acceleration,				NoEventData)
	STATE_DECLARE(CentrifugeTest, 	WaitForAcceleration,		NoEventData)
	EXIT_DECLARE(CentrifugeTest, 	ExitWaitForAcceleration)
	STATE_DECLARE(CentrifugeTest, 	Deceleration,				NoEventData)
	STATE_DECLARE(CentrifugeTest, 	WaitForDeceleration,		NoEventData)
	EXIT_DECLARE(CentrifugeTest, 	ExitWaitForDeceleration)

	// State map to define state object order. Each state map entry defines a
	// state object.
	BEGIN_STATE_MAP_EX
		STATE_MAP_ENTRY_ALL_EX(&Idle, 0, &EntryIdle, 0)
		STATE_MAP_ENTRY_EX(&Completed)
		STATE_MAP_ENTRY_EX(&Failed)
		STATE_MAP_ENTRY_ALL_EX(&StartTest, &GuardStartTest, 0, 0)
		STATE_MAP_ENTRY_EX(&Acceleration)
		STATE_MAP_ENTRY_ALL_EX(&WaitForAcceleration, 0, 0, &ExitWaitForAcceleration)
		STATE_MAP_ENTRY_EX(&Deceleration)
		STATE_MAP_ENTRY_ALL_EX(&WaitForDeceleration, 0, 0, &ExitWaitForDeceleration)
	END_STATE_MAP_EX	

	CALLBACK_DECLARE_NO_DATA(CentrifugeTest, Poll)
};

#endif
