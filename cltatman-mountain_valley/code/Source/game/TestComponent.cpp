#include <game/TestComponent.hpp>
#include <global/FileIO.hpp>
#include <global/Components.hpp>
#include <back/Game.hpp>



REGISTER( TestComponent );


TestComponent::TestComponent() : Component( "TestComponent" )
{}


void TestComponent::save( std::ofstream& file )
{
  file << "This is a TestComponent calling the 'save' function!" << std::endl;
}


void TestComponent::load( std::ifstream& file )
{
  getLine( file ); // Clean up the line that we wrote in the save.
}
