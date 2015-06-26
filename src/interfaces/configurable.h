/**
 * \file configurable.h
 * Defines the Configurable Interface. 
 */
#ifndef _CONFIGURABLE_H_
#define _CONFIGURABLE_H_

//==============================================================================
// Header Files 
//==============================================================================

#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>

#include "rapidxml/rapidxml.hpp"
 
#include "enums.h"

//==============================================================================
// Constants.
//==============================================================================

#define KEY            "key"
#define VALUE          "value"
#define VAR_LIST       "variables"
#define VAR_ELEMENT    "variable"
#define SCRIPT_LIST    "scripts"
#define SCRIPT_ELEMENT "script"
#define SCRIPT_EARLY   "early"
#define SCRIPT_LATE    "late"
#define CONFIG_ROOT    "config/"


/**
 * @brief Interface which allows an implementing classes to be configurable.
 * 
 * @detail The configurable interface allows for configurabilty via an XML 
 * config file. The format of the .config file is described below:
 *
 * @code{.xml}
 * <!-- Format of .config file -->
 * <moduleName> 
 *     <scripts>
 *         <script>
 *				<key>"early"</key>
 *				<value>checkHardware.sh</value>
 *			</script>
 *         <script>
 *				<key>"late"</key>
 *				<value>connectToHardware1.sh</value>
 *			</script>
 *         <script>
 *				<key>"late"</key>
 *				<value>connectToHardware2.sh</value>
 *			</script>
 *         ...
 *     </scripts>
 *     <variables>
 *         <variable>
 *				<key>"variableName1"</key>
 *				<value>variableValue1</value>
 *			</variable>
  *         <variable>
 *				<key>"variableName2"</key>
 *				<value>variableValue2</value>
 *			</variable>
 *         ...
 *     </variables>
 * </moduleName>
 * @endcode
 *
 * It is important to know how configuration is carried out. When an object 
 * implementing Configurable is constructed, the first thing that is done is
 * to perform "early" configuration. Early configurations basically executes
 * scripts before the module is constructed, to ensure that the components are
 * working well and that the enviornment is good. After the module has been
 * constructed, "late" configuration is executed. Late configuration is 
 * performed just before the module is run on a new thread of execution. It is
 * up to the designer to choose what is done when, by designating the scripts
 * with "early" or "late" tags as appropriate.
 * 
 * The constructor parses the file into a DOM tree, and immediately executes 
 * scripts that are given the "early" tag. Scripts which are given 
 * the "late" tag are placed in the \c scripts vector and saved for later use.
 *
 * The variables are then placed in the \c variablesValues vector, with the
 * corresponding name keys placed in the \c variablesNames vector at the same
 * index. To get the value of a specified variable, find the index of the
 * variable name is \c variablesNames and access \c variablesValues at that 
 * index.
 */
class Configurable {

private:

	// Extract the content from an 
	ErrorCode extractContent(rapidxml::xml_document<>& doc);

	// Configure
	ErrorCode parseConfigFile();
	
protected:

	// Protected constructor.
	Configurable(const char* _configFile);
	
	/**
	 * Configuration file which includes the settings and parameters for
	 * configuration.
	 */
	const char* configFile;
	
	/**
	 * Boolean flag indicating whether or not the object has been configured
	 * or not.
	 */
	bool isConfigured;

	/**
	 * Collection which maps member variable names (strings) to their 
	 * corresponding initial values (also strings). Note that the values must
	 * be cast to the appropriate data value before assigning them to their
	 * corresponding variable. Complex member values (objects and structs) 
	 * must be initialized in parts, such as by parsing the individual vales
	 *  like so:
	 * 
	 * @code{.cpp}
	 * 		int int_1 = atoi(initialValuesMap["obj.int_1"]);
	 * 		int int_2 = atoi(initialValuesMap["obj.int_2"]);
	 * 		string str_1 = atoi(initialValuesMap["obj.str_1"]);
	 * 		Object obj.initialize(int_1, int_2, str_1);
	 * @endcode
	 */
	std::map<std::string, std::string> initialValuesMap;

	/**
	 * Vector of strings indicating the paths of scripts which can be 
	 * executed during initialization.
	 */
	std::vector<std::string> lateScripts;

	/**
	 * Pure virtual function which must be overridden. Allows the functionality
	 * to configure a module right before the module is run on an independent
	 * thread of execution.
	 *
	 * @returns ErrorCode indicating success or the cause of failure.
	 */
	virtual ErrorCode configureVariables() = 0;

};

#endif