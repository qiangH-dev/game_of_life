PARTY_LIBS += \
    gflags \
    absl \

#amster 需要加载的lib库
AMSTER_LIB_DIRS += \
    $$PWD/../../../bbs/bb \


include(../simmodelqt-config.pri)

HEADERS += \
    $$PWD/../multiply/multiplysimmodel.h \
    $$PWD/../../common.h \

SOURCES += \
    $$PWD/../multiply/multiplysimmodel.cpp

win32{
    DEFINES += NOMINMAX
}

linux{
    LIBS += -lpthread
}
