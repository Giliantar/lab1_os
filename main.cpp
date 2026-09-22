#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

void printBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);

    employee emp;

    while (file.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        cout << "ID: " << emp.num
             << ", Name: " << emp.name
             << ", Hours: " << emp.hours << endl;
    }

    file.close();
}

void printReport(const string& filename) {
    ifstream file(filename);

    string line;

    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

int runProgram(const string& program, char* const args[]) {
    pid_t pid = fork();

    if (pid == -1) {
        cerr << "fork failed" << endl;
        return -1;
    }

    if (pid == 0) {
        execvp(program.c_str(), args);
        cerr << "cannot execute " << program << endl;
        exit(1);
    }
    else {

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            cout << program << " finished with code " << exit_code << endl;
            return exit_code;
        }
        else {
            cerr << "[MAIN] " << program << " terminated abnormally" << endl;
            return -1;
        }
    }
}

int main() {
    string binary_file;
    string report_file;
    int record_count;
    double hourly_rate;

    cout << "Enter binary file name: ";
    cin >> binary_file;

    cout << "Enter number of records: ";
    cin >> record_count;

    cout << endl;

    string count_str = to_string(record_count);
    char* creator_args[] = {
        (char*)"./creator",
        (char*)binary_file.c_str(),
        (char*)count_str.c_str(),
        NULL                                
    };

    int result = runProgram("./creator", creator_args);
    if (result != 0) {
        cerr << "creator failed" << endl;
        return 1;
    }

    printBinaryFile(binary_file);

    cout << "Enter report file name: ";
    cin >> report_file;

    cout << "Enter hourly rate ";
    cin >> hourly_rate;

    cout << endl;

    string rate_str = to_string(hourly_rate);
    char* reporter_args[] = {
        (char*)"./reporter",
        (char*)binary_file.c_str(),
        (char*)report_file.c_str(),
        (char*)rate_str.c_str(),
        NULL
    };

    result = runProgram("./reporter", reporter_args);
    if (result != 0) {
        cerr << "[MAIN] Reporter failed! Exiting..." << endl;
        return 1;
    }

    printReport(report_file);

    return 0;
}