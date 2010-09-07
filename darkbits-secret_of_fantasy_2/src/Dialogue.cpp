#include "Precompiled.hpp"
#include "Dialogue.hpp"

void Dialogue::addText(const std::string& aText)
{
	myTexts.push_back(aText);
}

int Dialogue::getNumberOfTexts() const
{
	return myTexts.size();
}

std::string Dialogue::getText( int i )
{
	return myTexts[i];
}
