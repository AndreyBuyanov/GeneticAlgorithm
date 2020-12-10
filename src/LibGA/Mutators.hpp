#pragma once

#include <random>
#include <bitset>
#ifdef _DEBUG
#   include <iostream>
#endif

#include "IntegerGene.hpp"
#include "RealGene.hpp"
#include "Individual.hpp"

namespace GA
{

// TODO: Реализовать другие мутации

/**
 * Мутатор, инвертирующий случайный бит.
 * Данный класс применим только к особям с целочисленным кодированием гена
 * "Генетические алгоритмы на Python", ДМК Пресс, стр. 48
 */
template<
    typename RealType,
    typename IntegerType>
class BitInvertMutator
{
public:
    // Тип особи - особь с целочисленным геном
    using individual_type = Individual<IntegerGene<RealType, IntegerType>>;
public:
    /**
     * Конструктор.
     *
     * \param mutation Коэффициент мутации
     */
    BitInvertMutator(
        const double mutation) :
        m_mutation(mutation),
        m_bitDistribution(0, sizeof(IntegerType) * 8 - 1),
        m_mutationDistribution(0.0, 1.0) {}

    /**
     * Применение мутатора к особи
     *
     * \param individual Особь
     * \param engine Движок генерации случайных чисел
     * \return 
     */
    template<
        typename Engine>
    void operator() (
        individual_type& individual,
        Engine& engine) const
    {
        // Генерируем случайное число из диапазона от 0 до 1,
        // и если это число больше коэффициента мутации,
        // применяем мутацию к особи
        if (m_mutationDistribution(engine) > m_mutation) {
#ifdef _DEBUG
            std::cout << "\tBit Invert Mutator" << std::endl;
#endif
            // Генерируем номер бита
            const std::size_t mutationBit = m_bitDistribution(engine);
#ifdef _DEBUG
            std::cout << "\t\tMutation: bit = " << mutationBit << std::endl;
            std::bitset<sizeof(IntegerType) * 8> individualBeforeMutationBitSet(individual.GetGene().GetGene());
            std::cout << "\t\tGene before mutation: " << individualBeforeMutationBitSet << std::endl;
#endif
            // Инвертируем бит в гене особи
            individual.GetGene().InvertBit(mutationBit);
#ifdef _DEBUG
            std::bitset<sizeof(IntegerType) * 8> individualAfterMutationBitSet(individual.GetGene().GetGene());
            std::cout << "\t\tGene after mutation:  " << individualAfterMutationBitSet << std::endl;
#endif
        }
    }
private:
    // Распределение для выбора номера бита
    mutable std::uniform_int_distribution<std::size_t> m_bitDistribution;
    // Распределение для генерации коэффициента мутации
    mutable std::uniform_real_distribution<double> m_mutationDistribution;
    // Коэффициент мутации
    double m_mutation;
};

/**
 * Нормально распределённая (или гауссова) мутация.
 * Данный класс применим только к особям с вещественным кодированием гена
 * "Генетические алгоритмы на Python", ДМК Пресс, стр. 53
 */
template<
    typename RealType>
class GaussianMutator
{
public:
    // Тип особи - особь с целочисленным геном
    using individual_type = Individual<RealGene<RealType>>;
public:
    /**
     * Конструктор.
     *
     * \param mutation Коэффициент мутации
     * \param stddev Стандартное отклонение
     */
    GaussianMutator(
        const double mutation,
        const double stddev) :
        m_mutation(mutation),
        m_stddev(stddev) {}

    /**
     * Применение мутатора к особи
     *
     * \param individual Особь
     * \param engine Движок генерации случайных чисел
     * \return
     */
    template<
        typename Engine>
    void operator() (
        individual_type& individual,
        Engine& engine) const
    {
        // Генерируем случайное число из диапазона от 0 до 1,
        // и если это число больше коэффициента мутации,
        // применяем мутацию к особи
        if (m_mutationDistribution(engine) > m_mutation) {
#ifdef _DEBUG
            std::cout << "\tGaussian Mutator" << std::endl;
#endif
            // Нормальное распределение в окресности значения особи
            std::normal_distribution<double> distribution(individual(), m_stddev);
#ifdef _DEBUG
            std::cout << "\t\tGene before mutation: " << individual.GetGene()() << std::endl;
#endif
            // Генерируем вещественное число, находящееся рядом со значением особи и
            // задаём новое значение особи
            individual.GetGene().SetValue(distribution(engine));
#ifdef _DEBUG
            std::cout << "\t\tGene after mutation:  " << individual.GetGene()() << std::endl;
#endif
        }
    }
private:
    // Распределение для генерации коэффициента мутации
    mutable std::uniform_real_distribution<double> m_mutationDistribution;
    // Коэффициент мутации
    double m_mutation;
    // Стандартное отклонение
    double m_stddev;
};

}
