#ifndef BST_IMP_HH
#define BST_IMP_HH

#include <cstddef>
#include <memory>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <exception>
#include "bst.hh"


template <typename T>
BST<T>::BST(std::initializer_list<T> list) 
    : BST(list.begin(), list.end()) 
{}


template <typename T>
template <typename Iter>
BST<T>::BST(Iter begin, Iter end) 
    : m_root(begin == end ? 
        nullptr : (BST<T>(begin, std::prev(end)) + *std::prev(end)).m_root)
{}


template <typename T>
BST<T>::BST(T value, BST left, BST right) 
    : m_root(std::make_shared<Node const>(Node(value, left.m_root, right.m_root)))
{}


template <typename T>
BST<T> BST<T>::left() const {
    if (empty()) throw std::logic_error("Cannot perform on an empty tree.");
    return BST<T>(m_root->m_left);
}


template <typename T>
BST<T> BST<T>::right() const {
    if (empty()) throw std::logic_error("Cannot perform on an empty tree.");
    return BST<T>(m_root->m_right);
}


template <typename T>
T const & BST<T>::value() const {
    if (empty()) throw std::logic_error("Cannot perform on an empty tree.");
    return m_root->m_value;
}


template <typename T>
bool BST<T>::empty() const {
    return m_root == nullptr;
}


template <typename T>
T const & BST<T>::min() const {
    if (empty()) throw std::logic_error("Cannot perform on an empty tree.");
    return left().empty() ? value() : left().min();
}


template <typename T>
T const & BST<T>::max() const {
    if (empty()) throw std::logic_error("Cannot perform on an empty tree.");
    return right().empty() ? value() : right().max();
}


template <typename T>
template <typename Acc, typename Functor>
Acc BST<T>::fold(Acc a, Functor f) const {
    return empty() ? a : right().fold(f(left().fold(a, f), value()), f);
}


template <typename T>
BST<T> BST<T>::find(T const & t) const {
    if (empty()) throw std::logic_error("Element not found.");
    return (t == value()) ? *this : 
        (t < value()) ? left().find(t) : right().find(t);
}


template <typename T>
std::size_t BST<T>::size() const {
    return empty() ? 
        0 : 1 + left().size() + right().size();
}


template <typename T>
std::size_t BST<T>::height() const {
    return empty() ? 
        0 : 1 + std::max(left().height(), right().height());
}



template <typename T>
BST<T> spine(BST<T> tree) {
    return tree.fold(BST<T>({}),
        [](BST<T> left, const T& t) { return BST<T>(t, left, BST<T>({})); }
    );
}


template <typename T>
BST<T> operator+(T t, BST<T> tree) {
    return tree + t;
}


template <typename T>
BST<T> operator+(BST<T> tree, T t) {
    return tree.empty() ?
        BST<T>(t, BST<T>({}), BST<T>({})) :
        (t <= tree.value()) ?
            BST<T>(tree.value(), tree.left() + t, tree.right()) :
            BST<T>(tree.value(), tree.left(), tree.right() + t);
}


template <typename T>
std::ostream & operator<<(std::ostream & os, BST<T> tree) {
    return *(tree.fold(&os,
        [](std::ostream* ost, const T& t) { return &((*ost) << t << ' '); }
    ));
}


template <typename T>
T max_diff(BST<T> tree) {
    if (tree.size() < 2) throw std::logic_error("Cannot perform on a tree of size < 2.");
    // using std::pair<const T, const T> as an accumulator type Acc
    // Acc::first  - value from prev. node
    // Acc::second - current max. difference
    typedef std::pair<const T, const T> Acc;
    return tree.fold(
        Acc(tree.min(), static_cast<const T>(0)),
        [](Acc acc, const T& t) { return Acc(t, std::max(acc.second, t - acc.first)); }
    ).second;
}

#endif /* BST_IMP_HH */
