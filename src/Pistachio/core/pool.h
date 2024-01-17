//
// Created by thomas on 21/09/23.
//

#pragma once

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
    static const std::uint32_t k_blockSize = 1000;

    /// The blocks of the pool
    std::vector<std::shared_ptr<Block<T>>> _blocks = {};

    /// the total number of elements in the pool
    std::uint32_t _size = 0;

    /**
     * An iterator over the elements of the pool that allows us to use range-based for loops.
     */
    class Iterator {
        // NOTE: this iterator uses a reference to the class it is iterating over...
        Pool<T> *_pool;
        std::uint32_t _index;

    public:
        Iterator(Pool<T> *pool, std::uint32_t index) : _pool(pool), _index(index) {};

        T &operator*() const {
            return _pool->at(_index);
        }

        Iterator& operator++() {
            ++_index;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return _index != other._index;
        }
    };


public:
    /**
     * @brief Access an element of the pool by its index like in a regular array.
     * @param uuid the index of the element
     * @return the element at the given index
     */
    T &operator[](std::uint32_t uuid) {
        return _blocks[uuid / k_blockSize]->at(uuid % k_blockSize);
    }

    Iterator begin() {
        return Iterator(this, 0);
    }
    Iterator end() {
        return Iterator(this, _size);
    }

    /**
     * @brief Push an element at the end of the pool.\n
     * If the last block is full, a new block is reserved.
     * @param e the element to add
     */
    void add(T e) {
        // allocate a new block if the last one is full
        if (_size % k_blockSize == 0) {
            _blocks.push_back(Pool<T>::allocateBlock());
        }
        auto lastBlock = _blocks.back();
        lastBlock->push_back(e);
        _size++;
    }

    /**
     * @brief Remove an element from the pool by its index.\n
     * The element is exchanged with the last one of the corresponding block and then removed to avoid memory fragmentation.
     * If the block is empty after the removal, it is freed and removed from the pool.
     * @param uuid
     */
    unsigned remove(uint32_t uuid) {
        // remove the element inside the block and replace it with the last one
        auto block = _blocks[uuid / k_blockSize];
        auto lastBlock = _blocks.back();
        block->at(uuid % k_blockSize) = lastBlock->back();
        // remove the last element
        lastBlock->pop_back();
        _size--;
        // if the block is empty, free it and remove it from the _blocks vector
        if (lastBlock->empty()) {
            Pool<T>::freeBlock(block);
            _blocks.pop_back();
        }
        return _size + 1;
    }
    /**
     * Access an element of the pool by its index like in a regular array (like operator[]).
     */
    T &at(std::uint32_t uuid) {
        return _blocks[uuid / k_blockSize]->at(uuid % k_blockSize);
    }

    /**
     * @return true if the pool is empty and false otherwise
     */
    bool empty() {
        return _blocks.empty() && _blocks.front()->empty();
    }

    /**
     * @brief Getter for the number of elements in the pool.
     * @return the number of elements in the pool
     */
    std::size_t size() {
        return _size;
    }

private:
    /**
     * @brief Allocate a new empty block in memory and reserve @c k_blockSize elements.
     * @return a pointer to the newly allocated block
     */
    static std::shared_ptr<Block<T>> allocateBlock() {
        auto block = std::make_shared<Block<T>>();
        block->reserve(k_blockSize);
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
