#   #Run macdeployqt if it can be found, to copy all the qt dlls.

# Copy library files to installed directory
install(TARGETS
    ${CMAKE_PROJECT_NAME}
    imageprocessing
    DESTINATION .
    CONFIGURATIONS Debug
)
install(TARGETS
    ${CMAKE_PROJECT_NAME}
    imageprocessing
    DESTINATION .
    CONFIGURATIONS Release
)

# install(FILES ${QML_FILES}
  # DESTINATION .
  # CONFIGURATIONS Debug Release
# )

# install(FILES ${RESOURCE_SOURCES}
  # DESTINATION .
  # CONFIGURATIONS Debug Release
# )

# Copy resources images
#add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_CURRENT_SOURCE_DIR}/resources/images
#	$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
#	COMMENT "Copying images directory to Runtime directory: " $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
# )

# Qt Libraries
install(FILES
  $<TARGET_FILE:Qt5::Core>
  $<TARGET_FILE:Qt5::Gui>
  $<TARGET_FILE:Qt5::Widgets>
  $<TARGET_FILE:Qt5::Qml>
  $<TARGET_FILE:Qt5::3DCore>
  $<TARGET_FILE:Qt5::OpenGL>
  $<TARGET_FILE:Qt5::Quick>
  $<TARGET_FILE:Qt5::Multimedia>
  $<TARGET_FILE:Qt5::MultimediaWidgets>
  $<TARGET_FILE:Qt5::Network>
  qt.conf
  DESTINATION .
  CONFIGURATIONS Release
)
install(FILES
  $<TARGET_FILE:Qt5::Core>
  $<TARGET_FILE:Qt5::Gui>
  $<TARGET_FILE:Qt5::Widgets>
  $<TARGET_FILE:Qt5::Qml>
  $<TARGET_FILE:Qt5::3DCore>
  $<TARGET_FILE:Qt5::OpenGL>
  $<TARGET_FILE:Qt5::Quick>
  $<TARGET_FILE:Qt5::Multimedia>
  $<TARGET_FILE:Qt5::MultimediaWidgets>
  $<TARGET_FILE:Qt5::Network>
  qt.conf
  DESTINATION .
  CONFIGURATIONS Debug
)
#Python
# OpenCV DLLs
install(FILES
  ${OPENCV_DLLS_RELEASE}
  DESTINATION .
  CONFIGURATIONS Release
)
install(FILES
  ${OPENCV_DLLS_DEBUG}
  DESTINATION .
  CONFIGURATIONS Debug
)

# find_program(MACDEPLOYQT_EXECUTABLE NAMES macdeployqt HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
# add_custom_command(TARGET install POST_BUILD
#   COMMAND "${MACDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/../.."  -always-overwrite
# )
# add_custom_command(TARGET install POST_BUILD
#    COMMAND if 1==$<CONFIG:Release> ./${MACDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
# )
