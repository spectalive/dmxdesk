cmake_minimum_required(VERSION 3.21)
project(qt_desk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.9 REQUIRED COMPONENTS Core Gui Qml Quick)

add_executable(qt-desk main.cpp DeskBridge.cpp DeskBridge.h
    DeskAgentControl.cpp DeskAgentControl.h desk_text_audit.cpp desk_text_audit.h
    desk_alignment_audit.cpp desk_alignment_audit.h
    desk_layout_audit.cpp desk_layout_audit.h
    desk_effects_layout_audit.cpp desk_effects_layout_audit.h
    desk_view_audit.cpp desk_view_audit.h)
target_link_libraries(qt-desk PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick)

include(CTest)
if(BUILD_TESTING)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    add_executable(qt-desk-bridge-test tests/DeskBridgeTest.cpp DeskBridge.cpp DeskBridge.h)
    target_link_libraries(qt-desk-bridge-test PRIVATE Qt6::Core)
    add_test(NAME qt-desk-bridge COMMAND qt-desk-bridge-test)
    add_executable(qt-desk-alignment-test tests/DeskAlignmentAuditTest.cpp
        desk_alignment_audit.cpp desk_alignment_audit.h)
    target_link_libraries(qt-desk-alignment-test PRIVATE Qt6::Gui Qt6::Quick)
    add_test(NAME qt-desk-alignment COMMAND qt-desk-alignment-test)
    set_tests_properties(qt-desk-alignment PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
    add_executable(qt-desk-layout-test tests/DeskLayoutAuditTest.cpp
        desk_layout_audit.cpp desk_layout_audit.h
        desk_effects_layout_audit.cpp desk_effects_layout_audit.h)
    target_link_libraries(qt-desk-layout-test PRIVATE Qt6::Gui Qt6::Qml Qt6::Quick)
    target_compile_definitions(qt-desk-layout-test PRIVATE
        DESK_TEST_FIXTURE="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/direct-effects.json"
        DESK_TEST_QML="${CMAKE_CURRENT_SOURCE_DIR}/qml/Main.qml")
    add_test(NAME qt-desk-layout COMMAND qt-desk-layout-test)
    set_tests_properties(qt-desk-layout PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QT_QUICK_BACKEND=software")
    add_executable(qt-desk-status-icons-test tests/DeskStatusIconsTest.cpp tests/advance_scene.cpp)
    target_link_libraries(qt-desk-status-icons-test PRIVATE Qt6::Gui Qt6::Qml Qt6::Quick)
    target_compile_definitions(qt-desk-status-icons-test PRIVATE
        DESK_TEST_FIXTURE="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/direct-effects.json"
        DESK_TEST_QML="${CMAKE_CURRENT_SOURCE_DIR}/qml/Main.qml")
    add_test(NAME qt-desk-status-icons COMMAND qt-desk-status-icons-test)
    set_tests_properties(qt-desk-status-icons PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QT_QUICK_BACKEND=software;QT_SCALE_FACTOR=1")
    add_executable(qt-desk-motion-test tests/DeskMotionTest.cpp tests/advance_scene.cpp
        desk_text_audit.cpp desk_alignment_audit.cpp desk_layout_audit.cpp
        desk_effects_layout_audit.cpp desk_view_audit.cpp)
    target_link_libraries(qt-desk-motion-test PRIVATE Qt6::Gui Qt6::Qml Qt6::Quick)
    target_compile_definitions(qt-desk-motion-test PRIVATE
        DESK_TEST_FIXTURE="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/direct-effects.json"
        DESK_TEST_QML="${CMAKE_CURRENT_SOURCE_DIR}/qml/Main.qml")
    add_test(NAME qt-desk-motion COMMAND qt-desk-motion-test)
    set_tests_properties(qt-desk-motion PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QT_QUICK_BACKEND=software;QT_SCALE_FACTOR=1")
    add_test(NAME qt-desk-capture-gate COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/../tests/qt_capture_gate_test.py)
    set_tests_properties(qt-desk-capture-gate PROPERTIES
        ENVIRONMENT "DMXDESK_QT_EXECUTABLE=$<TARGET_FILE:qt-desk>")
    add_test(NAME qt-desk-preview-layout COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/../tests/qt_layout_preview_bridge_test.py)
endif()
