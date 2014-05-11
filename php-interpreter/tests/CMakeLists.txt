cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0020 OLD)

add_definitions(-D_VARIADIC_MAX=10)

project(PHP_tests)
set(EXECUTABLE_NAME PHP_tests)

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
    include
    ../../externals/gtest-1.7.0/include
	../lib/include
)

message(STATUS ${CMAKE_PREFIX_PATH})
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)


link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../externals/gtest-1.7.0/bin/${EX_PLATFORM_NAME}/${CMAKE_BUILD_TYPE}
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/${Configuration}
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
	PHP_lib
    gtest
)

qt5_use_modules(${EXECUTABLE_NAME} Widgets)

install(TARGETS ${EXECUTABLE_NAME}
    RUNTIME
    DESTINATION bin
)

install(FILES data/source.php
    DESTINATION bin/Data
)
