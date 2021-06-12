#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <list>
#include <tuple>
#include <utility>
namespace printip
{
/// @file

/// References:
/// - https://en.cppreference.com/w/cpp/types/enable_if
/// - http://www.cplusplus.com/reference/type_traits/is_integral/
/// - http://www.cplusplus.com/reference/type_traits/integral_constant/
/// Helper types
/// template< bool B, class T = void >
/// using enable_if_t = typename enable_if<B,T>::type;

/// @brief  Prints IP, represented as a integral type
/// @tparam T - integral type (bool, char, short, uint, int, long, etc...)
/// @param  value - IP address
/// @author btv <example@example.com>
    template<typename T>
    std::enable_if_t<std::is_integral<T>::value> print_ip(const T& value) {
        for (size_t sz=sizeof(T);sz>0;--sz) {
            std::cout << ((value >>(8 * (sz-1))) & ((uint8_t) -1))
            << (sz!=1 ? "." : "");
        }
    }

    template<typename T>
    struct is_string : std::false_type{};    
    template<>
    struct is_string<std::string>: std::true_type{};
/// @brief  Prints IP, represented as std::string type
/// @tparam T - std::string type
/// @param  value - IP address
/// @author btv <example@example.com>
    template<typename T>
    std::enable_if_t<is_string<T>::value> print_ip(const T& value) {
        std::cout<< value;
    }

    template<typename T>
    struct is_vector_list : std::false_type{};
    template<typename... Args>
    struct is_vector_list<std::vector<Args...>> : std::true_type{};
    template<typename... Args>
    struct is_vector_list<std::list<Args...>> : std::true_type{};
    template<typename T>
/// @brief  Prints IP, represented as std::vector or std::list
/// @tparam T - std::vector / std::list type
/// @param  value - IP address
/// @author btv <example@example.com>
    std::enable_if_t<is_vector_list<T>::value> print_ip(const T& value) {
        for(const auto& it : value) {
            std::cout << it 
            << (&it!=&value.back() ? "." : "");
        }
    }
 
/// @brief
/// Check, if arguments have the same type
    template<typename... Args>
    struct all_same : std::false_type{};
    template<typename T>
    struct all_same <T>: std::true_type{};
    template <typename T,typename... Args>
    struct all_same<T,T,Args...> : all_same<T,Args...>{}; 
/// @brief
/// References:
/// - https://en.cppreference.com/w/cpp/language/fold
/// Helper to print tuple elements divided by "." by used fold expression
/// Note: Dot "." after last octet shouldn't be printed
    template<typename T,size_t... N>
    constexpr void print_tuple(const T& tup,std::index_sequence<N...>) {
            (...,(std::cout<<(N==0?"":".")<<std::get<N>(tup)));
    }
/// @brief  Prints IP, represented as std::tuple, where all arguments have the same type
/// @tparam T - std::tuple
/// @param  value - IP address
/// @author btv <example@example.com>
    template<typename... Args>
    std::enable_if_t<all_same<Args...>::value> print_ip(const std::tuple<Args...>& value) {
        print_tuple(value,std::make_index_sequence<sizeof...(Args)>());
    }

    int version();
}
