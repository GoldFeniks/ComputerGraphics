#include "utils.hpp"
#include <fstream>

std::string My::ReadFile(std::string path) {
	std::ifstream stream(path.c_str());
	std::string out, line;
	while (std::getline(stream, line))
		out.append(line + "\n");
	return out;
}
