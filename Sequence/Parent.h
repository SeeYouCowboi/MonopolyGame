#ifndef INCLUDED_SEQUENCE_PARENT_H
#define INCLUDED_SEQUENCE_PARENT_H

namespace Sequence{
class Child;

class Parent{
public:
	enum NextSequence{
		NEXT_TITLE,
		NEXT_GAME,
		NEXT_GAME_OVER,
		NEXT_ENDING,

		NEXT_NONE,
	};
	enum Mode{
		MODE_4P,
		MODE_5P,
		MODE_6P,
		MODE_NONE,
	};
	void update();
	void moveTo( NextSequence );
	Mode getMode() const;
	void setMode( Mode );

	static void create();
	static void destroy();
	static Parent* instance();
private:
	Parent();
	~Parent();

	NextSequence mNextSequence;
	Mode mMode;

	Child* mChild;

	static Parent* mInstance;
};

} //namespace Sequence

#endif
