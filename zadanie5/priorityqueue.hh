#ifndef PRIORITYQUEUE_HH
#define PRIORITYQUEUE_HH

#include <cstddef>
#include <memory>
#include <set>
#include <map>
#include <exception>
#include <utility>
#include <algorithm>


/* Hierarchy of exceptions */

class PriorityQueueException : public std::exception {};
class PriorityQueueNotFoundException : public PriorityQueueException {};
class PriorityQueueEmptyException : public PriorityQueueException {};



/* PriorityQueue class template */

template<typename K, typename V>
class PriorityQueue
{
private:
    typedef std::shared_ptr<K> keyptr_t;
    typedef std::shared_ptr<V> valptr_t;

    template<typename T1, typename T2> struct compare;

    std::multiset<std::pair<keyptr_t, valptr_t>, compare<K, V>> keys;
    std::multiset<std::pair<valptr_t, keyptr_t>, compare<V, K>> container;

    template<typename T1, typename T2>
    static std::pair<T2, T1> flip(const std::pair<T1, T2>&);

    typedef typename decltype(keys)::iterator key_iterator;
    key_iterator findKey(const K&);

public:
    /* Member types */
    typedef K key_type;
    typedef V value_type;
    typedef std::size_t size_type;

    /* Constructors & destructor */
    PriorityQueue() = default;
    PriorityQueue(const PriorityQueue<K, V>&);
    PriorityQueue(PriorityQueue<K, V>&&);
    ~PriorityQueue() = default;

    /* Assignment operators */
    PriorityQueue<K, V>& operator =(const PriorityQueue<K, V>&);
    PriorityQueue<K, V>& operator =(PriorityQueue<K, V>&&);

    /* Capacity */
    bool empty() const;
    size_type size() const;

    /* Element access */
    const V& minValue() const;
    const V& maxValue() const;
    const K& minKey() const;
    const K& maxKey() const;

    /* Modifiers */
    void insert(const K&, const V&);
    void changeValue(const K&, const V&);
    void deleteMin();
    void deleteMax();
    void clear();
    void merge(PriorityQueue<K, V>&);
    void swap(PriorityQueue<K, V>&);

//friend:
    /* Relational operators */
    template<typename _K, typename _V>
    friend bool operator ==(const PriorityQueue<_K, _V>&, const PriorityQueue<_K, _V>&);
    template<typename _K, typename _V>
    friend bool operator  <(const PriorityQueue<_K, _V>&, const PriorityQueue<_K, _V>&);
};


// The comparison functor that defines the lexicographical order of pairs.
template<typename K, typename V>
template<typename T1, typename T2>
struct PriorityQueue<K, V>::compare {
    bool operator()(const std::pair<std::shared_ptr<T1>, std::shared_ptr<T2>>& x, 
                    const std::pair<std::shared_ptr<T1>, std::shared_ptr<T2>>& y) {
        if (*(x.first) < *(y.first)) return true;
        if (*(y.first) < *(x.first)) return false;
        return *(x.second) < *(y.second);
    }
};


// Reverses the order of the elements in the pair
template<typename K, typename V>
template<typename T1, typename T2>
std::pair<T2, T1> PriorityQueue<K, V>::flip(const std::pair<T1, T2>& x) {
    return std::pair<T2, T1>(x.second, x.first);
}


// Returns an iterator pointing to the first element in the queue of the key.
// If the queue does not store any pair of the key, the exception is thrown.
// Complexity:        O(log size())
// Exception safety:  Strong guarantee
template<typename K, typename V>
typename PriorityQueue<K, V>::key_iterator PriorityQueue<K, V>::findKey(const K& key) {
    if (this->empty()) throw PriorityQueueNotFoundException();
    keyptr_t key_ptr = std::make_shared<K>(key);
    valptr_t val_ptr = std::make_shared<V>(minValue());
    auto it = keys.lower_bound(std::make_pair(key_ptr, val_ptr));
    if (it == keys.end() || *it->first < key || key < *it->first)
        throw PriorityQueueNotFoundException();
    return it;
}


// Copy constructor
// Complexity:        O(queue.size())
// Exception safety:  Strong guarantee
template<typename K, typename V>
PriorityQueue<K, V>::PriorityQueue(const PriorityQueue<K, V>& queue) {
    PriorityQueue<K, V> temp_queue;
    for (auto it = queue.keys.begin(); it != queue.keys.end(); ++it)
        temp_queue.insert(*it->first, *it->second);
    keys = std::move(temp_queue.keys);
    container = std::move(temp_queue.container);
}


// Move constructor
// Complexity:        O(1)
// Exception safety:  Strong guarantee
template<typename K, typename V>
PriorityQueue<K, V>::PriorityQueue(PriorityQueue<K, V>&& queue) {
    keys = std::move(queue.keys);
    container = std::move(queue.container);
}


