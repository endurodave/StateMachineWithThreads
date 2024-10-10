#ifndef _SELF_TEST_ENGINE_H
#define _SELF_TEST_ENGINE_H

#include "SelfTest.h"
#include "CentrifugeTest.h"
#include "PressureTest.h"
#include "WorkerThreadStd.h"
#include "AsyncCallback.h"
#include <string>

struct SelfTestStatus
{
	std::string message;
};

/// @brief The master self-test state machine used to coordinate the execution of the 
/// sub self-test state machines. The class is thread-safe. 
class SelfTestEngine : public SelfTest
{
public:
	// Clients register for asynchronous self-test status callbacks
	static AsyncCallback<SelfTestStatus> StatusCallback;

	// Singleton instance of SelfTestEngine
	static SelfTestEngine& GetInstance();

	// Start the self-tests 
	void Start();

	WorkerThread& GetThread() { return m_thread; }
	static void InvokeStatusCallback(std::string msg);

private:
	AsyncCallback<> StartCallback;
	void StartPrivateCallback();

	SelfTestEngine();
	void Complete();

	// Sub self-test state machines 
	CentrifugeTest m_centrifugeTest;
	PressureTest m_pressureTest;

	// Worker thread used by all self-tests
	WorkerThread m_thread;

	// State enumeration order must match the order of state method entries
	// in the state map.
	enum States
	{
		ST_START_CENTRIFUGE_TEST = SelfTest::ST_MAX_STATES,
		ST_START_PRESSURE_TEST,
		ST_MAX_STATES
	};

	// Define the state machine state functions with event data type
	STATE_DECLARE(SelfTestEngine, 	StartCentrifugeTest,	NoEventData)
	STATE_DECLARE(SelfTestEngine, 	StartPressureTest,		NoEventData)

	// State map to define state object order. Each state map entry defines a
	// state object.
	BEGIN_STATE_MAP
		STATE_MAP_ENTRY(&Idle)
		STATE_MAP_ENTRY(&Completed)
		STATE_MAP_ENTRY(&Failed)
		STATE_MAP_ENTRY(&StartCentrifugeTest)
		STATE_MAP_ENTRY(&StartPressureTest)
	END_STATE_MAP	

	// Declare state machine events that receive async callbacks
	CALLBACK_DECLARE_NO_DATA(SelfTestEngine,	StartPrivateCallback)
	CALLBACK_DECLARE_NO_DATA(SelfTestEngine,	Complete)
	CALLBACK_DECLARE_NO_DATA(SelfTest,			Cancel)
};

#endif