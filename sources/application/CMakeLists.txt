file(GLOB_RECURSE UI_FILES *.ui)
file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE QML_FILES *.qml)
file(GLOB_RECURSE RESOURCE_FILES *.qrc)

set(SOURCE_FILES ${SOURCE_FILES})
set(${CMAKE_PROJECT_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(${CMAKE_PROJECT_NAME}_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})


qt5_wrap_ui(UI_HEADERS ${UI_FILES})
qt5_add_resources(RESOURCE_SOURCES ${RESOURCE_FILES})

# Create groups
source_group(Resources FILES ${RESOURCE_SOURCES})
source_group(Resources FILES ${RESOURCE_FILES})
source_group(Qml FILES  ${QML_FILES})
source_group(Resources FILES ${CMAKE_CURRENT_BINARY_DIR}/resources.rc)

if(WIN32)
  add_executable(${CMAKE_PROJECT_NAME} WIN32
    ${UI_HEADERS}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${RESOURCE_SOURCES}
    ${QML_FILES}
  )
endif()

if(APPLE)
  add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE
    ${UI_HEADERS}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${RESOURCE_SOURCES}
    ${QML_FILES}
  )
endif()

target_link_libraries(${CMAKE_PROJECT_NAME}
  Qt5::Widgets
  Qt5::Core
  Qt5::Quick
  Qt5::PrintSupport
  Qt5::Gui
  Qt5::Multimedia
  Qt5::MultimediaWidgets
  ${OpenCV_LIBS}
  imageprocessing
  Python3::Python
  Python3::Module
)

if(WIN32)
 install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION .
  )
endif()

if(APPLE)
 install(TARGETS ${CMAKE_PROJECT_NAME}
    BUNDLE DESTINATION .
  )
endif()
