#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

class Image
{
public:
	// constructor takes a filepath to read the data from to make the object
	Image(const std::string& filePath, uint32_t index);
	// debugging function
	void printMetaData() const;
	// updates the the exposure offset with a given value
	void updateExposureMetaData(float exposureValue);
	// compares 2 images to find a difference in exposure
	float compareImages(const Image& img);
	// utility funcions for getting these values if needed
	uint32_t getISO() const { return m_iso; }
	float getShutterSpeed() const { return m_shutterSpeed; }
	float const getFStop() const { return m_fstop;  }
private:
	// returns a calculated value in stops for the difference in exposure by ISO
	float calculateISOExposure(float next_iso);
	// returns a calculated value in stops for the difference in exposure by shutter speed
	float calculateShutterSpeedExposure(float s_speed);
	// retrieves the shutter speed from the string that contains it
	void parseShutterSpeed(const std::string& line);
	// retrieves the ISO from the string that contains it
	void parseISO(const std::string& line);
	// performs the actual writing of the file to the xmp file
	void writeMetaData();
	
private:
	// these are static strings that are what the algorithm needs to look for to find the exposure values
	static const std::string s_exposureLine;
	static const std::string s_aperatureLine;  // not in use because it will be hard coded to f/2.8
	static const std::string s_shutterSpeedLine;
	static const std::string s_ISOLine;
	std::string m_filePath;
	std::vector<std::string> m_file;   // contains all the lines for the file
	float m_shutterSpeed;
	float m_fstop;
	uint32_t m_iso;
	uint32_t m_indexID;   // just an identifier
};

// for easy outputing of the image data
std::ostream& operator<<(std::ostream& os, const Image& img);
