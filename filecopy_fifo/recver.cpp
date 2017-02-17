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
class Recver
{
public:
	Recver(std::string df, std::string fn);
	int32_t Recv_File();
	~Recver();
private:
	File dst_;
	File fifo_;
};

Recver::Recver(std::string df, std::string fn):
	dst_(df, O_WRONLY|O_CREAT),	fifo_(fn, O_RDONLY)
{
}
Recver::~Recver()
{
}


int32_t Recver::Recv_File()
{
	dst_.open();
	fifo_.open();
	const int BUF_SIZE = 4096;
	char *buf = new char[BUF_SIZE];
	int32_t total = 0;
	while(true){
		int len = fifo_.read(buf, BUF_SIZE);
		if(len <= 0)
			break;
		int wbytes = 0;
		while(wbytes < len){
			int ret = dst_.write(buf+wbytes, len-wbytes);
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
		fprintf(stderr,"Usage:./recver dst_file fifo_name\n");
		return -1;
	}
	Recver rec(argv[1], argv[2]);
	try{
		int32_t size = rec.Recv_File();
		printf("receive %d bytes\n", size);
	}catch(std::exception &e){
		fprintf(stderr,"%s", e.what());
	}
	return 0;
}
