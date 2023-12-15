#ifndef INCLUDED_GAME_DYNAMIC_OBJECT_H
#define INCLUDED_GAME_DYNAMIC_OBJECT_H

class Image;
class StaticObject;

class DynamicObject{
public:
	enum Type{
		TYPE_PLAYER,
		TYPE_BOT,
		TYPE_NONE, //死了
	};
	DynamicObject();
	DynamicObject(Type mType, int mPosi, int mMoney, unsigned mPlayerID);
	void set( int x, int y, Type );
	bool hasPressedRollButton();
	bool hasPressedBuyButton();
	void draw( const Image* ) const;

	//便利函数
	bool isPlayer() const;
	int getID() const;
	void die(); //死亡（将mType设置为NONE）
	bool isDead() const; //死了吗

	//不要将直接修改的内容字段设为私有。因为会经常从State使用它。
	Type mType;
	int mMoney;
	int mPosi;
	//坐标（等同于目前位于的StaticObject对应的下标）
	
private:
	
	unsigned mPlayerID;
};

#endif
