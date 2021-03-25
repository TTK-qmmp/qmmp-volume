include(../../plugins.pri)
include(common/common.pri)

TARGET = $$PLUGINS_PREFIX/Visual/volume

HEADERS += volume.h \
           visualvolumefactory.h
           
SOURCES += volume.cpp \
           visualvolumefactory.cpp

unix{
    target.path = $$PLUGIN_DIR/Visual
    INSTALLS += target
}
