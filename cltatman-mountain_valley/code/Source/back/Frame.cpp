#include <Frame.hpp>
#include <ImageBank.hpp>
#include <global/FileIO.hpp>


using namespace std;


Frame::Frame( const string& imageFile, double time )
  : _imageFile( imageFile ), _time( time )
{
  setOpacity( 1.0 );
  setPlacement( 0,0,0 );
  setScale( 1.0,1.0 );
  setTint( 0.0,0.0,0.0,0.0 );

  ol::Bitmap* image = ImageBank::_()->get( imageFile );

  _width = image->Width();
  _height = image->Height();

  if( _width <= 0 )
    _width = 500;
  if( _height <= 0 )
    _height = 500;

  image = NULL;
}


void Frame::setPlacement( double x, double y, double rotation )
{
  _placement = ol::Placement( ol::Vec2D( x,y ), rotation );
}


void Frame::setScale( double x, double y )
{
  if( x < 0.0 )
    x = 0.0;

  if( y < 0.0 )
    y = 0.0;

  _scale = ol::Vec2D( x,y );
}


void Frame::setSize( double w, double h )
{
  ol::Bitmap* bitmap = ImageBank::_()->get( _imageFile );

  w /= (double)bitmap->Width();
  h /= (double)bitmap->Height();

  setScale( w,h );
}


void Frame::setTint( double r, double g, double b, double a )
{
  if( r < 0.0 ) r = 0.0; else if( r > 1.0 ) r = 1.0;
  if( g < 0.0 ) g = 0.0; else if( g > 1.0 ) g = 1.0;
  if( b < 0.0 ) b = 0.0; else if( b > 1.0 ) b = 1.0;
  if( a < 0.0 ) a = 0.0; else if( a > 1.0 ) a = 1.0;

  _tint = ol::Rgba( r,g,b,a );
}


void Frame::setOpacity( double value )
{
  if( value < 0.0 )
    value = 0.0;
  else if( value > 1.0 )
    value = 1.0;

  _opacity = value;
}


void Frame::draw()
{
  ol::Bitmap* bitmap = ImageBank::_()->get( _imageFile );

  double x = _placement.GetPosition().x,
         y = _placement.GetPosition().y,
         w = bitmap->Width()  * _scale.x,
         h = bitmap->Height() * _scale.y,
         rotation = _placement.GetRotation();

  bitmap->BlitTransformed( x,y, w,h, rotation, ol::Tinted( _tint ), _opacity );
}


void Frame::save( ofstream& file )
{
  file << "--Frame 0"                << endl
       << _imageFile                 << endl
       << _width                     << endl
       << _height                    << endl
       << _placement.GetPosition().x << endl
       << _placement.GetPosition().y << endl
       << _z                         << endl
       << _placement.GetRotation()   << endl
       << _scale.x                   << endl
       << _scale.y                   << endl
       << _tint.r                    << endl
       << _tint.g                    << endl
       << _tint.b                    << endl
       << _tint.a                    << endl
       << _time                      << endl
       << _opacity                   << endl;
}


void Frame::load( ifstream& file )
{
  if( !file.is_open() )
    return;
  if( getLine( file ) != "--Frame 0" )
    return;

  string imageFile = getLine( file );

  _width  = fromLine< double >( file );
  _height = fromLine< double >( file );

  double x =         fromLine< double >( file ),
         y =         fromLine< double >( file ),
         z =         fromLine< double >( file ),
         rotation =  fromLine< double >( file ),
         xS =        fromLine< double >( file ),
         yS =        fromLine< double >( file ),
         r =         fromLine< double >( file ),
         g =         fromLine< double >( file ),
         b =         fromLine< double >( file ),
         a =         fromLine< double >( file ),
         time =      fromLine< double >( file ),
         opacity =   fromLine< double >( file );

  _imageFile = imageFile;
  _time = time;

  setPlacement( x,y, rotation );
  setScale( xS,yS );
  setOpacity( opacity );
  setTint( r,g,b,a );
  _z = z;
}

