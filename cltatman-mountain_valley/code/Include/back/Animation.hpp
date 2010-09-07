#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <string>

#include <Frame.hpp>

/** @brief A container of Frames.
 *
 *  A container that manages Frames to produce animation.
 */
class Animation
{
  public:
    /** @brief Constructor.
     *
     *  Creates an empty animation with the given title.
     *
     *  @param title The title of the animation.  This will be used to refer to
     *               the animation through the Sprite class.
     */
    Animation( const std::string& _title = "default" );


    /** @brief Constructs an animation from a directory.
     *
     *  Creates an animation with the given title, filling it with Frames based
     *  on a query string.
     *
     *  @param title The title of the animation.  This will be used to refer to
     *               the animation through the Sprite class.
     *
     *  @param query A string of the form "/player/jump/ *.png", a directory path
     *               followed by an asterix and a file extension.  This example
     *               creates a Frame for each of png files in '/player/jump/',
     *               pushing them into the animation.
     *
     *  @param ticks The duration of each Frame.
     */
    Animation( const std::string& _title, const std::string& query,
               double ticks = 1.0, double x = 0.0, double y = 0.0,
               double rotation = 0, double xScale = 1.0, double yScale = 1.0,
               double r = 0.0, double g = 0.0, double b = 0.0, double a = 0.0,
               double opacity = 1.0 );


    /** @brief Adds a frame to the animation.
     *
     *  Pushes a new Frame onto the end of the animation.
     *
     *  @param frame The Frame to be added.
     */
    void addFrame( Frame frame );


    /** @brief Updates the animation.
     *
     *  Cycles through the Frames based on the duration of each.
     *
     *  @param ticks A faked delta-time used to adjust animation speed.
     *               Negative values result in backward animation.
     */
    void update( double ticks );


    /** @brief Gets the current frame.
     *
     *  Returns a pointer to the current Frame.
     */
    Frame getCurrentFrame() const;

    void setRotation( double rotation );


    void save( std::ofstream& file );
    void load( std::ifstream& file );


    std::string title;
    double      time;
    unsigned    current;

    std::vector< Frame > frames;
};


#endif
