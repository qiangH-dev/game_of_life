PARTY_LIBS += \
    gflags \

#amster 需要加载的lib库
AMSTER_LIB_DIRS += \
    $$PWD/../../bbs/bb \

include(../agent-config.pri)

HEADERS += \
    testagent.h

SOURCES += \
    testagent.cpp