// Copy assignment operator [=]
// Copies the resources held by the right operand into the left operand.
// Complexity:        O(queue.size())
// Exception safety:  Strong guarantee
template<typename K, typename V>
PriorityQueue<K, V>& PriorityQueue<K, V>::operator =(const PriorityQueue<K, V>& queue) {
    PriorityQueue<K, V> copy_queue(queue);
    *this = std::move(copy_queue);
    return *this;
}


// Move assignment operator [=]
// Moves the resources held by the right operand to the left operand and leaves 
// right operand in an empty state.
// Complexity:        O(1)
// Exception safety:  Strong guarantee
template<typename K, typename V>
PriorityQueue<K, V>& PriorityQueue<K, V>::operator =(PriorityQueue<K, V>&& queue) {
    if (*this != queue) {
        keys = std::move(queue.keys);
        container = std::move(queue.container);
        queue.clear();
    }
    return *this;
}


// Returns true if and only if the queue is empty.
// Complexity:        O(1)
// Exception safety:  No-throw guarantee
template<typename K, typename V>
bool PriorityQueue<K, V>::empty() const {
    return container.empty();
}


// Returns a number of pairs (key, value) stored in the queue.
// Complexity:        O(1)
// Exception safety:  No-throw guarantee
template<typename K, typename V>
typename PriorityQueue<K, V>::size_type PriorityQueue<K, V>::size() const {
    return container.size();
}


// Returns the smallest value stored in the queue; 
// In the case of calling on an empty queue, the exception is thrown.
// Complexity:        O(1)
// Exception safety:  Strong guarantee
template<typename K, typename V>
const V& PriorityQueue<K, V>::minValue() const {
    if (this->empty()) throw PriorityQueueEmptyException();
    return *(container.begin()->first);
}


// Returns the greatest value stored in the queue; 
// In the case of calling on an empty queue, the exception is thrown.
// Complexity:        O(1)
// Exception safety:  Strong guarantee
template<typename K, typename V>
const V& PriorityQueue<K, V>::maxValue() const {
    if (this->empty()) throw PriorityQueueEmptyException();
    return *(container.rbegin()->first);
}


// Returns the key related to the smallest value stored in the queue; 
// In the case of calling on an empty queue, the exception is thrown.
// Complexity:        O(1)
// Exception safety:  Strong guarantee
template<typename K, typename V>
const K& PriorityQueue<K, V>::minKey() const {
    if (this->empty()) throw PriorityQueueEmptyException();
    return *(container.begin()->second);
}


// Returns the key related to the greatest value stored in the queue; 
// In the case of calling on an empty queue, the exception is thrown.
// Complexity:        O(1)
// Exception safety:  Strong guarantee
template<typename K, typename V>
const K& PriorityQueue<K, V>::maxKey() const {
    if (this->empty()) throw PriorityQueueEmptyException();
    return *(container.rbegin()->second);
}


// Inserts into the queue the pair (key, value). It is possible to insert 
// to the queue multiple pairs with the same key.
// Complexity:        O(log size())
// Exception safety:  Strong guarantee
template<typename K, typename V>
void PriorityQueue<K, V>::insert(const K& key, const V& value) {
    keyptr_t key_ptr = std::make_shared<K>(key);
    valptr_t val_ptr = std::make_shared<V>(value);
    auto inserted = keys.insert(std::make_pair(key_ptr, val_ptr));
    try {
        container.insert(std::make_pair(val_ptr, key_ptr));
    }
    catch (...) {
        keys.erase(inserted);
        throw;
    }
}


// Changes the current value related to the given key to the new value;
// If the queue does not store any pair of the key, the exception is thrown.
// In the case when the queue stores more than one pair of the key, 
// changes the value of one of them.
// Complexity:        O(log size())
// Exception safety:  Strong guarantee
template<typename K, typename V>
void PriorityQueue<K, V>::changeValue(const K& key, const V& value) {
    auto key_it = findKey(key);
    auto val_it = container.find(flip(*key_it));
    keys.erase(key_it);
    container.erase(val_it);
    insert(key, value);
}


// Removes from the queue the pair of the smallest value.
// Complexity:        O(log size())
// Exception safety:  No-throw guarantee
template<typename K, typename V>
void PriorityQueue<K, V>::deleteMin() {
    if (this->empty()) return;
    auto val_it = container.begin();
    auto key_it = keys.find(flip(*val_it));
    container.erase(val_it);
    keys.erase(key_it);
}


