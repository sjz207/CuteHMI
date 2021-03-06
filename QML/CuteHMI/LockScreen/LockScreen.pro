include(../../../common.pri)

TEMPLATE = lib
TARGET = $$qtLibraryTarget(cutehmi_lockscreen_1_qml)
DESTDIR = $$PWD/plugins	# QtDesigner can find library only in a location relative to qmldir file.

QT += qml quick

CONFIG += plugin

# Translations.
TRANSLATIONS = locale/cutehmi_lockscreen_1_qml_pl.ts

include(../../../cutehmi_utils_1_lib/import.pri)
include(../../../cutehmi_base_1_lib/import.pri)

HEADERS += \
    src/CuteHMILockScreenQMLPlugin.hpp \
    src/cutehmi/lock_screen/Auth.hpp \
    src/cutehmi/lock_screen/InternalSettings.hpp

SOURCES += \
    src/CuteHMILockScreenQMLPlugin.cpp \
    src/cutehmi/lock_screen/Auth.cpp \
    src/cutehmi/lock_screen/InternalSettings.cpp

DISTFILES += \ 
    qmldir \
    KeyButton.qml \
    LockScreen.qml \
    LockScreenSettings.qml \
    ChangePasswordWizard.qml \
    LockScreenPopup.qml

RESOURCES +=
