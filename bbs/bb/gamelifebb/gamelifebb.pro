IMPORT_PROTOS_CPP_GENERATED = 1

INSTALL_FILES += \

include(../../bbs-config.pri)

PROTO_FILES += \
    $$BBS_PROTO_DIR/net/phoneyou/gamelife/bb/Messages.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gamelife/bb/Entities.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gamelife/bb/Common.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gamelife/bb/Descriptors.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gamelife/bb/SingletonDescriptors.proto \

GENERATOR_DLLEXPORT = GAMELIFE_GENERATOR_DLLEXPORT

include($$AMSTERFWK_SRC_DIR/protobufconfig.pri)

DISTFILES += $$PROTO_FILES \

SOURCES += \
    register.cpp

