#pragma once

#include <random>
#include <cassert>
#include <bitset>
#ifdef _DEBUG
#   include <iostream>
#endif

#include "IntegerGene.hpp"
#include "RealGene.hpp"
#include "Individual.hpp"

namespace GA
{

// TODO: Реализовать другие скрещивания

/**
 * Одноточечное скрещивание.
 * Данный класс применим только к особям с целочисленным кодированием гена
 * "Генетические алгоритмы на Python", ДМК Пресс, стр. 43
 */
template<
    typename RealType,
    typename IntegerType>
class OnePointCrossover
{
public:
    // Тип особи - особь с целочисленным геном
    using individual_type = Individual<IntegerGene<RealType, IntegerType>>;
    // Тип результата скрещивания - пара особей
    using result_type = std::pair<individual_type, individual_type>;
public:
    /**
     * Конструктор.
     */
    OnePointCrossover() :
        m_distribution(0, sizeof(IntegerType) * 8) {}

    /**
     * Применение скрещивания к особям
     *
     * \param parent1 Первый родитель
     * \param parent1 Второй родитель
     * \param engine Движок генерации случайных чисел
     * \return Пара особей-детей
     */
    template<
        typename Engine>
    result_type operator() (
        const individual_type& parent1,
        const individual_type& parent2,
        Engine& engine) const
    {
        // Минимальное и максимальное значение гена родителей должно быть одинаковым
        assert(parent1.GetGene().GetMinValue() == parent2.GetGene().GetMinValue());
        assert(parent1.GetGene().GetMaxValue() == parent2.GetGene().GetMaxValue());

#ifdef _DEBUG
        std::cout << "\tOne Point Corssover" << std::endl;
#endif

        // В качестве примера рассмотрим 8-и битный ген (IntegerType == uint8_t)
        // Генерируем число, которое будет задавать точку скрещивания,
        // в случае 8-и битного гена это будет число от 0 до 7
        const std::size_t crossingoverPoint = m_distribution(engine);
        // Вычисляем маски, пригодятся ниже.
        // Пример: crossingoverPoint == 3, std::numeric_limits<uint8_t>::max() == 11111111
        // тогда mask1 == 11111111 << 3 == 11111000
        const IntegerType mask1 = std::numeric_limits<IntegerType>::max() << crossingoverPoint;
        // mask2 == 11111111 >> (8 - 3) == 00000111
        const IntegerType mask2 = std::numeric_limits<IntegerType>::max() >> ((sizeof(IntegerType) * 8) - crossingoverPoint);

        // Получаем гены родителей
        const auto parent1Gene = parent1.GetGene();
        const auto parent2Gene = parent2.GetGene();

#ifdef _DEBUG
        std::bitset<sizeof(IntegerType) * 8> mask1BitSet(mask1);
        std::bitset<sizeof(IntegerType) * 8> mask2BitSet(mask2);
        std::cout << "\t\tMask 1 =   " << mask1BitSet << std::endl;
        std::cout << "\t\tMask 2 =   " << mask2BitSet << std::endl;

        std::bitset<sizeof(IntegerType) * 8> parent1GeneBitSet(parent1Gene.GetGene());
        std::bitset<sizeof(IntegerType) * 8> parent2GeneBitSet(parent2Gene.GetGene());
        std::cout << "\t\tParent 1 = " << parent1GeneBitSet << " Value = " << parent1Gene() << " Fitness = " << parent1.GetFitness() << std::endl;
        std::cout << "\t\tParent 2 = " << parent2GeneBitSet << " Value = " << parent2Gene() << " Fitness = " << parent2.GetFitness() << std::endl;
#endif
        // Пусть ген первого родителя = 11010010, второго = 00101110, тогда
        //(parent1Gene.GetGene() & mask1) == (11010010 & 11111000) == 11010000
        //(parent2Gene.GetGene() & mask2) == (00101110 & 00000111) == 00000110
        // child1Gene == 11010000 | 00000110 == 11010110
        // Ген первого ребёнка - это первые 5 бит первого родителя и последние 3 бита второго
        //(parent2Gene.GetGene() & mask1) == (00101110 & 11111000) == 00101000
        //(parent1Gene.GetGene() & mask2) == (11010010 & 00000111) == 00000010
        // child2Gene == 00101000 | 00000010 == 00101010
        // Ген второго ребёнка - это первые 5 бит второго родителя и последние 3 бита первого
        const IntegerType child1Gene = (parent1Gene.GetGene() & mask1) | (parent2Gene.GetGene() & mask2);
        const IntegerType child2Gene = (parent2Gene.GetGene() & mask1) | (parent1Gene.GetGene() & mask2);

#ifdef _DEBUG
        std::bitset<sizeof(IntegerType) * 8> child1GeneBitSet(child1Gene);
        std::bitset<sizeof(IntegerType) * 8> child2GeneBitSet(child2Gene);
        std::cout << "\t\tChild 1 =  " << child1GeneBitSet << std::endl;
        std::cout << "\t\tChild 2 =  " << child2GeneBitSet << std::endl;
#endif

        // Получаем минимальное и максимальное значение гена. Пригодится при конструировании результата
        const auto minValue = parent1.GetGene().GetMinValue();
        const auto maxValue = parent1.GetGene().GetMaxValue();

        // Создаём двух детей
        individual_type child1({ child1Gene, minValue, maxValue });
        individual_type child2({ child2Gene, minValue, maxValue });

        // Возвращаем результат
        return result_type { child1, child2 };
    }
private:
    // Распределение для генерации точки скрещивания
    std::uniform_int_distribution<std::size_t> m_distribution;
};

/**
 * Скрещивание смешением.
 * Данный класс применим только к особям с вещественным кодированием гена
 * "Генетические алгоритмы на Python", ДМК Пресс, стр. 50
 */
template<
    typename RealType>
class BlendCrossover
{
public:
    // Тип особи - особь с вещественным геном
    using individual_type = Individual<RealGene<RealType>>;
    // Тип результата скрещивания - пара особей
    using result_type = std::pair<individual_type, individual_type>;
public:
    /**
     * Конструктор.
     * 
     * \param alpha Коэффициент α
     */
    BlendCrossover(
        const double alpha) :
        m_alpha(alpha) {}

