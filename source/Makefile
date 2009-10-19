CC =		g++
AR =		ar
#ARFLAGS =	@ARFLAGS@
INCLUDES =	-I. -I../NDSE-SDK/include 
CFLAGS =	-fPIC -g -O2 -fvisibility=hidden $(INCLUDES)
LDFLAGS =

SRCS=KEYPAD_Actor.cpp



OBJS	:=	$(SRCS:.cpp=.o)

all: libKEYPAD_Actor.a libKEYPAD_Actor.so

$(OBJS): %.o:
	$(CC) $(CFLAGS) -c $*.cpp -o $@


libKEYPAD_Actor.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

libKEYPAD_Actor.so: $(OBJS)
	$(CC) -shared -Wl,-soname=libKEYPAD_Actor.so $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f *.o
	rm -f libKEYPAD_Actor.a
	rm -f libKEYPAD_Actor.so*






