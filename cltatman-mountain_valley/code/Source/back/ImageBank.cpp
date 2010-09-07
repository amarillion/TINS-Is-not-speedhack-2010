#include <ImageBank.hpp>

using namespace ol;


ImageBank* ImageBank::instance = NULL;


ImageBank::ImageBank()
{
  images[ "load failed" ] = new Bitmap( 50,50, Rgba::GREEN );
}


ImageBank* ImageBank::_()
{
  return instance ? instance : instance = new ImageBank();
}


bool ImageBank::load( const std::string& filepath )
{
  if( images.find( filepath ) != images.end() )
    return true;

  Bitmap* temp = new Bitmap( filepath );

  if( !temp->IsValid() )
    return false;

  images[ filepath ] = temp;

  return true;
}


Bitmap* ImageBank::get( const std::string& filepath )
{
  return load( filepath ) ? images[ filepath ] : images[ "load failed" ];
}


void ImageBank::flush()
{
  images.clear();
}


void ImageBank::set( const std::string& name, ol::Bitmap* image )
{
  images[ name ] = image;
}
