IMPORT_PROTOS_CPP_GENERATED = 1
include($$PWD/../../amster-config.pri)

AMSTER_LIB_DIRS += \
    $$PWD/libs \

LIBS *= -L$$AMSTERFWK_DIR/lib/amster
include($$AMSTERFWK_SRC_DIR/amstersimmodel.pri)

PARTY_LIBS += \
    pluma \
    gflags \
    g3logwrapper \
    protobuf

include($$(PPM_HOME)/ppm.pri)

CONFIG += warn_off

INCLUDEPATH += \
    $$PWD/../

# 避免编译器内存不足
#CONFIG           -= build_all
