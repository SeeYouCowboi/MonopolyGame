﻿#ifndef INCLUDED_SEQUENCE_GAME_PARENT_H
#define INCLUDED_SEQUENCE_GAME_PARENT_H

#include "Sequence/Child.h"
class State;

namespace Sequence{
class Parent;
namespace Game{
class Child;

class Parent : public Sequence::Child{
public:
	typedef Sequence::Parent GrandParent;
	enum NextSequence{
		NEXT_CLEAR,
		NEXT_READY,
		NEXT_PAUSE,
		NEXT_PLAY,
		NEXT_FAILURE,
		NEXT_JUDGE,
		NEXT_ENDING,
		NEXT_GAME_OVER,
		NEXT_TITLE,

		NEXT_NONE,
	};
	enum Mode{
		MODE_4P,
		MODE_5P,
		MODE_6P,
		MODE_NONE,
	};
	enum PlayerID{
		PLAYER_1,
		PLAYER_2,
		PLAYER_NONE,
	};
	Parent( GrandParent::Mode );
	~Parent();
	void update( GrandParent* );
	void moveTo( NextSequence );

	PlayerID getWinner() const;
	void setWinner( PlayerID );
	State* getState();
	void drawState() const; //游戏绘制
	bool hasFinalStageCleared() const; //
	int getLifeNumber() const;
	Mode getMode() const;
	void startLoading();
private:
	State* mState;
	int mStageID;
	int mLife;
	PlayerID mWinner; //
	static const int FINAL_STAGE = 2; //
	static const int INITIALI_LIFE_NUMBER = 2;

	NextSequence mNextSequence;

	Game::Child* mChild;
};

} //namespace Game
} //namespace Sequence

#endif