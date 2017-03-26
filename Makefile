VERSION := 0.1a
CC      := gcc
CFLAGS  := -Wall -Werror -DVERSION=\"$(VERSION)\"
LIBS    := 
DESTDIR := /usr

TARGET	:= dirsize 
SOURCES := $(shell find src/ -type f -name *.c)
OBJECTS := $(patsubst src/%,build/%,$(SOURCES:.c=.o))
OBJECTS += 
DEPS	:= $(OBJECTS:.o=.deps)

$(TARGET): $(OBJECTS)
	@echo "  Linking '$(TARGET)'..."; $(CC) $^ -o $(TARGET) $(LIBS)

build/%.o: src/%.c
	@mkdir -p build/
	@echo "  CC $<"; $(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -o $@ $<

clean:
	@echo "  Cleaning..."; $(RM) -r build/ $(TARGET)

install: $(TARGET)
	@cp -p $(TARGET) ${DESTDIR}${PREFIX}/bin/

-include $(DEPS)

.PHONY: clean
