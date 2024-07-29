CC = gcc
CFLAGS = -Wall -Wextra -pedantic

SRCDIR = src
BLDDIR = build
TARGET = agog
AGOG_INSTALL_DIR ?= /usr/local/bin/

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:%.c=$(BLDDIR)/%.o)

$(BLDDIR)/$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

$(BLDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: install
install:
	echo "$(AGOG_INSTALL_DIR)"
	cp $(BLDDIR)/$(TARGET) $(AGOG_INSTALL_DIR)

.PHONY: clean
clean:
	rm -r $(BLDDIR)

