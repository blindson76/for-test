cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_JS_SRC)
    set (CMAKE_JS_SRC "C:/Users/user/AppData/Roaming/npm/node_modules/cmake-js/lib/cpp/win_delay_load_hook.cc")
endif()
if(NOT DEFINED CMAKE_JS_INC)
    set (CMAKE_JS_INC "C:/Users/user/.cmake-js/electron-x86/v14.0.1/include/node")
endif()
if(NOT DEFINED CMAKE_JS_LIB)
    set (CMAKE_JS_LIB "C:/Users/user/.cmake-js/electron-x86/v14.0.1/node.lib")
endif()
# Name of the project (will be the name of the plugin)
project(wrapped)

# Build a shared library named after the project from the files in `src/`
file(GLOB SOURCE_FILES "src/*.cc" "src/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project

target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})