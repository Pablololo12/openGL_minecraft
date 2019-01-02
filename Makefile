TARGET = game

FLAGS = -O2 -march=native
LIBS = -lglfw -framework OpenGL
CC = gcc
LD = gcc
LDFLAGS =

SRCDIR   = src
OBJDIR   = obj

SOURCES  := $(wildcard $(SRCDIR)/*.c)
OBJS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(TARGET)
 
$(TARGET): $(OBJS)
	@echo LD $<
	@$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p ${OBJDIR}
	@echo CC $<
	@$(CC) $(FLAGS) -c -o $@ $<

clean::
	@if [ -n "$(OBJS)" ]; then rm -f $(OBJS); fi
	@if [ -f core ]; then rm -f core; fi
	@rm -f tags *.linkinfo

mrproper::
	@if [ -n "$(TARGET)" ]; then rm -f $(TARGET); fi
	@if [ -n "$(OBJS)" ]; then rm -f $(OBJS); fi
	@if [ -f core ]; then rm -f core; fi
	@rm -r -f ${OBJDIR}
	@rm -f tags *.linkinfo