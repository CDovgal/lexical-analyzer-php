cmake_minimum_required(VERSION 2.8.9)
cmake_policy(SET CMP0020 OLD)

project(PHP_client)
set(EXECUTABLE_NAME PHP_client)

message(STATUS ${CMAKE_PREFIX_PATH})

set(QT_USE_QTMAIN TRUE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

include_directories(
    src
	../lib/include
)

# sources
file(
	GLOB
	SEL_client_sources
	src/*
)

# forms
file(
	GLOB
	SEL_client_forms 
	forms/*.ui
)

QT5_WRAP_UI(SEL_client_uis ${SEL_client_forms})
source_group("Forms" FILES ${SEL_client_forms})

add_executable(
    ${EXECUTABLE_NAME}
    WIN32
    ${SEL_client_sources}
	${SEL_client_forms}
)

qt5_use_modules(${EXECUTABLE_NAME} Widgets)

add_dependencies(
    ${EXECUTABLE_NAME}
    PHP_lib
)

target_link_libraries(
    ${EXECUTABLE_NAME}
    PHP_lib
)

install(TARGETS ${EXECUTABLE_NAME}
    RUNTIME
    DESTINATION bin
)

#install(DIRECTORY
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../../externals/qt5runtime/x86/${CMAKE_BUILD_TYPE}/imageformats
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../../externals/qt5runtime/x86/${CMAKE_BUILD_TYPE}/platforms
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../../externals/qt5runtime/x86/${CMAKE_BUILD_TYPE}/printsupport
#    DESTINATION bin
#    COMPONENT Applications
#)
