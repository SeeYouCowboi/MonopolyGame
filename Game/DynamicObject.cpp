#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Game/DynamicObject.h"
#include "Game/StaticObject.h"
#include "Image.h"
#include "Pad.h"

namespace {

//参数组
//速度 单位是内部单位/帧
static const int PLAYER_SPEED = 1000;
static const int ENEMY_SPEED = 500;
static const int HALF_SIZE = 6000;

//到内部单元
int convertCellToInner( int x ){
	return x * 16000 + 8000;
}
//从内部单位到像素单位
int convertInnerToPixel( int x ){
	return  ( x - 8000 + 500 ) / 1000;
}

int convertPosiToPixelx(int x) {
	if (x >= 0 && x <= 10) {
		return x * 52 + 15;
	}
	else if (x >= 11 && x <= 20) {
		return 562;
	}
	else if (x >= 21 && x <= 30) {
		return 483 - (x - 21) * 52;
	}
	else if (x >= 31 && x <= 39) {
		return 6;
	}
	else HALT("Invalid");
}

int convertPosiToPixely(int y) {
	if (y >= 0 && y <= 10) {
		return 17;
	}
	else if (y >= 11 && y <= 20) {
		return 64 + (y-11)*52;
	}
	else if (y >= 21 && y <= 30) {
		return 552;
	}
	else if (y >= 31 && y <= 39) {
		return 480 - (y-31)*52;
	}
	else HALT("Invalid");
}

} //namespace{}

//尽可能输入错误的值。如果不调用set就会中断
DynamicObject::DynamicObject() : 
mType( TYPE_NONE ),
mPosi( 0 ),
mPlayerID( 0 ),
mMoney( -1 ){
}

DynamicObject::DynamicObject(Type mType, int mPosi, int mMoney, unsigned mPlayerID):
	mType(mType),
	mPosi(mPosi),
	mPlayerID(mPlayerID),
	mMoney(mMoney) {
}

bool DynamicObject::hasPressedRollButton() {
	if (mType == TYPE_PLAYER) {
		return Pad::isTriggered(Pad::A);
	}
	else {
		return false;
	}
}

bool DynamicObject::hasPressedBuyButton() {
	if (mType == TYPE_PLAYER) {
		return Pad::isTriggered(Pad::B);
	}
	else {
		return false;
	}
}

void DynamicObject::set( int x, int y, Type type ){
	////转换为内部坐标
	//mX = convertCellToInner( x );
	//mY = convertCellToInner( y );
	//mType = type;
	////仅限敌人。移动方向初始化
	//if ( mType == TYPE_ENEMY ){
	//	mDirectionX = mDirectionY = 0;
	//	Framework f = Framework::instance();
	//	switch ( f.getRandom( 4 ) ){
	//		case 0 : mDirectionX = 1; break;
	//		case 1 : mDirectionX = -1; break;
	//		case 2 : mDirectionY = 1; break;
	//		case 3 : mDirectionY = -1; break;
	//	}
	//}
}

void DynamicObject::draw( const Image* image ) const {
	//将内部坐标转换为像素坐标（+500四舍五入）
	int dstX = convertPosiToPixelx( mPosi );
	int dstY = convertPosiToPixely( mPosi );
	//
	int srcX, srcY;
	srcX = srcY = -1;
	switch ( mType ){
		case TYPE_PLAYER:
			switch ( mPlayerID ){
				case 0: srcX = 0; srcY = 0; break;
				case 1: srcX = 32; srcY = 0; break;
				case 2: srcX = 64; srcY = 0; break;
				case 3: srcX = 96; srcY = 0; break;
				case 4: srcX = 0; srcY = 32; break;
				case 5: srcX = 32; srcY = 32; break;
			}
			break;
		default: HALT( "arienai" ); break;
	}
	image->draw( dstX , dstY , srcX, srcY, 32, 32 ); //由于分辨率提高了一倍，因此此处的位置翻倍
}




bool DynamicObject::isPlayer() const {
	return ( mType == TYPE_PLAYER );
}

int DynamicObject::getID() const {
	return mPlayerID;
}

//bool DynamicObject::isEnemy() const {
//	return ( mType == TYPE_ENEMY );
//}

void DynamicObject::die(){
	mType = TYPE_NONE;
}

bool DynamicObject::isDead() const {
	return ( mType == TYPE_NONE );
}
