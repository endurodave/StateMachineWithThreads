#include "CentrifugeTest.h"
#include "SelfTestEngine.h"
#include <sstream>

//------------------------------------------------------------------------------
// CentrifugeTest
//------------------------------------------------------------------------------
CentrifugeTest::CentrifugeTest() :
	SelfTest(ST_MAX_STATES),
	m_speed(0)
{
}

//------------------------------------------------------------------------------
// Start
//------------------------------------------------------------------------------
void CentrifugeTest::Start()
{
	BEGIN_TRANSITION_MAP			              			// - Current State -
		TRANSITION_MAP_ENTRY (ST_START_TEST)				// ST_IDLE
		TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)				// ST_COMPLETED
		TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)				// ST_FAILED
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)				// ST_START_TEST
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)				// ST_ACCELERATION
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)				// ST_WAIT_FOR_ACCELERATION
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)				// ST_DECELERATION
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)				// ST_WAIT_FOR_DECELERATION
	END_TRANSITION_MAP(NULL)
}

//------------------------------------------------------------------------------
// Poll
//------------------------------------------------------------------------------
void CentrifugeTest::Poll()
{
	BEGIN_TRANSITION_MAP			              			// - Current State -
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)				// ST_IDLE
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)				// ST_COMPLETED
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)				// ST_FAILED
		TRANSITION_MAP_ENTRY (EVENT_IGNORED)				// ST_START_TEST
		TRANSITION_MAP_ENTRY (ST_WAIT_FOR_ACCELERATION)		// ST_ACCELERATION
		TRANSITION_MAP_ENTRY (ST_WAIT_FOR_ACCELERATION)		// ST_WAIT_FOR_ACCELERATION
		TRANSITION_MAP_ENTRY (ST_WAIT_FOR_DECELERATION)		// ST_DECELERATION
		TRANSITION_MAP_ENTRY (ST_WAIT_FOR_DECELERATION)		// ST_WAIT_FOR_DECELERATION
	END_TRANSITION_MAP(NULL)
}

//------------------------------------------------------------------------------
// Idle - Idle state here overrides the SelfTest Idle state. 
//------------------------------------------------------------------------------
STATE_DEFINE(CentrifugeTest, Idle, NoEventData)
{
	SelfTestEngine::InvokeStatusCallback("CentrifugeTest::ST_Idle");

	// Call base class Idle state
	SelfTest::ST_Idle(data);	

	// Unregister for timer callbacks 
	m_pollTimer.Expired.Unregister(&CentrifugeTest::Poll, &SelfTestEngine::GetInstance().GetThread(), this);
	m_pollTimer.Stop();
}

//------------------------------------------------------------------------------
// StartTest - Start the centrifuge test state.
//------------------------------------------------------------------------------
STATE_DEFINE(CentrifugeTest, StartTest, NoEventData)
{
	SelfTestEngine::InvokeStatusCallback("CentrifugeTest::ST_StartTest");

	// Register for timer callbacks 
	m_pollTimer.Expired.Register(&CentrifugeTest::Poll, &SelfTestEngine::GetInstance().GetThread(), this);

	InternalEvent(ST_ACCELERATION);
}

//------------------------------------------------------------------------------
// GuardStartTest - Guard condition to detemine whether StartTest state is executed.
//------------------------------------------------------------------------------
GUARD_DEFINE(CentrifugeTest, GuardStartTest, NoEventData)
{
	SelfTestEngine::InvokeStatusCallback("CentrifugeTest::GD_GuardStartTest");
	if (m_speed == 0)
		return TRUE;	// Centrifuge stopped. OK to start test.
	else
		return FALSE;	// Centrifuge spinning. Can't start test.
}

//------------------------------------------------------------------------------
// Acceleration - Start accelerating the centrifuge.
//------------------------------------------------------------------------------
STATE_DEFINE(CentrifugeTest, Acceleration, NoEventData)
{
	SelfTestEngine::InvokeStatusCallback("CentrifugeTest::ST_Acceleration");

	// Start polling while waiting for centrifuge to ramp up to speed
	m_pollTimer.Start(std::chrono::milliseconds(10));
}

//------------------------------------------------------------------------------
// WaitForAcceleration - Wait in this state until target centrifuge speed is reached.
//------------------------------------------------------------------------------
STATE_DEFINE(CentrifugeTest, WaitForAcceleration, NoEventData)
{
	std::ostringstream ss;
	ss << "CentrifugeTest::ST_WaitForAcceleration : Speed is " << m_speed;
	SelfTestEngine::InvokeStatusCallback(ss.str());

	if (++m_speed >= 5)
		InternalEvent(ST_DECELERATION);
}

//------------------------------------------------------------------------------
// ExitWaitForAcceleration - Exit action when WaitForAcceleration state exits.
//------------------------------------------------------------------------------
EXIT_DEFINE(CentrifugeTest, ExitWaitForAcceleration)
{
	SelfTestEngine::InvokeStatusCallback("CentrifugeTest::EX_ExitWaitForAcceleration");

	// Acceleration over, stop polling
	m_pollTimer.Stop();
}

//------------------------------------------------------------------------------
// Deceleration - Start decelerating the centrifuge.
//------------------------------------------------------------------------------
STATE_DEFINE(CentrifugeTest, Deceleration, NoEventData)
{
	SelfTestEngine::InvokeStatusCallback("CentrifugeTest::ST_Deceleration");

	// Start polling while waiting for centrifuge to ramp down to 0
	m_pollTimer.Start(std::chrono::milliseconds(10));
}

//------------------------------------------------------------------------------
// WaitForDeceleration - Wait in this state until centrifuge speed is 0.
//------------------------------------------------------------------------------
STATE_DEFINE(CentrifugeTest, WaitForDeceleration, NoEventData)
{
	std::ostringstream ss;
	ss << "CentrifugeTest::ST_WaitForDeceleration : Speed is " << m_speed;
	SelfTestEngine::InvokeStatusCallback(ss.str());

	if (m_speed-- == 0)
		InternalEvent(ST_COMPLETED);
}

//------------------------------------------------------------------------------
// ExitWaitForDeceleration - Exit action when WaitForDeceleration state exits.
//------------------------------------------------------------------------------
EXIT_DEFINE(CentrifugeTest, ExitWaitForDeceleration)
{
	SelfTestEngine::InvokeStatusCallback("CentrifugeTest::EX_ExitWaitForDeceleration");

	// Deceleration over, stop polling
	m_pollTimer.Stop();
}
