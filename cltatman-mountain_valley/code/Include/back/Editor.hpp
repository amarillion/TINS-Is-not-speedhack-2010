#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <OpenLayer.hpp>

#include <Frame.hpp>
#include <Entity.h>

#include <vector>
#include <string>


class Editor
{
  public:
    Editor();

    void initialize();

    void update();

    void draw();

    std::string mode,
                metaMode;

    double cursorX,
           cursorY,
           cursorZ;
    bool enabled;
    unsigned gridSize,
             imageStripSelection,
             workingPointsSelection;
    ol::TextRenderer font;

    std::vector< ol::Vec2D > workingPoints;
    std::vector< std::string > imageFiles;
    std::vector< Frame > images;

    Frame workingFrame;
};

#endif // EDITOR_HPP
