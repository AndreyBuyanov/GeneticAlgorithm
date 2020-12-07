#pragma once

#include <vector>
#include <algorithm>

#include "Individual.hpp"

namespace GA
{

/**
 * Популяция.
 */
template<
    typename GeneType>
class Population
{
public:
    // Тип особи
    using individual_type = Individual<GeneType>;
    // Тип функции приспособленности
    using fitness_function = typename Individual<GeneType>::fitness_function;
public:
    /**
     * Конструктор.
     * 
     * \param populationSize Размер популяции
     */
    Population(
        const std::size_t populationSize) :
        m_population(populationSize) {}     // Задаём размер массива особей

    /**
     * Инициализация популяции
     *
     * \param generator Алгоритм генерации популяции
     * \param engine Движок генерации случайных чисел
     * \return
     */
    template<
        typename Generator,
        typename Engine>
    void Init(
        const Generator& generator,
        Engine& engine)
    {
        // Проходим по каждой особи.
        // На данном этапе особи созданы, но не содержат полезной информации
        // поскольку были сконструированы дефолтным конструктором
        for (auto& individual : m_population) {
            // individual - неконстантная ссылка на текущую особь
            // Герерируем новую особь
            individual = generator(engine);
        }
    }

    /**
     * Получение размера популяции
     *
     * \return
     */
    std::size_t GetSize() const
    {
        return m_population.size();
    }

    /**
     * Доступ к особям
     *
     * \param index Индекс особи
     * \return Ссылка на особь
     */
    individual_type& operator [] (
        const std::size_t index)
    {
        return m_population[index];
    }
    /**
     * Доступ к особям
     *
     * \param index Индекс особи
     * \return Константная ссылка на особь
     */
    const individual_type& operator [] (
        const std::size_t index) const
    {
        return m_population[index];
    }
    /**
     * Вычисление приспособленности у каждой особи
     *
     * \param fitnessFn Функция приспособленности
     * \return 
     */
    void CalculateFitness(
        const fitness_function& fitnessFn)
    {
        // Проходим по каждой особи
        for (auto& individual : m_population) {
            // и вычисляем приспособленность
            individual.CalculateFitness(fitnessFn);
        }
    }
    /**
     * Мутация популяции
     *
     * \param mutator Алгоритм мутации
     * \param engine Движок генерации случайных чисел
     * \return
     */
    template<
        typename Mutator,
        typename Engine>
    void Mutate(
        const Mutator& mutator,
        Engine& engine)
    {
        // Проходим по каждой особи
        for (auto& individual : m_population) {
            // и применяем алгоритм мутации
            mutator(individual, engine);
        }
    }

    /**
     * Получение наиболее приспособленной особи
     *
     * \return
     */
    individual_type GetBestIndividual()
    {
        // Отсортируем массив выбранных особей по возрастанию
        // TODO: Добавить предикат сравнения функций приспособленности,
        // поскольку сейчас реализована задача минимизации, но необходимо
        // предусмотреть возможность решать задачу максимизации
        std::sort(m_population.begin(), m_population.end(),
            [] (const auto& individual1, const auto& individual2)
        {
            return individual1.GetFitness() < individual2.GetFitness();
        });
        // и вернём наиболее приспособленную
        return m_population.front();
    }

private:
    // Массив особей
    std::vector<individual_type> m_population;
};

}
