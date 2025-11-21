#pragma once
#include <iostream>
#include <string>
using namespace std;


class Places
{
private:
    string *titles;
    int reserved; // розмір зарезервованої пам'яті
    int used;     // поточний розмір масиву

public:
    Places();
    Places(string *a, int n);
    Places(const Places &other);
    Places &operator=(const Places &other);
    string &operator[](int n);
    const string& operator[](int n) const;
    ~Places() { delete[] titles; }; 

    void addPlace(string t);
    int countPlace() const { return used; }; // порахувати місця для циклів, введення/виведення
    void printOn(ostream &os) const;
    void readFrom(istream &is);
};
ostream &operator<<(ostream &os, const Places &p);
istream &operator>>(istream &is, Places &p);

/* Базовий клас (із завдання 1) зробити абстрактним. Для цього додати один 
абстрактний метод до вашого класу, наприклад, вивід на консоль у якомусь особливому виді. */

class Worker
{
protected:      
    string name;
    int years;         // стаж роботи
    int workhours;     // відпрацьовані години
    int standardhours; // нормативні години

    Places Workplaces;

    static double PayPerHour; // норма оплати за годину           
    // можна легко змінити PayPerHour через set
    // один спільний для всіх працівників (економить пам'ять)

public:
    Worker();
    Worker(string n, int y, int w, int s, string *t, int k);
    Worker(const Worker &other);
    // задати норму оплати для всіх
    static void setPay(double d) { PayPerHour = d; }


    virtual void printOn(ostream &os) const;  
    virtual void readFrom(istream &is);    

    // *** --------------------------------------------------------------

    // особливий метод виведення в потік
    virtual void printWithTypenameOn(ostream& os) const abstract;

    // виведення в потік у форматі, придатному для читання
    virtual void storeOn(ostream& os) const abstract;

    // особливий метод копіювання
    virtual Worker* clone() const abstract;

    // статичний метод розпізнавання рядків
    static Worker* Parse(string line);

    // додає нове місце праці
    Worker& addPlace(string t) { this->Workplaces.addPlace(t); return *this; }

    string getName() const { return name; }
    int getExperience() const { return years; }
    int countOfWorkplaces() const { return Workplaces.countPlace(); }

    // *** --------------------------------------------------------------

    void addPrefixToName(const string prefix)
    {
        this->name = prefix + this->name;
    }

    //треба у readFrom:
    virtual void setYears(int y); // subordinate, ++стаж
    void setWorkhours(int w) { workhours = w > 0 ? w : 0; }
    void setStandardhours(int s) { standardhours = s > 20 ? s : 20; }

    virtual double salary() const; 
    string getShorterName() const; //треба для короткого виводу по зарплатах

    // для Bubble sort:
    bool operator<(const Worker& other) const { return this->salary() < other.salary(); }
    bool operator>(const Worker& other) const { return other < *this; }
    Worker& operator=(const Worker& other); // + для subordinate

    // збільшити стаж на 1 рік
    virtual Worker &operator++(); // префіксний  // для manager

    virtual ~Worker() { };
};
ostream &operator<<(ostream &os, const Worker &p);
istream &operator>>(istream &is, Worker &p);



class Subordinate : public Worker
{
private:
    int level;
public:
    Subordinate() : Worker(), level(0) {}
    Subordinate(string n, int y, int w, int s, string* t, int k) : Worker(n, y, w, s, t, k)
    {
        level = years / 10; //рівень залежить від інших даних
    }
    void printOn(ostream& os) const override;
    // void readFrom(istream& is) override; // немає потреби перевизначати, бо поле level - залежне
    void setYears(int y) override;  // автоматично змінює level залежно від years
    double salary() const override; // +1% за рівень
    Subordinate& operator=(const Worker& worker);
    //~Subordinate() = default;

    // *** --------------------------------------------------------------

    // особливий метод виведення в потік
    virtual void printWithTypenameOn(ostream& os) const override;

    // виведення в потік у форматі, придатному для читання
    virtual void storeOn(ostream& os) const override;

    // особливий метод копіювання
    virtual Subordinate* clone() const override;

    // *** --------------------------------------------------------------
};



class Manager : public Worker
{
private:
    int subordinates;
public:
    static double PercentPerSubordinate() { return 0.2; }
    Manager() : Worker(), subordinates(1) {}
    Manager(string n, int y, int w, int s, string* t, int k, int sub) : Worker(n, y, w, s, t, k)
    {
        setSubordinates(sub); //безпечніше + легше змінити 
    }
    void setSubordinates(int sub)
    {
        subordinates = sub > 1 ? sub : 1;
    }
    void printOn(ostream& os) const override;
    void readFrom(istream& is) override;
    Worker& operator++() override;
    double salary() const override;
   //~Manager() = default;
    // *** --------------------------------------------------------------

    // особливий метод виведення в потік
    virtual void printWithTypenameOn(ostream& os) const override;

    // виведення в потік у форматі, придатному для читання
    virtual void storeOn(ostream& os) const override;

    // особливий метод копіювання
    virtual Manager* clone() const override;

    // *** --------------------------------------------------------------
};