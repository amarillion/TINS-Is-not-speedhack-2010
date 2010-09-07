#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <string>

#include <Frame.hpp>


/** @brief Handles rendering.
 *
 *  A singleton that establishes a rendering window and handles batch rendering.
 *  Graphics are z-sorted here before being drawn.
 */
class Renderer
{
  public:
    /** @brief Gets the instance.
     *
     *  Returns an instance of the renderer. If the instance has not yet been
     *  created, the given arguments are used to make a new rendering window.
     *
     *  @param width The width of the rendering window.
     *  @param height The height of the rendering window.
     *  @param fullscreen Tries to run in a fullscreen video mode if true.
     *  @param title The window title.
     *
     *  @return A pointer to the renderer instance.
     */
    static Renderer* _( int width = 640, int height = 480,
                        bool fullscreen = false, const std::string& title = "Dorr" );

    /** @brief Adds an image to the queue.
     *
     *  Adds an image to the rendering queue.  It will be drawn to the screen
     *  the next time that the render() function is called.
     *
     *  @param frame The image to be drawn.
     */
    void add( Frame* frame );


    /** @brief Renders the queue.
     *
     *  Sorts the images in the rendering queue, and then blasts them onto the
     *  screen.  The queue is emptied after rendering.
     */
    void render();

  private:
    Renderer( int width, int height, bool fullscreen, const std::string& title );

    static Renderer* instance;

    std::vector< Frame* > renderQueue;
};


#endif
