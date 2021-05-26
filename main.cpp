#include "version.hpp"
#include <iostream>
int main() {
	version::version ver{ 1, 0 };
	std::cout << ver << '\n';
	std::cout << version::compiler_version << '\n';
	std::cout << std::boolalpha << (ver < version::compiler_version);
}