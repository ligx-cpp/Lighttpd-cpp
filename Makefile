CC=g++
CC_FLAG= -std=c++11 -g -Wall
INCLUDE=-Iinclude
LIB=-pthread
LDFLAGS=-levent



SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))

target=main
$(target):$(OBJS)
	$(CC) $(LIB) $(LDFLAGS) $^ -o $@
%.o:%.cpp
	$(CC) -c $^ $(INCLUDE) $(CC_FLAG)  -o $@
.PRONY:clean
clean:	
	@echo "Removing linked and compiled files......"	
	rm -f *.o
