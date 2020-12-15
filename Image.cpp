#include "Image.h"

const std::string Image::s_exposureLine = "Exposure2012";
const std::string Image::s_aperatureLine = " ";
const std::string Image::s_shutterSpeedLine = "ExposureTime";
const std::string Image::s_ISOLine = "RecommendedExposureIndex";

Image::Image(const std::string& filePath, uint32_t index)
	: m_filePath(filePath), m_fstop(2.8f), m_shutterSpeed(0.0f), m_iso(0), m_indexID(index)
{
	std::ifstream stream;
	stream.open(m_filePath);
	if (!stream.fail())
	{
		std::string line;
		while (std::getline(stream, line))
		{
			if (line.find(s_ISOLine, 0) != std::string::npos)
				parseISO(line);
			if (line.find(s_shutterSpeedLine, 0) != std::string::npos)
				parseShutterSpeed(line);
			m_file.push_back(line);
		}
	}
	stream.close();
}
void Image::printMetaData() const 
{
	for (const std::string& str : m_file)
		std::cout << str << std::endl;
}

void Image::updateExposureMetaData(float exposureValue)
{
	uint32_t lineIndex = 0;
	std::string expLine;
	for (const std::string& line : m_file)
	{
		if (line.find(s_exposureLine, 0) != std::string::npos)
		{
			int i = 0;
			while (true)
			{
				expLine.push_back(line[i]);
				if (line[i] == '+')
					break;
				i++;
			}
			break;
		}
		lineIndex++;
	}
	std::stringstream ss;
	ss << exposureValue;
	std::string expStr;
	ss >> expStr;
	expLine += expStr;
	expLine += '"';
	m_file[lineIndex] = expLine;
	std::cout << "[ Edited Image # " << m_indexID << " Exposure: +" << exposureValue << " ]" << std::endl;
	writeMetaData();
}

float Image::compareImages(const Image& img)
{
	float stopsDifference = 0.0f;
	if (m_iso != img.m_iso)
		stopsDifference += calculateISOExposure((float)img.m_iso);
	if (m_shutterSpeed != img.m_shutterSpeed)
		stopsDifference += calculateShutterSpeedExposure(img.m_shutterSpeed);
	return stopsDifference;
}
float Image::calculateISOExposure(float next_iso)
{
	return (std::log(next_iso / m_iso) / std::log(2));
}

float Image::calculateShutterSpeedExposure(float s_speed)
{
	return (std::log(s_speed / m_shutterSpeed) / std::log(2));
}
void Image::parseShutterSpeed(const std::string& line)
{
	std::stringstream ss;
	float numerator = 0.0f;
	float denominator = 0.0f;
	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] == '"')
		{
			i++;
			while (line[i] != '/')
			{
				ss << line[i];
				i++;
			}
			ss >> numerator;
			ss.clear();
			i++;
			while (line[i] != '"')
			{
				ss << line[i];
				i++;
			}
			ss >> denominator;
			m_shutterSpeed = numerator / denominator;
		}
	}
}
void Image::parseISO(const std::string& line)
{
	std::stringstream ss;
	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] == '"')
		{
			i++;
			uint32_t index = i;
			while (line[index] != '"')
			{
				ss << line[index];
				index++;
			}
			break;
		}
	}
	ss >> m_iso;
}

void Image::writeMetaData()
{
	std::ofstream stream;
	stream.open(m_filePath);
	for (const std::string& line : m_file)
		stream << line << std::endl;
	stream.close();
}
std::ostream& operator<<(std::ostream& os, const Image& img)
{
	os << "ISO: " << img.getISO() << " Shutter Speed: " << img.getShutterSpeed() << " f Stop: " << img.getFStop();
	return os;
}