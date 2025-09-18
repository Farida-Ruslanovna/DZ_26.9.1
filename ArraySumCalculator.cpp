// DZ_26.9.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <algorithm>

// Класс для подсчёта суммы элементов массива с использованием многопоточности
class ArraySumCalculator {
public:
    // Конструктор: принимает массив и количество потоков
    ArraySumCalculator(const std::vector<int>& arr, int thread_count)
        : array(arr), num_threads(thread_count), total_sum(0) {
    }

    // Метод для вычисления суммы
    long long computeSum() {
        std::vector<std::thread> threads; // Вектор для хранения потоков
        int N = array.size();             // Размер массива
        int part_size = N / num_threads;  // Размер части для каждого потока

        // Создаем и запускаем потоки
        for (int i = 0; i < num_threads; ++i) {
            int start_idx = i * part_size; // Начальный индекс части
            int end_idx = (i == num_threads - 1) ? N : start_idx + part_size; // Конечный индекс (учитываем остаток для последнего)

            // Создаем поток, который выполнит функцию worker для данной части массива
            threads.emplace_back(&ArraySumCalculator::worker, this, start_idx, end_idx);
        }

        // Ожидаем завершения всех потоков
        for (auto& th : threads) {
            th.join();
        }

        // Возвращаем итоговую сумму
        return total_sum;
    }

private:
    const std::vector<int>& array; // Ссылка на исходный массив
    int num_threads;               // Количество потоков
    long long total_sum;           // Общая сумма элементов
    std::mutex mtx;                // Мьютекс для защиты общей суммы

    // Функция, выполняемая каждым потоком
    void worker(int start_idx, int end_idx) {
        long long local_sum = 0; // Локальная переменная для суммы части массива
        // Подсчет суммы элементов своей части
        for (int i = start_idx; i < end_idx; ++i) {
            local_sum += array[i];
        }
        // Защита доступа к общей сумме с помощью мьютекса
        std::lock_guard<std::mutex> lock(mtx);
        total_sum += local_sum; // Добавляем локальную сумму в общую
    }
};

int main() {

    setlocale(LC_ALL, "");
    // Размер массива
    const size_t N = 10'000'000;

    // Создаем массив и заполняем его случайными числами от 1 до 10
    std::vector<int> array(N);
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(1, 10);
    std::generate(array.begin(), array.end(), [&]() { return dist(gen); });

    // Получение количества ядер процессора
    unsigned int cpu_cores = std::thread::hardware_concurrency();
    std::cout << "Количество ядер процессора: " << cpu_cores << "\n";

    // Массив вариантов количества потоков для тестирования
    std::vector<int> thread_counts = { 1, 4, 8, 10 };

    // Для каждого варианта запускаем подсчет суммы
    for (int thread_count : thread_counts) {
        ArraySumCalculator calculator(array, thread_count);

        // Засекаем время выполнения
        auto start_time = std::chrono::high_resolution_clock::now();

        // Выполняем подсчет суммы
        long long total = calculator.computeSum();

        auto end_time = std::chrono::high_resolution_clock::now();

        // Вычисляем разницу времени
        std::chrono::duration<double> duration = end_time - start_time;

        // Выводим результаты
        std::cout << "\nИспользование " << thread_count << " потоков:\n";
        std::cout << "Время выполнения: " << duration.count() << " секунд\n";
        std::cout << "Подсчитанная сумма: " << total << "\n";
    }

    return 0;
}