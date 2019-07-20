QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        messagelogger/messagelogger.cpp \
        pifcompiler.cpp \
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
    syntacticanalyser/blockparser/blockparser.h \
    syntacticanalyser/controlparser/controlparser.h \
    syntacticanalyser/expresionparser/expressionparser.h \
    syntacticanalyser/ioparser/ioparser.h \
    syntacticanalyser/lexicalanalyser/lexicalanalyser.h \
    syntacticanalyser/lexicalanalyser/token.h \
    syntacticanalyser/mainprogramparser/mainprogramparser.h \
    syntacticanalyser/programitem.h \
    syntacticanalyser/syntacticanalyser.h
