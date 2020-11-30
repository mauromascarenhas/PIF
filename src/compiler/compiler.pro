#    This file is part of PIFC (Compiler).
#
#    PIFC (Compiler) is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Foobar is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.

TARGET = pifc

QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

win32 {
    VERSION = 0.9.08

    QMAKE_TARGET_COMPANY = PIF Project
    QMAKE_TARGET_PRODUCT = PIFC
    QMAKE_TARGET_DESCRIPTION = PIF Compiler
    QMAKE_TARGET_COPYRIGHT = Copyright (c) 2019 - PIF Project

    RC_ICONS = logo\pif_logo_x64.ico
    RC_LANG = 0x0800
}
else {
    VERSION = 0.9.08
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
           PROG_VERSION=\\\"$$VERSION\\\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        messagelogger/messagelogger.cpp \
        pifcompiler.cpp \
        syntacticanalyser/blankitem/blankitem.cpp \
        syntacticanalyser/blockparser/blockparser.cpp \
        syntacticanalyser/controlparser/controlparser.cpp \
        syntacticanalyser/expresionparser/expressionparser.cpp \
        syntacticanalyser/ioparser/ioparser.cpp \
        syntacticanalyser/lexicalanalyser/lexicalanalyser.cpp \
        syntacticanalyser/lexicalanalyser/token.cpp \
        syntacticanalyser/mainprogramparser/mainprogramparser.cpp \
        syntacticanalyser/programitem.cpp \
        syntacticanalyser/syntacticanalyser.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    messagelogger/messagelogger.h \
    pifcompiler.h \
    syntacticanalyser/blankitem/blankitem.h \
    syntacticanalyser/blockparser/blockparser.h \
    syntacticanalyser/controlparser/controlparser.h \
    syntacticanalyser/expresionparser/expressionparser.h \
    syntacticanalyser/ioparser/ioparser.h \
    syntacticanalyser/lexicalanalyser/lexicalanalyser.h \
    syntacticanalyser/lexicalanalyser/token.h \
    syntacticanalyser/mainprogramparser/mainprogramparser.h \
    syntacticanalyser/programitem.h \
    syntacticanalyser/syntacticanalyser.h
