cmake_minimum_required(VERSION 3.2)
project(CppLondonUni_Pomodoro)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if (WIN32)
	message(NOTE "When building on Windows, You are expected to have QT installed to \\QT. Otherwise, you must set CMAKE_PREFIX_PATH manually")
	message(NOTE "Qt 5.10.1 is the default expected, please override this via QT_VERSION if you have a newer/older version of Qt5")
	set(QT_VERSION "5.10.1" CACHE PATH "Version of Qt5 you have installed")
	if (MSVC)
		message(NOTE "We currently only support MSVC2017. You will need to modify the CMakeLists.txt if you wish to use an older version.")
		set(CMAKE_PREFIX_PATH "C:\\Qt\\${QT_VERSION}\\msvc2017_64\\")
	else()
		set(CMAKE_PREFIX_PATH /Qt/${QT_VERSION}/mingw53_32/)
	endif()
	message(STATUS "Got Prefix path: ${CMAKE_PREFIX_PATH}")
endif()

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

add_executable(pomodoro
    src/main.cpp
    src/pomodoro.cpp
    src/timer.cpp)
target_link_libraries(pomodoro Qt5::Widgets Qt5::Multimedia)
set_target_properties(pomodoro PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES)

FILE(COPY Annoying_Alarm_Clock.wav DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY bensound-acousticbreeze.wav DESTINATION ${CMAKE_BINARY_DIR})

