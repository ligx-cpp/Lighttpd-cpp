CC=g++
CC_FLAG= -std=c++11 -g -Wall 
INCLUDE=-I./include 
LIB= -pthread 
LDFLAGS=-levent -ldl #动态链接库“在链接时”要放在$@的后面，不然不会起作用



SRCS0=$(wildcard *.cpp)
OBJS0=$(patsubst %.cpp,%.o,$(SRCS0))

SRCS1=$(wildcard *.c)
OBJS1=$(patsubst %.c,%.o,$(SRCS1))

%.o:%.cpp
	$(CC) -c $^ $(INCLUDE) $(CC_FLAG)  -o $@
%.o:%.c
	$(CC) -c $^ $(INCLUDE) $(CC_FLAG)  -o $@

#上面这8行少那一行都不行

target=main
$(target):$(OBJS0) $(OBJS1)
	$(CC) $(LIB) $^ -o $@ $(LDFLAGS)  


.PRONY:clean
clean:	
	@echo "Removing linked and compiled files......"	
	rm -f *.o
