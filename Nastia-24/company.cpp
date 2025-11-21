#include "company.h"
#include <fstream>

void Firm::check_mem()
{
	// За недостатньо виділеній пам’яті для масиву (виході за його межі) варто виділити новий 
	// масив, розмір якого вдвічі більший, ніж попередній. При цьому дані мають бути збережені.
	if (used == len)
	{
		len *= 2;
		Worker** newMem = new Worker*[len];
		for (int i = 0; i < used; ++i) newMem[i] = mem[i];
		delete[] mem;
		mem = newMem;
	}
}

Firm::Firm(const char* line, Worker* workers[], int n) :name(line), used(n), len(n)
{
	// компанію створюють за назвою і готовим масивом працівників
	// кожен працівник тепер працює в компанії
	mem = new Worker*[len];
	internal_copy(workers, used);
	for (int i = 0; i < used; ++i) mem[i]->addPlace(this->name);
}

Firm::Firm(const Firm& F) :name(F.name), len(F.len), used(F.used)
{
	mem = new Worker*[len];
	internal_copy(F.mem, used);
}

Firm& Firm::operator=(const Firm& F)
{
	if (this != &F)
	{
		this->name = F.name;
		this->internal_clear();
		delete[] this->mem;
		this->len = F.len;
		this->used = F.used;
		this->mem = new Worker*[len];
		internal_copy(F.mem, F.used);
	}
	return *this;
}

Firm::~Firm()
{
	internal_clear();
	delete[] mem;
}

Firm& Firm::set_name(const string& new_name)
{
	// possible log to a file "this->name was changed to new_name"
	this->name = new_name;
	return *this;
}

void Firm::loadFromFile(const char* path)
{
	/* Вважати, що інформація про конкретні об’єкти розташована у текстовому файлі
	та задана у вигляді одного рядка для однієї сутності. Екземпляри різних похідних
	класів можуть бути розташовані у файлі в довільному порядку. У файлі розташовані
	лише екземпляри двох похідних класів, оскільки базовий клас абстрактний.
	На початку кожного рядка заданий символ-маркер, що визначає тип об’єкта. */
	std::ifstream fin(path);
	if (!fin.is_open()) throw FileNotFound("The file is not opened", path);

	internal_clear(); used = 0;
	bool error_happend = false;
	std::ofstream ferr("errors.txt");

	while (!fin.eof())
	{
		string line; getline(fin, line);
		if (line == "") continue;
		Worker* worker = Worker::Parse(line);
		if (worker == nullptr)
		{
			error_happend = true;
			ferr << line << '\n';
		}
		else
		{
			check_mem();
			worker->addPlace(this->name);
			mem[used++] = worker;
		}
	}
	ferr.close(); fin.close();
	if (error_happend) throw std::runtime_error("Some lines where bad formatted");
}

void Firm::printOn(std::ostream& os)
{
	os << "Company name: " << name << "\n has " << used << " workers\n";
	for (int i = 0; i < used; ++i) mem[i]->printOn(os);
}

void Firm::shortPrintOn(std::ostream& os)
{
	os << "Company name: " << name << '\n';
	for (int i = 0; i < used; ++i) mem[i]->printWithTypenameOn(os);
}

void Firm::storeOn(const char* path)
{
	std::ofstream fout(path);
	for (int i = 0; i < used; ++i) mem[i]->storeOn(fout);
	fout.close();
}

Firm& Firm::sortBy(bool(*criteria)(const Worker* A, const Worker* B))
{
	for (int i = 0; i < used - 1; ++i)
	{
		int ind_of_min = i;
		for (int j = i + 1; j < used; ++j)
			if (criteria(mem[j], mem[ind_of_min])) ind_of_min = j;
		if (ind_of_min != i)
		{
			Worker* to_swap = mem[i];
			mem[i] = mem[ind_of_min];
			mem[ind_of_min] = to_swap;
		}
	}
	return *this;
}

Firm& Firm::sortByName()
{
	return this->sortBy([](const Worker* A, const Worker* B) { return A->getName() < B->getName(); });
}

Firm& Firm::sortBySalary()
{
	return this->sortBy([](const Worker* A, const Worker* B) { return A->salary() < B->salary(); });
}

