#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Game/StaticObject.h"
#include "Image.h"

StaticObject::StaticObject() :
mFlag( FLAG_NONE ){
}

void StaticObject::setFlag( Flag f ){
	mFlag = f;
}

bool StaticObject::checkFlag( Flag f ) const {
	return ( mFlag == f ) ? true : false;
}

void StaticObject::draw( int x, int y, const Image* image ) const {
	int srcX = -1;
	int srcY = -1;
	bool floor = false;
	if ( mFlags & FLAG_WALL ){
		srcX = 96; 
		srcY = 32;
	}else if ( mFlags & FLAG_BRICK ){
		if ( mFlags & ( FLAG_FIRE_X | FLAG_FIRE_Y ) ){ //烧了
			srcX = 0; 
			srcY = 96;
		}else{
			srcX = 0; 
			srcY = 64;
		}
	}else{
		srcX = 32; 
		srcY = 64; 
		floor = true;
	}
	image->draw( x*32, y*32, srcX, srcY, 32, 32 );
	//可选绘制
	if ( floor ){
		srcX = -1; //
		if ( ( mFlags & FLAG_BOMB ) && !( mFlags & FLAG_EXPLODING ) ){
			srcX = 64;
			srcY = 64;
		}else if ( mFlags & FLAG_ITEM_BOMB ){
			srcX = 64;
			srcY = 0;
		}else if ( mFlags & FLAG_ITEM_POWER ){
			srcX = 96;
			srcY = 0;
		}
		if ( srcX != -1 ){
			image->draw( x*32, y*32, srcX, srcY, 32, 32 );
		}
	}
}

void StaticObject::drawExplosion( int x, int y, const Image* image ) const {
	int srcX = -1;
	int srcY = -1;
	if ( !( mFlags & FLAG_WALL ) && !( mFlags & FLAG_BRICK ) ){ //墙上不用绘制爆炸
		if ( mFlags & FLAG_EXPLODING ){
			srcX = 96;
			srcY = 64;
		}else if ( mFlags & FLAG_FIRE_X ){
			if ( mFlags & FLAG_FIRE_Y ){
				srcX = 96;
				srcY = 64;
			}else{
				srcX = 0;
				srcY = 32;
			}
		}else if ( mFlags & FLAG_FIRE_Y ){
			srcX = 32;
			srcY = 32;
		}
	}
	if ( srcX != -1 ){
		image->draw( x*32, y*32, srcX, srcY, 32, 32 );
	}
}


std::string Estate::getCountry() const {
	return mCountry;
}

std::string Estate::getCity() const {
	return mCity;
}

unsigned Estate::getPurPrice(unsigned i) const {
	ASSERT(i < 3);
	return mPurcPrice[i];
}

unsigned Estate::getTollPrice(unsigned i) const {
	ASSERT(i < 6);
	return mTollPrice[i];
}

unsigned Estate::getBelonging() const {
	return mBelonging;
}

unsigned Estate::getState() const {
	return mState;
}