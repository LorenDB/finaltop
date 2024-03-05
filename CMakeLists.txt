# SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
#
# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.5)

project(finaltop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(final final)

add_executable(finaltop
    CpuDialog.cpp
    CpuDialog.h
    main.cpp
    MainDialog.cpp
    MainDialog.h
    MemoryDialog.cpp
    MemoryDialog.h
    ProcessList.cpp
    ProcessList.h
)
target_link_libraries(finaltop PRIVATE final)

include(GNUInstallDirs)
install(TARGETS finaltop
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
