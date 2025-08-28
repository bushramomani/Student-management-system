# Student-management-system (C++)
a simple **console based** CRUD app for managing student records

## Features 
1. add, list, search (by ID or name), update, and delete students
2. data saved to 'students.csv' (loads on start, saves on exit)
3. input validation and friendly UI

## Build and run
```bash
#mac/linux
g++ -std=gnu++17 -O2 -0 sms main.cpp
./sms

#windows(mingw)
g++ -std=c++17 -O2 -o sms.exe main.cpp
sms.exe
