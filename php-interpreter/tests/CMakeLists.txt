cmake_minimum_required(VERSION 2.8)

add_definitions(-D_VARIADIC_MAX=10)

project(php_interpreter_tests)
set(EXECUTABLE_NAME      php_interpreter_tests)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( "64 bits compiler detected" )
    SET( EX_PLATFORM 64 )
    SET( EX_PLATFORM_NAME "x64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 ) 
    MESSAGE( "32 bits compiler detected" )
    SET( EX_PLATFORM 32 )
    SET( EX_PLATFORM_NAME "x86" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

include_directories(
#    ${CMAKE_CURRENT_BINARY_DIR}
    include
    ../../externals/gtest-1.7.0/include
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../externals/gtest-1.7.0/bin/${EX_PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
)

file(
    GLOB
    source_files
    src/*.c*
)

file(
    GLOB
    include_files
    include/*.h*
)

# Assign sources to binary

add_executable(
    ${EXECUTABLE_NAME}
    ${source_files} ${include_files}
)

target_link_libraries(
    ${EXECUTABLE_NAME}
    gtest
)

# Install

#install(TARGETS php_interpreter_tests
#    RUNTIME
#    DESTINATION bin
#    COMPONENT Autotests)

#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/test_0.txt
#    DESTINATION bin/Data
#    COMPONENT Autotests
#)
