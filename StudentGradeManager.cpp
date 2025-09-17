// StudentGradeManager.cpp
// A simple Student Grade Management System demonstrating:
// variables, expressions, conditionals, loops, functions, classes,
// STL containers (vector, map), file I/O, inheritance & virtual functions,
// and dynamic allocation with new/delete.

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// ---------- Helpers ----------
vector<string> split(const string& s, char delim) {
    vector<string> elems;
    string item;
    stringstream ss(s);
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// ---------- Classes & Inheritance ----------

class Person {
protected:
    string name;
    int id;
public:
    Person(const string& name_, int id_) : name(name_), id(id_) {}
    virtual ~Person() {}
    virtual void displayInfo() const {
        cout << "ID: " << id << " | Name: " << name << '\n';
    }
    int getId() const { return id; }
    string getName() const { return name; }
};

class Student : public Person {
protected:
    map<string, double> grades; // subject -> grade
public:
    Student(const string& name_, int id_) : Person(name_, id_) {}
    virtual ~Student() {}

    virtual void displayInfo() const override {
        cout << "ID: " << id << " | Name: " << name;
        if (!grades.empty()) {
            cout << " | Average: " << fixed << setprecision(2) << average();
        } else {
            cout << " | Average: N/A";
        }
        cout << '\n';

        if (!grades.empty()) {
            cout << "  Grades:\n";
            for (const auto& kv : grades) {
                cout << "    " << kv.first << " : " << kv.second << '\n';
            }
        }
    }

    void addGrade(const string& subject, double grade) {
        grades[subject] = grade; // add or update
    }

    double average() const {
        if (grades.empty()) return 0.0;
        double sum = 0.0;
        for (const auto& kv : grades) sum += kv.second;
        return sum / grades.size();
    }

    string serializeGrades() const {
        ostringstream oss;
        bool first = true;
        for (const auto& kv : grades) {
            if (!first) oss << ",";
            oss << kv.first << ":" << kv.second;
            first = false;
        }
        return oss.str();
    }

    void deserializeGrades(const string& s) {
        grades.clear();
        if (s.empty()) return;
        auto items = split(s, ',');
        for (const auto& it : items) {
            auto pair = split(it, ':');
            if (pair.size() == 2) {
                string subj = trim(pair[0]);
                try {
                    double g = stod(trim(pair[1]));
                    grades[subj] = g;
                } catch (...) {
                    // skip invalid numbers
                }
            }
        }
    }

    // Extra info for save/load (empty for regular Student)
    virtual string extraInfo() const { return ""; }
    virtual string getType() const { return "STU"; }
};

class GraduateStudent : public Student {
private:
    string thesisTitle;
public:
    GraduateStudent(const string& name_, int id_, const string& thesis_)
        : Student(name_, id_), thesisTitle(thesis_) {}

    virtual void displayInfo() const override {
        cout << "ID: " << id << " | Name: " << name << " (Graduate Student)";
        if (!grades.empty()) {
            cout << " | Average: " << fixed << setprecision(2) << average();
        } else {
            cout << " | Average: N/A";
        }
        cout << '\n';
        cout << "  Thesis: " << thesisTitle << '\n';
        if (!grades.empty()) {
            cout << "  Grades:\n";
            for (const auto& kv : grades) {
                cout << "    " << kv.first << " : " << kv.second << '\n';
            }
        }
    }

    virtual string extraInfo() const override { return thesisTitle; }
    virtual string getType() const override { return "GRD"; }
};

// ---------- Program functions ----------

int findIndexById(const vector<Student*>& students, int id) {
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i]->getId() == id) return static_cast<int>(i);
    }
    return -1;
}

