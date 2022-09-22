IDIR =.
CC=/opt/pi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
CFLAGS=-I$(IDIR) -Wall -std=c99

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = esbe3_drv.h fwd_ctrl.h mylog.h rpi_gpio.h sig.h temp.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ =esbe3_drv.o fwd_ctrl.o gpio.o log.o main.o sig.o temp.o boiler.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

