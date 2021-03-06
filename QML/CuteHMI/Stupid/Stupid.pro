include(../../../common.pri)

TEMPLATE = lib
TARGET = $$qtLibraryTarget(cutehmi_stupid_1_qml)
DESTDIR = $$PWD/plugins	# QtDesigner can find library only in a location relative to qmldir file.

QT += qml quick

CONFIG += plugin

# Translations.
TRANSLATIONS = locale/cutehmi_stupid_qml_1_pl.ts

include(../../../cutehmi_utils_1_lib/import.pri)
include(../../../cutehmi_base_1_lib/import.pri)
include(../../../cutehmi_charts_1_lib/import.pri)
include(../../../cutehmi_stupid_1_lib/import.pri)

HEADERS += \
    src/CuteHMIStupidQMLPlugin.hpp

SOURCES += \
    src/CuteHMIStupidQMLPlugin.cpp

DISTFILES += \ 
    qmldir \
    DS18B20Controller.qml \
    DS18B20Controller.js

RESOURCES +=
