#include <map>
#include <string>
#include "ReflectionMacros.h"

#ifndef ASSOCIATIONMAP
#define ASSOCIATIONMAP

typedef std::map <std::string, std::string> AssociationMap;
REFLECTION_COLLECTION (AssociationMap)

#endif // ASSOCIATIONMAP

