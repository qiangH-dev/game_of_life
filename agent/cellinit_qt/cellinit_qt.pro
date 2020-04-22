PARTY_LIBS += \
    gflags \


#amster 需要加载的lib库
AMSTER_LIB_DIRS += \
    $$PWD/../../bbs/bb \

include(../agentqt-config.pri)

HEADERS += \
    $$PWD/../cellinit/cellinitagent.h

SOURCES += \
    $$PWD/../cellinit/cellinitagent.cpp
