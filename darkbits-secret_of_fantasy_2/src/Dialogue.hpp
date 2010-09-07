#pragma once

class Dialogue
{
public:
	void addText(const std::string& aText);
	int getNumberOfTexts() const;
	std::string getText(int i);
private:

	std::vector<std::string> myTexts;
};