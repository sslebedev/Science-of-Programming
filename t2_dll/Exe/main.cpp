#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <windows.h>

typedef void (*FooType)();

int main()
{
    // Attaching to library
    std::cout << "Loading dll 'libtest_d.dll'" << std::endl;

    HINSTANCE hDll = LoadLibrary("libtest_d.dll");
    if (hDll == NULL) {
        std::cout << "Bad dll 'libtest_d.dll'." << std::endl;
        return 0;
    }

    // Loading functions list
    std::cout << "Opening file 'in.txt'" << std::endl;

    std::ifstream in("in.txt");

    std::vector<std::string> functions(
        (std::istream_iterator<std::string>(in)),
        std::istream_iterator<std::string>()
    );

    // Calling
    std::cout << "Calling functions:" << std::endl;
    for (int i = 0; i < functions.size(); ++i) {
        FooType f = (FooType)GetProcAddress(hDll, functions[i].c_str());

        if (f) {
            std::cout << "Calling function " + functions[i] + ":";
            f();
            std::cout << std::endl;
        } else {
            std::cout << "Function " + functions[i] + " not found." << std::endl;
        }
    }

    FreeLibrary(hDll);
    return 0;
}