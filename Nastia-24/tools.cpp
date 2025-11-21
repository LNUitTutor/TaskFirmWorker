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
    cout << "Enter your choice (s-Subordinate, m-Manager): ";
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

void testEmpty()
{
    // новостворена фірма порожня, доступ до працівників мав би закінчитися винятком
    Firm F;
    F.printOn(cout);
    F.storeOn("empty.txt");
    try
    {
        cout << "first worker: " << F.first() << '\n';
    }
    catch (Firm::BadIndex& ex)
    {
        cout << "try get first ERROR: " << ex.what() << " with index " << ex.index << '\n';
    }
    try
    {
        cout << "last worker: " << F.last() << '\n';
    }
    catch (Firm::BadIndex& ex)
    {
        cout << "try get last ERROR: " << ex.what() << " with index " << ex.index << '\n';
    }

    // Прийняли на фірму двох працівників, повторили ті ж дії
    F.set_name("Two Geniuses");
    Worker* worker = input_worker();
    F.addWorker(*worker);
    delete worker;
    worker = input_worker();
    F.addWorker(*worker);
    delete worker;
    cout << "\n\n After two addition\n\n";
    F.printOn(cout);
    F.storeOn("two_workers.txt");
    try
    {
        cout << "first worker: " << F.first() << '\n';
    }
    catch (Firm::BadIndex& ex)
    {
        cout << "try get first ERROR: " << ex.what() << " with index " << ex.index << '\n';
    }
    try
    {
        cout << "last worker: " << F.last() << '\n';
    }
    catch (Firm::BadIndex& ex)
    {
        cout << "try get last ERROR: " << ex.what() << " with index " << ex.index << '\n';
    }
}

void testLoad()
{
    Firm F("Load Files Co");
    char filename[80] = { 0 };
 loop:
    cout << "\nInput a name of file to load the firm. Possible names are:\n"
        << "'empty.txt', 'two_workers.txt', 'workers.txt': ";
    (cin >> filename).get();
    try
    {
        F.loadFromFile(filename);
        F.printOn(cout);
    }
    catch (Firm::FileNotFound& ex)
    {
        cout << " *** ERROR : " << ex.what()
            << "\n issue occured with the path '" << ex.path << "'\n";
    }
    catch (std::runtime_error& ex)
    {
        cout << " *** ERROR : " << ex.what()
            << "\n See the 'errors.txt' file.\n\n";
        F.printOn(cout);
    }
    cout << "Do you want to repeat (y/n)? ";
    char answer; (cin >> answer).get();
    if (answer == 'y' || answer == 'Y') goto loop;
}

void testRunners()
{
    string companies[7] = { "Epam", "LNU", "Google", "UN", "Toyota", "HP", "Electron"};
    Worker* workers[5] = {
        new Manager("Manager Worker Runner", 19, 40, 35, companies, 7, 5),
        new Subordinate("Subordinate Worker Tester", 19, 40, 35, companies + 2, 1),
        new Manager("Manager Worker PM", 19, 40, 35, companies + 2, 3, 26),
        new Subordinate("Subordinate Data Analyst", 9, 40, 35, companies + 1, 2),
        new Subordinate("Subordinate Frontend Designer", 19, 36, 36, companies, 6)
    };
    Firm F("Test", workers, 5);
    F.printOn(cout);
    cout << "\n --- The best runner is:\n" << F.mostMoveableWorker() << '\n';
    int* runners = nullptr;
    int places = 8;
    int count_runners = F.getMoveableWorkers(places, runners);
    if (count_runners == 0)
        cout << " --- There are not workers in the company " << F.get_name()
        << " that change more then " << places << " places\n\n";
    else
    {
        cout << " --- Workers of the company " << F.get_name()
            << " that change more then " << places << " places are:\n";
        for (int i = 0; i < count_runners; ++i)
            cout << F[runners[i]] << '\n';
        delete[] runners;
    }
    places = 5;
    count_runners = F.getMoveableWorkers(places, runners);
    if (count_runners == 0)
        cout << " --- There are not workers in the company " << F.get_name()
        << " that change more then " << places << " places\n\n";
    else
    {
        cout << " --- Workers of the company " << F.get_name()
            << " that change more then " << places << " places are:\n";
        for (int i = 0; i < count_runners; ++i)
            cout << F[runners[i]] << '\n';
        delete[] runners;
    }
}

namespace environment
{
    // Тестові дані для формування фірм за допомогою конструкторів
    string companies[7] = { "Epam", "LNU", "Google", "UN", "Toyota", "HP", "Electron" };
    Worker* workers[5] = {
        new Manager("Manager Worker Runner", 19, 40, 35, companies, 7, 5),
        new Subordinate("Subordinate Worker Tester", 19, 40, 35, companies + 2, 1),
        new Manager("Manager Worker PM", 19, 40, 35, companies + 2, 3, 26),
        new Subordinate("Subordinate Data Analyst", 9, 40, 35, companies + 1, 2),
        new Subordinate("Subordinate Frontend Designer", 19, 36, 36, companies, 6)
    };
    Worker* subordinates[3] = { workers[1], workers[3],
        new Subordinate("Subordinate Worker Coder", 1, 40, 40, companies, 0) };

    const int n = 5;
    Firm firms[n] = { Firm("Startup"), Firm("Little_Co", 2),
        Firm("Runners", workers, 5), Firm("Headless", subordinates, 3) };

    bool check_index(int index)
    {
        if (index < 0 || index >= n)
        {
            cout << "Error :: Wrong index " << index << '\n';
            return false;
        }
        return true;
    }

    void print_names()
    {
        cout << "\n  COMPANIES:\n--------------------\n";
        for (int i = 0; i < n; ++i)
            cout << i << " - " << firms[i].get_name() << '\n';
    }

    void print_firm(int index)
    {
        int answer;
        cout << "What kind of output do you want (1-short, 2-detailed)? ";
        (cin >> answer).get(); cout << '\n';
        switch (answer)
        {
        case 1: firms[index].shortPrintOn(cout); break;
        case 2: firms[index].printOn(cout); break;
        default: cout << "Unrecognized answer " << answer << '\n';
            break;
        }
    }

    void load_firm(int index)
    {
        char filename[80] = { 0 };
        cout << "\nInput a name of file to load the firm. Possible names are:\n"
            << "'empty.txt', 'two_workers.txt', 'workers.txt' or other: ";
        (cin >> filename).get();
        try
        {
            firms[index].loadFromFile(filename);
            cout << "The firm " << firms[index].get_name() << " loaded successfully\n";
        }
        catch (Firm::FileNotFound& ex)
        {
            cout << " *** ERROR : " << ex.what()
                << "\n issue occured with the path '" << ex.path << "'\n";
        }
        catch (std::runtime_error& ex)
        {
            cout << " *** ERROR : " << ex.what()
                << "\n See the 'errors.txt' file.\n\n";
        }
    }

    void add_worker(int index)
    {
        Worker* worker = input_worker();
        firms[index].addWorker(*worker);
        delete worker;
    }

}