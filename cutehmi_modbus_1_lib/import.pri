# This file sets qmake variables for linkage against cutehmi_modbus_1_lib.

include(libdef.pri)

DEFINES += CUTEHMI_MODBUS_DYNAMIC
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
LIBS += -L$$PWD/../../bin -l$$qtLibraryTarget($$CUTEHMI_MODBUS_LIBNAME)