void addStudent(vector<Student*>& students) {
    cout << "Add Student - Regular (1) or Graduate (2)? ";
    int t;
    while (!(cin >> t) || (t != 1 && t != 2)) {
        cout << "Enter 1 (Regular) or 2 (Graduate): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter student name: ";
    string name;
    getline(cin, name);

    cout << "Enter numeric ID: ";
    int id;
    while (!(cin >> id)) {
        cout << "Invalid. Enter numeric ID: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (findIndexById(students, id) != -1) {
        cout << "A student with ID " << id << " already exists. Cancelled.\n";
        return;
    }

    if (t == 1) {
        Student* s = new Student(name, id); // dynamic allocation
        students.push_back(s);
        cout << "Regular student added.\n";
    } else {
        cout << "Enter thesis title: ";
        string thesis;
        getline(cin, thesis);
        Student* s = new GraduateStudent(name, id, thesis); // dynamic allocation
        students.push_back(s);
        cout << "Graduate student added.\n";
    }
}

void removeStudent(vector<Student*>& students) {
    cout << "Enter ID of student to remove: ";
    int id;
    while (!(cin >> id)) {
        cout << "Invalid. Enter numeric ID: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int idx = findIndexById(students, id);
    if (idx == -1) {
        cout << "Student not found.\n";
        return;
    }
    delete students[idx];             // free memory
    students.erase(students.begin() + idx); // remove from vector
    cout << "Student removed and memory freed.\n";
}

void addOrUpdateGrade(vector<Student*>& students) {
    cout << "Enter student ID: ";
    int id;
    while (!(cin >> id)) {
        cout << "Invalid. Enter numeric ID: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int idx = findIndexById(students, id);
    if (idx == -1) {
        cout << "Student not found.\n";
        return;
    }
    cout << "Enter subject name: ";
    string subject;
    getline(cin, subject);
    cout << "Enter grade (numeric): ";
    double grade;
    while (!(cin >> grade)) {
        cout << "Invalid. Enter numeric grade: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    students[idx]->addGrade(subject, grade);
    cout << "Grade saved.\n";
}

void displayAll(const vector<Student*>& students) {
    if (students.empty()) {
        cout << "No students to display.\n";
        return;
    }
    cout << "=== All Students ===\n";
    for (const Student* s : students) {
        s->displayInfo(); // virtual call -> derived override invoked if any
        cout << "-------------------\n";
    }
}

bool saveToFile(const string& filename, const vector<Student*>& students) {
    ofstream fout(filename);
    if (!fout) return false;
    for (const Student* s : students) {
        // Format: TYPE|id|name|extra|subject:grade,subject2:grade2
        fout << s->getType() << "|" << s->getId() << "|" << s->getName()
             << "|" << s->extraInfo() << "|" << s->serializeGrades() << "\n";
    }
    return true;
}

bool loadFromFile(const string& filename, vector<Student*>& students) {
    ifstream fin(filename);
    if (!fin) return false;

    // Clear existing students (free memory) before loading
    for (Student* s : students) delete s;
    students.clear();

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        auto parts = split(line, '|');
        // expect at least 5 parts; handle missing gracefully
        if (parts.size() < 5) continue;
        string type = parts[0];
        int id = 0;
        try { id = stoi(parts[1]); } catch (...) { continue; }
        string name = parts[2];
        string extra = parts[3];
        string gradesStr = parts[4];

        if (type == "GRD") {
            Student* s = new GraduateStudent(name, id, extra);
            s->deserializeGrades(gradesStr);
            students.push_back(s);
        } else { // STU or unknown -> treat as Student
            Student* s = new Student(name, id);
            s->deserializeGrades(gradesStr);
            students.push_back(s);
        }
    }
    return true;
}

void showPassFail(const vector<Student*>& students) {
    if (students.empty()) {
        cout << "No students available.\n";
        return;
    }
    cout << "Enter cutoff percentage for pass (e.g., 50): ";
    double cutoff;
    while (!(cin >> cutoff)) {
        cout << "Invalid. Enter numeric cutoff: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Pass/Fail report (cutoff = " << cutoff << ")\n";
    for (const Student* s : students) {
        double avg = s->average();
        cout << "ID: " << s->getId() << " | Name: " << s->getName()
             << " | Average: " << fixed << setprecision(2) << avg
             << " | " << (avg >= cutoff ? "PASS" : "FAIL") << '\n';
    }
}

// ---------- Main ----------
int main() {
    vector<Student*> students;
    const string defaultFile = "students_db.txt";

    cout << "Welcome to Student Grade Manager\n";
    // Optionally load existing data
    if (ifstream(defaultFile)) {
        cout << "Load saved data from " << defaultFile << "? (y/n): ";
        char c; cin >> c; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (c == 'y' || c == 'Y') {
            if (loadFromFile(defaultFile, students)) {
                cout << "Data loaded from file.\n";
            } else {
                cout << "Failed to load file or file corrupted.\n";
            }
        }
    }

    while (true) {
        cout << "\nMenu:\n"
             << "1. Add Student\n"
             << "2. Remove Student\n"
             << "3. Add/Update Grade\n"
             << "4. Display All Students\n"
             << "5. Save to file\n"
             << "6. Load from file (will replace current in-memory data)\n"
             << "7. Show Pass/Fail report\n"
             << "0. Exit (saves automatically)\n"
             << "Choice: ";

        int choice;
        if (!(cin >> choice)) {
            cout << "Invalid input. Try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) {
            cout << "Saving to " << defaultFile << " ... ";
            if (saveToFile(defaultFile, students)) cout << "Done.\n";
            else cout << "Failed to save file.\n";
            break;
        }

        switch (choice) {
            case 1: addStudent(students); break;
            case 2: removeStudent(students); break;
            case 3: addOrUpdateGrade(students); break;
            case 4: displayAll(students); break;
            case 5:
                {
                    cout << "Enter filename to save (or press Enter for default '" << defaultFile << "'): ";
                    string fname;
                    getline(cin, fname);
                    if (fname.empty()) fname = defaultFile;
                    if (saveToFile(fname, students)) cout << "Saved to " << fname << '\n';
                    else cout << "Failed to save to " << fname << '\n';
                }
                break;
            case 6:
                {
                    cout << "Enter filename to load (or press Enter for default '" << defaultFile << "'): ";
                    string fname;
                    getline(cin, fname);
                    if (fname.empty()) fname = defaultFile;
                    if (loadFromFile(fname, students)) cout << "Loaded from " << fname << '\n';
                    else cout << "Failed to load from " << fname << '\n';
                }
                break;
            case 7: showPassFail(students); break;
            default: cout << "Unknown choice.\n"; break;
        }
    }

    // cleanup
    for (Student* s : students) delete s;
    students.clear();

    cout << "Goodbye!\n";
    return 0;
}
