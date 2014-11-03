cmake_minimum_required(VERSION 2.8.4)
project(t2_dll)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
    set(BUILD_SHARED_LIBS OFF)
elseif (MSVC)
    #add_definitions(
    #    /DWIN32 /D_DEBUG /D_CONSOLE /D_MBCS
    #    /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_DEPRECATE
    #)
    #set(CMAKE_CXX_FLAGS "/EHsc /W4 /nologo /Od /Ob0" CACHE STRING "" FORCE)
endif ()

#---------------------------------------------------------

add_subdirectory(Dll)
add_subdirectory(Exe)