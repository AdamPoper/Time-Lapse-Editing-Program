#include "Application.h"

Application Application::s_instance;
const std::string Application::s_imagePrefix = "IMG_";

void Application::Init()
{
	std::cout << "Please Enter File Path of the Images: ";
	std::cin >> m_filePath;
	m_filePath += s_imagePrefix;
	std::cout << "Please Enter the Number of Images to be processed: ";
	std::cin >> m_imageCount;
	bool done = false;
	uint32_t imgIndex = 1;
	while (!done)
	{
		std::stringstream ss;
		std::string fileName = m_filePath;
		if (imgIndex < 10)
			appendToString(fileName, "000", imgIndex);
		else if (imgIndex < 100)
			appendToString(fileName, "00", imgIndex);
		else if (imgIndex < 1000)
			appendToString(fileName, "0", imgIndex);
		m_images.emplace_back(fileName, imgIndex-1);
		imgIndex++;
		if (imgIndex == m_imageCount + 1)
			done = true;
	}
}
void Application::Run()
{
	int choice = 0;
	while (choice != 3)
	{
		std::cout << "1. Set static offset accross all images" << std::endl;
		std::cout << "2. Adjust exposure offset for all images individually" << std::endl;
		std::cout << "3. Create Timelapse From Images" << std::endl;
		std::cout << "4. Quit Application" << std::endl;
		std::cout << "Choice: ";
		std::cin >> choice;
		switch (choice)
		{
			case 1:
				setStaticExposureValue();
			break;
			case 2:
				setCalculatedExposureOffset();
			break;
			case 3:
				createTimeLapse();
			break;
			case 4:
				std::cout << "Good Bye" << std::endl;
			break;
			default:
				std::cout << "Enter A Valid Choice" << std::endl;
		}
	}
}
void Application::appendToString(std::string& fileName, const char* c, uint32_t index)
{
	std::stringstream ss;
	fileName += c;
	ss << index;
	std::string temp;
	ss >> temp;
	fileName += temp;
	fileName += ".xmp";
}
void Application::setStaticExposureValue()
{
	float exposureValue;
	std::cout << "Enter Target Exposure Value: ";
	std::cin >> exposureValue;
	for (Image& img : m_images)
		img.updateExposureMetaData(exposureValue);
}

void Application::setCalculatedExposureOffset()
{
	uint32_t lastSequenceUpdated = 0;
	for (int i = 0; i < m_images.size() - 1; i++)
	{
		float stops = m_images[i].compareImages(m_images[i + 1]);
		if (stops != 0.0f)
		{
			float exposureIncrement = stops / (i - lastSequenceUpdated);
			float exposure = 0.0f;
			for (int j = lastSequenceUpdated; j <= i; j++)
			{
				m_images[j].updateExposureMetaData(exposure);
				exposure += exposureIncrement;
			}
			lastSequenceUpdated = i + 1;
			std::cout << "Last edited squence index " << lastSequenceUpdated << std::endl;
		}
	}
	// this is if there are images left after the last exposure change
	// by default it increases each exposure by 0.04f
	if (lastSequenceUpdated <= m_imageCount)
	{
		uint32_t imagesLeft = m_images.size() - lastSequenceUpdated;
		float exposureIncrement = 0.04f;
		float exposure = 0.0f;
		for (int i = lastSequenceUpdated; i < m_images.size(); i++)
		{
			m_images[i].updateExposureMetaData(exposure);
			exposure += exposureIncrement;
		}
	}
	std::cout << "Successfully Edited " << m_imageCount << " Images" << std::endl;
}
void Application::createTimeLapse()
{
	std::string imagesFilePath, imageFormat, videoPath;
	uint32_t width, height, frameRate;

	std::cout << "Enter Path To Images: ";
	std::cin >> imagesFilePath;
	std::cout << std::endl;
	std::cout << "Enter Image Format And Extension: ";
	std::cin >> imageFormat;
	std::cout << std::endl;
	std::cout << "Enter Video Width: ";
	std::cin >> width;
	std::cout << "Enter Video Height: ";
	std::cin >> height;
	std::cout << std::endl;
	std::cout << "Enter Video Frame Rate: ";
	std::cin >> frameRate;
	std::cout << std::endl;
	std::cout << "Enter Video Output Path And Name: ";
	std::cin >> videoPath;
	std::string operation = "ffmpeg -r ";
	operation += std::to_string(frameRate);
	operation += " -f image2 -s ";
	operation += std::to_string(width);
	operation += "x";
	operation += std::to_string(height);
	operation += " -i ";
	operation += (imagesFilePath + imageFormat);
	operation += " -vcodec libx264 -crf 25  -pix_fmt yuv420p ";
	operation += videoPath;
	std::cout << operation << std::endl;
	FILE* pipe;
	pipe = _popen(operation.c_str(), "w");
	_pclose(pipe);
}
void Application::InitApplication()
{
	s_instance.Init();
}
void Application::RunApplication()
{
	s_instance.Run();
}