#pragma once

#include <numeric>
#include <random>

#include "Individual.hpp"
#include "IntegerGene.hpp"
#include "RealGene.hpp"

namespace GA
{

template<
    typename GeneType>
class DefaultPopulationGenerator
{
public:
    using individual_type = Individual<GeneType>;
    using value_type = typename Individual<GeneType>::value_type;
    using gene_type = typename Individual<GeneType>::gene_type;
public:
    DefaultPopulationGenerator(
        const value_type minValue,
        const value_type maxValue) :
        m_minValue(minValue),
        m_maxValue(maxValue),
        m_distribution(minValue, maxValue) {}

    template<
        typename Engine>
    individual_type operator () (
            Engine& engine) const
    {
        if constexpr (GeneType::is_integer) {
            return Individual(IntegerGene<value_type, gene_type> {
                m_distribution(engine), m_minValue, m_maxValue});
        }
        else {
            return Individual(RealGene<value_type> {
                m_distribution(engine) });
        }
    }
    value_type GetMinValue() const
    {
        return m_minValue;
    }
    value_type GetMaxValue() const
    {
        return m_maxValue;
    }
private:
    value_type m_minValue;
    value_type m_maxValue;
    mutable std::uniform_real_distribution<value_type> m_distribution;
};

}