    /**
     * Применение скрещивания к особям
     *
     * \param parent1 Первый родитель
     * \param parent1 Второй родитель
     * \param engine Движок генерации случайных чисел
     * \return Пара особей-детей
     */
    template<
        typename Engine>
    result_type operator() (
        const individual_type& parent1,
        const individual_type& parent2,
        Engine& engine) const
    {
#ifdef _DEBUG
        std::cout << "\tBlend Crossover" << std::endl;
#endif
        // Получаем гены родителей
        const auto parent1GeneValue = parent1.GetGene()();
        const auto parent2GeneValue = parent2.GetGene()();
#ifdef _DEBUG
        std::cout << "\t\tParent 1 = " << parent1GeneValue << std::endl;
        std::cout << "\t\tParent 2 = " << parent2GeneValue << std::endl;
#endif
        // Вычисляем гены детей. Формула тут:
        // "Генетические алгоритмы на Python", ДМК Пресс, стр. 50
        RealType child1 = parent1GeneValue - m_alpha * (parent2GeneValue - parent1GeneValue);
        RealType child2 = parent2GeneValue + m_alpha * (parent2GeneValue - parent1GeneValue);
#ifdef _DEBUG
        std::cout << "\t\tChild 1 =  " << child1 << std::endl;
        std::cout << "\t\tChild 2 =  " << child2 << std::endl;
#endif
        // Возвращаем результат
        return { { child1 }, { child2 } };
    }
private:
    // Коэффициент α
    double m_alpha;
};

}
