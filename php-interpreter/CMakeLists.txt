cmake_minimum_required(VERSION 2.8.9)
cmake_policy(SET CMP0020 OLD)

project(php-interpreter)


add_subdirectory(lexical-analyzer)
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
