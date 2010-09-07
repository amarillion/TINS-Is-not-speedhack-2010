#ifndef FRAME_HPP
#define FRAME_HPP

#include <string>

#include <OpenLayer.hpp>

/** @brief Defines a single frame.
 *
 *  Defines a single graphic frame including positional information, scaling,
 *  tinting, duration and opacity.
 */
class Frame
{
  public:
    /** @brief Constructor.
     *
     *  @param file The path to the image file.
     *  @param time The duration of the frame, in system ticks.
     */
    Frame( const std::string& imageFile = "load failed", double time = 1.0 );


    /** @brief Sets the placement.
     *
     *  Sets the position and rotation of the frame, relative to the containing
     *  sprite.
     *
     *  @param x X offset from sprite position.
     *  @param y Y offset from sprite position.
     *  @param rotation Rotation relative to sprite rotation.
     */
    void setPlacement( double x, double y, double rotation = 0.0 );


    /** @brief Sets the scale by factors.
     *
     *  Sets the scale of the frame by an x and y factor.  Values are clamped
     *  to a 0 minimum.
     *
     *  @param x Horizontal scaling factor.
     *  @param y Vertical scaling factor.
     */
    void setScale( double x, double y );


    /** @brief Sets the scale by size.
     *
     *  A convenience function to allow scaling to an arbitrary size.  This
     *  function calls setScaleByFactors, and so negative values will be clamped
     *  to 0.  This function also grabs a copy of the image from the ImageBank,
     *  which may be relatively expensive.
     *
     *  @param w The desired width of the image.
     *  @param h The desired height of the image.
     */
    void setSize( double w, double h );


    /** @brief Sets the tinting of the frame.
     *
     *  Sets the tinting of the frame based on four color components.  Values
     *  should be between 0 and 1 inclusive, and will be clamped to this range.
     *
     *  @param r Red component.
     *  @param g Green component.
     *  @param b Blue component.
     *  @param a Alpha component.
     */
    void setTint( double r, double g, double b, double a );


    /** @brief Sets the opacity.
     *
     *  Sets the opacity with which the frame should be drawn, given a value
     *  between 0 and 1 inclusive.
     *
     *  @param value The desired opacity.
     */
    void setOpacity( double value );


    /** @brief Draws the frame.
     *
     *  This draws the frame with appropriate position, rotation, tinting,
     *  scaling and opacity.  This should not be called directly under normal
     *  circumstances.  This function is used by the renderer.  See the Sprite
     *  class for appropriate drawing functions.
     */
    void draw();


    void load( std::ifstream& file );
    void save( std::ofstream& file );

    /** @brief The filepath of the image. */
    std::string _imageFile;


    ol::Placement _placement;
    ol::Vec2D     _scale;
    ol::Rgba      _tint;

    double _z,
           _time,
           _opacity,
           _width,
           _height;
};


#endif
