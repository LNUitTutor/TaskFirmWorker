#include "tools.h"
using namespace std;
void bubble_sort(Worker* workers[], size_t nWorkers)
{
    for (size_t i = nWorkers; i > 1; --i)
    {
        for (size_t j = 1; j < i; ++j)
        {
            if (*workers[j - 1] > *workers[j])
            {
                Worker* to_swap = workers[j - 1];
                workers[j - 1] = workers[j];
                workers[j] = to_swap;
            }
        }
    }
}

void print_workers(const char* title, Worker* workers[], size_t nWorkers, ostream& os)
{
    os << title << "\n\n";
    for (size_t i = 0; i < nWorkers; ++i)
        os << "Worker " << i + 1 << ":\n" << *workers[i] << '\n';
    os << endl;
}


Worker* input_worker()
{
    Worker* worker = nullptr;
    cout << "Enter your choice (w-Worker, s-Subordinate, m-Manager): ";
    char c; 
    (cin >> c).get(); //відразу зчитати ентер
    switch (c)
    {
    /*case 'w': case 'W': worker = new Worker();
        cout << "Input name, years, workhours, standard hours, count of places, places (one by one)\n";
        cin >> *worker;
        break;*/
    case 's': case 'S': worker = new Subordinate();
        cout << "Input name, years, workhours, standard hours, count of places, places (one by one)\n";
        cin >> *worker;
        break;
    case 'm': case 'M': worker = new Manager();
        cout << "Input name, years, workhours, standard hours, count of places, places (one by one), number of subordinates\n";
        cin >> *worker;
        break;
    }
    return worker;
}
