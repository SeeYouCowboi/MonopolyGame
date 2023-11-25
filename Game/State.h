#ifndef INCLUDED_GAME_STATE_H
#define INCLUDED_GAME_STATE_H


#include <vector> 

class Image; //只需要声明。不必包含它。
class StaticObject;
class DynamicObject;

class State{
public:
	//给出阶段编号并构建。0是两个人
	static const int STAGE_ID_2PLAYERS = 0;
	State( const char* stageData, int size);
	~State();
	void update();
	void draw() const;
	//从外部获取的信息
private:

	//运动物体
	DynamicObject* mDynamicObjects;
	int mDynamicObjectNumber;


	Image* mObjImage; 
	Image* mMapImage; //图片
	std::vector<StaticObject*> mStaticObjs;
	int mStageDataSize;
};

#endif
