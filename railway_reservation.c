/*
 * Railway Reservation System
 * --------------------------
 * A console-based train ticket reservation program written in C.
 *
 * C concepts used : Arrays, Structures, File Handling
 * Features        : Display trains, book ticket, cancel ticket,
 *                   search by PNR, display all reservations
 * Data storage    : reservations are saved in a binary file (reservations.dat)
 *                   so they are still there the next time you run the program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRAINS  5
#define MAX_SEATS   40
#define MAX_RES     (MAX_TRAINS * MAX_SEATS)
#define NAME_LEN    50
#define TIME_LEN    8
#define DATA_FILE   "reservations.dat"
#define REFUND_RATE 0.80f   /* 80% of the fare is refunded on cancellation */

/* ---------- Structures ---------- */

typedef struct {
    int   id;
    char  name[NAME_LEN];
    char  source[NAME_LEN];
    char  destination[NAME_LEN];
    char  departure[TIME_LEN];
    float fare;
} Train;

typedef struct {
    int  pnr;
    int  trainId;
    int  seatNo;
    int  age;
    char passenger[NAME_LEN];
} Reservation;

/* ---------- Global data (arrays) ---------- */

/* Sample train data. Change or extend this array to add more trains. */
static Train trains[MAX_TRAINS] = {
    {101, "Rajdhani Express",  "Delhi",   "Mumbai",     "16:30", 1850.0f},
    {102, "Shatabdi Express",  "Delhi",   "Chandigarh", "07:40",  750.0f},
    {103, "Duronto Express",   "Chennai", "Kolkata",    "22:15", 1600.0f},
    {104, "Himalayan Queen",   "Kalka",   "Shimla",     "10:30",  250.0f},
    {105, "Garib Rath",        "Mumbai",  "Bengaluru",  "19:00",  900.0f}
};

static Reservation reservations[MAX_RES];
static int resCount = 0;
static int nextPnr  = 1001;
static int seatTaken[MAX_TRAINS][MAX_SEATS + 1];   /* index 1..MAX_SEATS used */

/* ---------- Input helpers ---------- */

static void readLine(const char *prompt, char *buf, size_t size)
{
    for (;;) {
        printf("%s", prompt);
        if (fgets(buf, (int)size, stdin) == NULL) {
            printf("\nInput closed. Exiting.\n");
            exit(0);
        }
        buf[strcspn(buf, "\n")] = '\0';
        if (buf[0] != '\0')
            return;
        printf("  Input cannot be empty. Try again.\n");
    }
}

static int readInt(const char *prompt, int min, int max)
{
    char buf[32];
    char *end;
    long value;

    for (;;) {
        readLine(prompt, buf, sizeof buf);
        value = strtol(buf, &end, 10);
        if (*end == '\0' && value >= min && value <= max)
            return (int)value;
        printf("  Please enter a number between %d and %d.\n", min, max);
    }
}

/* ---------- Lookup helpers ---------- */

static int findTrain(int id)
{
    int i;
    for (i = 0; i < MAX_TRAINS; i++)
        if (trains[i].id == id)
            return i;
    return -1;
}

static int findReservation(int pnr)
{
    int i;
    for (i = 0; i < resCount; i++)
        if (reservations[i].pnr == pnr)
            return i;
    return -1;
}

static int availableSeats(int trainIdx)
{
    int s, count = 0;
    for (s = 1; s <= MAX_SEATS; s++)
        if (!seatTaken[trainIdx][s])
            count++;
    return count;
}

/* ---------- File handling ---------- */

static void rebuildSeats(void)
{
    int i, t;

    memset(seatTaken, 0, sizeof seatTaken);
    nextPnr = 1001;

    for (i = 0; i < resCount; i++) {
        t = findTrain(reservations[i].trainId);
        if (t >= 0 && reservations[i].seatNo >= 1 && reservations[i].seatNo <= MAX_SEATS)
            seatTaken[t][reservations[i].seatNo] = 1;
        if (reservations[i].pnr >= nextPnr)
            nextPnr = reservations[i].pnr + 1;
    }
}

static void loadData(void)
{
    FILE *fp = fopen(DATA_FILE, "rb");

    resCount = 0;
    if (fp != NULL) {
        while (resCount < MAX_RES &&
               fread(&reservations[resCount], sizeof(Reservation), 1, fp) == 1)
            resCount++;
        fclose(fp);
    }
    rebuildSeats();
}

static void saveData(void)
{
    FILE *fp = fopen(DATA_FILE, "wb");

    if (fp == NULL) {
        printf("  Warning: could not save data to %s\n", DATA_FILE);
        return;
    }
    if (resCount > 0)
        fwrite(reservations, sizeof(Reservation), (size_t)resCount, fp);
    fclose(fp);
}

/* ---------- Display functions ---------- */

static void showTrains(void)
{
    int i;

    printf("\n%-6s %-18s %-12s %-12s %-6s %-9s %-9s\n",
           "No.", "Train Name", "From", "To", "Dep.", "Fare(Rs)", "Seats Left");
    printf("-------------------------------------------------------------------------------\n");
    for (i = 0; i < MAX_TRAINS; i++)
        printf("%-6d %-18s %-12s %-12s %-6s %-9.2f %-9d\n",
               trains[i].id, trains[i].name, trains[i].source,
               trains[i].destination, trains[i].departure,
               trains[i].fare, availableSeats(i));
}

