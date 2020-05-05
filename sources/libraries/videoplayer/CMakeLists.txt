include(GenerateExportHeader)

set(LIBRARY_NAME videoplayer)

file(GLOB_RECURSE UI_FILES *.ui)
file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

set(VIDEOPLAYER_LIB_INCLUDE_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

include_directories(
  ${VIDEOPLAYER_LIB_INCLUDE_PATHS}
)

add_library(${LIBRARY_NAME}
  SHARED
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${UI_HEADERS}
)

# Debug postfix of DLLs:
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
  Qt5::Xml
  Qt5::Network
  Qt5::Multimedia
  Qt5::MultimediaWidgets
)

target_include_directories(
  ${LIBRARY_NAME}
  PUBLIC
  ${VIDEOPLAYER_LIB_INCLUDE_PATHS}
)
