#pragma once

#include <functional>

namespace GA
{

/**
 * Особь.
 */
template<
    typename GeneType>
class Individual
{
public:
    // Тип гена
    using gene_type = typename GeneType::gene_type;
    // Тип значения гена
    using value_type = typename GeneType::value_type;
    // Тип функции приспособленности
    using fitness_function = std::function<
        value_type(const value_type)>;
public:
    Individual() = default;
    /**
     * Конструктор.
     *
     * \param gene Ген
     */
    Individual(
        const GeneType& gene) :
        m_gene(gene) {}

    /**
     * Вычисление приспособленности
     *
     * \param fitnessFn Функция приспособленности
     * \return
     */
    void CalculateFitness(
        const fitness_function& fitnessFn)
    {
        // Вычисляем приспособленность особи
        // передав в функцию приспособленности
        // ЗНАЧЕНИЕ гена (не то, чем он закодирован)
        m_fitness = fitnessFn(m_gene());
    }

    /**
     * Получение значения приспособленности
     *
     * \return Значение приспособленности
     */
    value_type GetFitness() const
    {
        return m_fitness;
    }
    /**
     * Получение константной ссылки на ген
     *
     * \return Константная ссылка на ген
     */
    const GeneType& GetGene() const
    {
        return m_gene;
    }
    /**
     * Получение ссылки на ген
     *
     * \return Ссылка на ген
     */
    GeneType& GetGene()
    {
        return m_gene;
    }
    /**
     * Получение значения гена
     *
     * \return Значения гена
     */
    value_type operator () () const
    {
        return m_gene();
    }

private:
    // Ген
    GeneType m_gene;
    // Приспособленность особи
    value_type m_fitness = static_cast<value_type>(0);
};

}
