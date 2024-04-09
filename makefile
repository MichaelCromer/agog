CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/agogo

$(TARGET): $(OBJ)
	mkdir -p $(BINDIR)
	$(CC) $(OBJ) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean 

clean:
	rm -rf $(OBJDIR) $(BINDIR)



