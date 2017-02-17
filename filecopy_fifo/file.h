#ifndef __FILE_H__
#define __FILE_H__
#include <string>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <exception>
#include <stdexcept>

class File
{
public:
	File(std::string name, int flag = O_RDONLY);
	void open();
	int32_t read(char *buf, int32_t len);
	int32_t write(const char *buf, int32_t len);
	~File();
private:
	int fd_;
	int flag_;
	std::string name_;
};

#endif
