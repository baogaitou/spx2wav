# Convert speex to wav
# By ly9@qq.com

CC = gcc
CXX = g++
CFLAGS = -g
CPPFLAGS = 

INCLUDE=./speex
LNK_OPT = -lspeex

all: spx2wav

spx2wav: SpeexDecode.o TRSpeex.o fifo.o
	$(CC) $(CFLAGS) -o $@ $^ -I$(INCLUDE)  $(LNK_OPT)

SpeexDecode.o:./SpeexDecode.c
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INCLUDE)  $(LNK_OPT)

TRSpeex.o:./TRSpeex.c
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INCLUDE)  $(LNK_OPT)

fifo.o:./fifo.c
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INCLUDE)  $(LNK_OPT)


clean:
	rm *.o
	rm spx2wav

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< -fPIC -g -O0 -I$(INCLUDE) -v
.cc.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< -fPIC -I$(INCLUDE) -v

