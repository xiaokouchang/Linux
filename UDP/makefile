.PHONY:all
all: udpserver udpclient
udpserver:main.cc
	g++ -o $@ $^ -std=c++11
udpclient:UDPClient.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f udpclient udpserver