#pragma once

#include <fstream>      // std::filebuf, std::ifstream
#include <string>

class RxBuffer
{
public:
	
	//RxBuffer();
	RxBuffer(std::string filename);		//open file as rx buffer

	int16_t peekNext();					//look at next char
	int16_t getNext();					//get next char, index++

	~RxBuffer();						//close file

private:
	uint32_t index = 0;
	std::size_t size = 0;
	std::filebuf* pbuf = nullptr;
	std::ifstream ifs;
};

