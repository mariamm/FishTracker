file(GLOB_RECURSE UI_FILES *.ui)
file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE QML_FILES *.qml)
file(GLOB_RECURSE RESOURCE_FILES *.qrc)

set(SOURCE_FILES ${FT_SOURCE_FILES} ${SOURCE_FILES} CACHE INTERNAL "source files")
set(${CMAKE_PROJECT_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "application source dir")
set(${CMAKE_PROJECT_NAME}_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "application binary dir")


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
  ${OpenCV_LIBS}
  imageprocessing
  videoplayer
)

# Ensure windows.h does not define min and max macros
if(WIN32)
  add_definitions(-DNOMINMAX)
endif()

if(WIN32)
  install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION .
  )
endif()

# install(FILES ${QML_FILES}
  # DESTINATION .
  # CONFIGURATIONS Debug Release
# )

# install(FILES ${RESOURCE_SOURCES}
  # DESTINATION .
  # CONFIGURATIONS Debug Release
# )


if (MSVC)
  # Post-Build Step: Copy DEBUG OpenCV DLLs to Runtime directory
  foreach(file ${OPENCV_DLLS_DEBUG})
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
      COMMAND if 1==$<CONFIG:Debug> ${CMAKE_COMMAND} -E copy_if_different
      ${file}
      $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
      COMMENT "Copying file to Runtime directory: " ${file}
    )
  endforeach()
  foreach(file ${OPENCV_PDBS_DEBUG})
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
      COMMAND if 1==$<CONFIG:Debug> ${CMAKE_COMMAND} -E copy_if_different
      ${file}
      $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
      COMMENT "Copying file to Runtime directory: " ${file}
    )
  endforeach()

  # Post-Build Step: Copy RELEASE OpenCV DLLs to Runtime directory
  foreach(file ${OPENCV_DLLS_RELEASE})
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
      COMMAND if 1==$<CONFIG:Release> ${CMAKE_COMMAND} -E copy_if_different
      ${file}
      $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
      COMMENT "Copying file to Runtime directory: " ${file}
    )
  endforeach()
  foreach(file ${OPENCV_PDBS_RELEASE})
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
      COMMAND if 1==$<CONFIG:Release> ${CMAKE_COMMAND} -E copy_if_different
      ${file}
      $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
      COMMENT "Copying file to Runtime directory: " ${file}
    )
  endforeach()

  # Post-Build Step: Copy imported library file to Runtime directory
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:imageprocessing>
	$<TARGET_FILE:videoplayer>
	$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
	COMMENT "Copying file to Runtime directory: " $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
  )

  # Post-Build Step: Copy example images
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/images
	$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
	COMMENT "Copying images directory ("${CMAKE_CURRENT_SOURCE_DIR}/resources/images") to Runtime directory: " $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
  )

  # Run winddeployqt if it can be found, to copy all the qt dlls.
  find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${WINDEPLOYQT_EXECUTABLE} --debug --pdb $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
  )
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND if 1==$<CONFIG:Release> ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
  )
endif()
