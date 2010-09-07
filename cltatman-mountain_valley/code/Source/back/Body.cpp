#include <Body.hpp>
#include <Game.hpp>
#include <global/FileIO.hpp>


using namespace ol;
using namespace std;


Body::Body( double _x, double _y )
{
  x = _x;
  y = _y;
}


void Body::addPoint( double _x, double _y )
{
  points.push_back( ol::Vec2D( _x,_y ) );

  if( _x < left   ) left   = _x;
  if( _x > right  ) right  = _x;
  if( _y < top    ) top    = _y;
  if( _y > bottom ) bottom = _y;
}


void Body::insertPoint( double _x, double _y )
{
  double sumX = 0, sumY = 0;


  for( unsigned i = 0 ; i < points.size() ; i++ )
  {
    sumX += points[ i ].x;
    sumY += points[ i ].y;
  }

  sumX /= points.size();
  sumY /= points.size();

  ol::Vec2D centroid( sumX,sumY );
  ol::Vec2D point( _x,_y );

  double givenAngle = ( centroid - point ).GetAngle(),
         lesserAngle = 360;

  unsigned lesserPoint = 0xFFFFFFFF;

  for( unsigned i = 0; i < points.size() ; i++ )
  {
    double angle = ( centroid - points[i] ).GetAngle();

    if( angle < givenAngle && angle > lesserAngle )
    {
      lesserAngle = angle;
      lesserPoint = i;
    }
  }

  if( lesserPoint == points.size() - 1 )
    points.push_back( ol::Vec2D( _x,_y ) );

  else
    points.insert( points.begin() + lesserPoint + 1, ol::Vec2D( _x,_y ) );
}


void Body::removePoint()
{
  points.pop_back();
}


void Body::draw()
{
  std::vector< Vec2D > p = absolutePoints();

  double cx = game.camera.x,
         cy = game.camera.y;

  for( unsigned i = 0; i < p.size() - 1; i++ )
    ol::Line( p[ i ] - ol::Vec2D( cx,cy ),
              p[ i+1 ] - ol::Vec2D( cx,cy ) ).Draw( ol::Rgba::BLUE );

  ol::Line( p.back() - ol::Vec2D( cx,cy ),
            p[ 0 ] - ol::Vec2D( cx,cy ) ).Draw( ol::Rgba::BLUE );
}


void Body::updateBounds()
{
  for( unsigned i = 0; i < points.size(); i++ )
  {
    if( points[ i ].x < left   ) left   = points[ i ].x;
    if( points[ i ].x > right  ) right  = points[ i ].x;
    if( points[ i ].y < top    ) top    = points[ i ].y;
    if( points[ i ].y > bottom ) bottom = points[ i ].y;
  }
}


Vec2D Body::testCollision( Body body )
{
  Vec2D separator,
        a, b,
        bestSeparator( 0,0 );
  double cast,
         difference,
         bestDifference = 0;

  std::vector< Vec2D > p1 = absolutePoints(),
                       p2 = body.absolutePoints();

  bool set = false;

  for( unsigned i = 0; i < p1.size(); i++ )
  {
    if( i == 0 )
    {
      separator = ~Vec2D( p1.back().y - p1[ 0 ].y,
                          p1[ 0 ].x - p1.back().x );
    }
    else
    {
      separator = ~Vec2D( p1[ i-1 ].y - p1[ i ].y,
                          p1[ i ].x - p1[ i-1 ].x );
    }

    a.x = a.y = p1[ 0 ].x * separator.x + p1[ 0 ].y * separator.y;

    for( unsigned j = 1; j < p1.size(); j++ )
    {
      cast = p1[ j ].x * separator.x +
             p1[ j ].y * separator.y;

      if( cast > a.y ) a.y = cast;
      else if( cast < a.x ) a.x = cast;
    }

    b.x = b.y = p2[ 0 ].x * separator.x + p2[ 0 ].y * separator.y;

    for( unsigned j = 1; j < p2.size(); j++ )
    {
      cast = p2[ j ].x * separator.x +
             p2[ j ].y * separator.y;

      if( cast > b.y ) b.y = cast;
      else if( cast < b.x ) b.x = cast;
    }

    if( a.y < b.x || a.x > b.y )
      return Vec2D( 0.0,0.0 );


    if( a.y - b.x < b.y - a.x )
      difference = ( a.y - b.x );
    else difference = -( b.y - a.x );
    separator *= difference;

    if( !set )
    {
      set = true;
      bestSeparator = separator;
      bestDifference = difference;
    }

    else if( fabs( difference ) < fabs( bestDifference ) )
    {
      bestDifference = difference;
      bestSeparator = separator;
    }
  }

  for( unsigned i = 0; i < p2.size(); i++ )
  {
    if( i == 0 )
    {
      separator = ~Vec2D( p2.back().y - p2[ 0 ].y,
                          p2[ 0 ].x - p2.back().x );
    }
    else
    {
      separator = ~Vec2D( p2[ i-1 ].y - p2[ i ].y,
                          p2[ i ].x - p2[ i-1 ].x );
    }

    a.x = a.y = p1[ 0 ].x * separator.x + p1[ 0 ].y * separator.y;

    for( unsigned j = 1; j < p1.size(); j++ )
    {
      cast = p1[ j ].x * separator.x +
             p1[ j ].y * separator.y;

      if( cast > a.y ) a.y = cast;
      else if( cast < a.x ) a.x = cast;
    }

    b.x = b.y = p2[ 0 ].x * separator.x + p2[ 0 ].y * separator.y;

    for( unsigned j = 1; j < p2.size(); j++ )
    {
      cast = p2[ j ].x * separator.x +
             p2[ j ].y * separator.y;

      if( cast > b.y ) b.y = cast;
      else if( cast < b.x ) b.x = cast;
    }

    if( a.y < b.x || a.x > b.y ) return Vec2D( 0.0,0.0 );

    if( a.y - b.x < b.y - a.x )
      difference = ( a.y - b.x );
    else difference = -( b.y - a.x );
    separator *= difference;

    if( fabs( difference ) < fabs( bestDifference ) )
    {
      bestSeparator = separator;
      bestDifference = difference;
    }
  }

  return bestSeparator;
}


std::vector< Vec2D > Body::absolutePoints()
{
  std::vector< Vec2D > absPoints;

  for( unsigned i = 0; i < points.size(); i++ )
    absPoints.push_back( points[ i ] + Vec2D( x,y ) );

  return absPoints;
}


void Body::save( std::ofstream& file )
{
  file << "--Body 0"    << endl
       << x             << endl
       << y             << endl
       << top           << endl
       << bottom        << endl
       << left          << endl
       << right         << endl
       << points.size() << endl;

  for( unsigned j = 0; j < points.size(); j++ )
    file << points[ j ].x << endl
         << points[ j ].y << endl;
}


void Body::load( std::ifstream& file )
{
  if( !file.is_open() )
    return;
  if( getLine( file ) != "--Body 0" )
    return;

  x      = fromLine< double >( file );
  y      = fromLine< double >( file );
  top    = fromLine< double >( file );
  bottom = fromLine< double >( file );
  left   = fromLine< double >( file );
  right  = fromLine< double >( file );

  int numberOfPoints = fromLine< int >( file );

  for( int j = 0; j < numberOfPoints; j++ )
    points.push_back( ol::Vec2D( fromLine< double >( file ),
                                 fromLine< double >( file ) ) );
}
