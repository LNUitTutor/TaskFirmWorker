#pragma once
#include "worker.h"
#include <iostream>
using namespace std;

// впорядковує поліморфний масив працівників за зростанням методом бульбашки
void bubble_sort(Worker *workers[], size_t nWorkers); //масив вказівників на об'єкти типу Worker

// друкує заголовок і поліморфний масив працівників у вказаному потоці
void print_workers(const char *title, Worker *workers[], size_t nWorkers, ostream &os);

Worker *input_worker();