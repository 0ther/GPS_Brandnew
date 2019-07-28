cmake_minimum_required(VERSION 2.8)

project(GPS_Client)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Setup include directories
include_directories(.)
include_directories(./rapidjson)
include_directories(./rapidjson/error)
include_directories(./rapidjson/internal)
include_directories(./rapidjson/msinttypes)


# Escape warnings
if (MSVC)
	add_definitions(-Dsnprintf=_snprintf)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()


# Setup global compiler/linker settings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -frtti -fvisibility=hidden")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fvisibility=hidden")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-declarations -Wno-unknown-pragmas")
	
	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-undefined")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--no-undefined")
	else()
		# TODO: prevent clang from linking incomplete shared objects
	endif()	
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(configurations
		CMAKE_C_FLAGS_DEBUG
		CMAKE_C_FLAGS_MINSIZEREL
		CMAKE_C_FLAGS_RELEASE
		CMAKE_C_FLAGS_RELWITHDEBINFO
		CMAKE_CXX_FLAGS_DEBUG
		CMAKE_CXX_FLAGS_MINSIZEREL
		CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_RELWITHDEBINFO
	)
	foreach (configuration ${configurations})
		string(REGEX REPLACE "/MDd" "/MTd" ${configuration} "${${configuration}}")
		string(REGEX REPLACE "/MD" "/MT" ${configuration} "${${configuration}}")
	endforeach()
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /OPT:REF")
endif()


# Add libraries and some other stuff
set(SOURCE_EXE MVC_GPS_Client.cpp)		# Установка переменной со списком исходников для исполняемого файла

set(SOURCE_LIB RMC_Parser.cpp)			# Тоже самое, но для библиотеки

add_library(foo STATIC ${SOURCE_LIB})	# Создание статической библиотеки с именем foo

add_executable(client ${SOURCE_EXE})	# Создает исполняемый файл с именем main

target_link_libraries(client foo)		# Линковка программы с библиотекой





