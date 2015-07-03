CC      = gcc  
LD      = ld 
AR      = ar
NM      = nm
OBJCOPY = objcopy
OBJDUMP = objdump
READELF = readelf

SOURCES = RegCtl_SpiHw.c regSpi_test.c 
OBJS = $(SOURCES:.c=.o)
EXEC = spiTest

CFLAGS += -Wall 			
LDADD = 
.PHONY: all
all: $(EXEC) 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXEC): $(OBJS)
	$(CC) $(LDADD) -o $@ $?

.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC)
