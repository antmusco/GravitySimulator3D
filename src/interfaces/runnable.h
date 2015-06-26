/**
 * \file runnable.h
 * Defines the Runnable Interface. 
 */
#ifndef _RUNNABLE_H_
#define _RUNNABLE_H_


#include <iostream>
#include <vector>
#include <thread>

#include "basetypes.h"
#include "enums.h"


/**
 * \brief Interface which allows an implementing class to be run on a thread.
 * 
 * The Runnable Interface allows implementing classes to be run and managed on
 * and independent thread of execution. The design of the interface ensures
 * that a particular instance can only be running once, and also that the
 * thread can be run, reset, and terminated quickly and efficiently.
 *
 * To create a thread for a Runnable object, call the 'spawnThread()' method.
 * This will create and run the thread with the specified arguments (args),
 * and place the thread in the vector passed to the function for future
 * reference.
 */
class Runnable {

protected:

	/**
	 * Constructor. Initializes all flags to false.
	 */
	Runnable();

	/**
	 * Boolean flag inticating whether or not the object has been initialized.
	 */
	bool isInitialized;

	/**
	 * Boolean flag indicating whether or not the object is currently running
	 * on its thread.
	 */
	bool isRunning;

	/**
	 * Boolean flag indicating whether or not the object is to terminate its
	 * thread of execution.
	 */
	bool terminateSelf;

	/**
	 * Pure virtual function which must be overridden. A 'hook' function which
	 * allows the implementing class to customize how it is initialized to be
	 * run. This will be called and completed before the thread is initiated,
	 * so any task needed to be done before the primary run loop begins should
	 * be executed here. 
	 *
	 * \returns ErrorCode indicating success or the cause of failure.
	 */
	virtual ErrorCode initialize() = 0;

	/**
	 * Pure virtual function which must be overridden. A 'hook' function which
	 * allows the implementing class to implement the primary loop for the 
	 * thread. This function will loop until the thread is told to terminate.
	 *
	 * \params args Pointer representing any arguments needed for the thread
	 * to run. Must be defined by the implementing class.
	 *
	 * \returns ErrorCode indicating success or the cause of failure.
	 */	
	virtual void* run(void* args) = 0;

public:

	/**
	 * Pure virtual function which must be overridden. Function which allows
	 * a currently running object to be reset to its initial state. May be 
	 * called by any outside object that has a reference to this object.
	 *
	 * \returns ErrorCode indicating success or the cause of failure.
	 */
	virtual ErrorCode reset() = 0;

	/**
	 * Pure virtual function which must be overridden. Function which allows
	 * a currently running object to be terminated gracefully. May be 
	 * called by any outside object that has a reference to this object.
	 *
	 * \returns ErrorCode indicating success or the cause of failure.
	 */
	virtual ErrorCode terminate() = 0;

	/**
	 * Function which abstracts the functionality of running a Runnable on an 
	 * independent thread of execution. A Runnable object can only be running
	 * once, so this method does nothing if the \c isRunning flag is set to
	 * true. 
	 *
	 * After calling this method, the module is initialized and the run() 
	 * function is called, which contains the primary loop for execution.
	 *
	 * \param threads Reference to a vector of threads to which the new thread
	 * 		will be added.
	 * \param args The arguments to be passed to the new thread.
	 * \returns ErrorCode representing success or the cause for failure.
	 */
	ErrorCode spawnThread(std::vector<std::thread>& threads, void* args);

};

#endif