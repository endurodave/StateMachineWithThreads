#ifndef _SELF_TEST_H
#define _SELF_TEST_H

#include "StateMachine.h"
#include "AsyncCallback.h"

/// @brief SelfTest is a subclass state machine for other self-tests to 
/// inherit from. The class has common states for all derived classes to
/// share. 
class SelfTest : public StateMachine
{
public:
	// Callbacks generated when the self-test completes or fails.
	AsyncCallback<> CompletedCallback;
	AsyncCallback<> FailedCallback;

	SelfTest(INT maxStates);

	// Starts the self-test
	virtual void Start() = 0;

	// Cancels the self-test
	void Cancel();

protected:
	// State enumeration order must match the order of state method entries
	// in the state map.
	enum States
	{
		ST_IDLE,
		ST_COMPLETED,
		ST_FAILED,
		ST_MAX_STATES
	};

	// Define the state machine states
	STATE_DECLARE(SelfTest, 	Idle,			NoEventData)
	ENTRY_DECLARE(SelfTest, 	EntryIdle,		NoEventData)
	STATE_DECLARE(SelfTest, 	Completed,		NoEventData)
	STATE_DECLARE(SelfTest, 	Failed,			NoEventData)
};

#endif