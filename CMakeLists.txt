cmake_minimum_required(VERSION 3.20)
project(InterfaceLab VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(INTERFACE_LAB_QT "Build the real Qt desktop and HTTP frontends" OFF)

# Stable executable locations for Make, Ninja, Visual Studio and Xcode.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
foreach(config Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER "${config}" upper_config)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${upper_config} "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${upper_config} "${CMAKE_BINARY_DIR}/bin")
endforeach()

function(teaching_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_library(studydesk_core src/core.cpp)
target_include_directories(studydesk_core PUBLIC include)
teaching_warnings(studydesk_core)

# MODULE is a runtime-loaded plugin; the executable does NOT link against it.
add_library(pomodoro_plugin MODULE plugins/pomodoro.cpp)
target_include_directories(pomodoro_plugin PRIVATE include)
set_target_properties(pomodoro_plugin PROPERTIES PREFIX "")
teaching_warnings(pomodoro_plugin)

add_executable(interface_lab
    src/main.cpp src/registry.cpp
    examples/01_plugins.cpp
    examples/02_new_features.cpp
    examples/03_loggers.cpp
    examples/04_frontends.cpp
    examples/05_strategy.cpp
    examples/06_repositories.cpp
    examples/07_notifications.cpp
    examples/08_payments.cpp
    examples/09_export_formats.cpp
    examples/10_sensors.cpp
    examples/11_clocks.cpp
    examples/12_small_interfaces.cpp
    examples/13_observer.cpp
    examples/14_decorator.cpp
    examples/15_adapter.cpp
    examples/16_factory.cpp
    examples/17_undo_commands.cpp
    examples/18_state.cpp
    examples/19_validation_pipeline.cpp
    examples/20_cache_proxy.cpp
    examples/21_test_doubles.cpp
)
target_link_libraries(interface_lab PRIVATE studydesk_core ${CMAKE_DL_LIBS})
target_compile_definitions(interface_lab PRIVATE STUDY_PLUGIN_FILENAME="$<TARGET_FILE_NAME:pomodoro_plugin>")
add_dependencies(interface_lab pomodoro_plugin)
teaching_warnings(interface_lab)

include(CTest)
if(BUILD_TESTING)
    add_executable(core_tests tests/core_tests.cpp)
    target_link_libraries(core_tests PRIVATE studydesk_core)
    teaching_warnings(core_tests)
    add_test(NAME core_contract COMMAND core_tests)
    foreach(id RANGE 1 21)
        add_test(NAME lesson_${id} COMMAND interface_lab --example ${id}
            --plugin $<TARGET_FILE:pomodoro_plugin>
            --output "${CMAKE_BINARY_DIR}/test-output/lesson-${id}")
    endforeach()
    add_test(NAME invalid_example COMMAND interface_lab --example 22)
    set_tests_properties(invalid_example PROPERTIES WILL_FAIL TRUE)
    add_test(NAME missing_plugin COMMAND interface_lab --example 1
        --plugin "${CMAKE_BINARY_DIR}/does-not-exist.plugin")
    set_tests_properties(missing_plugin PROPERTIES WILL_FAIL TRUE)
endif()

if(INTERFACE_LAB_QT)
    find_package(Qt6 6.4 REQUIRED COMPONENTS Widgets Network HttpServer)
    add_executable(studydesk_gui frontends/gui.cpp)
    target_link_libraries(studydesk_gui PRIVATE studydesk_core Qt6::Widgets)
    teaching_warnings(studydesk_gui)
    add_executable(studydesk_web frontends/web.cpp)
    target_link_libraries(studydesk_web PRIVATE studydesk_core Qt6::Network Qt6::HttpServer)
    teaching_warnings(studydesk_web)
    if(BUILD_TESTING)
        add_test(NAME gui_smoke COMMAND studydesk_gui --smoke-test)
        set_tests_properties(gui_smoke PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 20)
        find_package(Python3 COMPONENTS Interpreter)
        if(Python3_Interpreter_FOUND)
            add_test(NAME web_smoke COMMAND "${Python3_EXECUTABLE}"
                "${CMAKE_SOURCE_DIR}/tests/web_smoke.py" $<TARGET_FILE:studydesk_web>)
            set_tests_properties(web_smoke PROPERTIES TIMEOUT 30)
        else()
            message(STATUS "Python 3 not found: skipping HTTP integration test only")
        endif()
    endif()
endif()
