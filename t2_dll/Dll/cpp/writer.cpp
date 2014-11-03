#include <iostream>

#include "../writer.h"

T2_API void T2_CALL FooA()
{
    std::cout << "Called 'FooA()'";
}

void FooB()
{
    std::cout << "Called 'FooB()'";
}
