#include "Image.h"
#include "Vector2.h"

#include "GameLib/Framework.h"
using namespace GameLib;

Image::Image( const char* filename ) : 
mWidth( 0 ),
mHeight( 0 ),
mTextureWidth( 0 ),
mTextureHeight( 0 ),
mTexture( 0 ){
	Framework::instance().createTexture(
		&mTexture,
		filename );
	//获取大小。尽管在本文中没有解释，但是提供了这样的功能。
	Framework::instance().getTextureSizes(
		mTexture,
		&mTextureWidth,
		&mTextureHeight,
		&mWidth,
		&mHeight );
}


Image::~Image(){
	Framework::instance().destroyTexture( &mTexture );
}

int Image::width() const {
	return mWidth;
}

int Image::height() const {
	return mHeight;
}

//与Alpha混合
void Image::draw(
int dstX, 
int dstY, 
int srcX, 
int srcY, 
int width, 
int height ) const {
	//计算x，y范围
	double x0 = static_cast< double >( dstX );
	double y0 = static_cast< double >( dstY );
	double x1 = x0 + static_cast< double >( width );
	double y1 = y0 + static_cast< double >( height );
	//移动后的顶点
	Vector2 p0( x0, y0 );
	Vector2 p1( x1, y0 );
	Vector2 p2( x0, y1 );
	Vector2 p3( x1, y1 );
	//纹理坐标生成
	double rcpTw = 1.0 / static_cast< double >( mTextureWidth );
	double rcpTh = 1.0 / static_cast< double >( mTextureHeight );
	double u0 = static_cast< double >( srcX ) * rcpTw;
	double u1 = static_cast< double >( srcX + width ) * rcpTw;
	double v0 = static_cast< double >( srcY ) * rcpTh;
	double v1 = static_cast< double >( srcY + height ) * rcpTh;
	Vector2 t0( u0, v0 );
	Vector2 t1( u1, v0 );
	Vector2 t2( u0, v1 );
	Vector2 t3( u1, v1 );
	
	Framework f = Framework::instance();
	//纹理集
	f.setTexture( mTexture );
	//线性合成
	f.setBlendMode( Framework::BLEND_LINEAR );
	//绘制
	//复制到3D排列（因为叫drawTriangle 3D）
	double p0_3[ 3 ];
	double p1_3[ 3 ];
	double p2_3[ 3 ];
	double p3_3[ 3 ];
	p0_3[ 0 ] = p0.x; p0_3[ 1 ] = p0.y; p0_3[ 2 ] = 0.0;
	p1_3[ 0 ] = p1.x; p1_3[ 1 ] = p1.y; p1_3[ 2 ] = 0.0;
	p2_3[ 0 ] = p2.x; p2_3[ 1 ] = p2.y; p2_3[ 2 ] = 0.0;
	p3_3[ 0 ] = p3.x; p3_3[ 1 ] = p3.y; p3_3[ 2 ] = 0.0;

	f.drawTriangle3D( p0_3, p1_3, p2_3, &t0.x, &t1.x, &t2.x );
	f.drawTriangle3D( p3_3, p1_3, p2_3, &t3.x, &t1.x, &t2.x );
}

void Image::draw() const {
	draw( 0, 0, 0, 0, mWidth, mHeight );
}

