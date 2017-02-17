#include "cstdio"
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <exception>
#include <stdexcept>
#include "file.h"

File::File(std::string name, int flag /*= O_RDONLY*/):
	fd_(-1), flag_(flag), name_(name)
{
}
File::~File()
{
	if(fd_ != -1)
		close(fd_);
	fprintf(stderr, "~File()\n");
}

void File::open()
{
	if(flag_ & O_CREAT)
		fd_ = ::open(name_.c_str(), flag_, 0644);	
	else
		fd_ = ::open(name_.c_str(), flag_);	
	if(fd_ == -1)
		throw std::runtime_error(strerror(errno));

}
int32_t File::read(char *buf, int len)
{
	return ::read(fd_, buf, len);
}
int32_t File::write(const char *buf, int len)
{
	return ::write(fd_, buf, len);
}

