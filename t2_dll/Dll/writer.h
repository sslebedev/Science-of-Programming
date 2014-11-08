#ifndef _WRITER_H_
#define _WRITER_H_


// Alternative usage example, without .def file
#if 0
    #ifdef T2_DLL_EXPORTS
    #define T2_API __declspec(dllexport)
    #else
    #define T2_API __declspec(dllimport)
    #endif

    #define T2_CALL __cdecl

    T2_API void T2_CALL FooA();
#endif



typedef void (*FooType)();
typedef bool (*SelectorType)(const void *obj, int idx);
typedef void (*FooSelectType)(
    void *aSrc[], int lengthSrc,
    void *aDst[], int lengthDst,
    int *lengthOutput,
    SelectorType selector);

#ifdef __cplusplus
extern "C"
{
#endif

void FooA();

void FooB();

void FooSelect( void *aSrc[], int lengthSrc, void *aDst[], int lengthDst, int *lengthOutput, SelectorType selector );

#ifdef __cplusplus
}
#endif

#endif // _WRITER_H_