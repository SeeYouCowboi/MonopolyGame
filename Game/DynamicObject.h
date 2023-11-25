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
	void set( int x, int y, Type );
	void draw( const Image* ) const;
	//移动
	void move( const int* wallsX, int* wallsY, int wallNumber );
	//查询哪个网格中心。
	void getCell( int* x, int* y ) const;
	//返回在数组中被dx和dy移动时重叠的质量坐标。
	//参数是两个int [4]。返回值是输入的数字。1,2,4之一。

	//便利函数
	bool isPlayer() const;
	void die(); //死亡（将mType设置为NONE）
	bool isDead() const; //死了吗

	//不要将直接修改的内容字段设为私有。因为会经常从State使用它。
	Type mType;
	//玩家专用
private:
	//获取当前帧的移动量
	void getVelocity( int* dx, int* dy ) const;
	//取得方向
	void getDirection( int* dx, int* dy ) const;
	//坐标（等同于目前位于的StaticObject对应的下标）
	int mPosi; 
	int mMoney;
	unsigned mPlayerID;
};

#endif
