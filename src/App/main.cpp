#include <iostream>
#include <random>

#include "GeneticAlgorithm.hpp"
#include "PopulationGenerators.hpp"

#if defined(WIN32)
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <Windows.h>
#endif

// Тип вещественных чисел
using RealType = double;

/**
 * Функция приспособленности для заданной задачи.
 * 
 * \param input Входное значение
 * \return Значение функции приспособленности
 */
static RealType FitnessFunction(const RealType input)
{
    return input * input + 4;
}

// Размер популяции
const std::size_t populationSize = 20;
// Количество особей, учавствующих в турнирном отборе
const std::size_t tournamentSize = 2;
// Коэффициент мутации
const double mutation = 0.65;
// Минимальное значение в гене
const RealType minValue = -100.0;
// Максимальное значение в гене
const RealType maxValue = 10.0;
// Количество поколений
const std::size_t numGenerations = 20;

// Значения, хранящиеся в генах находятся в диапазоне от -100 до 10
// Значения специально подобраны с большим разбросом от начала координат,
// чтобы поиск решения задачи происходил дольше

/**
 * Запуск решения задачи, используя целочисленное кодирование.
 * 
 * \param engine Движок генерации случайных чисел
 */
template<typename Engine>
void IntegerGA(Engine& engine)
{
    std::cout << "Integer Genetic Algorithm" << std::endl;
    // Генетический алгоритм, 16-и битный ген
    GA::IntegerGeneticAlgorithm<RealType, uint16_t> ga {
        populationSize,
        tournamentSize,
        {},
        mutation
    };
    // Генератор популяции
    GA::DefaultPopulationGenerator<decltype(ga)::gene_type> generator(minValue, maxValue);
    // Инициализируем генетический алгоритм
    ga.Init(generator, engine);
    // Запускаем генетический алгоритм
    auto result = ga.Run(numGenerations, FitnessFunction, engine);
    // Выводим результат
    std::cout << "Result = " << result << std::endl;
}

/**
 * Запуск решения задачи, используя вещественное кодирование.
 *
 * \param engine Движок генерации случайных чисел
 */
template<typename Engine>
void RealGA(Engine& engine)
{
    // Коэффициент для скрещивания смешением
    const double blendAlpha = 0.5;
    // Стандартное отклонение для Гауссовой мутации
    const double stddev = 0.1;
    std::cout << "Real Genetic Algorithm" << std::endl;
    // Генетический алгоритм
    GA::RealGeneticAlgorithm<RealType> ga {
        populationSize,
        tournamentSize,
        blendAlpha,
        {
            mutation,
            stddev
        }
    };
    // Генератор популяции
    GA::DefaultPopulationGenerator<decltype(ga)::gene_type> generator(minValue, maxValue);
    // Инициализируем генетический алгоритм
    ga.Init(generator, engine);
    // Запускаем генетический алгоритм
    auto result = ga.Run(numGenerations, FitnessFunction, engine);
    // Выводим результат
    std::cout << "Result = " << result << std::endl;
}

// TODO: Добавить возможность задавать параметры генетического алгоритма из командной строки
int main (int argc, char *argv[]){
    // Костыль для винды
#if defined(WIN32)
    SetConsoleOutputCP(65001);
#endif
    std::random_device rd;
    std::mt19937 rng(rd());
    IntegerGA(rng);
    RealGA(rng);
    return 0;
}
