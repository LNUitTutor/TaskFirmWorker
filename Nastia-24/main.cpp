#include "tools.h"
#include "company.h"
#include <iostream>
#include <fstream>
using namespace std;


int main()
{
    testEmpty(); cin.get();
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
    F.shortPrintOn(cout);
    return 0;
}