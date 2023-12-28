#include "GameLib/Framework.h"

#include "Game/State.h"
#include "Game/StaticObject.h"
#include "Game/DynamicObject.h"
#include "Game/StaticObjFactory.h"
#include "SoundManager.h"
#include "Image.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <cstdio>  // 包含头文件以使用 sprintf_s

using namespace GameLib;

namespace {
	const int MAX_ROWS = 15;
	const int MAX_COLS = 50;

	int currentRow = 0;
}


State::State( const char* stageData, int size, int sid) :
mObjImage( 0 ),
mMapImage( 0 ),
mStageID( sid ),
mDynamicObjects( 0 ),
mDynamicObjectNumber( 0 ),
mStaticObjs(0),
mTakeTurn(0),
mTurnState(0),
mStageDataSize(size),
buffer() {
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
			mStaticObjs.push_back(StaticObjectFactory::create(c, stageDataStream, iid));
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

	for (int i = 0; i < 15; i++) {
		buffer[i][0] = '\0';
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
	char tempbuff[50];
	// 绘制DynamicObjects
	for (int i = 0; i < mDynamicObjectNumber; i++) {
		mDynamicObjects[i].draw(mObjImage);
		sprintf_s(tempbuff, "Playe %d bank: %d\$", mDynamicObjects[i].getID(), mDynamicObjects[i].mMoney);
		f.drawDebugString(14, 5 + i, tempbuff);
	}
	// 绘制StaticObjects
	for (int i = 0; i < mStaticObjs.size(); i++) {
		mStaticObjs[i]->draw(mStaBelongImage, mStaUpgradeImage);
	}
	// 绘制系统消息
	for (int i = (currentRow-1+MAX_ROWS)%MAX_ROWS, j=0; i != currentRow; i = (i-1 + MAX_ROWS) % MAX_ROWS)
	{
		if (buffer[i][0] == '\0')break;
		sprintf_s(tempbuff, buffer[i]);
		f.drawDebugString(16, 12 + j, tempbuff, FRONT_COLOR);
		j++;
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


int State::getAirportNum(int playerID)
{
	int num = 0;
	for (int i = 0; i < mStaticObjs.size(); i++) {
		if (mStaticObjs[i]->checkFlag(StaticObject::FLAG_AIRPORT) && mStaticObjs[i]->getBelonging() == playerID) {
			num++;
		}
	}
	return num;
}

int State::getWaterNum(int playerID)
{
	int num = 0;
	for (int i = 0; i < mStaticObjs.size(); i++) {
		if (mStaticObjs[i]->checkFlag(StaticObject::FLAG_WATERCOMP) && mStaticObjs[i]->getBelonging() == playerID) {
			num++;
		}
	}
	return num;
}

void State::update(){
	char tempbuff[200];
	DynamicObject& o = mDynamicObjects[mTakeTurn];
	// 回合暂停判断
	if (o.inPrison) {
		sprintf_s(tempbuff, "Player %d still in prison, stop for a round =. =", o.getID());
		pushToBuffer(tempbuff);
		o.inPrison = false;
		mTakeTurn = (mTakeTurn + 1) % mDynamicObjectNumber;
		DynamicObject& o = mDynamicObjects[mTakeTurn];
	}

	if (o.inVaccation) {
		sprintf_s(tempbuff, "Player %d still in vacation, stop for a round =. =", o.getID());
		pushToBuffer(tempbuff);
		o.inVaccation = false;
		mTakeTurn = (mTakeTurn + 1) % mDynamicObjectNumber;
		DynamicObject& o = mDynamicObjects[mTakeTurn];
	}


	if (o.hasPressedRollButton() && !o.inPrison && !o.inVaccation && !o.pass) {
		o.mPosi = (o.mPosi + 1) % 40;
		//o.mPosi = (o.mPosi + ((rand()%5)+(rand()%5) + 1)) % 40;
		// 收过路费
		StaticObject* s = mStaticObjs[o.mPosi];
		if (s->getBelonging() != NO_BELONGING) {
			if (s->checkFlag(StaticObject::FLAG_ESTATE)) {
				Estate* es = dynamic_cast<Estate*> (s);
				if (es->getBelonging() != o.getID()) {
					o.mMoney -= es->getTollPrice(es->getState());
					mDynamicObjects[es->getBelonging()].mMoney += es->getTollPrice(es->getState());
					sprintf_s(tempbuff, "Player %d pay %d$ to player %d for toll!", o.getID(), es->getTollPrice(es->getState()), es->getBelonging());
					pushToBuffer(tempbuff);
				}
			}
			else if (s->checkFlag(StaticObject::FLAG_AIRPORT)) {
				if (s->getBelonging() != o.getID()) {
					int n = getAirportNum(s->getBelonging());
					int toll = 0;
					switch (n)
					{
					case 1:
						toll = (rand() % 4 + 1) * 35;
					case 2:
						toll = (rand()%4 + rand()%4 + 2) * 45;
					case 3:
						toll = (rand()%5 + rand()%5 + rand()%4 + 3) * 55;
					default:
						ASSERT("Bakana!");
						break;
					}
					o.mMoney -= toll;
					mDynamicObjects[s->getBelonging()].mMoney += toll;
					sprintf(tempbuff, "Player %d pay %d$ to player %d for Airport construction fee =w=", o.getID(), toll, s->getBelonging());
					pushToBuffer(tempbuff);
				}
			}
			else if (s->checkFlag(StaticObject::FLAG_WATERCOMP)) {
				if (s->getBelonging() != o.getID()) {
					int n = getWaterNum(s->getBelonging());
					int toll = 0;
					switch (n)
					{
					case 1:
						toll = (rand() % 3 + rand()% 3) * 35;
					case 2:
						toll = (rand() % 4 + rand() % 4 + rand()%4) * 40;
					case 3:
						toll = (rand() % 6 + rand() % 6 + rand()%6) * 50;
					default:
						ASSERT("Bakana!");
						break;
					}
					o.mMoney -= toll;
					mDynamicObjects[s->getBelonging()].mMoney += toll;
					sprintf(tempbuff, "Player %d pay %d$ to player %d for his water charge", o.getID(), toll, s->getBelonging());
					pushToBuffer(tempbuff);
				}
			}
		}
		
		// 中立地块判断
		if (s->checkFlag(StaticObject::FLAG_TREASURE)) {
			int treasure = rand() % 400 + 200;
			o.mMoney += treasure;
			sprintf_s(tempbuff, "Player %d get a treasure! That's %d$!!!", o.getID(), treasure);
			pushToBuffer(tempbuff);
		}
		else if (s->checkFlag(StaticObject::FLAG_TAX)) {
			o.mMoney -= 250;
			sprintf_s(tempbuff, "Player %d pay 250$ for tax :-(", o.getID());
			pushToBuffer(tempbuff);
		}
		else if (s->checkFlag(StaticObject::FLAG_TOPRISON)) {
			o.mPosi = 10;
			o.inPrison = 1;
			sprintf_s(tempbuff,"Oops, player %d is sent to prison!", o.getID());
			pushToBuffer(tempbuff);
		}
		else if (s->checkFlag(StaticObject::FLAG_VACATION)) {
			o.inVaccation = true;
			o.mMoney -= 100;
			sprintf_s(tempbuff, "Player %d pays 100$ for a vacation =3=", o.getID());
			pushToBuffer(tempbuff);
		}
		else if (s->checkFlag(StaticObject::FLAG_START)) {
			o.mMoney += 400;
			sprintf_s(tempbuff, "Player %d get 400$ for passing the start!", o.getID());
			pushToBuffer(tempbuff);
		}
		else if (s->checkFlag(StaticObject::FLAG_CHANCE)) {
			int tempdice = rand() % 4;
			switch (tempdice) {
			case 0:
				o.mMoney -= 200;
				o.inVaccation = true;
				sprintf(tempbuff, "The player twisted their foot while mountain climbing, incurring an expense of 200 dollars and taking a one-turn break.");
				pushToBuffer(tempbuff);
				break;
			case 1:
				o.mMoney += 300;
				sprintf(tempbuff, "The player acted bravely in a just case,received praise from the police officer, and was awarded a bonus of 300 dollars.");
				pushToBuffer(tempbuff);
				break;
			case 2:
				o.mPosi = rand() % 39 + 1;
				sprintf(tempbuff,"The player was abducted by aliens for experimentation. Upon waking up, he found himself at location %d.",o.mPosi);
				pushToBuffer(tempbuff);
				break;
			case 3:
				int lot = (rand() % 4 + 2) * 100;
				o.mMoney += lot;
				sprintf(tempbuff, "Oh wow, the player won the lottery and received a grand prize of %d dollars!",lot);
				pushToBuffer(tempbuff);
				break;
			}
		}

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
					sprintf_s(tempbuff, "OMG, player %d has bought %s in %s!!",
								po.getID(), es->getCity().c_str(), es->getCountry().c_str());
					pushToBuffer(tempbuff);
				}
				else {
					// 无钱购买
					sprintf_s(tempbuff, "Sorry, you don't have enough money to buy this QwQ");
					pushToBuffer(tempbuff);
				}
			}
			else if (es->getBelonging() == po.getID() && isUpgradable(po.mPosi)) {
				// 地产是自己的且满足升级条件
				if (po.mMoney >= es->getPurPrice(es->getState())) {
					po.mMoney -= es->getPurPrice(es->getState());
					es->setState(es->getState() + 1);
				}
				else {
					// 无钱升级
					sprintf_s(tempbuff, "Sorry, you don't have enough money to upgrade this QwQ");
					pushToBuffer(tempbuff);
				}
			}
		}else if (s->checkFlag(StaticObject::FLAG_AIRPORT)) {
			// 机场
			if (s->getBelonging() == 0xffff) {
				if (po.mMoney >= s->price) {
					po.mMoney -= s->price;
					s->setBelonging(po.getID());
					sprintf_s(tempbuff, "Oh my dear lord, player %d has bought an fucking  airport!!",po.getID());
					pushToBuffer(tempbuff);
				}
			}
		}else if (s->checkFlag(StaticObject::FLAG_WATERCOMP)) {
			// 水电公司
			if (s->getBelonging() == 0xffff) {
				if (po.mMoney >= s->price) {
					po.mMoney -= s->price;
					s->setBelonging(po.getID());
					sprintf_s(tempbuff, "Jeasus, player %d has bought an freaking WaterFactory!!", po.getID());
				}
			}
		}
		
	
	}

}

void State::pushToBuffer(char* str1) {
	int strLength = strlen(str1);

	// 处理超过50字符的情况
	if (strLength >= MAX_COLS) {
		// 先将剩余部分递归调用pushToBuffer，处理下一行
		pushToBuffer(str1 + MAX_COLS - 1);
		// 将前50个字符放入当前行
		strncpy(buffer[currentRow], str1, MAX_COLS-1);
		buffer[currentRow][MAX_COLS-1] = '\0';  // 确保字符串结尾
		// 切换到下一行
		currentRow = (currentRow + 1) % MAX_ROWS;
	}
	else {
		// 将字符串放入当前行
		strncpy(buffer[currentRow], str1, MAX_COLS-1);
		buffer[currentRow][MAX_COLS-1] = '\0';  // 确保字符串结尾
		currentRow = (currentRow + 1) % MAX_ROWS;
	}
}



