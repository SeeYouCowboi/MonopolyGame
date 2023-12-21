#ifndef INCLUDED_GAME_STATE_H
#define INCLUDED_GAME_STATE_H
#define STATICOBJ_NUM 40
const int  FRONT_COLOR = 0xffd05299;


#include <vector> 

class Image; //只需要声明。不必包含它。
class StaticObject;
class DynamicObject;

class State{
public:
	//给出阶段编号并构建。0是两个人
	static const int STAGE_ID_2PLAYERS = 0;
	State( const char* stageData, int size, int sid);
	~State();
	bool isUpgradable(int posi) const;
	void update();
	void draw() const;	
	//从外部获取的信息
private:

	//运动物体
	DynamicObject* mDynamicObjects;
	int mDynamicObjectNumber;

	int mStageID;
	Image* mStaBelongImage;
	Image* mStaUpgradeImage;
	Image* mObjImage; 
	Image* mMapImage; //图片
	std::vector<StaticObject*> mStaticObjs;
	int mStageDataSize;
	int mTakeTurn;
	int mTurnState;
	int getAirportNum(int);
	int getWaterNum(int);

	char buffer[15][50];
	void pushToBuffer(char* str);
};

#endif
