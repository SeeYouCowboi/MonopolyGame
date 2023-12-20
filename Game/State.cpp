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
#include <cstdio>  // 包含头文件以使用 sprintf_s

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
	mStaBelongImage = new Image("data/image/StaticStateBelonging2.tga");
	mStaUpgradeImage = new Image("data/image/StaticStateUpgrade.tga");
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
			Estate* ee;
			StaticObject* so = new StaticObject();
			so->setID(iid);
			so->setPosi(iid - 1);
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
				mStaticObjs.push_back(ee);
				break;
			case 's':
				so->setFlag(StaticObject::FLAG_START);
				mStaticObjs.push_back(so);
				break;
			case 'w':
				so->setFlag(StaticObject::FLAG_WATERCOMP);
				mStaticObjs.push_back(so);
				break;
			case 't':
				so->setFlag(StaticObject::FLAG_TREASURE);
				mStaticObjs.push_back(so);
				break;
			case 'x':
				so->setFlag(StaticObject::FLAG_TAX);
				mStaticObjs.push_back(so);
				break;
			case 'a':
				so->setFlag(StaticObject::FLAG_AIRPORT);
				mStaticObjs.push_back(so);
				break;
			case 'o':
				so->setFlag(StaticObject::FLAG_TOPRISON);
				mStaticObjs.push_back(so);
				break;
			case 'v':
				so->setFlag(StaticObject::FLAG_VACATION);
				mStaticObjs.push_back(so);
				break;
			case 'p':
				so->setFlag(StaticObject::FLAG_PRISON);
				mStaticObjs.push_back(so);
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
	Framework f = Framework::instance();
	
	// 绘制背景
	mMapImage->draw();
	char buff[50];
	// 绘制DynamicObjects
	for (int i = 0; i < mDynamicObjectNumber; i++) {
		mDynamicObjects[i].draw(mObjImage);
		sprintf_s(buff, "Playe %d bank: %d\$", mDynamicObjects[i].getID(), mDynamicObjects[i].mMoney);
		f.drawDebugString(14, 5 + i, buff);
	}
	// 绘制StaticObjects
	for (int i = 0; i < mStaticObjs.size(); i++) {
		mStaticObjs[i]->draw(mStaBelongImage, mStaUpgradeImage);
	}
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
	DynamicObject& o = mDynamicObjects[mTakeTurn];
	if (o.hasPressedRollButton()) {
		o.mPosi = (o.mPosi + ((rand()%5)+(rand()%5) + 1)) % 40;
		mTakeTurn = (mTakeTurn + 1) % mDynamicObjectNumber;
	}

	int preTakeTurn = (mTakeTurn - 1 + mDynamicObjectNumber) % mDynamicObjectNumber;
	DynamicObject& po = mDynamicObjects[preTakeTurn];
	StaticObject* s = mStaticObjs[po.mPosi];
	if (po.hasPressedBuyButton() && s->isBuyable()) {
		if (s->checkFlag(StaticObject::FLAG_ESTATE))
		{// 地产
			Estate* es = dynamic_cast<Estate*> (s);
			if (es->getBelonging() == 0xffff) {
				// 地产是无主的
				if (po.mMoney >= es->getPurPrice(0)) {
					po.mMoney -= es->getPurPrice(0);
					es->setBelonging(po.getID());
				}
				else {
					// TODO
					// 无钱购买
				}
			}
			else if (es->getBelonging() == po.getID() && isUpgradable(po.mPosi)) {
				// 地产是自己的且满足升级条件
				if (po.mMoney >= es->getPurPrice(es->getState())) {
					po.mMoney -= es->getPurPrice(es->getState());
					es->setState(es->getState() + 1);
				}
				else {
					// TODO
					// 无钱升级
				}
			}
		}else if (s->checkFlag(StaticObject::FLAG_AIRPORT)) {
			// 机场
			if (s->getBelonging() == 0xffff) {
				if (po.mMoney >= s->price) {
					po.mMoney -= s->price;
					s->setBelonging(po.getID());
				}
			}
		}else if (s->checkFlag(StaticObject::FLAG_WATERCOMP)) {
			// 水电公司
			if (s->getBelonging() == 0xffff) {
				if (po.mMoney >= s->price) {
					po.mMoney -= s->price;
					s->setBelonging(po.getID());
				}
			}
		}
		
	
	}
	
}



