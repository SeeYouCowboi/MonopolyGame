#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Sequence/Title.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"

namespace Sequence{

Title::Title() : mImage( 0 ), mCursorImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( "data/image/MoTitle.tga" );
	mCursorImage = new Image( "data/image/cursor.tga" );
}

Title::~Title(){
	SAFE_DELETE( mImage );
	SAFE_DELETE( mCursorImage );
}

void Title::update( Parent* parent ){
	//获取输入
	if ( Pad::isTriggered( Pad::U ) ){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ //
 			mCursorPosition = 2;
		}
		//光标声音
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 2 ){ //1越过0就循环成0
			mCursorPosition = 0;
		}
		//光标声音
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::A ) ){
		parent->moveTo( Parent::NEXT_GAME );
		if ( mCursorPosition == 0 ){
			parent->setMode( Parent::MODE_4P );
		}else if ( mCursorPosition == 1 ){
			parent->setMode( Parent::MODE_5P );
		}
		else if (mCursorPosition == 1) {
			parent->setMode(Parent::MODE_6P);
		}else{
			HALT( "arienai" );
		}
		//决定音
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	//绘制
	mImage->draw();
	//光标
	mCursorImage->draw( 224, 354 + mCursorPosition * 38, 0, 0, 32, 32 );
}

} //namespace Sequence