cmake_minimum_required(VERSION 3.22.1)
project(cjit_interpreter)

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

    include(FetchContent)
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
        utils/utils.cpp
        utils/utils.hpp
        cnp/execution/CnpFunction.cpp
        cnp/execution/CnpFunction.h
        cnp/stencil/CnpStencil.cpp
        cnp/stencil/CnpStencil.h
        cnp/CnpSymbol.cpp
        cnp/CnpSymbol.h
        cnp/stencil/CnpStencilFactory.cpp
        cnp/stencil/CnpStencilFactory.h
        cnp/stencil/CnpStencilCollection.cpp
        cnp/stencil/CnpStencilCollection.h
        cnp/execution/CnpInterpreter.cpp
        cnp/execution/CnpInterpreter.h
        cnp/codegen/CnpCodegen.cpp
        cnp/codegen/CnpCodegen.h
        cnp/debug/CnpExecutionTrace.cpp
        cnp/debug/CnpExecutionTrace.h
        cnp/parsing/BinaryWrapper.cpp
        cnp/parsing/BinaryWrapper.hpp
        cnp/parsing/MachOBinaryWrapper.cpp
        cnp/parsing/MachOBinaryWrapper.hpp
        cnp/parsing/SymbolWrapper.cpp
        cnp/parsing/SymbolWrapper.h
        cnp/parsing/RelocationWrapper.cpp
        cnp/parsing/RelocationWrapper.h
        cnp/parsing/ElfBinaryWrapper.cpp
        cnp/parsing/ElfBinaryWrapper.h
        bytecode/Bytecode.cpp
        bytecode/Bytecode.h
        bytecode/Instruction.cpp
        bytecode/Instruction.h
        bytecode/Opcode.cpp
        bytecode/Opcode.h
        builtins/Builtins.cpp
        builtins/Builtins.h
        bytecode/io/BytecodeReader.cpp
        bytecode/io/BytecodeReader.h
        bytecode/examples/BytecodeExamples.cpp
        bytecode/examples/BytecodeExamples.h

)

target_link_libraries(cjit_interpreter LIEF::LIEF)

add_custom_target(copy_stencils ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/stencils"
        "${CMAKE_CURRENT_BINARY_DIR}/stencils"
)

add_dependencies(cjit_interpreter copy_stencils)
