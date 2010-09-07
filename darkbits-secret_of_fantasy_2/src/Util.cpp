#include "Precompiled.hpp"
#include "Util.hpp"
#include "Resource.hpp"

std::string loadFile(const std::string &fileName, bool fullpath)
{
	std::string realFilename;

	if (fullpath)
	{
		realFilename = fileName;
	}
	else
	{
		realFilename = Resource::getRealFilename(fileName);
	}

	std::ifstream is(realFilename.c_str());

	if (!is.good())
	{
		throw std::string("Unable to open " + fileName);
	}

	std::string result;

	while (is.good()) {
		int c = is.get();
		if (c >= 0) {
			result += (unsigned char)c;
		} else {
			break;
		}
	}

	return result;
}

std::vector<std::string> tokenize(const std::string &input, const std::string separator, bool ignoreEmpty)
{
	bool done = false;
	int pos = 0;
	int lastpos = 0;

	std::vector<std::string> result;

	while(!done)
	{
		pos = input.find(separator, lastpos);
		if (pos == std::string::npos)
		{
			done = true;
			pos = input.length();
		}

		std::string sub = input.substr(lastpos, pos - lastpos);
		if (sub.size() > 0 || !ignoreEmpty)
		{
			result.push_back(sub);
		}
		lastpos = pos + separator.length();
	}

	return result;
}