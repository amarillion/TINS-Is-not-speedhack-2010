#pragma once

// TODO 
//
// Catmull-Rom
// Lerp
// Smoothstep
// Step

const int MAX_INT = 2147483647;

inline unsigned long min(unsigned long x, unsigned long y)
{
	return x < y ? x : y;
}

inline unsigned long max(unsigned long x, unsigned long y)
{
	return x > y ? x : y;
}

inline float frand() 
{
	return rand() / (float)RAND_MAX;
}

int inline clamp(int value, int min, int max)
{
	return value < min ? min : (value > max ? max : value);
}

float inline clamp(float value, float min, float max)
{
	return value < min ? min : (value > max ? max : value);
}

int inline sign(int value)
{
	return value < 0 ? -1 : 1;
}

unsigned inline int closestPowerOf2(unsigned int aValue)
{
	int bit = 1;
	for (unsigned int i = 0; i < 32; i++)
	{
		if (aValue & (1 << i))
		{
			bit = i;
		}
	}

	return 1 << (bit + 1);
}

bool inline isProbability(unsigned int aProbabilty, unsigned int aTotalOutcome)
{
	return (rand() % aTotalOutcome) < aProbabilty;
}

template<typename T>
T fromString(const std::string& str)
{
    std::istringstream is(str);
    T t;
    is >> t;
    return t;
}

template<typename T>
std::string toString(T t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}

std::vector<std::string> inline tokenize(const std::string& aString, char aDelimiter)
{
	std::string token;
	std::istringstream iss(aString);
	std::vector<std::string> tokens;

	while ( getline(iss, token, aDelimiter) )
	{
		tokens.push_back(token);
	}

	return tokens;
}

inline std::string intToStringFixed4(int aValue)
{
	if (aValue < 10)
	{
		return "   " + toString(aValue);
	}
	if (aValue < 100)
	{
		return "  " + toString(aValue);
	}
	if (aValue < 1000)
	{
		return " " + toString(aValue);
	}

	return toString(aValue);
}

inline std::string intToStringFixed3(int aValue)
{
	if (aValue < 10)
	{
		return "  " + toString(aValue);
	}
	if (aValue < 100)
	{
		return " " + toString(aValue);
	}

	return toString(aValue);
}

std::string loadFile(const std::string &fileName, bool fullpath = false);

std::vector<std::string> tokenize(const std::string &input, const std::string separator, bool ingoreEmpty = false);