/*
 * Calculator.cpp
 *
 * Date:1.17.2026
 * Author: S. Goforth
 */

#include <iostream>
using namespace std;

// main should return int in C++
int main() {

    // Use double so calculator works with ints or decimals
    double op1 = 0.0, op2 = 0.0;

    // Holds the operator 
    char operation = '\0';

    // Use char for Y/N input and start as Y so loop runs
    char answer = 'Y';

    // Loop continues while user enters Y or y
    while (answer == 'Y' || answer == 'y') {

        cout << "Enter expression (example: 2 + 2): " << endl;

        // Read first number, operator, second number
        cin >> op1 >> operation >> op2;

        // Perform operation
        if (operation == '+') {
            cout << op1 << " + " << op2 << " = " << (op1 + op2) << endl;
        }
        else if (operation == '-') {
            cout << op1 << " - " << op2 << " = " << (op1 - op2) << endl;
        }
        else if (operation == '*') {
            cout << op1 << " * " << op2 << " = " << (op1 * op2) << endl;
        }
        else if (operation == '/') {

            // Prevent division by zero
            if (op2 == 0) {
                cout << "Error: Division by zero is not allowed." << endl;
            }
            else {
                cout << op1 << " / " << op2 << " = " << (op1 / op2) << endl;
            }
        }
        else {
            // invalid operators
            cout << "Error: Invalid operator." << endl;
        }

        cout << "Do you wish to evaluate another expression? (Y/N): " << endl;
        cin >> answer;
    }

    // End message 
    cout << "Program Finished." << endl;

    return 0;
}