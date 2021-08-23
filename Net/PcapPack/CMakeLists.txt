cmake_minimum_required(VERSION 2.6)
project(pcappack)


include_directories(./)
include_directories(./)

SET(SRC_FILES ./main.cpp)
SET(SRC_FILES ./)

aux_source_directory("./" SRC_FILES)


add_executable(pcappack ${SRC_FILES})

#target_link_libraries(getpolicyfile  ${EXTRA_LIBS}) # libcems-api.a
target_link_libraries(pcappack  pthread)

target_link_libraries(pcappack  dl)
target_link_libraries(pcappack  pcap)
if(SYSTYPE MATCHES "uniontech")
target_link_libraries(pcappack  idn2)
endif()

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")