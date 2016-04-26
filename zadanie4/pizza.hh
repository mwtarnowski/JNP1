#ifndef PIZZA_HH
#define PIZZA_HH

#include <cstddef>
#include <array>
#include <type_traits>
#include <algorithm>

// Identifies whether first typename is an element of the following 
// multiset of typenames.
template<typename... Ts>
struct is_element_of {
    static constexpr bool value = false;
};
template<typename T, typename T1, typename... Ts>
struct is_element_of<T, T1, Ts...> {
    static constexpr bool value = std::is_same<T, T1>::value || is_element_of<T, Ts...>::value;
};


// Identifies whether Ts is a valid set (not multiset) of typenames.
template<typename... Ts>
struct are_different {
    static constexpr bool value = true;
};
template<typename T, typename... Ts>
struct are_different<T, Ts...> {
    static constexpr bool value = !is_element_of<T, Ts...>::value && are_different<Ts...>::value;
};


// Computes a maximum of yumminess of Kind in the range [0, Bound].
template<typename Kind, size_t Bound>
struct max_yumminess {
    static constexpr auto value =
        std::max(max_yumminess<Kind, Bound - 1>::value, Kind::yumminess(Bound));
    static constexpr size_t arg =
        (value > max_yumminess<Kind, Bound - 1>::value) ? Bound : max_yumminess<Kind, Bound - 1>::arg;
};
template<typename Kind>
struct max_yumminess<Kind, 0> {
    static_assert(!Kind::yumminess(0),
                  "Kind does not implement a valid yumminess method.");
    static constexpr auto value = Kind::yumminess(0);
    static constexpr size_t arg = 0;
};


// Pizzeria; parametrized by the kinds of pizzas.
// Repetition of any kind is forbidden. Multisets of kinds are not allowed.
template<typename... Kinds> 
struct Pizzeria { 
    static_assert(are_different<Kinds...>::value, 
                  "Repetition of any kind is forbidden.");
private:
    template<size_t... Pieces> struct Pizza;
    template<typename Pizza1, typename Pizza2> struct YummyMix;
public:
    template<typename Kind> struct make_pizza;
//friend:
    template<typename Pizza1, typename Pizza2> friend struct best_mix;
};


// Pizza type; parametrized by the numbers of pieces of each kind.
// There should be one-to-one correspondence between the available 
// kinds and the numbers of pieces of each kind.
template<typename... Kinds>
   template<size_t... Pieces> 
struct Pizzeria<Kinds...>::Pizza {
    static_assert(sizeof...(Pieces) == sizeof...(Kinds), 
                  "Number of pieces parameters is not equal the number of available kinds.");
private:
    typedef Pizzeria<Kinds...> producer;
public:
    typedef Pizza<2 * Pieces...> sliced_type;
    static constexpr std::array<size_t, sizeof...(Kinds)> as_array();
    template<typename Kind> static constexpr size_t count();
//friend:
    template<typename Pizza1, typename Pizza2> friend struct best_mix;
};


template<typename... Kinds>
    template<size_t... Pieces>
constexpr std::array<size_t, sizeof...(Kinds)> Pizzeria<Kinds...>::Pizza<Pieces...>::as_array() {
    return {{Pieces...}};
}


template<typename... Kinds>
    template<size_t... Pieces>
        template<typename Kind> 
constexpr size_t Pizzeria<Kinds...>::Pizza<Pieces...>::count() {
    return ((std::is_same<Kind, Kinds>::value ? Pieces : 0) + ...);
}


template<typename... Kinds>
    template<typename Pizza1, typename Pizza2> 
struct Pizzeria<Kinds...>::YummyMix {
private:
    typedef Pizza<(Pizza1::template count<Kinds>() + Pizza2::template count<Kinds>()) ...> mix;
public:
    typedef Pizza<max_yumminess<Kinds, mix::template count<Kinds>()>::arg...> type;
};


template<typename... Kinds>
    template<typename Kind> 
struct Pizzeria<Kinds...>::make_pizza {
    static_assert(is_element_of<Kind, Kinds...>::value, 
                  "Kind is not one of the available kinds.");
private:
    static const size_t INITIAL_PIECES = 8;
public:
    typedef Pizza<INITIAL_PIECES * std::is_same<Kind, Kinds>::value...> type;
};


template<typename Pizza1, typename Pizza2> 
struct best_mix {
    static_assert(std::is_same<typename Pizza1::producer, typename Pizza2::producer>::value, 
                  "Cannot mix pizzas from different producers.");
private:
    typedef typename Pizza1::producer pizzeria;
public:
    typedef typename pizzeria::template YummyMix<Pizza1, Pizza2>::type type;
};


#endif /* PIZZA_HH */
