#include "StaticObjFactory.h"
#include "StaticObject.h"

StaticObject* StaticObjectFactory::create(char c, std::istringstream& stageDataStream, unsigned iid) {
	unsigned pri;
	Estate* ee;
	StaticObject* so = new StaticObject();
	so->setID(iid);
	so->setPosi(iid - 1);
    StaticObject* obj = nullptr;
	std::string cntry, city, emp;
	switch (c)
	{
	case 'e':
		std::getline(stageDataStream, emp);
		std::getline(stageDataStream, cntry);
		std::getline(stageDataStream, city);
		city.erase(city.length() - 1);
		cntry.erase(cntry.length() - 1);
		ee = new Estate(iid, cntry, city);
		ee->setFlag(StaticObject::FLAG_ESTATE);
		ee->setID(iid);
		ee->setPosi(iid - 1);
		for (int i = 0; i < 3; i++) {
			stageDataStream >> pri;
			ee->setPurPrice(i, pri);
		}
		for (int i = 0; i < 6; i++) {
			stageDataStream >> pri;
			ee->setTollPrice(i, pri);
		}
		return ee;
		break;
	case 's':
		so->setFlag(StaticObject::FLAG_START);
		return so;
		break;
	case 'w':
		so->setFlag(StaticObject::FLAG_WATERCOMP);
		return so;
		break;
	case 't':
		so->setFlag(StaticObject::FLAG_TREASURE);
		return so;
		break;
	case 'x':
		so->setFlag(StaticObject::FLAG_TAX);
		return so;
		break;
	case 'a':
		so->setFlag(StaticObject::FLAG_AIRPORT);
		return so;
		break;
	case 'o':
		so->setFlag(StaticObject::FLAG_TOPRISON);
		return so;
		break;
	case 'v':
		so->setFlag(StaticObject::FLAG_VACATION);
		return so;
		break;
	case 'p':
		so->setFlag(StaticObject::FLAG_PRISON);
		return so;
		break;
	case 'c':
		so->setFlag(StaticObject::FLAG_CHANCE);
		return so;
		break;
	default:
		ASSERT("Bakana!");
		break;
	}
    return obj;
}