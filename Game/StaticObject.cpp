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

void StaticObject::setID(unsigned id) {
	mID = id;
}

void StaticObject::draw( int x, int y, const Image* image ) const {
	//int srcX = -1;
	//int srcY = -1;
	//bool floor = false;
	//if ( mFlags & FLAG_WALL ){
	//	srcX = 96; 
	//	srcY = 32;
	//}else if ( mFlags & FLAG_BRICK ){
	//	if ( mFlags & ( FLAG_FIRE_X | FLAG_FIRE_Y ) ){ //烧了
	//		srcX = 0; 
	//		srcY = 96;
	//	}else{
	//		srcX = 0; 
	//		srcY = 64;
	//	}
	//}else{
	//	srcX = 32; 
	//	srcY = 64; 
	//	floor = true;
	//}
	//image->draw( x*32, y*32, srcX, srcY, 32, 32 );
}


Estate::Estate(unsigned id, std::string cntry, std::string city) :
	mCountry(cntry),
	mCity(city),
	mBelonging(0),
	mState(0)
	{
	StaticObject::setFlag(FLAG_ESTATE);
	StaticObject::setID(id);
}

void Estate::setPurPrice(int i, unsigned price) {
	ASSERT(i < 3);
	mPurPrice[i] = price;
}
void Estate::setTollPrice(int i, unsigned price) {
	ASSERT(i < 6);
	mTollPrice[i] = price;
}

std::string Estate::getCountry() const {
	return mCountry;
}

std::string Estate::getCity() const {
	return mCity;
}

unsigned Estate::getPurPrice(unsigned i) const {
	ASSERT(i < 3);
	return mPurPrice[i];
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