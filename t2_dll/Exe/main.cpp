#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <windows.h>

#include "../Dll/writer.h"

bool SelectBar(const void *obj, int idx)
{
    int &bar = *((int*)(obj));

    return bar >= 0 && idx < 5;
}

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

    // Selector example
    std::cout << "Test FooSelect" << std::endl;
    FooSelectType f = (FooSelectType)GetProcAddress(hDll, "FooSelect");
    if (!f) {
        std::cout << "Function FooSelect not found." << std::endl;
        return 0;
    }

    int s[] = {0, -1, 1, -2, 2, -3, 3, -4, 4};
    int a;
    int *sp[] = {s + 0, s + 1, s + 2, s + 3, s + 4, s + 5, s + 6, s + 7, s + 8};
    int *dp[sizeof(sp)];
    int l;

    for (int i = 0; i < sizeof(sp) / sizeof(sp[0]); ++i) {
        std::cout << "\t Src[" << i << "]: "<< *sp[i] << std::endl;
    }

    std::cout << "Calling Select(): bar >= 0 && idx < 5" << std::endl;
    f((void **)sp, sizeof(sp) / sizeof(sp[0]), (void **)dp, sizeof(dp) / sizeof(dp[0]), &l, SelectBar);

    for (int i = 0; i < l; ++i) {
        std::cout << "\t Selected: " << *dp[i] << std::endl;
    }

    FreeLibrary(hDll);
    return 0;
}