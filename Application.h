#pragma once
#include "Image.h"

class Application
{
public:
	static Application s_instance;
public:
	static void InitApplication();
	static void RunApplication();
private:
	void appendToString(std::string& fileName, const char* c, uint32_t index);
	void Init();
	void Run();
	void setStaticExposureValue();
	void setCalculatedExposureOffset();
	void createTimeLapse();
	
private:
	static const std::string s_imagePrefix;
	std::string m_filePath;
	std::vector<Image> m_images;
	uint32_t m_imageCount;
};
