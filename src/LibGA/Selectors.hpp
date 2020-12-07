#pragma once

#include <random>
#include <vector>
#include <algorithm>
#ifdef _DEBUG
#   include <iostream>
#endif

#include "Population.hpp"

namespace GA
{

// TODO: Реализовать другие виды отбора

/**
 * Турнирный отбор.
 * "Генетические алгоритмы на Python", ДМК Пресс, стр. 42
 */
template<
    typename GeneType>
class TournamentSelection
{
public:
    // Тип популяции
    using population_type = Population<GeneType>;
    // Тип особи
    using individual_type = typename Population<GeneType>::individual_type;
public:
    /**
     * Конструктор.
     *
     * \param tournamentSize Размер турнира
     */
    TournamentSelection(
        const std::size_t tournamentSize) :
        m_tournamentSize(tournamentSize) {}

    /**
     * Выбор особи
     *
     * \param population Популяция
     * \param engine Движок генерации случайных чисел
     * \return Особь
     */
    template<
        typename Engine>
    individual_type Select(
        const population_type& population,
        Engine& engine)
    {
#ifdef _DEBUG
        std::cout << "\tTournament Selection" << std::endl;
#endif
        // Равномерное распределение для выбора особи из популяции (от 0 до РАЗМЕР_ПОПУЛЯЦИИ - 1)
        std::uniform_int_distribution<std::size_t> distribution(0, population.GetSize() - 1);
        // Массив, в который будем помещать выбранные особи
        std::vector<individual_type> selectedIndividuals(m_tournamentSize);
        // Выбираем особи
        for (auto& current : selectedIndividuals) {
            current = population[distribution(engine)];
        }
#ifdef _DEBUG
        for (const auto& current : selectedIndividuals) {
            std::cout << "\t\tIndividual: " << current.GetGene()() << std::endl;
        }
#endif
        // Отсортируем массив выбранных особей по возрастанию
        // TODO: Добавить предикат сравнения функций приспособленности,
        // поскольку сейчас реализована задача минимизации, но необходимо
        // предусмотреть возможность решать задачу максимизации
        std::sort(selectedIndividuals.begin(), selectedIndividuals.end(),
            [] (const auto& individual1, const auto& individual2)
        {
            return individual1.GetFitness() < individual2.GetFitness();
        });
        // и вернём наиболее приспособленную среди выбранных
        return selectedIndividuals.front();
    }
private:
    // Размер турнира
    std::size_t m_tournamentSize;
};

}
