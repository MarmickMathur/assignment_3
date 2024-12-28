#pragma once
#include <string>

class action {
public:
	std::string type;
	std::string name;
	action(std::string name, std::string type);
};