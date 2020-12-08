cmake_minimum_required(VERSION 3.19)
project(Collision VERSION 0.0.1)

find_package(OpenCL REQUIRED)



set(SOURCES 
        host.c
        host2d.c 
        host3d.c 
        output_.c 
        preferences.c 
        )
set(HEADERS
        Types.h 
        interdim.h 
        )

add_executable(generate ${SOURCES} ${HEADERS})

set_target_properties(generate PROPERTIES
C_STANDART 18
C_STANDART_REQUIERED ON
LINKER_LANGUAGE C 
)
target_include_directories(
        generate PRIVATE
        ${OpenCL_INCLUDE_DIRS}
)
target_link_libraries(generate OpenCL::OpenCL)

set(CPP_SOURCES
        program_loader.cpp 
        array_gen.cpp
        )
set(CPP_HEADERS
        Types.h
        )

add_library(cpp_tools STATIC ${CPP_SOURCES} ${CPP_HEADERS})
set_target_properties(cpp_tools PROPERTIES
LINKER_LANGUAGE CXX
CXX_STANDART 20
CXX_STANDART_REQUIRED ON)

target_link_libraries(generate cpp_tools)