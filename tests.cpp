#include "yasvl.hpp"
#include <iostream>
int main() {
	yasvl::version ver{ 1, 0 };
	std::cout << ver << '\n';
	std::cout << yasvl::compiler_version << '\n';
	std::cout << std::boolalpha << (ver < yasvl::compiler_version);
	std::cout << "Got version:" << yasvl::version::from_string("v1.0.0-alpha");
	ver.major;
	ver.minor;
	ver.patch;
	ver.pre_release_type;
	ver.pre_release_number;
	ver.build;
	ver.to_string();
	//ver.format_to(OutputIterator);
}