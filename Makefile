CC     = gcc
CFLAGS = -std=c99 -Wall -Wextra

all: railway

railway: railway_reservation.c
	$(CC) $(CFLAGS) -o railway railway_reservation.c

run: railway
	./railway

clean:
	rm -f railway railway.exe reservations.dat

.PHONY: all run clean
