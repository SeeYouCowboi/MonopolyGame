#include "GameLib/Framework.h"

#include "Game/State.h"
#include "Game/StaticObject.h"
#include "Game/DynamicObject.h"
#include "SoundManager.h"
#include "Image.h"
#include <iostream>
#include <cstring>
#include <sstream>

using namespace GameLib;


State::State( const char* stageData, int size) :
mObjImage( 0 ),
mMapImage( 0 ),
mDynamicObjects( 0 ),
mDynamicObjectNumber( 0 ),
mStaticObjs(0),
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
	

}

State::~State(){
	SAFE_DELETE( mObjImage );
	SAFE_DELETE( mMapImage );
	SAFE_DELETE_ARRAY( mDynamicObjects );
}

void State::draw() const {

	// TODO
	// 绘制StaticObjects
	
	// 绘制DynamicObjects
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		mDynamicObjects[ i ].draw( mObjImage );
	}
	// 绘制背景
	mMapImage->draw();
}

void State::update(){
	//todo
	//炸弹处理
	//for ( int y = 0; y < height; ++y ){
	//	for ( int x = 0; x < width; ++x ){
	//		staticobject& o = mstaticobjects( x, y );
	//		if ( o.checkflag( staticobject::flag_bomb ) ){
	//			1.更新炸弹计数
	//			++o.mcount;
	//			2.爆破开始、结束判定
	//			if ( o.checkflag( staticobject::flag_exploding ) ){ //灭火判断
	//				if ( o.mcount == explosion_life ){ //爆炸结束时间
	//					o.resetflag( staticobject::flag_exploding | staticobject::flag_bomb );
	//					o.mcount = 0;
	//				}
	//			}else{ //爆炸判断
	//				if ( o.mcount == explosion_time ){ //到了爆炸时刻
	//					o.setflag( staticobject::flag_exploding );
	//					o.mcount = 0;
	//					soundmanager::instance()->playse( soundmanager::se_explosion );
	//				}else if ( o.checkflag( staticobject::flag_fire_x | staticobject::flag_fire_y ) ){ //爆炸
	//					o.setflag( staticobject::flag_exploding );
	//					o.mcount = 0;
	//					soundmanager::instance()->playse( soundmanager::se_explosion );
	//				}
	//			}
	//		}else if ( o.checkflag( staticobject::flag_brick ) ){ //对于砖，需要判断烧尽
	//			if ( o.checkflag( staticobject::flag_fire_x | staticobject::flag_fire_y ) ){ //
	//				++o.mcount; //
	//				if ( o.mcount == explosion_life ){
	//					o.mcount = 0;
	//					o.resetflag( staticobject::flag_brick ); //烧毁了
	//				}
	//			}
	//		}
	//		3.因为爆炸气浪每架重新放置，所以关掉一次。
	//		o.resetflag( staticobject::flag_fire_x | staticobject::flag_fire_y );
	//	}
	//}
	//火焰设定
	//for ( int y = 0; y < height; ++y ){
	//	for ( int x = 0; x < width; ++x ){
	//		if ( mstaticobjects( x, y ).checkflag( staticobject::flag_exploding ) ){
	//			setfire( x, y );
	//		}
	//	}
	//}

	//计算1p、2p的设置炸弹数量
	//int bombnumber[ 2 ];
	//bombnumber[ 0 ] = bombnumber[ 1 ] = 0;
	//for ( int y = 0; y < height; ++y ){
	//	for ( int x = 0; x < width; ++x ){
	//		const staticobject& o = mstaticobjects( x, y );
	//		if ( o.checkflag( staticobject::flag_bomb ) ){
	//			++bombnumber[ o.mbombowner->mplayerid ];
	//		}
	//	}
	//}
	//与动态对象循环
	//for ( int i = 0; i < mdynamicobjectnumber; ++i ){
	//	dynamicobject& o = mdynamicobjects[ i ];
	//	if ( o.isdead() ){ //死了 结束
	//		continue;
	//	}
	//	检查它是否与放置的炸弹接触
	//	for ( int j = 0; j < 2; ++j ){
	//		if ( o.mlastbombx[ j ] >= 0 ){ //0以上的话里面有什么。
	//			if ( !o.isintersectwall( o.mlastbombx[ j ], o.mlastbomby[ j ] ) ){
	//				o.mlastbombx[ j ] = o.mlastbomby[ j ] = -1;
	//			}
	//		}
	//	}
	//	获取当前单元格
	//	int x, y;
	//	o.getcell( &x, &y );
	//	从以此为中心的单元中找到墙并将其存储在数组中
	//	int wallsx[ 9 ];
	//	int wallsy[ 9 ];
	//	int wallnumber = 0;
	//	for ( int i = 0; i < 3; ++i ){
	//		for ( int j = 0; j < 3; ++j ){
	//			int tx = x + i - 1;
	//			int ty = y + j - 1;
	//			const staticobject& so = mstaticobjects( tx, ty );
	//			if ( so.checkflag( staticobject::flag_wall | staticobject::flag_brick | staticobject::flag_bomb ) ){ //在墙上
	//				bool mybomb0 = ( o.mlastbombx[ 0 ] == tx ) && ( o.mlastbomby[ 0 ] == ty );
	//				bool mybomb1 = ( o.mlastbombx[ 1 ] == tx ) && ( o.mlastbomby[ 1 ] == ty );
	//				if ( !mybomb0 && !mybomb1 ){ //不是自己放的炸弹
	//					wallsx[ wallnumber ] = x + i - 1;
	//					wallsy[ wallnumber ] = y + j - 1;
	//					++wallnumber;
	//				}
	//			}
	//		}
	//	}
	//	
	//	o.move( wallsx, wallsy, wallnumber );
	//	通过判断与移​​动后的位置的9个周围网格的碰撞来进行各种反应
	//	for ( int i = 0; i < 3; ++i ){
	//		for ( int j = 0; j < 3; ++j ){
	//			staticobject& so = mstaticobjects( x + i - 1, y + j - 1 );
	//			if ( o.isintersectwall( x + i - 1, y + j - 1 ) ){ //接触了
	//				if ( so.checkflag( staticobject::flag_fire_x | staticobject::flag_fire_y ) ){
	//					o.die(); //烧了
	//				}else if ( !so.checkflag( staticobject::flag_brick ) ){ //如果发现项目
	//					if ( so.checkflag( staticobject::flag_item_power ) ){
	//						so.resetflag( staticobject::flag_item_power );
	//						++o.mbombpower;
	//					}else if ( so.checkflag( staticobject::flag_item_bomb ) ){
	//						so.resetflag( staticobject::flag_item_bomb );
	//						++o.mbombnumber;
	//					}
	//				}
	//			}
	//		}
	//	}
	//	移动后获取网格编号
	//	o.getcell( &x, &y );
	//	放炸弹
	//	if ( o.hasbombbuttonpressed() ){ //炸弹放置按钮已被按下
	//		if ( bombnumber[ o.mplayerid ] < o.mbombnumber ){ //低于炸弹最大值
	//			staticobject& so = mstaticobjects( x, y );
	//			if ( !so.checkflag( staticobject::flag_bomb ) ){ //没有炸弹
	//				so.setflag( staticobject::flag_bomb );
	//				so.mbombowner = &o;
	//				so.mcount = 0;

	//				更新炸弹位置
	//				if ( o.mlastbombx[ 0 ] < 0 ){
	//					o.mlastbombx[ 0 ] = x;
	//					o.mlastbomby[ 0 ] = y;
	//				}else{
	//					o.mlastbombx[ 1 ] = x;
	//					o.mlastbomby[ 1 ] = y;
	//				}
	//				soundmanager::instance()->playse( soundmanager::se_set_bomb );
	//			}
	//		}
	//	}
	//}
	//下一个 判断敌人与玩家之间的碰撞。
	//for ( int i = 0; i < mdynamicobjectnumber; ++i ){
	//	for ( int j = i + 1; j < mdynamicobjectnumber; ++j ){
	//		mdynamicobjects[ i ].docollisionreactiontodynamic( &mdynamicobjects[ j ] );
	//	}
	//}
}


//bool State::hasCleared() const {
//	//清除是否没有敌人
//	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
//		if ( mDynamicObjects[ i ].isEnemy() ){
//			return false;
//		}
//	}
//	return true;
//}
//
//bool State::isAlive( int playerID ) const {
//	//如果还活着
//	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
//		if ( mDynamicObjects[ i ].mType == DynamicObject::TYPE_PLAYER ){
//			if ( mDynamicObjects[ i ].mPlayerID == playerID ){
//				return true;
//			}
//		}
//	}
//	return false;
//}

