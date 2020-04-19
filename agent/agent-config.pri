#定义导入由probuf生成的cpp 文件
IMPORT_PROTOS_CPP_GENERATED = 1

#amster 的通用配置
include(../amster-config.pri)


AMSTER_LIB_DIRS += \
    $$PWD/libs \
    $$PWD \

# make it first
LIBS *= -L$$AMSTERFWK_DIR/lib/amster
include($$AMSTERFWK_SRC_DIR/amsteragent.pri)

#amster 需要用到的lib 库
PARTY_LIBS += \
    pluma \
    gflags \
    g3logwrapper \
    protobuf \
    absl \
    cctz \

#引入PPM
include($$(PPM_HOME)/ppm.pri)

CONFIG += warn_off

INCLUDEPATH += \
    $$PWD/libs


# 避免编译器内存不足
#CONFIG           -= build_all

win32{
    DEFINES += NOMINMAX
}
posix{
    LIBS += -lpthread
}
