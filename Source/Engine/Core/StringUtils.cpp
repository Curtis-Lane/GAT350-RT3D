#include "StringUtils.h"

namespace nc {
	// I have to redefine the string counter here because c++ doesn't like initializing non-const static member variables
	unsigned int StringUtils::stringCounter = 0;

	std::string StringUtils::ToUpper(std::string input) {
		// Create a variable to store the output
		// I tried to use a char[], but c++ doesn't allow creating an array with a non-const variable, and I didn't want to use a vector, so I'm just using a string
		// Technicaly not great because of string immutability, but I don't care that much
		std::string output = "";

		// Loop over every char in input string and uppercase them, then add them to the output string
		for(char chara : input) {
			output += std::toupper(chara);
		}

		// Return the completed output string
		return output;
	}

	std::string StringUtils::ToLower(std::string input) {
		// Create a variable to store the output
		// I tried to use a char[], but c++ doesn't allow creating an array with a non-const variable, and I didn't want to use a vector, so I'm just using a string
		// Technicaly not great because of string immutability, but I don't care that much
		std::string output = "";

		// Loop over every char in input string and lowercase them, then add them to the output string
		for(char chara : input) {
			output += std::tolower(chara);
		}

		// Return the completed output string
		return output;
	}

	bool StringUtils::IsEqualIgnoreCase(std::string stringOne, std::string stringTwo) {
		// Lower case both strings to make case irrelevant, then compare
		if(ToLower(stringOne) == ToLower(stringTwo)) {
			return true;
		}

		return false;
	}

	std::string StringUtils::CreateUnique(std::string input) {
		// Increment counter to insure unique string on every call
		stringCounter += 1;

		// Return the input, combined with the counter to generate an always unique string
		return input + std::to_string(stringCounter);
	}
}