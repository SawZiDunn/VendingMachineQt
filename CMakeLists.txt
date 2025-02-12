cmake_minimum_required(VERSION 3.16)

project(vending-machine-gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ Find Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# ✅ Source Files
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
        db_controller.h moneyInstance.h
)

# ✅ Create Executable
qt_add_executable(vending-machine-gui
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}

    addstock.h addstock.cpp addstock.ui
    adminwindow.h adminwindow.cpp adminwindow.ui
    userwindow.h userwindow.cpp userwindow.ui
    setup_db.h
    refillstock.h refillstock.cpp refillstock.ui

)

# ✅ Link Libraries (QtSql for SQLite)
target_link_libraries(vending-machine-gui PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql)  # Linking QtSql for SQLite

# ✅ Set Windows/macOS/iOS Bundle Properties
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.vending-machine-gui)
endif()

set_target_properties(vending-machine-gui PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ✅ Install Directories
include(GNUInstallDirs)
install(TARGETS vending-machine-gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ✅ Finalize Qt Executable
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(vending-machine-gui)
endif()
