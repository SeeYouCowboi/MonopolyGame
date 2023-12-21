#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H
#define NO_BELONGING 0xffff
#include <string>

class Image;
class DynamicObject;

class StaticObject{
public:
	enum Flag {
		FLAG_ESTATE,
		FLAG_TREASURE, //宝藏点
		FLAG_CHANCE, //机会卡
		FLAG_START, //起点
		FLAG_VACATION, //休假点
		FLAG_TOPRISON, //送监点
		FLAG_PRISON, //监狱
		FLAG_TAX, //收税点
		FLAG_AIRPORT,
		FLAG_WATERCOMP,
		FLAG_NONE,
	};
	StaticObject();
	virtual ~StaticObject();

	bool checkFlag( Flag ) const;
	bool isBuyable() const;
	void setFlag( Flag );
	void setID(unsigned);
	virtual void draw(const Image*, const Image*) const;
	unsigned getBelonging()const;
	void setBelonging(unsigned);
	unsigned getPosi() const;
	void setPosi(unsigned);
	unsigned price;

protected:
	unsigned mPosi;
	unsigned mBelonging; //无主的和系统的地块该值设为0xffff


private:
	Flag mFlag; //标志
	unsigned mID;
};

class Estate : public StaticObject {
public:
	std::string getCountry()const;
	std::string getCity()const;
	unsigned getPurPrice( unsigned )const;
	unsigned getTollPrice( unsigned )const;
	unsigned getState()const;
	void setState(unsigned);
	void setPurPrice(int, unsigned);
	void setTollPrice(int, unsigned);
	void draw (const Image*,const Image*) const override;

	Estate(unsigned, std::string, std::string);
private:
	std::string mCountry;
	std::string mCity;
	unsigned int mPurPrice[3];
	unsigned int mTollPrice[6];
	unsigned mState;
};

#endif
