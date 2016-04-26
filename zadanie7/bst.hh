//autor Piotr Wygocki
//zadanie zaliczeniowe nr 7 dla studentow JNP1 2015/2016

#ifndef BST_HH
#define BST_HH

#include <memory> //for shared_ptr
#include <iosfwd> //for std::ostream fwd

//toy binary search tree interface
//BST invariant:
//for each node all values in the left subtree
//are smaller or equal to the node value
//all nodes on the right are bigger than the node value
//
//we assume, that value T is cheap to copy
template <typename T>
class BST {
    struct Node;
    using NodePtr = std::shared_ptr<Node const>;

    struct Node {
        Node (T const  value,  NodePtr const left, NodePtr const right) :
            m_value(value), m_left(std::move(left)), m_right(std::move(right)) {}

        T const  m_value;
        NodePtr const m_left;
        NodePtr const m_right;
    };

    NodePtr const m_root;

    BST(NodePtr root) : m_root(std::move(root)) {}

public:
    BST() = default;

    BST(std::initializer_list<T>);

    template <typename Iter>
    BST(Iter, Iter);

    BST(BST &&) = default;

    BST(BST const &) = default;

    BST(T value, BST left, BST right);

    BST<T> left() const;

    BST<T> right() const;

    T const & value() const;

    bool empty() const;

    T const & min() const;

    T const & max() const;

    //for (v_1, v_2, ..., _v_n) being the sorted elements of the bst
    //fold returns f(...f(f(a, v_1),_v2)..., v_n)
    template <typename Acc, typename Functor>
    Acc fold(Acc a, Functor f) const;

    //returns subtree with element
    BST find(T const & t) const;

    std::size_t size() const;

    std::size_t height() const;
};

//for (v_1, v_2, ..., _v_n) being the sorted elements of the bst
// returns tree
//          v_n
//          /
//       ...
//       /
//    v_2
//   /
// v_1
template <typename T>
BST<T> spine(BST<T>);

//returns tree with added element
template <typename T>
BST<T> operator+(T, BST<T>);

//returns tree with added element
template <typename T>
BST<T> operator+(BST<T>, T);

//for (v_1, v_2, ..., _v_n) being the sorted elements of the bst
// prints v_1 v_2 .. v_n separated by spaces.
// The terminal space is allowed
template <typename T>
std::ostream & operator<<(std::ostream &, BST<T>);

//for nonempty BST tree with at least 2 elements,
//for (v_1, v_2, ..., _v_n) being the sorted elements of the bst
//returns max(v_{i+1} - v_i)
//it must work at least for standard arithmetic types T (see std::is_arithmetic)
template <typename T>
T max_diff(BST<T>);

#endif /* BST_HH */
