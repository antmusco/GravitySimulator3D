/**
 * \file runnable.cpp
 * Implements functions declared in runnable.h. 
 */
 #include "runnable.h"

Runnable::Runnable() :
	isInitialized(false),
	isRunning(false),
	terminateSelf(false) {

	// Empty.

}

ErrorCode Runnable::spawnThread(std::vector<std::thread>& threads, void* args) {

	// Assume failure first.
	ErrorCode result = ErrorCode::Failure;

	// If the Runnable is already running, do nothing.
	if(isRunning) {

		result = ErrorCode::RunnableAlreadyRunning;

		std::cerr << "Error: Runnable can only be running on a single thread."
				 << std::endl;

	} else {

		// Allow the implementing class to initialize.
		result = initialize();

		// If the Runnable could not be initialized, do nothing.
		if (result != ErrorCode::Success) {

			std::cerr << "Thread could not be initialized properly." 
					 << std::endl;

		} else {

			// Use the implementing class's run function for the primary loop.
			threads.push_back(std::thread([&]() {

				run(args);

			}));

		}
	}

	// Return success or cause of failure.
	return result;

}