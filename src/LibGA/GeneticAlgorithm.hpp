#pragma once

#ifdef _DEBUG
#   include <iostream>
#endif

#include "Population.hpp"
#include "Selectors.hpp"
#include "Crossovers.hpp"
#include "Mutators.hpp"

namespace GA
{

/**
 * Генетический алгоритм.
 */
template<
    typename GeneType,
    typename Selector,
    typename Crossover,
    typename Mutator>
class GeneticAlgorithm
{
public:
    // Тип гена
    using gene_type = GeneType;
    // Тип популяции
    using population_type = Population<GeneType>;
    // Тип функции приспособленности
    using fitness_function = typename Population<GeneType>::fitness_function;
public:
    /**
     * Конструктор.
     *
     * \param populationSize Размер популяции
     * \param selector Алгоритм выбора
     * \param crossover Алгоритм скрещивания
     * \param mutator Алгоритм мутации
     */
    GeneticAlgorithm(
        const std::size_t populationSize,
        const Selector& selector,
        const Crossover& crossover,
        const Mutator& mutator) :
        m_population(populationSize),
        m_selector(selector),
        m_crossover(crossover),
        m_mutator(mutator) {}

    /**
     * Инициализация алгоритма
     *
     * \param generator Алгоритм генерации популяции
     * \param engine Движок генерации случайных чисел
     * \return
     */
    template<
        typename Generator,
        typename Engine>
    void Init(
        const Generator& generator, Engine& engine)
    {
        // Инициализируем популяцию
        m_population.Init(generator, engine);
    }

    /**
     * Запуск генетического алгоритма
     *
     * \param numGenerations Количество поколений
     * \param fitnessFunction Функция приспособленности
     * \param engine Движок генерации случайных чисел
     * \return Решение (значение функции приспособленности наиболее приспособленной особи)
     */
    template<
        typename Engine>
    typename GeneType::value_type Run(
        const std::size_t numGenerations,
        const fitness_function& fitnessFunction,
        Engine& engine)
    {
        // Запускаем цикл по поколениям
        for (std::size_t i = 0; i < numGenerations; ++i) {
#ifdef _DEBUG
            std::cout << "Generation " << i << std::endl;
#endif
            // Вычисляем приспособленность популяции
            m_population.CalculateFitness(fitnessFunction);
            // Создаём временную популяцию родителей. В неё будут помещены копии выбранных родителей
            population_type parents(m_population.GetSize());
            // Проходим по всей текущей популяции (в ней сейчас все родители)
            for (std::size_t j = 0; j < m_population.GetSize(); ++j) {
                // Выбираем родителя
                parents[j] = m_selector.Select(m_population, engine);
            }
            // Проходим по всей текущей популяции
            for (std::size_t j = 0; j < m_population.GetSize(); j += 2) {
                // Скрещиваем двух соседних родителей (среди выбранных) и получаем двух детей
                auto& [child1, child2] = m_crossover(parents[j], parents[j + 1], engine);
                // Заменяем родителей детьми
                m_population[j] = child1;
                m_population[j + 1] = child2;
            }
            // Теперь популяция состоит из детей. Добавляем мутацию к детям
            m_population.Mutate(m_mutator, engine);
            // Получили поколение детей. Идём на следующую итерацию
#ifdef _DEBUG
            std::cout << std::endl;
#endif
        }
        // Вычисляем приспособленность популяции
        m_population.CalculateFitness(fitnessFunction);
        // Выбираем наиболее приспособленную особь
        // и возвращаем значение её функции приспособленности
        return m_population.GetBestIndividual().GetFitness();
    }
private:
    // Популяция
    population_type m_population;
    // Алгоритм выбора
    Selector m_selector;
    // Алгоритм скрещивания
    Crossover m_crossover;
    // Алгоритм мутации
    Mutator m_mutator;
};

// Тип для целочисленного генетического алгоритма
template<
    typename RealType,
    typename IntegerType>
using IntegerGeneticAlgorithm = GeneticAlgorithm<
    IntegerGene<RealType, IntegerType>,
    TournamentSelection<IntegerGene<RealType, IntegerType>>,
    OnePointCrossover<RealType, IntegerType>,
    BitInvertMutator<RealType, IntegerType>>;

// Тип для вещественного генетического алгоритма
template<
    typename RealType>
using RealGeneticAlgorithm = GeneticAlgorithm<
    RealGene<RealType>,
    TournamentSelection<RealGene<RealType>>,
    BlendCrossover<RealType>,
    GaussianMutator<RealType>>;

}
