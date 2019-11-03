IDIR =.
CC=gcc
CFLAGS=-I$(IDIR) -Wall

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = esbe3_drv.h fwd_ctrl.h mylog.h rpi_gpio.h sig.h temp.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ =esbe3_drv.o fwd_ctrl.o gpio.o log.o main.o sig.o temp.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

