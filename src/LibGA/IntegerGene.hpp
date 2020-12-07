#pragma once

#include <numeric>

namespace GA
{

// TODO: Реализовать возможность задания количества битов гена, желательно в рантайме

/**
 * Ген с целочисленным кодированием.
 */
template<
    typename RealType,
    typename IntegerType>
class IntegerGene
{
public:
    // Тип значения гена
    using value_type = RealType;
    // Тип гена
    using gene_type = IntegerType;
    // Флаг, говорящий о том,
    // что это ген с целочисленным кодированием
    static constexpr bool is_integer = true;
public:
    IntegerGene() = default;
    /**
     * Конструктор.
     *
     * \param value Значение гена
     * \param minValue Минимальное кодируемое значение
     * \param maxValue Максимальное кодируемое значение
     */
    IntegerGene(
        const value_type value,
        const value_type minValue,
        const value_type maxValue) :
        m_minValue(minValue),
        m_maxValue(maxValue)
    {
        // "ПРИМЕНЕНИЕ ГЕНЕТИЧЕСКОГО АЛГОРИТМА
        // ДЛЯ РЕШЕНИЯ ЗАДАЧ ОПТИМИЗАЦИИ" В. Г. Cпицын, Ю. Р. Цой, стр. 6
        // Кодируем ген
        m_gene = ((value - m_minValue)
            * (std::numeric_limits<gene_type>::max() - 1))
            / (m_maxValue - m_minValue);
    }
    /**
     * Конструктор.
     *
     * \param value Закодированный ген
     * \param minValue Минимальное кодируемое значение
     * \param maxValue Максимальное кодируемое значение
     */
    IntegerGene(
        const gene_type value,
        const value_type minValue,
        const value_type maxValue) :
        m_minValue(minValue),
        m_maxValue(maxValue),
        m_gene(value) {}
    /**
     * Получение значения, закодированного геном
     *
     * \return Значение, закодированное геном
     */
    value_type operator () () const
    {
        // "ПРИМЕНЕНИЕ ГЕНЕТИЧЕСКОГО АЛГОРИТМА
        // ДЛЯ РЕШЕНИЯ ЗАДАЧ ОПТИМИЗАЦИИ" В. Г. Cпицын, Ю. Р. Цой, стр. 6
        // Декодируем ген
        return m_gene * (m_maxValue - m_minValue)
            / (std::numeric_limits<gene_type>::max() - 1)
            + m_minValue;
    }
    /**
     * Получение закодированного гена
     *
     * \return Закодированный ген
     */
    gene_type GetGene() const
    {
        return m_gene;
    }
    /**
     * Получение минимального кодируемого значения
     *
     * \return Минимальное кодируемое значение
     */
    value_type GetMinValue() const
    {
        return m_minValue;
    }
    /**
     * Получение максимального кодируемого значения
     *
     * \return Максимальное кодируемое значение
     */
    value_type GetMaxValue() const
    {
        return m_maxValue;
    }
    /**
     * Инвертирование бита
     *
     * \param position Позиция инвертируемого бита
     * \return
     */
    void InvertBit(
        const std::size_t position)
    {
        // Пример: пусть есть ген 01100110, и position == 3 (те 4-й бит),
        // тогда invertionMask == 00000001 << 3 == 00001000
        // m_gene ^= invertionMask это
        // 01100110 ^
        // 00001000 ==
        // 01101110
        // Те мы инвертировали 4-й бит
        const gene_type invertionMask = (1 << position);
        m_gene ^= invertionMask;
    }
private:
    // Минимальное кодируемое значение
    value_type m_minValue = static_cast<value_type>(0);
    // Максимальное кодируемое значение
    value_type m_maxValue = static_cast<value_type>(0);
    // Переменная для кодирования генна
    // Это не значение гена, значение вычисляется
    gene_type m_gene = static_cast<gene_type>(0);
};

}