// Removes from the queue the pair of the greatest value.
// Complexity:        O(log size())
// Exception safety:  No-throw guarantee
template<typename K, typename V>
void PriorityQueue<K, V>::deleteMax() {
    if (this->empty()) return;
    auto val_it = container.end(); val_it--;
    auto key_it = keys.find(flip(*val_it));
    container.erase(val_it);
    keys.erase(key_it);
}


// Removes all elements from the queue, leaving it with a size of 0.
// Complexity:        O(size())
// Exception safety:  No-throw guarantee
template<typename K, typename V>
void PriorityQueue<K, V>::clear() {
    container.clear();
    keys.clear();
}


// Merges the queue with the another queue; This operation removes all elements
// from the given queue and puts them to the queue *this.
// Complexity:        O(size() + queue.size() * log (queue.size() + size()))
// Exception safety:  Strong guarantee
template<typename K, typename V>
void PriorityQueue<K, V>::merge(PriorityQueue<K, V>& that) {
    if (this != &that) return;
    PriorityQueue<K, V> copy_this(*this), copy_that(that);
    while (!copy_that.empty()) {
        copy_this.insert(copy_that.maxKey(), copy_that.maxValue());
        copy_that.deleteMax();
    }
    swap(copy_this);
    that.clear();
}


// Exchanges the content of the queue by the another queue of the same type.
// Complexity:        O(1)
// Exception safety:  No-throw guarantee
template<typename K, typename V>
void PriorityQueue<K, V>::swap(PriorityQueue<K, V>& that) {
    keys.swap(that.keys);
    container.swap(that.container);
}


// Exchanges the contents of two queues of the same type.
// Complexity:        O(1)
// Exception safety:  No-throw guarantee
template<typename K, typename V>
void swap(PriorityQueue<K, V>& queue1, PriorityQueue<K, V>& queue2) {
    queue1.swap(queue2);
}


// Equality operator [==]
// Checks if the left operand stores the same content as the right operand.
// Complexity:        O(min(lhs.size(), rhs.size()))
// Exception safety:  Strong guarantee
template<typename K, typename V>
bool operator ==(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    auto lhs_it  = lhs.keys.begin();
    auto lhs_end = lhs.keys.end();
    auto rhs_it  = rhs.keys.begin();
    auto rhs_end = rhs.keys.end();
    while (lhs_it != lhs_end && rhs_it != rhs_end) {
        if (*lhs_it->first < *rhs_it->first) return false;
        if (*rhs_it->first < *lhs_it->first) return false;
        if (*lhs_it->second < *rhs_it->second) return false;
        if (*rhs_it->second < *lhs_it->second) return false;
        lhs_it++; rhs_it++;
    }
    return lhs_it == lhs_end && rhs_it == rhs_end;
}


// Not equal to operator [!=]
// Checks if the left operand stores different content than the right operand.
// Complexity:        O(min(lhs.size(), rhs.size()))
// Exception safety:  Strong guarantee
template<typename K, typename V>
bool operator !=(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    return !(lhs == rhs);
}


// Less than operator [<]
// Checks if the left operand is lexicographically less than the right operand.
// Complexity:        O(min(lhs.size(), rhs.size()))
// Exception safety:  Strong guarantee
template<typename K, typename V>
bool operator  <(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    auto lhs_it  = lhs.keys.begin();
    auto lhs_end = lhs.keys.end();
    auto rhs_it  = rhs.keys.begin();
    auto rhs_end = rhs.keys.end();
    while (lhs_it != lhs_end && rhs_it != rhs_end) {
        if (*lhs_it->first < *rhs_it->first) return true;
        if (*rhs_it->first < *lhs_it->first) return false;
        if (*lhs_it->second < *rhs_it->second) return true;
        if (*rhs_it->second < *lhs_it->second) return false;
        lhs_it++; rhs_it++; 
    }
    return rhs_it != rhs_end;
}


// Greater than operator [>]
// Checks if the left operand is lexicographically greater than the right operand.
// Complexity:        O(min(lhs.size(), rhs.size()))
// Exception safety:  Strong guarantee
template<typename K, typename V>
bool operator  >(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    return (rhs < lhs);
}


// Less than or equal to operator [<=]
// Checks if the left operand is lexicographically less than or equal to the right operand.
// Complexity:        O(min(lhs.size(), rhs.size()))
// Exception safety:  Strong guarantee
template<typename K, typename V>
bool operator <=(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    return !(rhs < lhs);
}


// Greater than or equal to operator [>=]
// Checks if the left operand is lexicographically greater than or equal to the right operand.
// Complexity:        O(min(lhs.size(), rhs.size()))
// Exception safety:  Strong guarantee
template<typename K, typename V>
bool operator >=(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    return !(lhs < rhs);
}


#endif /* PRIORITYQUEUE_HH */
