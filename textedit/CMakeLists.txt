cmake_minimum_required(VERSION 3.0.0)
project(textedit)

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

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(TE_SOURCES main.cpp textedit.cpp)
set(TE_MOCH textedit.h)
set(TE_RESOURCES textedit.qrc)

qt4_wrap_cpp(TE_HEADERS_MOC ${TE_MOCH})
qt4_add_resources(TE_RESOURCES_RCC ${TE_RESOURCES})

#---------------------------------------------------------

set(ALL_FILES ${TE_SOURCES}
              ${TE_HEADERS_MOC}
              ${TE_RESOURCES_RCC}
              )

add_executable(textedit ${ALL_FILES})
target_link_libraries(textedit ${QT_LIBRARIES})
