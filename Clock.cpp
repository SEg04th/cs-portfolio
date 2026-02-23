/*
  Name: Steven Goforth
  Course: CS-210
  Project One: Chada Tech Clocks
*/

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// Function prototypes
void displayMenu();
void displayClocks(int hours, int minutes, int seconds);
void addHour(int& hours);
void addMinute(int& hours, int& minutes);
void addSecond(int& hours, int& minutes, int& seconds);

int main() {
    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    // get start time
    cout << "Enter initial time.\n";
    cout << "Hours (0-23): ";
    cin >> hours;
    cout << "Minutes (0-59): ";
    cin >> minutes;
    cout << "Seconds (0-59): ";
    cin >> seconds;

    // fix bad input
    if (hours < 0) hours = 0;
    if (hours > 23) hours = hours % 24;
    if (minutes < 0) minutes = 0;
    if (minutes > 59) minutes = minutes % 60;
    if (seconds < 0) seconds = 0;
    if (seconds > 59) seconds = seconds % 60;

    int choice = 0;

    // menu loop
    while (choice != 4) {
        displayClocks(hours, minutes, seconds);
        displayMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            addHour(hours);
        }
        else if (choice == 2) {
            addMinute(hours, minutes);
        }
        else if (choice == 3) {
            addSecond(hours, minutes, seconds);
        }
        else if (choice == 4) {
            cout << "Exiting program.\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}

void displayMenu() {
    cout << "\n**************************\n";
    cout << "* 1 - Add One Hour       *\n";
    cout << "* 2 - Add One Minute     *\n";
    cout << "* 3 - Add One Second     *\n";
    cout << "* 4 - Exit Program       *\n";
    cout << "**************************\n";
}

void displayClocks(int hours, int minutes, int seconds) {
    // 24 hour time
    int hour24 = hours;

    // 12 hour time
    int hour12 = hour24 % 12;
    if (hour12 == 0) {
        hour12 = 12;
    }

    // am or pm
    string amPm = (hour24 >= 12) ? "PM" : "AM";

    // show clocks
    cout << "\n**************************   **************************\n";
    cout << "*      12-Hour Clock     *   *      24-Hour Clock     *\n";

    cout << "*      "
         << setw(2) << setfill('0') << hour12 << ":"
         << setw(2) << setfill('0') << minutes << ":"
         << setw(2) << setfill('0') << seconds << " " << amPm
         << "      *   *      "
         << setw(2) << setfill('0') << hour24 << ":"
         << setw(2) << setfill('0') << minutes << ":"
         << setw(2) << setfill('0') << seconds
         << "      *\n";

    cout << "**************************   **************************\n";

    // reset fill
    cout << setfill(' ');
}

void addHour(int& hours) {
    // hour rollover
    hours = (hours + 1) % 24;
}

void addMinute(int& hours, int& minutes) {
    // minute up
    minutes++;

    if (minutes >= 60) {
        minutes = 0;
        addHour(hours);
    }
}

void addSecond(int& hours, int& minutes, int& seconds) {
    // second up
    seconds++;

    if (seconds >= 60) {
        seconds = 0;
        addMinute(hours, minutes);
    }
}