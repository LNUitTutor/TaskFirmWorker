#include "worker.h"
#include <sstream>
#include <string>

// конструктор без параметрів
Places::Places()
{
    reserved = 5;
    used = 0;
    titles = new string[reserved];
}

// конструктор з параметрами вказівник на список назв і кількість
Places::Places(string *a, int n) : reserved(n), used(n) // список ініціалізації
{
    titles = new string[reserved];
    for (int i = 0; i < used; ++i)
        titles[i] = a[i]; // копіюємо всі рядки з a в titles
}

// конструктор копіювання
Places::Places(const Places &other)
{
    this->reserved = other.reserved;
    this->used = other.used;
    this->titles = new string[other.reserved]; // написати явно бо попередні ше не збереглися
    for (int i = 0; i < other.used; ++i)
        this->titles[i] = other.titles[i];
}

// перевизначення =
// треба у конструкторі копіювання для Worker
Places &Places::operator=(const Places &other)
// повертає & на поточний об'єкт (a = b = c)
{
    if (this != &other) // a = a
    {
        delete[] this->titles; // видаляємо елементи з titles
        this->reserved = other.reserved;
        this->used = other.used;
        this->titles = new string[other.reserved];
        for (int i = 0; i < other.used; ++i)
            this->titles[i] = other.titles[i];
    }
    return *this;
}

// перевизначення [] - доступ до/заміна елемента Places по індексу
string &Places::operator[](int n)
{
    if (n < 0 || n >= used) // виняток - якщо непр n
        throw n;
    return this->titles[n]; // & на елемент
}

// додати новий об'єкт в масив місць
void Places::addPlace(string t)
{
    if (used == reserved) // якщо місць не вистачає
    {
        reserved += 5;
        string *mem = new string[reserved];
        for (int i = 0; i < used; ++i) // в новий список перекопійовуємо старі
            mem[i] = titles[i];
        delete[] titles;
        titles = mem; // старому даємо вказівник на новий
    }
    titles[used++] = t;
}

void Places::printOn(ostream &os) const
{
    os << "Places of work (" << used << "): \n";
    for (int i = 0; i < used; ++i)
        os << " - " << titles[i] << '\n'; 
}

void Places::readFrom(istream &is)
{
    // спочатку читають розмір масиву
    // потім читають і додають по одному елементу
    int n;
    is >> n;
    is.get();       // взяти ентер після числа
    this->used = 0; // щоб перезаписалися старі значення якщо вони є
    for (int i = 0; i < n; ++i)
    {
        string title;
        getline(is, title); // зчитати з is записати в title
        this->addPlace(title);
    }
}

// введення і виведення в потоки (консоль, файл)
ostream &operator<<(ostream &os, const Places &p)
{
    p.printOn(os);
    return os;
}

istream &operator>>(istream &is, Places &p)
{
    p.readFrom(is);
    return is;
}

// ------------------- Worker ------------------------------------------------------------

// статична змінна - норма оплати за годину
double Worker::PayPerHour = 7.5;

// конструктор без параметрів
Worker::Worker()
{
    name = "Name Un Known";
    years = 0;
    workhours = 0;
    standardhours = 0;
    Workplaces = Places();
}

// конструктор зі всіма параметрами
Worker::Worker(string n, int y, int w, int s, string *t, int k) : name(n), Workplaces(t, k)
{
    years = y > 0 ? y : 0;
    workhours = w > 0 ? w : 0;
    standardhours = s > 0 ? s : 0;
}

// конструктор копіювання
Worker::Worker(const Worker &other)
{
    this->name = other.name;
    this->years = other.years;
    this->workhours = other.workhours;
    this->standardhours = other.standardhours;
    this->Workplaces = other.Workplaces; // викликається перевизначене "=" Places
}


void Worker::printOn(ostream &os) const
{
    os << this->name << "\n  "
       << this->years << " years of experience\n  "
       << this->workhours << " working hours\n  "
       << "(standard hours: " << this->standardhours << ")\n  "
       << "current salary: " << this->salary() << "$\n"
       << this->Workplaces;
}

void Worker::readFrom(istream &is)
{
    getline(is, name); //зчитати рядок з пробілами
    int y, w, s;
    is >> y >> w >> s;
    setYears(y); setWorkhours(w); setStandardhours(s); //безпечніше
    is >> Workplaces;
}

