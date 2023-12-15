#include "GameLib/Framework.h"

#include "Game/State.h"
#include "Game/StaticObject.h"
#include "Game/DynamicObject.h"
#include "SoundManager.h"
#include "Image.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <cstdlib>

using namespace GameLib;


State::State( const char* stageData, int size, int sid) :
mObjImage( 0 ),
mMapImage( 0 ),
mStageID( sid ),
mDynamicObjects( 0 ),
mDynamicObjectNumber( 0 ),
mStaticObjs(0),
mTakeTurn(0),
mTurnState(0),
mStageDataSize(size) {
	Framework::instance().setwidth(600);
	Framework f = Framework::instance(); //再用几次
	mObjImage = new Image( "data/image/MonopolyObj.tga" );
	mMapImage = new Image("data/image/MonopolyTable.tga");
	int maxs = size;
	std::istringstream stageDataStream(stageData);
	char c = '?';
	bool loopFlag = false;
	unsigned iid = 0;
	while (true) // 读取stageData.txt数据
	{
		while (c != '-') {
			stageDataStream >> c;
			if (c == '=') {
				loopFlag = true;
				break;
			}
		}
		if (loopFlag) break;
		if (c == '-') {
			iid++;
			stageDataStream >> c;
			std::string cntry, city, emp;
			unsigned pri;
			Estate ee = Estate::Estate(0, "unknown", "unknown");
			StaticObject so;
			switch (c)
			{
			case 'e':
				std::getline(stageDataStream, emp);
				std::getline(stageDataStream, cntry);
				std::getline(stageDataStream, city);
				city.erase(city.length() - 1);
				cntry.erase(cntry.length() - 1);
				ee = Estate::Estate(iid, cntry, city);
				for (int i = 0; i < 3; i++) {
					stageDataStream >> pri;
					ee.setPurPrice(i, pri);
				}
				for (int i = 0; i < 6; i++) {
					stageDataStream >> pri;
					ee.setTollPrice(i, pri);
				}
				mStaticObjs.push_back(&ee);
				break;
			case 's':
				so.setID(iid);
				so.setFlag(StaticObject::FLAG_START);
				mStaticObjs.push_back(&so);
				break;
			case 'w':
				so.setID(iid);
				so.setFlag(StaticObject::FLAG_WATERCOMP);
				mStaticObjs.push_back(&so);
				break;
			case 't':
				so.setID(iid);
				so.setFlag(StaticObject::FLAG_TREASURE);
				mStaticObjs.push_back(&so);
				break;
			case 'x':
				so.setID(iid);
				so.setFlag(StaticObject::FLAG_TAX);
				mStaticObjs.push_back(&so);
				break;
			case 'a':
				so.setID(iid);
				so.setFlag(StaticObject::FLAG_AIRPORT);
				mStaticObjs.push_back(&so);
				break;
			case 'o':
				so.setID(iid);
				so.setFlag(StaticObject::FLAG_TOPRISON);
				mStaticObjs.push_back(&so);
				break;
			case 'v':
				so.setID(iid);
				so.setFlag(StaticObject::FLAG_VACATION);
				mStaticObjs.push_back(&so);
				break;
			default:
				ASSERT("Bakana!");
				break;
			}
		}
		if (c == '\n')stageDataStream >> c;
		
	}

	// 分配动态对象
	int playernum = 0;
	switch (mStageID)
	{
	case 0: // 4人游戏
		playernum = 4;
		break;
	case 1: // 5人游戏
		playernum = 5;
		break;
	case 2: // 6人游戏
		playernum = 6;
		break;
	default:
		break;
	}
	mDynamicObjectNumber = playernum;
	mDynamicObjects = new DynamicObject[mDynamicObjectNumber];

	for (int i = 0; i < playernum; i++) {
		mDynamicObjects[i] = DynamicObject(DynamicObject::TYPE_PLAYER, 0, 1000, i);
	}

}

State::~State(){
	SAFE_DELETE( mObjImage );
	SAFE_DELETE( mMapImage );
	SAFE_DELETE_ARRAY( mDynamicObjects );
}

void State::draw() const {

	// 绘制背景
	mMapImage->draw();
	// 绘制DynamicObjects
	for (int i = 0; i < mDynamicObjectNumber; ++i) {
		mDynamicObjects[i].draw(mObjImage);
	}
	// TODO
	// 绘制StaticObjects

}

bool State::isUpgradable(int posi) const {
	if (!mStaticObjs[posi]->checkFlag(StaticObject::FLAG_ESTATE))return false;
	if (mStaticObjs[posi]->getBelonging() == 0xffff)return false;
	Estate* nowEstate = dynamic_cast<Estate*>(mStaticObjs[posi]);

	for (int i = 1; i <= 3; i++) {
		int lp = (posi + mStaticObjs.size() - i) % mStaticObjs.size();
		if (!(mStaticObjs[lp]->checkFlag(StaticObject::FLAG_ESTATE)) )
			continue;
		Estate* lEstate = dynamic_cast<Estate*> (mStaticObjs[lp]);
		if (lEstate->getCountry() != nowEstate->getCountry())
			break;
		else if (lEstate->getBelonging() != nowEstate->getBelonging())
			return false;
	}
	for (int i = 1; i <= 3; i++) {
		int rp = (posi + mStaticObjs.size() + i) % mStaticObjs.size();
		if (!(mStaticObjs[rp]->checkFlag(StaticObject::FLAG_ESTATE)))
			continue;
		Estate* rEstate = dynamic_cast<Estate*> (mStaticObjs[rp]);
		if (rEstate->getCountry() != nowEstate->getCountry())
			break;
		else if (rEstate->getBelonging() != nowEstate->getBelonging())
			return false;
	}
	return true;
}


void State::update(){
	//todo
	DynamicObject& o = mDynamicObjects[mTakeTurn];
	if (o.hasPressedRollButton()) {
		o.mPosi = (o.mPosi + rand()%12 + 1) % 40;
		mTakeTurn = (mTakeTurn + 1) % mDynamicObjectNumber;
	}
	StaticObject& s = *mStaticObjs[o.mPosi];
	if (o.hasPressedBuyButton() && s.isBuyable()) {
		if ( s.getBelonging() == 0xffff) {
			// 地产是无主的
		}
		else if (s.getBelonging() == o.getID() && isUpgradable(o.mPosi)) {
			// 地产是自己的且满足升级条件
		}
	}
	
}