void Firm::separate(Subordinate*& subs, int& k_subs, Manager*& mans, int& k_mans)
{
	// створює два нових масиви
	k_subs = 0;
	for (int i = 0; i < used; ++i)
		if (typeid(*mem[i]) == typeid(Subordinate)) ++k_subs;
	if (k_subs > 0) subs = new Subordinate[k_subs];
	else subs = nullptr;
	k_mans = used - k_subs;
	if (k_mans > 0) mans = new Manager[k_mans];
	else mans = nullptr;
	int curr_subs = 0, curr_mans = 0;
	for (int i = 0; i < used; ++i)
	{
		if (typeid(*mem[i]) == typeid(Subordinate))
			subs[curr_subs++] = *dynamic_cast<Subordinate*>(mem[i]);
		else
			mans[curr_mans++] = *dynamic_cast<Manager*>(mem[i]);
	}
}

Firm& Firm::addWorker(const Worker& worker)
{
	check_mem();
	Worker* new_worker = worker.clone();
	new_worker->addPlace(this->name);
	mem[used++] = new_worker;
	return *this;
}

Firm& Firm::addWorker(string n, int y, int w, int s, string* t, int k)
{
	check_mem();
	Subordinate* sub = new Subordinate(n, y, w, s, t, k);
	sub->addPlace(this->name);
	mem[used++] = sub;
	return *this;
}

Firm& Firm::addWorker(string n, int y, int w, int s, string* t, int k, int sub)
{
	check_mem();
	Manager* man = new Manager(n, y, w, s, t, k, sub);
	man->addPlace(this->name);
	mem[used++] = man;
	return *this;
}

Firm& Firm::remove(int i)
{
	check_index(i);
	delete mem[i];
	for (int j = i + 1; j < used; ++j)
		mem[j - 1] = mem[j];
	--used;
	return *this;
}

bool Firm::removeIf(bool(*cond)(const Worker* A))
{
	int place = 0;
	while (place < used && !cond(mem[place])) ++place;
	if (place >= used) return false;
	delete mem[place];
	for (int j = place + 1; j < used; ++j)
		mem[j - 1] = mem[j];
	--used;
	return true;
}

void Firm::printSalary() const
{
	cout << " Salary in company " << name
		<< "\n-------------------------------------------------\n";
	for (int i = 0; i < used; ++i)
		cout << mem[i]->getShorterName() << " : " << mem[i]->salary() << '\n';
}

const Worker& Firm::highestPaid() const
{
	if (used == 0) throw::logic_error("Cann't find a worker in the empty company");
	int index_max = 0;
	for (int i = 1; i < used; ++i)
		if (*mem[i] > *mem[index_max]) index_max = i;
	return *mem[index_max];
}

Worker** Firm::poorest(int k) const
{
	// знаходить найменш оплачуваних за допомогою сортування копії компанії
	// оригінальна компанія залишається незмінною, повертає масив заданого розміру
	Firm copy(*this);
	copy.sortBySalary();
	Worker** poor = new Worker*[k];
	for (int i = 0; i < k; ++i) poor[i] = copy.mem[i]->clone();
	return poor;
}

Worker** Firm::oldest(int k) const
{
	// знаходить найдосвідченіших працівників за допомогою сортування копії компанії
	// оригінальна компанія залишається незмінною, повертає масив заданого розміру
	Firm copy(*this);
	copy.sortBy([](const Worker* A, const Worker* B)
				{ return A->getExperience() > B->getExperience(); });
	Worker** old = new Worker * [k];
	for (int i = 0; i < k; ++i) old[i] = copy.mem[i]->clone();
	return old;
}

double Firm::totalSalary() const
{
	double total = 0.0;
	for (int i = 0; i < used; ++i) total += mem[i]->salary();
	return total;
}

double Firm::managersSalary() const
{
	double total = 0.0;
	for (int i = 0; i < used; ++i)
		if (typeid(*mem[i]) == typeid(Manager)) total += mem[i]->salary();
	return total;
}

const Worker& Firm::mostMoveableWorker() const
{
	if (used == 0) throw::logic_error("Cann't find a worker in the empty company");
	int index_max = 0;
	for (int i = 1; i < used; ++i)
		if (mem[i]->countOfWorkplaces() > mem[index_max]->countOfWorkplaces()) index_max = i;
	return *mem[index_max];
}

int Firm::getMoveableWorkers(int k, int*& indexes) const
{
	// пам'ять для індексів виділено по максимуму заради швидкого виконання
	indexes = new int[used];
	int curr = 0;
	for (int i = 0; i < used; ++i)
		if (mem[i]->countOfWorkplaces() > k) indexes[curr++] = i;
	return curr;
}

Firm& Firm::changeNamesOfMovable(int k, const string& prefix)
{
	for (int i = 0; i < used; ++i)
		if (mem[i]->countOfWorkplaces() > k)
			mem[i]->addPrefixToName(prefix);
	return *this;
}
