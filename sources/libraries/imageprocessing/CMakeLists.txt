include(GenerateExportHeader)

set(LIBRARY_NAME imageprocessing)

set(FT_LIBS ${FT_LIBS} ${LIBRARY_NAME} CACHE INTERNAL "libraries")

file(GLOB_RECURSE UI_FILES *.ui)
file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)

set(FT_SOURCE_FILES ${FT_SOURCE_FILES} ${SOURCE_FILES} CACHE INTERNAL "source files")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

set(IMAGEPROCESSING_LIB_INCLUDE_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

include_directories(
  ${IMAGEPROCESSING_LIB_INCLUDE_PATHS}
)

add_library(${LIBRARY_NAME}
  SHARED
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${UI_HEADERS}
)

# Postfix of DLLs:
if(WIN32)
  set(${LIBRARY_NAME}_DEBUG_POSTFIX d)
endif()

set_target_properties(${LIBRARY_NAME} PROPERTIES
  OUTPUT_NAME "${LIBRARY_NAME}"
  DEBUG_POSTFIX "${${LIBRARY_NAME}_DEBUG_POSTFIX}"
)

generate_export_header(${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME}
  Qt5::Widgets
  ${OpenCV_LIBS}
)

target_include_directories(
  ${LIBRARY_NAME}
  PUBLIC
  ${IMAGEPROCESSING_LIB_INCLUDE_PATHS}
)
