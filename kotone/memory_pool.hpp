#ifndef KOTONE_MEMORY_POOL_HPP
#define KOTONE_MEMORY_POOL_HPP 1

#include <vector>
#include <algorithm>
#include <cassert>

namespace kotone {

// A memory pool for efficient allocation with raw pointers.
template <typename T> struct memory_pool {
  private:
    struct block {
        block *_next;
    };

    static constexpr int _DEFAULT_CHUNK_SIZE = 8;
    int _chunk_size;
    std::vector<T*> _chunks;
    block *_free_list = nullptr;

    void _allocate_chunk() {
        T *chunk = static_cast<T*>(::operator new(sizeof(T) * _chunk_size));
        _chunks.push_back(chunk);
        for (std::size_t i = 0; i < _chunk_size; i++) {
            block *b = reinterpret_cast<block*>(chunk + i);
            b->_next = _free_list;
            _free_list = b;
        }
    }

  public:
    // Constructs an empty memory pool.
    memory_pool() : _chunk_size(_DEFAULT_CHUNK_SIZE) {}

    // Constructs an empty memory pool with the specified chunk size.
    memory_pool(int chunk_size) : _chunk_size(chunk_size) {
        assert(chunk_size > 0);
    }

    memory_pool(const memory_pool&) = delete;
    memory_pool(memory_pool&&) = delete;
    memory_pool& operator=(const memory_pool&) = delete;
    memory_pool& operator=(memory_pool&&) = delete;

    // Exchanges the content of the two memory pools.
    void swap(memory_pool &other) noexcept {
        std::swap(_chunk_size, other._chunk_size);
        std::swap(_chunks, other._chunks);
        std::swap(_free_list, other._free_list);
    }

    // Updates the chunk size used to allocate memory in bulk.
    void update_chunk_size(int chunk_size) {
        assert(chunk_size > 0);
        _chunk_size = chunk_size;
    }

    // Allocates memory and constructs the given object in place using args.
    template <typename ...Args> T* allocate(Args &&...args) {
        if (!_free_list) _allocate_chunk();
        block *b = _free_list;
        _free_list = b->_next;
        T *obj = reinterpret_cast<T*>(b);
        new (obj) T(std::forward<Args>(args)...);
        return obj;
    }

    // Frees the memory used by the given object.
    void deallocate(T *obj) {
        obj->~T();
        block *b = reinterpret_cast<block*>(obj);
        b->_next = _free_list;
        _free_list = b;
    }

    // Frees all allocated memory in the memory pool.
    // If `T` has a non-trivial destructor, `deallocate()` must be called first to prevent memory leak.
    void reset() noexcept {
        for (T *chunk : _chunks) ::operator delete(chunk);
        _chunks.clear();
        _free_list = nullptr;
    }

    // Frees all allocated memory and destroys the memory pool.
    // If `T` has a non-trivial destructor, `deallocate()` must be called first to prevent memory leak.
    ~memory_pool() noexcept {
        reset();
    }
};

}  // namespace kotone

#endif  // KOTONE_MEMORY_POOL_HPP
