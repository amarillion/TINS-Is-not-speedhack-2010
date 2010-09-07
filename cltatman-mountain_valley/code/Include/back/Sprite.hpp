#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <string>
#include <map>

#include <Animation.hpp>


/** @brief A container of Animations.
 *
 *  A container of Animations that handles updating and rendering.
 */
class Sprite
{
  public:
    Sprite();
    Sprite( std::string filename );

    /** @brief Add an animation to the sprite.
     *
     *  @param animation The animation to be added.
     */
    void addAnimation( Animation animation );


    /** @brief Set the current animation.
     *
     *  @param title The title of the animation to be set.
     */
    void setAnimation( const std::string& title );


    /** @brief Set the speed of the sprite.
     *
     *  Sets the update speed of the sprite.  A value of 1 updates at normal
     *  speed, a value of 0.5 updates at half speed, 2.0 updates at twice the
     *  normal speed, etc.  Negative values will result in backward animation.
     *
     *  @param value The speed to be set.
     */
    void setSpeed( double value );


    /** @brief Update the animation.
     *
     *  Updates the current animation.
     */
    void update();


    /** @brief Draws the sprite.
     *
     *  Spends the current frame of the current animation to the renderer.  It
     *  will be drawn to the screen after the renderer does its business.
     */
    void draw( double x, double y, double z = 1.0, double rotation = 0.0,
               double xScale = 1.0, double yScale = 1.0, double opacity = 1.0 );

    Animation getCurrentAnimation();

    void save( std::ofstream& file );
    void load( std::ifstream& file );

  private:
    std::string currentAnimation;

    double playSpeed;

    std::map< std::string, Animation > animations;
};

#endif
