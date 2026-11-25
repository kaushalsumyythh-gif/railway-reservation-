# 🚆 Railway Reservation System (C)

A console-based train ticket reservation system written in **C**. Book tickets, pick your seat, cancel with a refund, and search reservations by PNR. All bookings are saved to a file, so they are still there the next time you run the program.

---

## ✨ Features

- 📋 **Display trains** with route, departure time, fare and live seats left
- 🎫 **Book a ticket** with passenger details and a visual seat map
- ❌ **Cancel a ticket** by PNR (80% of the fare is refunded)
- 🔍 **Search a reservation** by PNR
- 📑 **Display all reservations**
- 💾 **Data persistence** using file handling (`reservations.dat`)
- 🛡️ **Input validation**: invalid numbers, empty input and already-booked seats are all handled

---

## 🧠 C Concepts Used

| Concept | Where it is used |
|---|---|
| **Structures** | `Train` and `Reservation` structs hold train and passenger details |
| **Arrays** | Array of trains, array of reservations and a 2D `seatTaken` array for the seat map |
| **File Handling** | `fopen`, `fread`, `fwrite` and `fclose` save and load reservations in a binary file |
| Functions | One function per feature (`bookTicket`, `cancelTicket`, `searchByPnr`, ...) |
| Loops and switch-case | Menu-driven program flow |

---

## 📁 Project Structure

```
railway-reservation-system/
├── railway_reservation.c   # Complete source code
├── Makefile                # Build shortcuts
├── .gitignore              # Ignores compiled files and saved data
└── README.md               # Project documentation
```

---

## ▶️ How to Run

### Requirements
A C compiler such as **GCC** (on Windows, install [MinGW](https://www.mingw-w64.org/) or use Code::Blocks / Dev-C++).

### Compile and run

```bash
git clone https://github.com/<your-username>/railway-reservation-system.git
cd railway-reservation-system

gcc railway_reservation.c -o railway
./railway          # on Windows: railway.exe
```

Or with `make`:

```bash
make
make run
```

---

## 🖥️ Sample Output

```
=====================================
   RAILWAY RESERVATION SYSTEM
=====================================

1. Display trains
2. Book a ticket
3. Cancel a ticket
4. Search reservation by PNR
5. Display all reservations
6. Exit
Enter your choice (1-6): 1

No.    Train Name         From         To           Dep.   Fare(Rs)  Seats Left
-------------------------------------------------------------------------------
101    Rajdhani Express   Delhi        Mumbai       16:30  1850.00   40
102    Shatabdi Express   Delhi        Chandigarh   07:40  750.00    40
103    Duronto Express    Chennai      Kolkata      22:15  1600.00   40
104    Himalayan Queen    Kalka        Shimla       10:30  250.00    40
105    Garib Rath         Mumbai       Bengaluru    19:00  900.00    40
```

**A booked ticket:**

```
========== TICKET ==========
PNR         : 1001
Passenger   : Rahul Sharma
Age         : 25
Train       : 101 - Rajdhani Express
Route       : Delhi to Mumbai
Departure   : 16:30
Fare        : Rs 1850.00
Seat No.    : 5
============================
```

---

## ⚙️ How It Works

- Each train has **40 seats**. A 2D array (`seatTaken[train][seat]`) tracks which seats are booked.
- Every reservation is stored in a `Reservation` structure with a unique **PNR** starting from 1001.
- After every booking or cancellation, all reservations are written to `reservations.dat`. When the program starts, they are read back and the seat map is rebuilt.
- The train data is sample data stored in the `trains` array in the source code. Add more trains by editing that array (and increasing `MAX_TRAINS`).

---

## 🚀 Future Improvements

- Different coach classes (Sleeper, AC) with different fares
- Waiting list when a train is full
- Multiple travel dates
- Passenger login and booking history
- Search trains by source and destination

---

## 🙏 Acknowledgement

Thank you to **[Sir's Name]** for the guidance and support throughout this project.

---

## 👤 Author

**[Your Name]**
🔗 [LinkedIn](https://www.linkedin.com/in/your-profile) | 💻 [GitHub](https://github.com/your-username)

---

⭐ If you found this project useful, consider giving it a star!
