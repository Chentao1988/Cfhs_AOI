#-------------------------------------------------
#
# Project created by QtCreator 2019-08-09T09:30:21
#
#-------------------------------------------------

QT       += core gui axcontainer

CONFIG += qwt
DEFINES += QT_DLL QWT_DLL
win32:CONFIG(release, debug|release): LIBS += -L"D:\Qt\QtPlugin\qwt-6.1.4\lib" -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L"D:\Qt\QtPlugin\qwt-6.1.4\lib" -lqwtd
INCLUDEPATH += D:\Qt\QtPlugin\qwt-6.1.4\src

CONFIG += Cfhs_Business
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/Lib/ -lCfhs_Business
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/Lib/ -lCfhs_Businessd

INCLUDEPATH += $$PWD/Lib
DEPENDPATH += $$PWD/Lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


DESTDIR += bin
UI_DIR  += tmp/ui
MOC_DIR += tmp/moc
RCC_DIR += tmp/rcc
OBJECTS_DIR  += tmp/obj

RC_FILE += Resource/start.rc

TARGET = Cfhs_AOI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        cfhs_barchart.cpp \
        cfhs_frame.cpp \
        cfhs_insertrecord.cpp \
        cfhs_logindialog.cpp \
        cfhs_productresultwidget.cpp \
        cfhs_pwdmodifydialog.cpp \
        cfhs_sidetool.cpp \
        cfhs_startbatchdetect.cpp \
        cfhs_statusbar.cpp \
        cfhs_tablewidget.cpp \
        main.cpp \
        cfhs_mainwindow.cpp \
        cfhs_global.cpp \
        cfhs_defectsmallimage.cpp \
        cfhs_messageinput.cpp \
        cfhs_messageselect.cpp \
        cfhs_taskinfowidget.cpp \
        cfhs_combobox.cpp \
        cfhs_lineedit.cpp \
        cfhs_showzoomimage.cpp \
        cfhs_mainwindows_img.cpp

HEADERS += \
        cfhs_base.h \
        cfhs_barchart.h \
        cfhs_frame.h \
        cfhs_insertrecord.h \
        cfhs_logindialog.h \
        cfhs_mainwindow.h \
        cfhs_productresultwidget.h \
        cfhs_pwdmodifydialog.h \
        cfhs_sidetool.h \
        cfhs_startbatchdetect.h \
        cfhs_statusbar.h \
        cfhs_tablewidget.h \
        cfhs_global.h \
        cfhs_defectsmallimage.h \
        cfhs_messageinput.h \
        cfhs_messageselect.h \
        cfhs_taskinfowidget.h \
        cfhs_combobox.h \
        cfhs_lineedit.h \
        cfhs_showzoomimage.h \
        Lib/Cfhs_Global.h \
        Lib/Cfhs_IBusiness.h \
        cfhs_mainwindows_img.h

FORMS += \
        cfhs_mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource/resource.qrc

TRANSLATIONS += Resource/lang_En.ts \
                Resource/lang_Cht.ts

include(Function/Function.pri);
include(Control/Control.pri);
include(Setting/Setting.pri);
include(Help/Help.pri);


