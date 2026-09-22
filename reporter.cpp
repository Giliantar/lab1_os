
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <binary_file> <report_file> <hourly_rate>" << endl;
        return 1;
    }

    string binary_file = argv[1];
    string report_file = argv[2];
    double rate = atof(argv[3]);

    if (rate <= 0) {
        cerr << "Hourly rate must be positive!" << endl;
        return 1;
    }

    ifstream bin_file(binary_file, ios::binary);
    if (!bin_file) {
        cerr << "Cannot open binary file: " << binary_file << endl;
        return 1;
    }

    ofstream rep_file(report_file);
    if (!rep_file) {
        cerr << "Cannot create report file: " << report_file << endl;
        bin_file.close();
        return 1;
    }

    rep_file << "Report on file \"" << binary_file << "\"" << endl;

    rep_file << "Employee ID, Name, Hours, Salary" << endl;

    employee emp;
    double salary;

    while (bin_file.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        salary = emp.hours * rate;

        rep_file << emp.num << ", " << emp.name << ", "
                 << emp.hours << ", " << salary << endl;
    }

    bin_file.close();
    rep_file.close();

    cout << "Report '" << report_file << "' generated successfully!" << endl;

    return 0;
}