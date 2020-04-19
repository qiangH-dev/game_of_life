#include <gbbinfra/entityregister.h>
#include <gbbinfra/descriptorregister.h>

#include "net/phoneyou/gamelife/bb/Descriptors.pb.h"
using namespace net::phoneyou::gamelife::bb::descriptor;
REGISTER_DESCRIPTORTABLE(CellInfo)
REGISTER_DESCRIPTORTABLE(Position)

#include "net/phoneyou/gamelife/bb/SingletonDescriptors.pb.h"

REGISTER_SINGLETON_DESCRIPTOR(Dimensions)

#include "net/phoneyou/gamelife/bb/Entities.pb.h"
using namespace net::phoneyou::gamelife::bb::entity;
REGISTER_ENTITY(Cell)
