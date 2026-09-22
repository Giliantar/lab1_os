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
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <filename> <record_count>" << endl;
        return 1;  // Выходим с ошибкой
    }

    string filename = argv[1];
    int count = atoi(argv[2]);

    if (count <= 0) {
        cerr << "Record count must be positive!" << endl;
        return 1;
    }

    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Cannot create file: " << filename << endl;
        return 1;
    }

    employee emp;
    cout << "Enter " << count << " employees." << endl;
    cout << "Format: ID Name Hours " << endl;

    for (int i = 0; i < count; i++) {
        cout << "Employee " << i + 1 << ": ";
        cin >> emp.num >> emp.name >> emp.hours;


        file.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    }

    file.close();  // Закрываем файл
    cout << "Binary file '" << filename << "' created successfully!" << endl;

    return 0;  
}