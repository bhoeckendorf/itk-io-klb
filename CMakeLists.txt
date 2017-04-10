project(itk-io-klb)

cmake_minimum_required(VERSION 3.5)

set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_STANDARD   99)
set(CMAKE_CXX_STANDARD 11)

include(ExternalProject)
set(KLB_INSTALL_DIR "${CMAKE_BINARY_DIR}/external/klb/")
ExternalProject_Add(
    klb-dl
    GIT_REPOSITORY	https://bitbucket.org/fernandoamat/keller-lab-block-filetype.git
    GIT_TAG			master
    CMAKE_ARGS		-DCMAKE_INSTALL_PREFIX=${KLB_INSTALL_DIR}
)
link_directories(${LINK_DIRECTORIES} "${KLB_INSTALL_DIR}/lib/")

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_subdirectory(src)

option(test "Build tests" OFF)
if (test)
    enable_testing()
    add_subdirectory(test)
endif()
