#pragma once
#include "worker.h"
#include <exception>

using namespace std;

/* Клас “Фірма” (поле: назва фірми у вигляді рядка).
   -------------------------------------------------
Написати програму, що дає змогу моделювати роботу бухгалтерії на фірмі. Створити такі сервіси:
 + наповнення фірми конкретними працівниками;
 + нарахування заробітної плати для всіх працівників;
 + знайти працівника, який отримує найбільшу заробітну плату;
 + визначити k (k менше за розмір масиву) працівників, що отримують найнижчу зарплату;
 + визначити k (k менше за розмір масиву) найстраших працівників;
 + обчислення сумарної заробітної плати для всіх лідерів і усіх підлеглих;
 + визначення сумарної заробітної плати лише для всіх лідерів;
 + долучення та вилучення працівників до списку;
 + знайти працівника, який змінив якнайбільшу кількість місць роботи;
 + для всіх працівників, які змінили більше k робіт, змінити поле прізвище, ім’я, по 
   батькові, додати на початок слово “Often”, можна через підкреслення _.
*/

class Firm
{
public:
	// Обов’язково реалізувати хоча б одну генерацію і перехоплення користувацького винятку.
	class FileNotFound: public std::runtime_error
	{
	public:
		string path;
		FileNotFound(const char* what, const string& fname) :runtime_error(what), path(fname) { }
	};
	class BadIndex : public std::out_of_range
	{
	public:
		int index;
		BadIndex(const char* what, int i) :out_of_range(what), index(i) { }
	};
private:
	string name;
	// Описати клас-колекцію, який містить колекцію сутностей, збережених у вигляді 
	// динамічного масиву вказівників на батьківський клас.
	Worker** mem;
	int len, used;

	void internal_clear()
	{	// для копіювального присвоєння, деструктора, зчитування з файлу
		for (int i = 0; i < used; ++i)
			delete mem[i];
	}
	void internal_copy(Worker* workers[], int n)
	{	// для конструкторів: за назвою і масивом, копіювання; 
		// для оператора присвоєння
		for (int i = 0; i < n; ++i) mem[i] = workers[i]->clone();
	}
	void check_mem(); // перевірити резерв і збільшити за потреби
	void check_index(int i) const
	{	// для видалення працівника, getFirst, getLast, operator[]
		if (i < 0 || i >= used)
			throw BadIndex("Index out of range of Firm", i);
	}
public:
	Firm() :name("Unknown"), used(0), len(5) { mem = new Worker*[len]; }
	explicit Firm(const char* line) :name(line), used(0), len(5) { mem = new Worker*[len]; }
	Firm(const char* line, int n) :name(line), used(0), len(n) { mem = new Worker*[len]; }
	Firm(const char* line, Worker* workers[], int n);
	Firm(const Firm& F);
	Firm& operator=(const Firm& F);
	~Firm();

	string get_name() const { return this->name; }
	Firm& set_name(const string& new_name);

	// повертає першого за списком працівника
	Worker& first() const { check_index(0); return *mem[0]; }
	// повертає останнього за списком працівника
	Worker& last() const { check_index(used - 1); return *mem[used - 1]; }
	// повертає кількість працівників
	int size() const { return used; }

	/*Передбачити методи заповнення масиву з файлу та його виведення у скороченій та 
      повній формах на консоль та у текстовий файл. */
	// Завантаження з указаного файлу. Може згенерувати виняток
	void loadFromFile(const char* path);
	// друк компанії у повній формі
	void printOn(std::ostream& os);
	// друк компаніїї у скороченій формі
	void shortPrintOn(std::ostream& os);

	// Зберігання до файла у форматі, придатному для зчитування
	void storeOn(const char* path);

	/* Надати можливість сортування масиву за двома різними критеріями */

	// загальний метод впорядкування методом вибору за заданим критерієм
	Firm& sortBy(bool (*criteria)(const Worker* A, const Worker* B));
	// спеціалізоване впорядкування в алфавітному порядку імен, викристовує sortBy
	Firm& sortByName();
	// спеціалізоване впорядкування за зростанням зарплат, викристовує sortBy
	Firm& sortBySalary();

	/*Забезпечити поділ створеного динамічного масиву вказівників на батьківський клас
	  на два окремі масиви, залежно від того, до якого з похідних класів належить об’єкт
	  (масив типу1 та типу2 маєте отримати) */
	// поділ на менеджерів і підлеглих:
	void separate(Subordinate*& subs, int& k_subs, Manager*& mans, int& k_mans);

	// наповнення фірми конкретними працівниками;
	/* Реалізувати методи додавання (нового) та вилучення (по критерію чи 
       індексу) об’єктів вашого класу. Та перевірити їх роботу.*/

	// додавання копії "готового" працівника
	Firm& addWorker(const Worker& worker);
	// створення і додавання нового підлеглого
	Firm& addWorker(string n, int y, int w, int s, string* t, int k);
	// створення і додавання нового менеджера
	Firm& addWorker(string n, int y, int w, int s, string* t, int k, int sub);
	// вилучення працівника за номером, може згенерувати виняток
	Firm& remove(int i);
	// вилучення працівника за критерієм поверне true, якщо було вилучено
	bool removeIf(bool (*cond)(const Worker* A));

	// нарахування заробітної плати для всіх працівників; - насправді друк відомості
	void printSalary() const;

	// знайти працівника, який отримує найбільшу заробітну плату;
	// використовує Worker::operator>, генерує виняток, якщо фірма без працівників
	const Worker& highestPaid() const;

	// визначити k (k менше за розмір масиву) працівників, що отримують найнижчу зарплату;
	Worker** poorest(int k) const;
	// визначити k (k менше за розмір масиву) найстраших працівників;
	Worker** oldest(int k) const;

	// обчислення сумарної заробітної плати для всіх лідерів і усіх підлеглих;
	// обчислює суму зарплати всіх працівників компнії
	double totalSalary() const;
	// визначення сумарної заробітної плати лише для всіх лідерів;
	double managersSalary() const;

	// знайти працівника, який змінив якнайбільшу кількість місць роботи;
	const Worker& mostMoveableWorker() const;

	// для всіх працівників, які змінили більше k робіт, змінити поле прізвище, ім’я, по
	//	батькові, додати на початок слово “Often”, можна через підкреслення _.
	/* Не можна змінювати імена людям. До того ж, клас Worker не має відповідних методів.
	   Альтернатива: створити масив індексів шуканих працівників. */

	// Повертає кількість працівників, які змінили більше k робіт. У параметрі indexes - масив
	// їхніх індексів.
	int getMoveableWorkers(int k, int*& indexes) const;

	Firm& changeNamesOfMovable(int k, const string& prefix);

	// Додаткові оператори для доступу до окремого працівника

	// Доступ до і-го працівника для внесення змін
	Worker& operator[](int i) { check_index(i); return *mem[i]; }
	// Доступ до і-го працівника для читання
	const Worker& operator[](int i) const { check_index(i); return *mem[i]; }
};