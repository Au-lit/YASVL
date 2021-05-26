# Yet another semantic verionning library (YASVL)

Yasvl is a C++20 library to handle versions:
```cpp
#include "version.hpp"

int main() {
	version::version aVersion{ 1, 0 };
	std::cout << "The compiler version(" << version::compiler_version << ") is over v1.0: " 
	          << std::boolalpha << (ver < version::compiler_version) << '\n';
}
```
