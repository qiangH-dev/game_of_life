URI = net.phoneyou.GAMELIFE.BB.Templates

API_VER = 1.0

TEMPLATE = aux

include($$PWD/../../../../amster-config.pri)

BBS_PROTO_DIR = $$PWD/../../../protos

PROTO_FILES += \
    $$BBS_PROTO_DIR/net/phoneyou/gamelife/bb/Messages.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gamelife/bb/Entities.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gamelife/bb/Common.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gamelife/bb/Descriptors.proto \
    $$BBS_PROTO_DIR/net/phoneyou/gamelife/bb/SingletonDescriptors.proto \

MODULE_QUALIFIERS = import net.phoneyou.AMSTER.PBControls 1.0;

PACKAGET_ROOT_PATH = $$BBS_PROTO_DIR

include($$AMSTERFWK_DIR/amster.pri)

DESTPATH = $$URI
DESTPATH ~= s,\.,/,g
DESTDIR = $$AMSTER_QML_IMPORT_PATH/$$DESTPATH
GENERATOR_OUTPUT_PATH = $$DESTDIR

include($$AMSTERFWK_SRC_DIR/protobufconfig_qml.pri)

DISTFILES += $$PROTO_FILES
