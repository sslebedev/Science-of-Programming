set(D_FILES main.cpp writer.h cpp/writer.cpp)

add_definitions(-DT2_DLL_EXPORTS)

add_library(test_d SHARED ${D_FILES})