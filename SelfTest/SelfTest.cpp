#include "SelfTest.h"
#include "SelfTestEngine.h"

//------------------------------------------------------------------------------
// SelfTest
//------------------------------------------------------------------------------
SelfTest::SelfTest(INT maxStates) :
	StateMachine(maxStates)
{
}

//------------------------------------------------------------------------------
// Cancel
//------------------------------------------------------------------------------
void SelfTest::Cancel()
{
	// State machine base classes can't use a transition map, only the 
	// most-derived state machine class within the hierarchy can. So external 
	// events like this use the current state and call ExternalEvent()
	// to invoke the state machine transition. 
	if (GetCurrentState() != ST_IDLE)
		ExternalEvent(ST_FAILED);
}

//------------------------------------------------------------------------------
// Idle
//------------------------------------------------------------------------------
STATE_DEFINE(SelfTest, Idle, NoEventData)
{
	SelfTestEngine::InvokeStatusCallback("SelfTest::ST_Idle");
}

//------------------------------------------------------------------------------
// EntryIdle
//------------------------------------------------------------------------------
ENTRY_DEFINE(SelfTest, EntryIdle, NoEventData)
{
	SelfTestEngine::InvokeStatusCallback("SelfTest::EN_EntryIdle");
}

//------------------------------------------------------------------------------
// Completed
//------------------------------------------------------------------------------
STATE_DEFINE(SelfTest, Completed, NoEventData)
{
	SelfTestEngine::InvokeStatusCallback("SelfTest::ST_Completed");

	if (CompletedCallback)
		CompletedCallback(NoData());

	InternalEvent(ST_IDLE);
}

//------------------------------------------------------------------------------
// Failed
//------------------------------------------------------------------------------
STATE_DEFINE(SelfTest, Failed, NoEventData)
{
	SelfTestEngine::InvokeStatusCallback("SelfTest::ST_Failed");

	if (FailedCallback)
		FailedCallback(NoData());

	InternalEvent(ST_IDLE);
}