static void showSeats(int trainIdx)
{
    int s;

    printf("\nSeat map for %s (XX = booked)\n", trains[trainIdx].name);
    for (s = 1; s <= MAX_SEATS; s++) {
        if (seatTaken[trainIdx][s])
            printf("[XX] ");
        else
            printf("[%2d] ", s);
        if (s % 10 == 0)
            printf("\n");
    }
}

static void printTicket(const Reservation *r)
{
    int t = findTrain(r->trainId);

    printf("\n========== TICKET ==========\n");
    printf("PNR         : %d\n", r->pnr);
    printf("Passenger   : %s\n", r->passenger);
    printf("Age         : %d\n", r->age);
    if (t >= 0) {
        printf("Train       : %d - %s\n", trains[t].id, trains[t].name);
        printf("Route       : %s to %s\n", trains[t].source, trains[t].destination);
        printf("Departure   : %s\n", trains[t].departure);
        printf("Fare        : Rs %.2f\n", trains[t].fare);
    }
    printf("Seat No.    : %d\n", r->seatNo);
    printf("============================\n");
}

static void showAllReservations(void)
{
    int i, t;

    if (resCount == 0) {
        printf("\nNo reservations found.\n");
        return;
    }

    printf("\n%-6s %-20s %-4s %-18s %-5s\n", "PNR", "Passenger", "Age", "Train", "Seat");
    printf("------------------------------------------------------------\n");
    for (i = 0; i < resCount; i++) {
        t = findTrain(reservations[i].trainId);
        printf("%-6d %-20s %-4d %-18s %-5d\n",
               reservations[i].pnr, reservations[i].passenger,
               reservations[i].age,
               t >= 0 ? trains[t].name : "Unknown",
               reservations[i].seatNo);
    }
    printf("\nTotal reservations: %d\n", resCount);
}

/* ---------- Core operations ---------- */

static void bookTicket(void)
{
    int trainId, t, seat, age;
    char name[NAME_LEN];
    Reservation r;

    showTrains();
    trainId = readInt("\nEnter train number (0 to go back): ", 0, 99999);
    if (trainId == 0)
        return;

    t = findTrain(trainId);
    if (t < 0) {
        printf("  Train %d not found.\n", trainId);
        return;
    }
    if (availableSeats(t) == 0) {
        printf("  Sorry, %s is fully booked.\n", trains[t].name);
        return;
    }

    readLine("Passenger name: ", name, sizeof name);
    age = readInt("Passenger age: ", 1, 120);

    showSeats(t);
    for (;;) {
        seat = readInt("\nChoose a seat number: ", 1, MAX_SEATS);
        if (!seatTaken[t][seat])
            break;
        printf("  Seat %d is already booked. Choose another seat.\n", seat);
    }

    r.pnr = nextPnr++;
    r.trainId = trainId;
    r.seatNo = seat;
    r.age = age;
    strncpy(r.passenger, name, NAME_LEN - 1);
    r.passenger[NAME_LEN - 1] = '\0';

    reservations[resCount++] = r;
    seatTaken[t][seat] = 1;
    saveData();

    printf("\nBooking successful!\n");
    printTicket(&r);
}

static void cancelTicket(void)
{
    int pnr, idx, t, i;
    float refund = 0.0f;
    char confirm[8];

    if (resCount == 0) {
        printf("\nThere are no reservations to cancel.\n");
        return;
    }

    pnr = readInt("\nEnter PNR to cancel (0 to go back): ", 0, 99999999);
    if (pnr == 0)
        return;

    idx = findReservation(pnr);
    if (idx < 0) {
        printf("  No reservation found with PNR %d.\n", pnr);
        return;
    }

    printTicket(&reservations[idx]);
    readLine("Cancel this ticket? (y/n): ", confirm, sizeof confirm);
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("  Cancellation aborted.\n");
        return;
    }

    t = findTrain(reservations[idx].trainId);
    if (t >= 0) {
        seatTaken[t][reservations[idx].seatNo] = 0;
        refund = trains[t].fare * REFUND_RATE;
    }

    for (i = idx; i < resCount - 1; i++)
        reservations[i] = reservations[i + 1];
    resCount--;
    saveData();

    printf("\nTicket %d cancelled. Refund amount: Rs %.2f\n", pnr, refund);
}

static void searchByPnr(void)
{
    int pnr, idx;

    pnr = readInt("\nEnter PNR to search (0 to go back): ", 0, 99999999);
    if (pnr == 0)
        return;

    idx = findReservation(pnr);
    if (idx < 0)
        printf("  No reservation found with PNR %d.\n", pnr);
    else
        printTicket(&reservations[idx]);
}

/* ---------- Main ---------- */

int main(void)
{
    int choice;

    loadData();

    printf("=====================================\n");
    printf("   RAILWAY RESERVATION SYSTEM\n");
    printf("=====================================\n");

    do {
        printf("\n1. Display trains\n");
        printf("2. Book a ticket\n");
        printf("3. Cancel a ticket\n");
        printf("4. Search reservation by PNR\n");
        printf("5. Display all reservations\n");
        printf("6. Exit\n");

        choice = readInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1: showTrains();          break;
            case 2: bookTicket();          break;
            case 3: cancelTicket();        break;
            case 4: searchByPnr();         break;
            case 5: showAllReservations(); break;
            case 6: printf("\nThank you for using the Railway Reservation System.\n"); break;
        }
    } while (choice != 6);

    return 0;
}
