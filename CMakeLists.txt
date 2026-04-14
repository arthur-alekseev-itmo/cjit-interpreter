cmake_minimum_required(VERSION 3.22.1)
project(cjit_interpreter)

include(FetchContent)

# cxxopts dependency
FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts
        GIT_TAG        v3.3.1
)

FetchContent_MakeAvailable(cxxopts)

# loguru dependency
FetchContent_Declare(
        loguru
        GIT_REPOSITORY https://github.com/emilk/loguru
        GIT_TAG        master
)

set(LOGURU_WITH_STREAMS TRUE)

FetchContent_MakeAvailable(loguru)

# LEAF dependency
# LIEF build config. Set the default options for LIEF's project setup
option(LIEF_DOC "Build LIEF docs" OFF)
option(LIEF_PYTHON_API "Build LIEF Python API" OFF)
option(LIEF_EXAMPLES "Build LIEF examples" OFF)
option(LIEF_TESTS "Build LIEF tests" OFF)

# NOTE: This submodule does not exist for this example, but it does the same
# thing as FetchContent without the download part
set(vendorLIEF_submodule_dir "${CMAKE_CURRENT_LIST_DIR}/LIEF")
if(EXISTS "${vendorLIEF_submodule_dir}")
    add_subdirectory("${vendorLIEF_submodule_dir}")

# Else, we'll specify how to obtain LIEF another way (downloading)
else()
    # URL of the LIEF repo (Can be your fork)
    set(LIEF_GIT_URL "https://github.com/lief-project/LIEF.git")

    # LIEF's version to be used (can be 'main')
    set(LIEF_VERSION 0.13.0)

    FetchContent_Declare(LIEF
            GIT_REPOSITORY  "${LIEF_GIT_URL}"
            GIT_TAG         ${LIEF_VERSION}
            # You may specify an existing LIEF source directory if you don't want to
            # download. Just comment out the above ``GIT_*`` commands and uncomment the
            # following ``SOURCE_DIR`` line
            #SOURCE_DIR      "${CMAKE_CURRENT_LIST_DIR}/../../.."
    )

    if(${CMAKE_VERSION} VERSION_LESS "3.14.0")
        # CMake 3.11 to 3.13 needs more verbose method to make LIEF available
        FetchContent_GetProperties(LIEF)
        if(NOT LIEF_POPULATED)
            FetchContent_Populate(LIEF)
            add_subdirectory(${LIEF_SOURCE_DIR} ${LIEF_BINARY_DIR})
        endif()
    else()
        # CMake 3.14+ has single function to make LIEF available (recommended)
        FetchContent_MakeAvailable(LIEF)
    endif()
endif()

if(APPLE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-4 -fno-limit-debug-info")
endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded" CACHE STRING "CRT option")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cjit_interpreter main.cpp
        core/utils/utils.cpp
        core/utils/utils.hpp
        core/cnp/execution/CnpFunction.cpp
        core/cnp/execution/CnpFunction.h
        core/cnp/stencil/CnpStencil.cpp
        core/cnp/stencil/CnpStencil.h
        core/cnp/CnpSymbol.cpp
        core/cnp/CnpSymbol.h
        core/cnp/stencil/CnpStencilFactory.cpp
        core/cnp/stencil/CnpStencilFactory.h
        core/cnp/stencil/CnpStencilCollection.cpp
        core/cnp/stencil/CnpStencilCollection.h
        core/cnp/execution/CnpInterpreter.cpp
        core/cnp/execution/CnpInterpreter.h
        core/cnp/codegen/CnpCodegen.cpp
        core/cnp/codegen/CnpCodegen.h
        core/cnp/debug/CnpExecutionTrace.cpp
        core/cnp/debug/CnpExecutionTrace.h
        core/cnp/parsing/BinaryWrapper.cpp
        core/cnp/parsing/BinaryWrapper.hpp
        core/cnp/parsing/MachOBinaryWrapper.cpp
        core/cnp/parsing/MachOBinaryWrapper.hpp
        core/cnp/parsing/SymbolWrapper.cpp
        core/cnp/parsing/SymbolWrapper.h
        core/cnp/parsing/RelocationWrapper.cpp
        core/cnp/parsing/RelocationWrapper.h
        core/cnp/parsing/ElfBinaryWrapper.cpp
        core/cnp/parsing/ElfBinaryWrapper.h
        core/bytecode/Bytecode.cpp
        core/bytecode/Bytecode.h
        core/bytecode/Instruction.cpp
        core/bytecode/Instruction.h
        core/bytecode/Opcode.cpp
        core/bytecode/Opcode.h
        core/builtins/Builtins.cpp
        core/builtins/Builtins.h
        core/bytecode/io/BytecodeReader.cpp
        core/bytecode/io/BytecodeReader.h
        core/bytecode/examples/BytecodeExamples.cpp
        core/bytecode/examples/BytecodeExamples.h

)

target_link_libraries(cjit_interpreter LIEF::LIEF)
target_link_libraries(cjit_interpreter cxxopts::cxxopts)
target_link_libraries(cjit_interpreter loguru::loguru)

add_custom_target(copy_stencils ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "stencils"
        "${CMAKE_CURRENT_BINARY_DIR}/stencils"
)

add_dependencies(cjit_interpreter copy_stencils)
