#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Game/StaticObject.h"
#include "Image.h"
#include <climits>

namespace {
	int convertPosiToPixelx(int x) {
		if (x >= 0 && x <= 10) {
			return x * 52 + 15;
		}
		else if (x >= 11 && x <= 19) {
			return 535;
		}
		else if (x >= 20 && x <= 30) {
			return 535 - (x - 20) * 52;
		}
		else if (x >= 31 && x <= 40) {
			return 6;
		}
		else HALT("Invalid");
	}

	int convertPosiToPixely(int y) {
		if (y >= 0 && y <= 10) {
			return 3;
		}
		else if (y >= 11 && y <= 20) {
			return  78 + (y - 11) * 52;
		}
		else if (y >= 21 && y <= 30) {
			return 554;
		}
		else if (y >= 31 && y <= 39) {
			return 554 - (y - 31) * 52;
		}
		else HALT("Invalid");
	}
}

StaticObject::StaticObject() :
mFlag( FLAG_NONE ),
mBelonging(NO_BELONGING),
mID( 0 ),
mPosi( 0 ),
price(150){
}

StaticObject::~StaticObject() {

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

bool StaticObject::isBuyable() const {
	if ((mFlag == FLAG_AIRPORT || mFlag == FLAG_ESTATE || mFlag == FLAG_WATERCOMP)) {
		return true;
	}
	else return false;
}

void StaticObject::setBelonging(unsigned b) {
	mBelonging = b;
}

unsigned StaticObject::getBelonging() const {
	return mBelonging;
}

unsigned StaticObject::getPosi() const {
	return mPosi;
}

void StaticObject::setPosi(unsigned p) {
	mPosi = p;
}



void StaticObject::draw( const Image* blImage, const Image* upImage ) const {
	if (mBelonging != 0xffff)
	{
		int srcX = (mBelonging % 4) * 32;
		int srcY = (mBelonging / 4) * 32;

		blImage->draw(convertPosiToPixelx(mPosi), convertPosiToPixely(mPosi), srcX, srcY, 32, 32);
	}
}


void Estate::draw(const Image* blImage, const Image* upImage) const {
	if (mBelonging != 0xffff)
	{
		int srcXbl = (mBelonging % 4) * 32;
		int srcYbl = (mBelonging / 4) * 32;
		blImage->draw(convertPosiToPixelx(mPosi), convertPosiToPixely(mPosi), srcXbl, srcYbl, 32, 32);

		int srcXup = (mState % 4) * 32;
		int srcYup = (mState % 4) * 32;
		upImage->draw(convertPosiToPixelx(mPosi), convertPosiToPixely(mPosi), srcXup, srcYup, 32, 32);
	}
}


Estate::Estate(unsigned id, std::string cntry, std::string city) :
	mCountry(cntry),
	mCity(city),
	mPurPrice(),
	mTollPrice(),
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
	if (i >= 3) return UINT32_MAX;
	else return mPurPrice[i];
}

unsigned Estate::getTollPrice(unsigned i) const {
	ASSERT(i < 6);
	return mTollPrice[i];
}



unsigned Estate::getState() const {
	return mState;
}

void Estate::setState(unsigned s) {
	mState = s;
}


