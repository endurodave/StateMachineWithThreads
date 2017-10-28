#include "PressureTest.h"
#include "SelfTestEngine.h"

//------------------------------------------------------------------------------
// PressureTest
//------------------------------------------------------------------------------
PressureTest::PressureTest() :
	SelfTest(ST_MAX_STATES)
{
}
	
//------------------------------------------------------------------------------
// Start
//------------------------------------------------------------------------------
void PressureTest::Start()
{
	BEGIN_TRANSITION_MAP			              			// - Current State -
		TRANSITION_MAP_ENTRY (ST_START_TEST)				// ST_IDLE
		TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)				// ST_COMPLETED
		TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)				// ST_FAILED
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)				// ST_START_TEST
	END_TRANSITION_MAP(NULL)
}


//------------------------------------------------------------------------------
// StartTest
//------------------------------------------------------------------------------
STATE_DEFINE(PressureTest, StartTest, NoEventData)
{
	SelfTestEngine::InvokeStatusCallback("PressureTest::ST_StartTest");
	InternalEvent(ST_COMPLETED);
}


