/**
 * @brief Печать IP-адреса для целочисленных типов

 * @details 
 * Адрес может быть представлен в виде произвольного целочисленного типа. Выводить побайтово в беззнаковом виде, начиная со старшего байта, с символом . (символ точки) в качестве разделителя. Выводятся все байты числа.
 * Выводит побайтово в беззнаковом виде, начиная со старшего байта
 */
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void print_ip(T value) {
    /**
     * Приводить к беззнаковому типу не обязательно, но теоретически это может привести 
     * к неправильному заполнению при сдвиге >> . 
     */
    using UnsignedType = std::make_unsigned_t<T>; 
    UnsignedType unsigned_value = static_cast<UnsignedType>(value);
    
    // constexpr size_t size = sizeof(T);
    size_t size = sizeof(T);
    bool first = true;
    
    /**
     * Проходим справа налево по байтам.
     */
    for (size_t i = 0; i < size; ++i) {
        size_t byte_index = size - 1 - i;  

        /**
         * Число после сдвига на >> byte_index * 8 (uint8)
         */
        uint8_t shifted_value = static_cast<uint8_t>((unsigned_value >> (byte_index * 8)));
        uint8_t byte = shifted_value & 0b11111111;
        
        if (!first) {
            std::cout << ".";
        }

        std::cout << static_cast<unsigned>(byte);
        first = false;
    }

    std::cout << std::endl;
}

/**
 * @brief Печать ip-адреса для строк

 * @details 
 * Адрес может быть представлен в виде строки. Выводится как есть, вне зависимости от содержимого.
 * Выводит строку как есть
 */
template<typename T,
         typename = std::enable_if_t<std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::string>>>
void print_ip(const T& value) {
    std::cout << value << std::endl;
}

/**
 * @brief Печать IP-адреса для const char*
 */
void print_ip(const char* value);

/**
 * @brief печать ip-адреса для контейнеров vector, list

 * @details Выводит полное содержимое контейнера поэлементно
 * Адрес может быть представлен в виде контейнеров std::list, std::vector. 
 * Выводится полное содержимое контейнера поэлементно и разделяется . (символом точка). 
 * Элементы выводятся как есть.
 */
template<template<typename, typename> class Container,
         typename T,
         typename = std::enable_if_t<
             std::is_same_v<Container<T, std::allocator<T>>, std::vector<T, std::allocator<T>>> ||
             std::is_same_v<Container<T, std::allocator<T>>, std::list<T, std::allocator<T>>>>>
void print_ip(const Container<T, std::allocator<T>>& container) {
    bool first = true;
    for (const auto& elem : container) {
        if (!first) {
            std::cout << ".";
        }
        std::cout << elem;
        first = false;
    }
    std::cout << std::endl;
}

/**
 * @brief Вспомогательная структура для проверки одинаковости типов в tuple
 */
// Общий шаблон
template<typename... Types>
struct all_types_same;

// Базовые случаи для остановки рекурсии (ноль и один типов)
template<>
struct all_types_same<> : std::true_type {};

template<typename T>
struct all_types_same<T> : std::true_type {};

// Рекурсивный переход
template<typename T, typename U, typename... Rest>
struct all_types_same<T, U, Rest...> 
    : std::conditional_t<
        std::is_same_v<T, U>,
        all_types_same<U, Rest...>,
        std::false_type
      > {};

/**
 * @brief вспомогательная функция для печати tuple
 * Базовый, остановка: когда index >= size
 */
template<size_t Index, typename... TuppleTypes>
typename std::enable_if_t<(Index >= sizeof...(TuppleTypes))>
print_tuple_impl(const std::tuple<TuppleTypes...>& tuple, bool& first) {
    // базовый случай
    // ничего 
    // не 
    // делаем
}

/**
 * @brief вспомогательная функция для печати tuple
 * Рекурсивный переход
 */
template<size_t Index, typename... TuppleTypes>
typename std::enable_if_t<(Index < sizeof...(TuppleTypes))>
print_tuple_impl(const std::tuple<TuppleTypes...>& tuple, bool& first) {
    size_t size = sizeof...(TuppleTypes);
    
    while (Index < size) {
        if (!first) {
            std::cout << ".";
        }
        std::cout << std::get<Index>(tuple);
        first = false;
        
        // рекурсивный, для следующего индекса
        print_tuple_impl<Index + 1, TuppleTypes...>(tuple, first);
        break; 
    }
}

/**
 * @brief печать ip для tuple
 *
 * @details 
 * Выводит полное содержимое кортежа 
 *
 * @note На этапе компиляции проверяет через all_types_same
 */
template<typename... TuppleTypes, typename = std::enable_if_t<all_types_same<TuppleTypes...>::value>>
void print_ip(const std::tuple<TuppleTypes...>& tuple) {
    bool first = true;
    print_tuple_impl<0, TuppleTypes...>(tuple, first);
    std::cout << std::endl;
}