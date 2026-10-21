cmake_minimum_required(VERSION 3.21)
project(qt_desk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.9 REQUIRED COMPONENTS Core Gui Qml Quick)

add_executable(qt-desk main.cpp DeskBridge.cpp DeskBridge.h
    DeskAgentControl.cpp DeskAgentControl.h desk_text_audit.cpp desk_text_audit.h)
target_link_libraries(qt-desk PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick)

include(CTest)
if(BUILD_TESTING)
    add_executable(qt-desk-bridge-test tests/DeskBridgeTest.cpp DeskBridge.cpp DeskBridge.h)
    target_link_libraries(qt-desk-bridge-test PRIVATE Qt6::Core)
    add_test(NAME qt-desk-bridge COMMAND qt-desk-bridge-test)
endif()
