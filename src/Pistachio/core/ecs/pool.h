//
// Created by thomas on 21/09/23.
//
#ifndef __POOL_H__
#define __POOL_H__

#include "common.h"


/**
 * A block is an atomic unit of memory that can contain a fixed number of elements.
 * It is used by the @c Pool class to store elements.
 * @tparam T the type of the elements
 */
template <typename T>
using Block = std::vector<T>;

/**
 * @brief A pool is a collection of blocks that allows us to strike a balance between memory
 * allocation and processor cache misses.\n By design, the pool avoid memory fragmentation
 * inside the blocks.
 * @tparam T the type of the elements
 */
template <typename T>
class Pool {
    /// The size of a block
    static const std::uint32_t _mkBlockSize = 1000;

    /// The blocks of the pool
    std::vector<std::shared_ptr<Block<T>>> _mBlocks = {};

    /// the total number of elements in the pool
    std::uint32_t _mSize = 0;

    /**
     * An iterator over the elements of the pool that allows us to use range-based for loops.
     */
    class Iterator {
        // NOTE: this iterator uses a reference to the class it is iterating over...
        Pool<T> *_mPool;
        std::uint32_t _mIndex;

    public:
        Iterator(Pool<T> *pool, std::uint32_t index) : _mPool(pool), _mIndex(index) {};

        T &operator*() const {
            return _mPool->at(_mIndex);
        }

        Iterator& operator++() {
            ++_mIndex;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return _mIndex != other._mIndex;
        }
    };


public:
    /**
     * @brief Access an element of the pool by its index like in a regular array.
     * @param uuid the index of the element
     * @return the element at the given index
     */
    T &operator[](std::uint32_t uuid) {
        if (uuid >= _mSize) {
            throw std::out_of_range("Index out of range");
        }
        return _mBlocks[uuid / _mkBlockSize]->at(uuid % _mkBlockSize);
    }

    friend std::ostream& operator<<(std::ostream& os, const Pool<T>& pool) {
        for (auto element : pool) {
            os << element << std::endl;
        }
        return os;
    }

    Iterator begin() const {
        return Iterator(const_cast<Pool<T>*>(this), 0);
    }
    Iterator end() const {
        return Iterator(const_cast<Pool<T>*>(this), _mSize);
    }

    /**
     * @brief Push an element at the end of the pool.\n
     * If the last block is full, a new block is reserved.
     * @param e the element to add
     */
    void add(T e) {
        // allocate a new block if the last one is full
        if (_mSize % _mkBlockSize == 0) {
            _mBlocks.push_back(Pool<T>::allocateBlock());
        }
        auto lastBlock = _mBlocks.back();
        lastBlock->push_back(e);
        _mSize++;
    }

    /**
     * @brief Remove an element from the pool by its index.\n
     * The element is exchanged with the last one of the corresponding block and then removed to avoid memory fragmentation.
     * If the block is empty after the removal, it is freed and removed from the pool.
     * @param uuid
     */
    unsigned remove(uint32_t uuid) {
        // remove the element inside the block and replace it with the last one
        auto block = _mBlocks[uuid / _mkBlockSize];
        auto lastBlock = _mBlocks.back();
        block->at(uuid % _mkBlockSize) = lastBlock->back();
        // remove the last element
        lastBlock->pop_back();
        _mSize--;
        // if the block is empty, free it and remove it from the _mBlocks vector
        if (lastBlock->empty()) {
            Pool<T>::freeBlock(block);
            _mBlocks.pop_back();
        }
        return _mSize + 1;
    }
    /**
     * Access an element of the pool by its index like in a regular array (like operator[]).
     */
    T &at(std::uint32_t uuid) {
        return _mBlocks[uuid / _mkBlockSize]->at(uuid % _mkBlockSize);
    }

    /**
     * @return true if the pool is empty and false otherwise
     */
    bool empty() {
        return _mBlocks.empty() && _mBlocks.front()->empty();
    }

    /**
     * @brief Getter for the number of elements in the pool.
     * @return the number of elements in the pool
     */
    std::size_t size() {
        return _mSize;
    }

private:
    /**
     * @brief Allocate a new empty block in memory and reserve @c kBlockSize elements.
     * @return a pointer to the newly allocated block
     */
    static std::shared_ptr<Block<T>> allocateBlock() {
        auto block = std::make_shared<Block<T>>();
        block->reserve(_mkBlockSize);
        return block;
    }

    /**
     * @brief Free a block from memory.
     * @param block the block to free
     */
    static void freeBlock(std::shared_ptr<Block<T>> block) {
        block->clear();
    }
};

#endif //__POOL_H__