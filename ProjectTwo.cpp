//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Steve Goforth
// Course      : CS 300
// Description : ABCU Advising Assistance Program
//============================================================================

#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

// Holds the information for each course.
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Hash table for storing the courses. Chaining handles collisions.
class CourseHashTable {
private:
    vector<list<Course>> buckets;

    size_t hashKey(const string& key) const {
        return hash<string>{}(key) % buckets.size();
    }

public:
    explicit CourseHashTable(size_t bucketCount = 101) : buckets(bucketCount) {}

    void clear() {
        for (auto& bucket : buckets) {
            bucket.clear();
        }
    }

    void insert(const Course& course) {
        size_t index = hashKey(course.courseNumber);

        // If the course is already here, just update it.
        for (Course& existing : buckets[index]) {
            if (existing.courseNumber == course.courseNumber) {
                existing = course;
                return;
            }
        }

        buckets[index].push_back(course);
    }

    bool search(const string& courseNumber, Course& result) const {
        size_t index = hashKey(courseNumber);

        for (const Course& course : buckets[index]) {
            if (course.courseNumber == courseNumber) {
                result = course;
                return true;
            }
        }
        return false;
    }

    vector<Course> getAllCourses() const {
        vector<Course> courses;

        for (const auto& bucket : buckets) {
            for (const Course& course : bucket) {
                courses.push_back(course);
            }
        }
        return courses;
    }
};

// Clean up any extra spaces around the text.
string trim(const string& value) {
    size_t first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) {
        return "";
    }

    size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

// Make course numbers uppercase so searches are consistent.
string toUpper(string value) {
    transform(value.begin(), value.end(), value.begin(),
              [](unsigned char ch) { return static_cast<char>(toupper(ch)); });
    return value;
}

// Break a line from the CSV into separate pieces.
vector<string> splitCsvLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream stream(line);

    while (getline(stream, field, ',')) {
        fields.push_back(trim(field));
    }

    // Keep an empty last field if the line ends with a comma.
    if (!line.empty() && line.back() == ',') {
        fields.push_back("");
    }

    return fields;
}

// Load the course file, check the data, and add it to the hash table.
bool loadCourseData(const string& fileName, CourseHashTable& courseTable) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Unable to open " << fileName << "." << endl;
        return false;
    }

    vector<Course> temporaryCourses;
    unordered_set<string> courseNumbers;
    string line;
    int lineNumber = 0;

    while (getline(inputFile, line)) {
        ++lineNumber;

        if (trim(line).empty()) {
            continue;
        }

        vector<string> fields = splitCsvLine(line);

        if (fields.size() < 2 || fields[0].empty() || fields[1].empty()) {
            cout << "Error: Invalid course data on line " << lineNumber << "." << endl;
            return false;
        }

        Course course;
        course.courseNumber = toUpper(fields[0]);
        course.courseTitle = fields[1];

        if (courseNumbers.count(course.courseNumber) > 0) {
            cout << "Error: Duplicate course number " << course.courseNumber << "." << endl;
            return false;
        }

        for (size_t i = 2; i < fields.size(); ++i) {
            if (!fields[i].empty()) {
                course.prerequisites.push_back(toUpper(fields[i]));
            }
        }

        courseNumbers.insert(course.courseNumber);
        temporaryCourses.push_back(course);
    }

    inputFile.close();

    if (temporaryCourses.empty()) {
        cout << "Error: The course data file is empty." << endl;
        return false;
    }

    // Once everything is loaded, make sure each prerequisite is a real course.
    for (const Course& course : temporaryCourses) {
        for (const string& prerequisite : course.prerequisites) {
            if (courseNumbers.count(prerequisite) == 0) {
                cout << "Error: Prerequisite " << prerequisite
                     << " for " << course.courseNumber
                     << " does not exist in the file." << endl;
                return false;
            }
        }
    }

    courseTable.clear();
    for (const Course& course : temporaryCourses) {
        courseTable.insert(course);
    }

    cout << "Course data loaded successfully." << endl;
    return true;
}

// Print the full course list in order by course number.
void printCourseList(const CourseHashTable& courseTable) {
    vector<Course> courses = courseTable.getAllCourses();

    sort(courses.begin(), courses.end(),
         [](const Course& left, const Course& right) {
             return left.courseNumber < right.courseNumber;
         });

    cout << "Here is a sample schedule:" << endl;

    for (const Course& course : courses) {
        cout << course.courseNumber << ", " << course.courseTitle << endl;
    }
}

// Print the course the user searched for and its prerequisites.
void printCourse(const CourseHashTable& courseTable, string courseNumber) {
    courseNumber = toUpper(trim(courseNumber));

    Course course;
    if (!courseTable.search(courseNumber, course)) {
        cout << "Course " << courseNumber << " not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    } else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i + 1 < course.prerequisites.size()) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// Show the main menu.
void printMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
}

int main() {
    CourseHashTable courseTable;
    bool dataLoaded = false;
    string input;

    cout << "Welcome to the course planner." << endl;

    while (true) {
        printMenu();
        cout << "What would you like to do? ";
        getline(cin, input);

        int choice;
        try {
            size_t processed = 0;
            choice = stoi(trim(input), &processed);

            if (processed != trim(input).size()) {
                throw invalid_argument("extra characters");
            }
        } catch (...) {
            cout << input << " is not a valid option." << endl;
            continue;
        }

        if (choice == 1) {
            cout << "What file would you like to load? ";
            string fileName;
            getline(cin, fileName);
            fileName = trim(fileName);

            if (fileName.empty()) {
                cout << "Error: A file name is required." << endl;
                dataLoaded = false;
            } else {
                dataLoaded = loadCourseData(fileName, courseTable);
            }
        } else if (choice == 2) {
            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;
            } else {
                printCourseList(courseTable);
            }
        } else if (choice == 3) {
            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;
            } else {
                cout << "What course do you want to know about? ";
                string courseNumber;
                getline(cin, courseNumber);
                printCourse(courseTable, courseNumber);
            }
        } else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
            break;
        } else {
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
