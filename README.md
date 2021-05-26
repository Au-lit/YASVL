# Yet another semantic verionning library (YASVL)

Yasvl is a C++20 library to handle versions:
```cpp
#include "yasvl.hpp"

int main() {
	yasvl::version aVersion{ 1, 0 };
	std::cout << "The compiler version(" << yasvl::compiler_version << ") is greater than " << aVersion << ": " 
	          << std::boolalpha << (yasvl::compiler_version > aVersion) << '\n';
}
```
Possible output:
```
The compiler version(v11.0.0) is over v1.0.0: true
```
Here is a resume of the api
```cpp
yasvl::version ver{ 1, 0 }; // agregate (in the order depicted below)
ver.major; // the major version
ver.minor; // the minor version
ver.patch; // the patch version
ver.pre_release_type; // the pre-release type 
                      // (decltype(pre_release_type) == enum class pre_release { alpha, beta, rc, none };)
ver.pre_release_number; // the pre-release number (for example beta.1, one being the number). 
                        // decltype(pre_release_number) == std::optional</* unsigned integer type*/>
ver.build; // the build number
           // decltype(build) == std::optional</* unsigned integer type of at least 10 bits*/>

ver.to_string(); // returns a string represntation of the version
ver.format_to(OutputIterator it); // writes to it a string representation of the version
```
In the near future `yasvl::version::from_string` will be working
