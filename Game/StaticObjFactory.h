#ifndef INCLUDED_GAME_STATIC_OBJECT_FACTORY_H
#define INCLUDED_GAME_STATIC_OBJECT_FACTORY_H
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include "GameLib/Framework.h"
#include "Game/StaticObject.h"

class StaticObjectFactory {
public:
    static StaticObject* create(char t, std::istringstream& stgStream, unsigned iid);
};

#endif