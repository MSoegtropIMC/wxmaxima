# -*- mode: CMake; cmake-tab-width: 4; -*-

if(APPLE)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
endif()

set(DATAFILES
        ${CMAKE_CURRENT_SOURCE_DIR}/io.github.wxmaxima_developers.wxMaxima.png
        ${CMAKE_CURRENT_SOURCE_DIR}/io.github.wxmaxima_developers.wxMaxima.svg)

set(PIXMAPS
        ${CMAKE_CURRENT_SOURCE_DIR}/text-x-wxmathml.svg
        ${CMAKE_CURRENT_SOURCE_DIR}/text-x-wxmaxima-batch.svg
        ${CMAKE_CURRENT_SOURCE_DIR}/wxmaxima-16.xpm
        ${CMAKE_CURRENT_SOURCE_DIR}/io.github.wxmaxima_developers.wxMaxima.png
        ${CMAKE_CURRENT_SOURCE_DIR}/io.github.wxmaxima_developers.wxMaxima.svg
        ${CMAKE_CURRENT_SOURCE_DIR}/wxmaxima-32.xpm)

if(WIN32)
    add_subdirectory(fonts)
else()
    string(TIMESTAMP CURRENTDATE "%Y-%m-%d")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wxmaxima.1.in ${CMAKE_CURRENT_BINARY_DIR}/wxmaxima.1)
    install(FILES ${PIXMAPS}                                                                     DESTINATION share/pixmaps)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/wxmaxima                                           DESTINATION share/bash-completion/completions)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wxmaxima.1                                         DESTINATION share/man/man1)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/io.github.wxmaxima_developers.wxMaxima.appdata.xml DESTINATION share/metainfo)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/io.github.wxmaxima_developers.wxMaxima.desktop     DESTINATION share/applications)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/x-wxmathml.xml                                     DESTINATION share/mime/packages)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/x-wxmaxima-batch.xml                               DESTINATION share/mime/packages)
endif()
install(FILES ${DATAFILES} DESTINATION share/wxMaxima)

# allow local execution (./wxmaxima-local) from the build directory without installation
if(UNIX)
    file(COPY ${DATAFILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

add_subdirectory(winrc)
