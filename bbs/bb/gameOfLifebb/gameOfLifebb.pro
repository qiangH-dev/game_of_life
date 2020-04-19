IMPORT_PROTOS_CPP_GENERATED = 1

INSTALL_FILES += \

include(../../bbs-config.pri)

PROTO_FILES += \
    $$BBS_PROTO_DIR/net/phoneyou/gameoflife/bb/Messages.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gameoflife/bb/Entities.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gameoflife/bb/Common.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gameoflife/bb/Descriptors.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gameoflife/bb/SingletonDescriptors.proto \

GENERATOR_DLLEXPORT = GAMEOFLIFE_GENERATOR_DLLEXPORT

include($$AMSTERFWK_SRC_DIR/protobufconfig.pri)

DISTFILES += $$PROTO_FILES \

SOURCES += \
    register.cpp


message($$AMSTERFWK_SRC_DIR)
