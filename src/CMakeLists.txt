file(GLOB HDRS *.h *.hpp)
file(GLOB SRCS *.c *.cpp)

set(tgt ITKIOKLB)
add_library(${tgt} SHARED ${HDRS} ${SRCS})
add_dependencies(${tgt} klb-dl)
target_compile_definitions(${tgt} PRIVATE COMPILE_SHARED_LIBRARY)
target_include_directories(${tgt} PUBLIC "${KLB_INSTALL_DIR}/include/")
target_link_libraries(${tgt} ${ITK_LIBRARIES} klb_static)

install(FILES ${HDRS} DESTINATION include)
install(TARGETS ${tgt} DESTINATION lib)
