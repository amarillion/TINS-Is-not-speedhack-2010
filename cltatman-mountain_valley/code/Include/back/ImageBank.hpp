#ifndef IMAGEBANK_HPP
#define IMAGEBANK_HPP

#include <string>

#include <OpenLayer.hpp>


/** @brief Manages image files.
 *
 *  A singleton that keeps track of loaded image files.  Supports bmp, tga, png.
 */
class ImageBank
{
  public:
    /** @brief Returns the ImageBank instance.
     *
     *  Returns a pointer to the static instance of ImageBank.  If the
     *  instance has not been created yet, it is created.
     */
    static ImageBank* _();


    /** @brief Loads an image from a file.
     *
     *  @param filepath The path to the image file.
     *
     *  @return True on a successful load, or if the file has already been
     *          loaded.  False otherwise.
     */
    bool load( const std::string& filepath );


    /** @brief Retrieves an image from the bank.
     *
     *  Grabs an image from the bank.  If the image is not found it is loaded.
     *  If loading fails a yellow placeholder image is returned.
     *  This operation has logarithmic speed, provided that the image has been
     *  loaded.
     *
     *  @return A pointer to the image, an ol::Bitmap.  If the image cannot be
     *          loaded, a yellow placeholder image is returned.
     */
    ol::Bitmap* get( const std::string& filepath );


    /** @brief Empties the image bank.
     *
     *  You should obviously avoid doing this in the middle of a game loop.  The
     *  memory being used to hold the images is freed, and any graphics will
     *  need to be reloaded.
     */
    void flush();

    void set( const std::string& name, ol::Bitmap* image );

  private:
    ImageBank();

    static ImageBank* instance;
    std::map< std::string, ol::Bitmap* > images;
};


#endif
