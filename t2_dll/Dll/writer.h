#ifndef _WRITER_H_
#define _WRITER_H_

#ifdef T2_DLL_EXPORTS
#define T2_API __declspec(dllexport)
#else
#define T2_API __declspec(dllimport)
#endif

#define T2_CALL __cdecl

#ifdef __cplusplus
extern "C"
{
#endif

T2_API void T2_CALL FooA();

void FooB();

#ifdef __cplusplus
}
#endif

#endif // _WRITER_H_