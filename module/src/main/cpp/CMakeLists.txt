## 规定cmake最低版本
cmake_minimum_required(VERSION 3.4.1)

## 设置项目名称
if (NOT DEFINED MODULE_NAME)
    message(FATAL_ERROR "MODULE_NAME is not set")
else ()
    project(${MODULE_NAME})
endif ()

# 添加 Dobby 框架
set(DobbyHome include/Dobby)
include_directories(
        ${DobbyHome}/include/
)
macro(SET_OPTION option value)
    set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()
SET_OPTION(DOBBY_GENERATE_SHARED OFF)
add_subdirectory(${DobbyHome} dobby)

add_definitions(-DRIRU_MODULE)

# 配置文件
configure_file(template/config.cpp config.cpp)

message("Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 11)

set(LINKER_FLAGS "-ffixed-x18 -Wl,--hash-style=both")
set(C_FLAGS "-Werror=format -fdata-sections -ffunction-sections")
set(CXX_FLAGS "${CXX_FLAGS} -fno-exceptions -fno-rtti")

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(C_FLAGS "${C_FLAGS} -O2 -fvisibility=hidden -fvisibility-inlines-hidden")
    set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,-exclude-libs,ALL -Wl,--gc-sections -Wl,--strip-all")
else ()
    set(C_FLAGS "${C_FLAGS} -O0")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C_FLAGS} ${CXX_FLAGS}")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")

find_package(riru REQUIRED CONFIG)

include_directories(include)

add_library(${MODULE_NAME} SHARED main.cpp # riru注入主程序
        ${CMAKE_CURRENT_BINARY_DIR}/config.cpp # 模块配置文件
        Hook.cpp # hook
        utilities/Tools.cpp # 添加工具支持
        utilities/Gui.cpp # ImGui
        utilities/GuiUtils.cpp # ImGui工具
        utilities/Game.cpp # 游戏渲染
        include/And64InlineHook/And64InlineHook.cpp # 添加64位的hook支持
        include/Substrate/hde64.c
        include/Substrate/SubstrateDebug.cpp
        include/Substrate/SubstrateHook.cpp
        include/Substrate/SubstratePosixMemory.cpp # 添加32位的hook支持
        include/Imgui/imgui.cpp # ImGui支持
        include/KittyMemory/KittyMemory.cpp # 补充
        )

target_link_libraries(${MODULE_NAME} log riru::riru android dobby  EGL GLESv2)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
            COMMAND ${CMAKE_STRIP} --strip-all --remove-section=.comment "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${MODULE_NAME}.so")
endif ()