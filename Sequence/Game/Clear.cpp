#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Clear::Clear() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/clear.tga" );
}

Clear::~Clear(){
	SAFE_DELETE( mImage );
}

void Clear::update( Parent* parent ){
	if ( mCount == 60 ){ //1等待秒
		if ( parent->hasFinalStageCleared() ){ //
			parent->moveTo( Parent::NEXT_ENDING ); 
		}else{
			parent->moveTo( Parent::NEXT_READY );
		}
	}
	//绘制
	//首先绘制游戏画面
	parent->drawState();
	//在顶部绘制庆祝消息
	mImage->draw();

	++mCount;
}

} //namespace Game
} //namespace Sequence
