# Add more folders to ship with the application, here
folder_01.source = qml/viewer
folder_01.target = qml
img_folder.source = img
img_folder.target = .
DEPLOYMENTFOLDERS = folder_01 img_folder

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    connection.cpp \
    util.cpp \
    simple-dds-image-reader/ddsreader.cpp \
    minimap.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    connection.h \
    util.h \
    simple-dds-image-reader/ddsreader.hpp \
    minimap.h

QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES += \
    img/AT-SPG.svg \
    img/heavyTank.svg \
    img/lightTank.svg \
    img/mediumTank.svg \
    img/SPG.svg
