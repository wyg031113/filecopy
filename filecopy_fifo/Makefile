all:sender recver
sender:sender.cpp file.cpp
	g++ sender.cpp file.cpp -g -o $@

recver:recver.cpp file.cpp
	g++ recver.cpp file.cpp -g -o $@
clean:
	rm sender recver
