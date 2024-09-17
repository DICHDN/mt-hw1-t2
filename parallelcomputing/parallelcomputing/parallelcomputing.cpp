/*
Параллельные вычисления
Напишите программу для расчёта суммы двух векторов.
Распараллельте эту программу на 2, 4, 8 и 16 потоков.
Определите, какое количество потоков даёт самый быстрый результат.
Сравните результаты выполнения для массивов из 1 000, 10 000, 100 000 и 1 000 000 элементов.
Время выполнения для каждого варианта сведите в таблицу и выведите её в консоль.
Первый запущенный поток должен вывести на экран доступное количество аппаратных ядер.
*/

#include <iostream>
#include<Windows.h>
#include<vector>
#include<algorithm>
#include<chrono>
#include<thread>
#include<execution>
//создать функцию заполнения массива
void completion(std::vector<int> a, int size) 
{
    for (int i = 0; i < size; ++i)
    {
        a.push_back(i);//rand() / 100
    };
}
// создать функцию принимающую на вход первый и последний элемент диапазона для сложения векторов а и в.
void summ_aria(std::vector<int> a, std::vector<int> b, int start, int stop)
{
    for (int i = start; i < stop; ++i) int c = a[i] + b[i];
}
// создать функцию посчета времени для выполнения многопоточного сложения и вычисления времени ее работы (вывод дожно быть время работы) на вход получение массивов и количество потоков
void time_thread(std::vector<double>& time_count, int count, std::vector<int> a, std::vector<int> b, int threads)
{
   
    std::vector<std::thread> completion_threads;
    int n = static_cast<int>(a.size() / threads);
    std::vector<int> start_stop{0};
    for (int q = 0; q < threads-1; ++q) // создаем вектор начало и конца для потоков
    {
        start_stop.push_back(q * n);
    };
    start_stop.push_back(a.size()-1);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < threads; i++)
    {
        completion_threads.push_back(std::thread(summ_aria, a, b, start_stop[i], start_stop[i + 1] - 1));
    };
    for (auto& t : completion_threads)
    {
        t.join();
    };
    auto now = std::chrono::high_resolution_clock::now();
    auto k = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start);
    double u = static_cast<double>(k.count()) / 1000000;
    time_count.push_back(u);   
}
// создать фунцию таблцы выводов сравнения работы по потокам.
void print_table(std::vector<double>& time_count, std::vector<int> threads, std::vector<int> size)
{
    std::cout << " \t\t\t\t\t\t\t Размер массивов для сложения" << std::endl;
    std::cout << "\t\t\t   ";
    for (auto i : size) { std::cout <<""<< i << "\t\t"; };
    std::cout << std::endl << "количесво потоков \t\t\t время затрачиваемое на работу (милисекунд)" << std::endl;
    int j = 0;
    for (int i = 0; i < threads.size(); ++i)
    {
        std::cout << "    " << threads[i] << "     Поток(ов)" ;
        for (int f=0; f < size.size(); ++f)
        {
            printf("  \t%4.4f ", time_count[j]);
            ++j;
        };
        std::cout << std::endl;
    };
}

int main()
{    
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::cout << "Количесво доступных потоков на этой машине: " << std::thread::hardware_concurrency() << std::endl; //физические потоки (ядра) на моей системе их 6 (виртуализация отключена)
    auto now = std::chrono::system_clock::now();
    time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::cout << "Время начала работы: " << (now_t / 3600 + 3) % 24 << ":" << (now_t / 60 % 60) << ":" << now_t % 60 << std::endl;// текущее время 
    std::vector<double> time_count;
    std::vector<int> size{ 1000,10000,100000,1000000,10000000 };
    std::vector<int> threads{ 1,2,4,8,16 };
    for (auto i : threads)
    {        
        for (auto j : size)
        {
            std::vector<int> a, b;
            completion(a, j);
            completion(b, j);
            time_thread(time_count, j, a, b, i);
        };
    };
    print_table(time_count, threads, size);    
}