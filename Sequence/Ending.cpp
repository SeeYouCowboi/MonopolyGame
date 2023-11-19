﻿#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Sequence/Ending.h"
#include "Sequence/Parent.h"
#include "Image.h"

namespace Sequence{

Ending::Ending() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/ending.tga" );
}

Ending::~Ending(){
	SAFE_DELETE( mImage );
}

void Ending::update( Parent* parent ){
	if ( mCount == 120 ){ //2等待秒
		parent->moveTo( Parent::NEXT_TITLE );
	}
	//绘制
	mImage->draw();

	++mCount;
}

} //namespace Sequence