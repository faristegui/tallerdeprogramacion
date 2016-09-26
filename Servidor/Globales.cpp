#include "Globales.h"

std::string IntAString(int UnInt) {
	std::stringstream ss;
	ss << UnInt;
	std::string str = ss.str();

	return str;
}

std::string ToLowerCase(std::string UnString) {
	transform(UnString.begin(), UnString.end(), UnString.begin(), (int(*)(int))tolower);

	return UnString;
}
