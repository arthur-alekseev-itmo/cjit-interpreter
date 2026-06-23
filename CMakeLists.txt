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
        core/BytecodeTransform/CnpExecutionTrace.cpp
        core/BytecodeTransform/CnpExecutionTrace.h
        core/Codegen/CnpCodegen.cpp
        core/Codegen/CnpCodegen.h
        core/Codegen/CnpPatcher.cpp
        core/Codegen/CnpPatcher.h
        core/Execution/CnpFunction.cpp
        core/Execution/CnpFunction.h
        core/Execution/CnpInterpreter.cpp
        core/Execution/CnpInterpreter.h
        core/Parsing/Bytecode/Data/Bytecode.cpp
        core/Parsing/Bytecode/Data/Bytecode.h
        core/Parsing/Bytecode/Data/BytecodeClass.cpp
        core/Parsing/Bytecode/Data/BytecodeClass.h
        core/Parsing/Bytecode/Data/BytecodeFile.cpp
        core/Parsing/Bytecode/Data/BytecodeFile.h
        core/Parsing/Bytecode/Data/Instruction.cpp
        core/Parsing/Bytecode/Data/Instruction.h
        core/Parsing/Bytecode/Data/Opcode.cpp
        core/Parsing/Bytecode/Data/Opcode.h
        core/Parsing/Bytecode/IO/BytecodeReader.cpp
        core/Parsing/Bytecode/IO/BytecodeReader.h
        core/Parsing/Stencil/Data/CnpStencil.cpp
        core/Parsing/Stencil/Data/CnpStencil.h
        core/Parsing/Stencil/Data/CnpStencilCollection.cpp
        core/Parsing/Stencil/Data/CnpStencilCollection.h
        core/Parsing/Stencil/IO/ElfParsing/BinaryWrapper.cpp
        core/Parsing/Stencil/IO/ElfParsing/BinaryWrapper.hpp
        core/Parsing/Stencil/IO/ElfParsing/ElfBinaryWrapper.cpp
        core/Parsing/Stencil/IO/ElfParsing/ElfBinaryWrapper.h
        core/Parsing/Stencil/IO/ElfParsing/MachOBinaryWrapper.cpp
        core/Parsing/Stencil/IO/ElfParsing/MachOBinaryWrapper.hpp
        core/Parsing/Stencil/IO/ElfParsing/RelocationWrapper.cpp
        core/Parsing/Stencil/IO/ElfParsing/RelocationWrapper.h
        core/Parsing/Stencil/IO/ElfParsing/SymbolWrapper.cpp
        core/Parsing/Stencil/IO/ElfParsing/SymbolWrapper.h
        core/Parsing/Stencil/IO/CnpStencilFactory.cpp
        core/Parsing/Stencil/IO/CnpStencilFactory.h
        core/Runtime/RtObjects/RtClass.cpp
        core/Runtime/RtObjects/RtClass.h
        core/Runtime/RtObjects/RtObject.cpp
        core/Runtime/RtObjects/RtObject.h
        core/Runtime/RtObjects/RtMember.cpp
        core/Runtime/RtObjects/RtMember.h
        core/Runtime/RtWrappers/RtClassObject.cpp
        core/Runtime/RtWrappers/RtClassObject.h
        core/Runtime/RtWrappers/RtConstructorWrapper.cpp
        core/Runtime/RtWrappers/RtConstructorWrapper.h
        core/Runtime/RtWrappers/RtClosureWrapper.cpp
        core/Runtime/RtWrappers/RtClosureWrapper.h
        core/Runtime/RtBuiltins.cpp
        core/Runtime/RtBuiltins.h
        core/Runtime/RtClassHierarchy.cpp
        core/Runtime/RtClassHierarchy.h
        core/Runtime/Runtime.cpp
        core/Runtime/Runtime.h
        core/Utils/utils.cpp
        core/Utils/utils.hpp
        core/Runtime/RtWrappers/RtFunctionWrapper.cpp
        core/Runtime/RtWrappers/RtFunctionWrapper.h
        core/Runtime/RtWrappers/RtPrimitiveWrappers/RtIntWrapper.cpp
        core/Runtime/RtWrappers/RtPrimitiveWrappers/RtIntWrapper.h
        core/Runtime/RtWrappers/RtPrimitiveWrappers/RtWrapperTypes.cpp
        core/Runtime/RtWrappers/RtPrimitiveWrappers/RtWrapperTypes.h
        core/Runtime/RtWrappers/RtPrimitiveWrappers/RtPrimitiveWrapper.cpp
        core/Runtime/RtWrappers/RtPrimitiveWrappers/RtPrimitiveWrapper.h
)

target_link_libraries(cjit_interpreter LIEF::LIEF)
target_link_libraries(cjit_interpreter cxxopts::cxxopts)
target_link_libraries(cjit_interpreter loguru::loguru)

add_custom_target(copy_stencils ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/stencils"
        "${CMAKE_CURRENT_BINARY_DIR}/stencils"
)

add_dependencies(cjit_interpreter copy_stencils)
