#include "Matrix23.h"
#include "Vector2.h"
#include "GameLib/Math.h"
using namespace GameLib;

Matrix23::Matrix23(){
}

void Matrix23::setTranslation( const Vector2& a ){
	m00 = m11 = 1.0;
	m01 = m10 = 0.0;
	m02 = a.x;
	m12 = a.y;
}

void Matrix23::setRotation( double r ){
	double s = sin( r );
	double c = cos( r );
	m00 = m11 = c;
	m01 = s;
	m10 = -s;
	m02 = m12 = 0.0;
}

void Matrix23::setScaling( const Vector2& a ){
	m00 = a.x;
	m11 = a.y;
	m01 = m10 = m02 = m12 = 0.0;
}

void Matrix23::operator*=( const Matrix23& m ){
	double tx, ty;
	tx = m00; ty = m01;
	m00 = tx * m.m00 + ty * m.m10;
	m01 = tx * m.m01 + ty * m.m11;
	m02 = tx * m.m02 + ty * m.m12 + m02;
	tx = m10; ty = m11;
	m10 = tx * m.m00 + ty * m.m10;
	m11 = tx * m.m01 + ty * m.m11;
	m12 = tx * m.m02 + ty * m.m12 + m12;
}

void Matrix23::multiply( Vector2* out, const Vector2& in ) const {
	double tx = in.x; //out和in可能相同，因此备份
	out->x = m00 * tx + m01 * in.y + m02; //左上方* x +中间上方* y +右上方* 1
	out->y = m10 * tx + m11 * in.y + m12; //左下* x +中间下* y +左下* 1
}

//和移动矩阵相乘的做法


void Matrix23::translate( const Vector2& a ){
	m02 += m00 * a.x + m01 * a.y; 
	m12 += m10 * a.x + m11 * a.y;
}

//和缩放矩阵相乘的做法

void Matrix23::scale( const Vector2& a ){
	m00 *= a.x;
	m01 *= a.y;
	m10 *= a.x;
	m11 *= a.y;
}

//和旋转旋转矩阵相乘的做法

void Matrix23::rotate( double r ){
	double c = cos( r );
	double s = sin( r );
	//m00需要备份。因为第一行覆盖了m00。
	double t = m00;
	m00 = t * c + m01 * s;
	m01 = t * ( -s ) + m01 * c;
	t = m10; //m10备份
	m10 = t * c + m11 * s;
	m11 = t * ( -s ) + m11 * c;
}
