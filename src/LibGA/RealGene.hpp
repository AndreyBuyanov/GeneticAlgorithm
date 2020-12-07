#pragma once

namespace GA
{

/**
 * Ген с вещественным кодированием.
 */
template<
    typename RealType>
class RealGene
{
public:
    // Тип значения гена
    using value_type = RealType;
    // Тип гена
    using gene_type = RealType;
    // Флаг, говорящий о том,
    // что это ген с вещественным кодированием
    static constexpr bool is_integer = false;
public:
    RealGene() = default;
    /**
     * Конструктор.
     *
     * \param value Значение гена
     */
    RealGene(
        const value_type value) :
        m_value(value) {}
    /**
     * Получение значения, закодированного геном
     *
     * \return Значение, закодированное геном
     */
    value_type operator () () const
    {
        return m_value;
    }
    /**
     * Получение закодированного гена
     *
     * \return Закодированный ген
     */
    gene_type GetGene() const
    {
        return m_value;
    }
    /**
     * Установка нового значения гена
     *
     * \return
     */
    void SetValue(
        const value_type newValue)
    {
        m_value = newValue;
    }
private:
    // Значение гена. При вещественном кодировании
    // нет необходимости кодировать ген -
    // закодированный ген и его значение это одно и тоже
    value_type m_value = static_cast<value_type>(0);
};

}
