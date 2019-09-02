CC=g++
CC_FLAG= -std=c++11 -g -Wall 
INCLUDE=-Iinclude
LIB= -pthread
LDFLAGS=-levent #Makefile中$(LDFLAGS)“在链接时”要放在$@的后面，不然不会起作用



SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))

target=main
$(target):$(OBJS)
	$(CC) $(LIB) $^ -o $@ $(LDFLAGS)  
%.o:%.cpp
	$(CC) -c $^ $(INCLUDE) $(CC_FLAG)  -o $@
.PRONY:clean
clean:	
	@echo "Removing linked and compiled files......"	
	rm -f *.o
