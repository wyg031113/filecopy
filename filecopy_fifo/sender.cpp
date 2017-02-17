#include <cstdio>
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
class Sender
{
public:
	Sender(std::string sf, std::string fn);
	int32_t Send_File();
	~Sender();
private:
	File src_;
	File fifo_;
};

Sender::Sender(std::string sf, std::string fn):
	src_(sf, O_RDONLY),	fifo_(fn, O_WRONLY)
{
}
Sender::~Sender()
{
}


int32_t Sender::Send_File()
{
	src_.open();
	fifo_.open();
	const int BUF_SIZE = 4096;
	char *buf = new char[BUF_SIZE];
	int32_t total = 0;
	while(true){
		int len = src_.read(buf, BUF_SIZE);
		if(len <= 0)
			break;
		int wbytes = 0;
		while(wbytes < len){
			int ret = fifo_.write(buf+wbytes, len-wbytes);
			if(ret <= 0)
				break;
			wbytes += ret;
		}
		total += wbytes;
	}
	delete []buf;

	return total;
}
int main(int argc, char *argv[])
{
	if(argc != 3){
		fprintf(stderr,"Usage:./sender src_file fifo_name\n");
		return -1;
	}
	Sender sen(argv[1], argv[2]);
	try{
		int32_t size = sen.Send_File();
		printf("send %d bytes.\n", size);
	}catch(std::exception &e){
		fprintf(stderr,"%s", e.what());
	}
	return 0;
}
