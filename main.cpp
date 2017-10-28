#include "WorkerThread.h"
#include "AsyncCallback.h"
#include "SelfTestEngine.h"
#include <iostream>

using namespace std;

// A thread to capture self-test status callbacks for output to the "user interface"
WorkerThread userInterfaceThread("UserInterface");

// Simple flag to exit main loop
BOOL selfTestEngineCompleted = FALSE;

//------------------------------------------------------------------------------
// SelfTestEngineStatusCallback
//------------------------------------------------------------------------------
void SelfTestEngineStatusCallback(const SelfTestStatus& status, void* userData)
{
	// Output status message to the console "user interface"
	cout << status.message.c_str() << endl;
}

//------------------------------------------------------------------------------
// SelfTestEngineCompleteCallback
//------------------------------------------------------------------------------
void SelfTestEngineCompleteCallback(const NoData& data, void* userData)
{
	selfTestEngineCompleted = TRUE;
}

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(void)
{	
	// Create the worker threads
	userInterfaceThread.CreateThread();
	SelfTestEngine::GetInstance().GetThread().CreateThread();

	// Register for self-test engine callbacks
	SelfTestEngine::StatusCallback.Register(&SelfTestEngineStatusCallback, &userInterfaceThread);
	SelfTestEngine::GetInstance().CompletedCallback.Register(&SelfTestEngineCompleteCallback, &userInterfaceThread);

	// Start the worker threads
	ThreadWin::StartAllThreads();

	// Start self-test engine
	SelfTestEngine::GetInstance().Start();

	// Wait for self-test engine to complete 
	while (!selfTestEngineCompleted)
		Sleep(10);

	// Exit the worker threads
	userInterfaceThread.ExitThread();
	SelfTestEngine::GetInstance().GetThread().ExitThread();

	return 0;
}

