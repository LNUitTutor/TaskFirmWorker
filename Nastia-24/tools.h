#pragma once
#include "worker.h"
#include "company.h"
#include <iostream>
using namespace std;

// впорядковує поліморфний масив працівників за зростанням методом бульбашки
void bubble_sort(Worker *workers[], size_t nWorkers); //масив вказівників на об'єкти типу Worker

// друкує заголовок і поліморфний масив працівників у вказаному потоці
void print_workers(const char *title, Worker *workers[], size_t nWorkers, ostream &os);

Worker *input_worker();

// --------------- Окремі тести
void testEmpty();
void testLoad();
void testRunners();

/* Простір імен environment містить набір функцій для побудови текстового меню
 * та демонстрації можливостей класу Firm. Простір локалізує масив екземплярів
 * класу Firm, щоб всі виклики методів скеровувати до наявних об'єктів.
 * Об'єкти-фірми створено з жорстко закодованих даних з використанням різних конструкторів.
 */
namespace environment
{
	// перевіряє правильність номера фірми
	bool check_index(int index);
	// друкує імена наявних фірм
	void print_names();
	// виконує початкове налаштування
	void print_firm(int index);
}