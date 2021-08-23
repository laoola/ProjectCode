#
#design of ARP sniffer
#

CFLAGS = -g
LDFLAGS = -lpcap

OBJS = test.o
TARGET = pcap

RM = rm -f

$(TARGET):$(OBJS)
    $(CC) $(LDFLAGS) -o $@ $^

%.o:%.c
    $(CC) $(CFLAGS) -c -o $@ $<

.PHONY:clean

clean:
    $(RM) $(TARGET) $(OBJS)