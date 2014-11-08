#include <iostream>
#include <stdexcept>
#include <assert.h>

#include "../writer.h"

void FooA()
{
    std::cout << "Called 'FooA()'";
}

void FooB()
{
    std::cout << "Called 'FooB()'";
}

void FooSelect( void *aSrc[], int lengthSrc, void *aDst[], int lengthDst, int *lengthOutput, SelectorType selector )
{
    if (lengthOutput == 0) {
       throw "lengthOutput == 0";
    }

    if (selector == 0) {
        throw "selector == 0";
    }

    int j = 0;

    for (int i = 0; i < lengthSrc; ++i) {
        if (!selector(aSrc[i], i)) {
            continue;
        }

        if (j + 1 == lengthDst) {
            throw "result is bigger than lengthDst";
        }

        aDst[j++] = aSrc[i];
    }

    *lengthOutput = j;
}
