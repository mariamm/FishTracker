# Copy library files to installed directory
install(TARGETS
    ${CMAKE_PROJECT_NAME}
    imageprocessing
    videoplayer
    DESTINATION .
    CONFIGURATIONS Debug
)
install(TARGETS
    ${CMAKE_PROJECT_NAME}
    imageprocessing
    videoplayer
    DESTINATION .
    CONFIGURATIONS Release
)

# Qt Libraries
install(FILES
  $<TARGET_FILE:Qt5::Core>
  $<TARGET_FILE:Qt5::Gui>
  $<TARGET_FILE:Qt5::Widgets>
  $<TARGET_FILE:Qt5::Qml>
  $<TARGET_FILE:Qt5::3DCore>
  $<TARGET_FILE:Qt5::OpenGL>
  $<TARGET_FILE:Qt5::Quick>
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
  qt.conf
  DESTINATION .
  CONFIGURATIONS Debug
)
# Qt Plugins
install(FILES
  ${QT_LIB_DIR}/../plugins/platforms/qwindows.dll
  DESTINATION plugins/platforms
  CONFIGURATIONS Release
)
install(FILES
  ${QT_LIB_DIR}/../plugins/platforms/qwindowsd.dll
  DESTINATION plugins/platforms
  CONFIGURATIONS Debug
)

install(FILES
  ${QT_LIB_DIR}/../plugins/imageformats/qico.dll
  ${QT_LIB_DIR}/../plugins/imageformats/qsvg.dll
  ${QT_LIB_DIR}/../plugins/imageformats/qjpeg.dll
  ${QT_LIB_DIR}/../plugins/imageformats/qgif.dll
  DESTINATION plugins/imageformats
  CONFIGURATIONS Release
)
install(FILES
  ${QT_LIB_DIR}/../plugins/imageformats/qicod.dll
  ${QT_LIB_DIR}/../plugins/imageformats/qsvgd.dll
  ${QT_LIB_DIR}/../plugins/imageformats/qjpegd.dll
  ${QT_LIB_DIR}/../plugins/imageformats/qgifd.dll
  DESTINATION plugins/imageformats
  CONFIGURATIONS Debug
)

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
