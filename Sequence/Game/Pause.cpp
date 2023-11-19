#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Pause::Pause() : mImage( 0 ), mCursorImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( "data/image/pause.tga" );
	mCursorImage = new Image( "data/image/cursor.tga" );
}

Pause::~Pause(){
	SAFE_DELETE( mImage );
	SAFE_DELETE( mCursorImage );
}

void Pause::update( Parent* parent ){
	//0：重做
	//1：面对面硒
	//2:进入标题画面
	//3：返回
	if ( Pad::isTriggered( Pad::U) ){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ //
 			mCursorPosition = 1;
		}
		//光标声音
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 1 ){ //1越过0就循环成0
			mCursorPosition = 0;
		}
		//光标声音
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosition == 0 ){ //返回游戏
			parent->moveTo( Parent::NEXT_PLAY );
		}else if ( mCursorPosition == 1 ){ //返回主题
			parent->moveTo( Parent::NEXT_TITLE );
		}
		//决定音
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	//绘制
	//首先是游戏画面
	parent->drawState();
	//
	mImage->draw();
	//画光标
	mCursorImage->draw( 180, 220 + mCursorPosition * 48, 0, 0, 32, 32 );
}



} //namespace Game
} //namespace Sequence