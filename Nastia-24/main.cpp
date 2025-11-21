#include "tools.h"
#include "company.h"
#include <iostream>
#include <fstream>
using namespace std;


int main()
{
    int command = 9, firm;
    do
    {
        environment::print_names();
        cout << "\nPossible commands are\n-----------------------\n";
        cout
            << "1. Print company to console\n"
            << "2. Load company from file\n"
            << "9. Exit\n";

        cout << "\nInput two numbers: command and company. ";
        cin >> command >> firm;
        if (!environment::check_index(firm)) continue;

        switch (command)
        {
        case 1: environment::print_firm(firm); break;
        case 2: environment::load_firm(firm); break;
        case 9: cout << "\nBye!\n\n"; break;
        default: cout << "Unknown command\n";
            break;
        }
    } while (command != 9);
    return 0;
}
/*testEmpty(); cin.get(); cin.get();
testLoad(); cin.get();
testRunners(); cin.get();

string companies[5] = {"Epam", "LNU", "Google", "UN", "Toyota"};
const int n = 4;
Worker *workers[n] = {
    new Manager("Standard Worker Coder", 19, 40, 35, companies, 2, 5),
    new Subordinate("Subordinate Worker Tester", 19, 40, 35, companies + 2, 1),
    new Manager("Manager Worker PM", 19, 40, 35, companies + 2, 3, 26),
    new Subordinate("Subordinate Data Analyst", 9, 40, 35, companies + 1, 2)};
Firm F("Test", workers, 4);
F.printOn(cout); cin.get();
try
{
    F.loadFromFile("workers.txt");
}
catch (std::runtime_error& e)
{
    cout << "\n The RUNTIME_ERROR: " << e.what();
    cout << "\n --- Problems with parsing of 'workers.txt' occured. See 'errors.txt'\n\n";
}
F.printOn(cout); cin.get();
F.shortPrintOn(cout);*/
