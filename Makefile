# Numele fișierului executabil
EXECUTABLE := my_octave

# Compiler și opțiuni de compilare
CC := gcc
CFLAGS := -Wall -Wextra -g

# Fișiere sursă și obiect
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)

# Regula implicită
all: build

# Regula de build specifică pentru scriptul 'check'
build: $(EXECUTABLE)

# Regula pentru construirea executabilului
$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regula pentru ștergerea fișierelor generate
clean:
	rm -f $(OBJ) $(EXECUTABLE)

.PHONY: all build clean
