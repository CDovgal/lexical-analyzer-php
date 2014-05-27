cmake_minimum_required(VERSION 2.8.9)
cmake_policy(SET CMP0020 OLD)

project(php-interpreter)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( "64 bits compiler detected" )
    SET( EX_PLATFORM 64 )
    SET( EX_PLATFORM_NAME "x64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 ) 
    MESSAGE( "32 bits compiler detected" )
    SET( EX_PLATFORM 32 )
    SET( EX_PLATFORM_NAME "x86" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

add_subdirectory(lib)
add_subdirectory(client)
add_subdirectory(tests)




#install(DIRECTORY
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../../externals/qt5runtime/x86/${CMAKE_BUILD_TYPE}/imageformats
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../../externals/qt5runtime/x86/${CMAKE_BUILD_TYPE}/platforms
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../../externals/qt5runtime/x86/${CMAKE_BUILD_TYPE}/printsupport
#    DESTINATION bin
#    COMPONENT Applications
#)

#file(
#	GLOB 
#	qt5_runtime_root
#	${CMAKE_CURRENT_SOURCE_DIR}/../../../externals/qt5runtime/x86/${CMAKE_BUILD_TYPE}/*.dll
#)

#install(FILES ${qt5_runtime_root}
#    DESTINATION bin
#    COMPONENT Applications
#)
