cmake_minimum_required(VERSION 2.8.9)
cmake_policy(SET CMP0020 OLD)

project(PHP_lib)
set(PHP_LIB_NAME PHP_lib)

message(STATUS ${CMAKE_PREFIX_PATH})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

include_directories(
    include
)

file(
	GLOB 
	PHP_Lib_sources
	src/*.c*
)

file(
	GLOB 
	PHP_Lib_headers
	include/*.h*
)

add_library(
    ${PHP_LIB_NAME}
	SHARED
    ${PHP_Lib_sources}
	${PHP_Lib_headers}
)

qt5_use_modules(${PHP_LIB_NAME} Widgets)


if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

  else()

  install(TARGETS ${PHP_LIB_NAME}
      RUNTIME
      DESTINATION bin/${CMAKE_BUILD_TYPE}
  )

  file(
  	GLOB 
  	qt5_runtime_root
  	${CMAKE_CURRENT_SOURCE_DIR}/../../externals/qt5runtime/${EX_PLATFORM_NAME}/${CMAKE_BUILD_TYPE}/*.dll
  )

  install(FILES ${qt5_runtime_root}
      DESTINATION bin/${CMAKE_BUILD_TYPE}
  )
endif()
