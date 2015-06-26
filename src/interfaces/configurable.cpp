/**
 * \file configurable.cpp
 * Implements functions decleared in configurable.h 
 */

//==============================================================================
// Header Files 
//==============================================================================
 
#include "configurable.h"

/**
 * Protected constructor for the Configurable abstract class. Parses the
 * config file indicated by \c _xmlFile and sets the initializer list and
 * the script list.
 *
 * @param _configFile The configuration file to be parsed.
 */
Configurable::Configurable(const char* _configFile) :
	configFile(_configFile), 
	isConfigured(false) {

	parseConfigFile();

}

/**
 * Extracts the necessary content out of the XML document and adds the
 * variables and scripts to the appropriate vectors.
 * 
 * @param doc Reference to an open XML document for extraction.
 * @returns ErrorCode indicating success or the cause of failure.
 */
ErrorCode Configurable::extractContent(rapidxml::xml_document<>& doc) {

	// Vector of scripts to be executed now.
	std::vector<std::string> earlyScripts;

	rapidxml::xml_node<>* key = NULL;
	rapidxml::xml_node<>* value = NULL;
	rapidxml::xml_node<>* root = doc.first_node();

	// Extract the variables.
	rapidxml::xml_node<>* variables = root->first_node(VAR_LIST);
	if (variables != NULL) {

		rapidxml::xml_node<>* variableNode = variables->first_node(VAR_ELEMENT);
		while(variableNode != NULL) {

			// Get the name of the variable.
			key = variableNode->first_node(KEY);
			value = variableNode->first_node(VALUE);

			// Add the name and value of the variable with matching indices.
			initialValuesMap[std::string(key->value())] = std::string(value->value());

			// Get the next variable.
			variableNode = variableNode->next_sibling(VAR_ELEMENT);

		}
		
	}

	// Extract the scripts.
	rapidxml::xml_node<>* scripts = root->first_node(SCRIPT_LIST);
	if (scripts != NULL) {

		rapidxml::xml_node<>* scriptNode = scripts->first_node(SCRIPT_ELEMENT);
		while(scriptNode != NULL) {

			// Determine whether the script is an early or late script.
			key = scriptNode->first_node(KEY);
			value = scriptNode->first_node(VALUE);
			if(!strcmp(key->value(), SCRIPT_EARLY)) {

				// If early, add to vector of early scripts to be executed now.
				earlyScripts.push_back(SCRIPTS_DIR + std::string(value->value()));

			} else {

				// Otherwise, add to vector of late scripts to be executed later.
				lateScripts.push_back(SCRIPTS_DIR + std::string(value->value()));

			}

			// Get the next script.
			scriptNode = scriptNode->next_sibling(SCRIPT_ELEMENT);

		}

	}

	// Execute early scripts.
	for(unsigned i = 0; i < earlyScripts.size(); i++) {

		std::cout << "Executing script '" << earlyScripts[i] << "'"
				  << std::endl;
		system(earlyScripts[i].c_str());

	}

	// Successful configuration.
	return ErrorCode::Success;

}

/**
 * Parses the indicated config file and runs any of the script given the 
 * "early" tag in the file. This method is run before the module is 
 * completely constructed.
 *
 * @returns ErrorCode indicating success or the cause of failure.
 */
ErrorCode Configurable::parseConfigFile() {

	// Assume failure first.
	ErrorCode result = ErrorCode::Failure;

	std::string path(CONFIG_ROOT);
	path.append(configFile);

	// Make sure file exists and open file.
	std::ifstream configStream(path);
	if(configStream) {

		// Sends text of the file to a string.
		std::string configString((std::istreambuf_iterator<char>(configStream)),
			(std::istreambuf_iterator<char>()));

		// Close the configStream.
		configStream.close();

		// Cread XML document and parse the string.
		rapidxml::xml_document<> doc;		
		doc.parse<0>(&configString[0]);

		// Extract the content from the XML document.
		result = extractContent(doc);

	} else {

		// Indicate that the file was not found.
		result = ErrorCode::ConfigFileNotFound;

	}

	// Return success or cause of failure.
	return result;
	
}
