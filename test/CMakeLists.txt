set(GTEST_INSTALL_DIR "${CMAKE_BINARY_DIR}/external/gtest/")
ExternalProject_Add(
    gtest-dl
    GIT_REPOSITORY	https://github.com/google/googletest.git
    GIT_TAG			release-1.8.0
    CMAKE_ARGS		-DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL_DIR}
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
link_directories(${LINK_DIRECTORIES} "${GTEST_INSTALL_DIR}/lib/")

file(GLOB HDRS *.h *.hpp)
file(GLOB SRCS *.c *.cpp)

set(tgt tests)
add_executable(${tgt} ${HDRS} ${SRCS})
add_dependencies(${tgt} gtest-dl ITKIOKLB)
target_include_directories(${tgt} PUBLIC "${GTEST_INSTALL_DIR}/include/" "${CMAKE_SOURCE_DIR}/src/")
target_link_libraries(${tgt} gtest gtest_main ITKIOKLB)
install(TARGETS ${tgt} DESTINATION lib)
