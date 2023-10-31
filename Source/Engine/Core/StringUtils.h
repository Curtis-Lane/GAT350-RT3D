#pragma once

#include <string>

namespace nc {
	class StringUtils {
		public:
			static std::string ToUpper(std::string input);
			static std::string ToLower(std::string input);
			static bool IsEqualIgnoreCase(std::string stringOne, std::string stringTwo);
			static std::string CreateUnique(std::string input);

		private:
			static unsigned int stringCounter;
	};
}