string Worker::getShorterName() const
{
    int pos = name.find(' '); // закінчення прізвища, pos+1 - початок імені
    pos += 2;
    string result = name.substr(0, pos); // вирізати до другої букви після ініціалу не включно
    pos = name.find(' ', pos);           // закінчення імені
    return result + ". " + name[pos + 1] + '.';
}

Worker* Worker::Parse(string line)
{
    /* Вважати, що інформація про конкретні об’єкти задана у вигляді одного рядка
       для однієї сутності. 
       На початку кожного рядка заданий символ-маркер, що визначає тип об’єкта.
    */
    string surname, firstname, lastname, place;
    int y, w, s, k;
    string* places;
    Worker* worker = nullptr;

    istringstream sin(line);
    char marker; sin >> marker;
    switch (marker)
    {
    case 'S': case 's':
        sin >> surname >> firstname >> lastname >> y >> w >> s >> k;
        places = new string[k];
        for (int i = 0; i < k; ++i) sin >> places[i];
        worker = new Subordinate(surname + ' ' + firstname + ' ' + lastname,
            y, w, s, places, k);
        delete[] places;
        break;
    case 'M': case 'm':
        int subs;
        sin >> surname >> firstname >> lastname >> y >> w >> s >> subs >> k;
        places = new string[k];
        for (int i = 0; i < k; ++i) sin >> places[i];
        worker = new Manager(surname + ' ' + firstname + ' ' + lastname,
            y, w, s, places, k, subs);
        delete[] places;
        break;
    default:
        worker = nullptr;
    }
    return worker;
}

void Worker::setYears(int y)
{
    if (y <= years) return;
    years = y > 0 ? y : 0;
}

double Worker::salary() const
{ // надбавка за стаж - відсоток від (НЕ)оштрафованої зарплати
    double base_salary = workhours * Worker::PayPerHour;
    if (workhours < standardhours)
        base_salary *= 0.8; //-20%
    if (years > 20)
        base_salary *= 1.2;         //+20%
    if (years >= 10 && years <= 20) // сюда якщо не спрацювала перша умова
        base_salary *= 1.1;         //+10%
    return base_salary;
}

ostream &operator<<(ostream &os, const Worker &p)
{
    p.printOn(os);
    return os;
}

istream &operator>>(istream &is, Worker &p)
{
    p.readFrom(is);
    return is;
}

Worker &Worker::operator=(const Worker &other)
{
    if (this != &other)
    {
        this->name = other.name;
        this->years = other.years;
        this->workhours = other.workhours;
        this->standardhours = other.standardhours;
        this->Workplaces = other.Workplaces; // виклик перевизначене "=" Places
    }
    return *this;
}

Worker &Worker::operator++()
{
    this->setYears(years + 1);
    return *this;
}

// -------------------- Subordinate ------------------------------------------------

void Subordinate::printOn(ostream& os) const
{
    Worker::printOn(os);
    os << "Subordinate's current level: " << this->level << '\n';
}

void Subordinate::setYears(int y)
{
    Worker::setYears(y);
    level = years / 10;
}

double Subordinate::salary() const
{
    double coef = level / 100.0 + 1.0;
    return Worker::salary() * coef;
}

Subordinate& Subordinate::operator=(const Worker& worker)
{
    Worker* ptr = this; // Приведення типу |  father = child
    *ptr = worker;      // Worker::operator= | child = father (без level)
    this->setYears(years); // задає level (і years повторно)
    return *this;
}

void Subordinate::printWithTypenameOn(ostream& os) const
{
    os << "Subordinate :: " << this->name << ' ' << this->years << " years of experience\n";
}

Subordinate* Subordinate::clone() const
{
    return new Subordinate(*this);
}

//----------------------- Manager ----------------------------------------------

void Manager::printOn(ostream& os) const
{
    Worker::printOn(os);
    os << "Manager of: " << subordinates << " subordinates\n";
}

void Manager::readFrom(istream& is)
{
    Worker::readFrom(is);
    int s; 
    is >> s;
    this->setSubordinates(s);
}

Worker& Manager::operator++()
{
    Worker::operator++();
    ++subordinates;
    return *this;
}

double Manager::salary() const
{
    double coef = 1.0 + subordinates * PercentPerSubordinate() / 100.0;
    return Worker::salary() * coef;
}

void Manager::printWithTypenameOn(ostream& os) const
{
    os << "Manager :: " << this->name << ' ' << this->years << " years of experience\n";
}

Manager* Manager::clone() const
{
    return new Manager(*this);
}
