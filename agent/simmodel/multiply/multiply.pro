PARTY_LIBS += \
    gflags \
    absl \


#amster 需要加载的lib库
AMSTER_LIB_DIRS += \
    $$PWD/../../../bbs/bb \


include(../simmodel-config.pri)


HEADERS += \
    multiplysimmodel.h  \
    $$PWD/../../common.h \


SOURCES += \
    multiplysimmodel.cpp

win32{
    DEFINES += NOMINMAX
}

linux{
    LIBS += -lpthread
}

