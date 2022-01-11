## 规定cmake最低版本
cmake_minimum_required(VERSION 3.4.1)

## 设置项目名称
if (NOT DEFINED MODULE_NAME)
    message(FATAL_ERROR "MODULE_NAME is not set")
else ()
    project(${MODULE_NAME})
endif ()

# 添加 Dobby 框架
# 定义 DobbyHome 变量为路径 include/Dobby
# set 可以设置普通变量 缓存条目 环境变量三种变量的值
set(DobbyHome include/Dobby)
# 类似于C预处理器对宏的处理
# 该函数macro和function类似
# 这两个都是创建一段有名字的代码稍后可以调用 还可以传参数。
macro(SET_OPTION option value)
    set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()
#
SET_OPTION(DOBBY_GENERATE_SHARED OFF)
# 为构建添加一个子路径 这样就将 include/Dobby 目录转变为 cmake 的子目录 dobby
add_subdirectory(${DobbyHome} dobby)

# 查询并导入第三方库
find_package(riru REQUIRED CONFIG)

# 定义宏
add_definitions(-DRIRU_MODULE)

# 配置文件
configure_file(template/config.cpp config.cpp)

# 传递项目构建类型的消息
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

# Cmake构建项目的路径包含
include_directories(
        include
        ${DobbyHome}/include/
        )

# 将指定的源文件生成链接文件，然后添加到工程中去
# 本程序相关源代码 连接到生成的文件
add_library(${MODULE_NAME} SHARED main.cpp # riru注入主程序
        ${CMAKE_CURRENT_BINARY_DIR}/config.cpp # 模块配置文件
        Hook.cpp # hook
        utilities/Tools.cpp # 添加工具支持
        utilities/GuiLib.cpp
        utilities/GuiUtils.cpp
        include/imgui/imconfig.h # 添加imgui支持
        include/imgui/imgui.cpp
        include/imgui/imgui.h
        include/imgui/imgui_draw.cpp
        include/imgui/imgui_impl_opengl3.cpp
        include/imgui/imgui_impl_opengl3.h
        include/imgui/imgui_internal.h
        include/imgui/imgui_widgets.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imstb_rectpack.h
        include/imgui/imstb_textedit.h
        include/imgui/imstb_truetype.h
        include/KittyMemory/KittyMemory.cpp # 附加工具
        include/KittyMemory/KittyUtils.cpp
        include/KittyMemory/KittyMemory.hpp
        include/KittyMemory/KittyUtils.hpp
        include/KittyMemory/MemoryBackup.cpp
        include/KittyMemory/MemoryBackup.hpp
        include/KittyMemory/MemoryPatch.cpp
        include/KittyMemory/MemoryPatch.hpp
        )

target_include_directories(${MODULE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)

# 连接其他库
target_link_libraries(${MODULE_NAME}
        log
        riru::riru
        android
        dobby
        EGL
        GLESv2
        )

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 通过编译类型的判断 使用自定义命令
    add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
            COMMAND ${CMAKE_STRIP} --strip-all --remove-section=.comment "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${MODULE_NAME}.so")
endif ()