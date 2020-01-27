TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        checksum.cpp \
        ip_change.cpp \
        main.cpp \
        util.cpp

HEADERS += \
    checksum.h \
    flow.h \
    sfdafx.h \
    util.h
LIBS += -lpcap \
        -lnetfilter_queue
