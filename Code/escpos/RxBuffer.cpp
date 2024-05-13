#include "RxBuffer.h"

RxBuffer::RxBuffer(std::string filename)
{
	ifs.open(filename.c_str(), std::ifstream::binary);
	if (ifs.is_open()) {
		pbuf = ifs.rdbuf();
		size = pbuf->pubseekoff(0, ifs.end, ifs.in);
		pbuf->pubseekpos(ifs.beg);
		//printf("size: %d\n", (int)size);
	}
}

int16_t RxBuffer::peekNext()
{
	int16_t c = -1;
	if (ifs.is_open()) {
		while (pbuf->sgetc() != std::streambuf::traits_type::eof()) {
			c = pbuf->sgetc();
			return c;
		}
	}
	return -1;
}

int16_t RxBuffer::getNext()
{
	int16_t c = -1;
	if (ifs.is_open()) {
		while (pbuf->sgetc() != std::streambuf::traits_type::eof()) {
			c = pbuf->sbumpc();
			return c;
		}
	}
	return -1;
}

RxBuffer::~RxBuffer()
{
	ifs.close();
	pbuf = nullptr;
}
