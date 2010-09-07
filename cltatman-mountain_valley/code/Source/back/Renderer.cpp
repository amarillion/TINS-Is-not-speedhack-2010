#include <Renderer.hpp>
#include <Frame.hpp>

#include <OpenLayer.hpp>
#include <algorithm>


Renderer* Renderer::instance = NULL;


Renderer::Renderer( int width, int height, bool fullscreen, const std::string& title )
{
  ol::Setup::SetupProgram();
  ol::Setup::SetupScreen( width, height, fullscreen );

  set_window_title( title.c_str() );
}


Renderer* Renderer::_( int width, int height, bool fullscreen, const std::string& title )
{
  return instance ? instance :
                    instance = new Renderer( width, height, fullscreen, title );
}


void Renderer::add( Frame* frame )
{
  renderQueue.push_back( frame );
}


void Renderer::render()
{
  std::sort( renderQueue.begin(), renderQueue.end() );

  std::vector< Frame* >::iterator it;


  ol::Canvas::Fill( ol::Rgba::BLACK );

  for( it = renderQueue.begin(); it != renderQueue.end(); it++ )
    (*it)->draw();

  ol::Canvas::Refresh();


  renderQueue.clear();
}
