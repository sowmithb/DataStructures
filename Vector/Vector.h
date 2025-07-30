#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() {
        if(_capacity == 0) {
            array = new T[1];
            _size = 0;
            _capacity = 1;
            return;
        } 

        T* temp_array = new T[_capacity * 2]();
        for (int i = 0; i < _size; i ++) {
            temp_array[i] = std::move(array[i]);
            
        }
        _capacity = _capacity * 2;
        delete[] array;
        array = temp_array;
        temp_array = nullptr;
    }

public:
    Vector() noexcept {
        array = nullptr; 
        _capacity = 0; 
        _size = 0;
    }

    Vector(size_t count, const T& value) {
        array = new T[count]; 
        for (int i = 0; i < count; i++) {
            array[i] = value;    
        } 
        _size = count;
        _capacity = count;
    }
    
    explicit Vector(size_t count) {array = new T[count](); _size = count; _capacity = count;}
    
    Vector(const Vector& other) { /* TODO */ 
        array = new T[other.capacity()];
        for (int i = 0; i < other.capacity(); i++) {
            array[i] = other[i];
        }
        _size = other.size();
        _capacity = other.capacity();
    }
    Vector(Vector&& other) noexcept { /* TODO */ 
        array = other.array;
        other.array = nullptr;
        _capacity = other.capacity();
        _size = other.size();

        other._size = 0;
        other._capacity = 0;
        
    }

    ~Vector() { /* TODO */ 
        delete[] array;
        array = nullptr;
    }

    Vector& operator=(const Vector& other) { /* TODO */
        bool eql = true;

        if (_size == other.size() && _capacity == other.capacity()) {
            
            for (int i = 0; i < other.size(); i++)
            {
                if (array[i] != other[i])
                {
                    eql = false;
                    break;
                }
            }

            if (eql) {
                return *this;
            }
        }
        eql = false;
        delete[] array;
        array = new T[other.capacity()];
        _capacity = other.capacity();

        for (int i = 0; i < other.size(); i++) {
            array[i] = other[i];
        }
        
        _size = other.size();
        return *this;
    }
    Vector& operator=(Vector&& other) noexcept { /* TODO */
        if (_size == other.size()) {
            bool eql = true;
            for (int i = 0; i < _size; i++) {
                if (array[i] != other[i]){
                    eql = false;
                    break;
                }
            }

            if(eql) {
                return other;
            }
        }
        
        delete [] array;
        array = other.array;
        other.array = nullptr;

        _size = other.size();
        _capacity = other.capacity();
        other._size = 0;
        other._capacity = 0;
        return *this;
    }

    // Should return an iterator pointing to the first element
    iterator begin() const noexcept
    { /* TODO */
        T *ptr = &array[0];
        iterator first(ptr);
        return first;
    }

    // Should return an iterator pointing past the last element of the container
    // This is because you want to stop when you reach end
    iterator end() const noexcept
    { /* TODO */

        iterator last = iterator(&array[0]);
        last += _size;
        return last;
    }

    [[nodiscard]] bool empty() const noexcept { /* TODO */ 
        if (_size == 0) {return true;}
        if (_capacity == 0) {return true;}
        bool empt = true;
        for (int i = 0; i < _capacity; i++) {
            if (array[i] > 0 || array[i] < 0) {
                empt = false;
                return false;
            }
        }
        if (empt == true) {
            return true;
        }
    }
    size_t size() const noexcept { /* TODO */ 
        return _size;
    }
    size_t capacity() const noexcept { /* TODO */ 
        return _capacity;
    }

    T& at(size_t pos) { /* TODO */ 
        if (pos >= _capacity || pos < 0) {
            throw std::out_of_range("out of bounds");
        }else{
            return array[pos];
        }
    }
    const T& at(size_t pos) const { /* TODO */
        if (pos >= _capacity || pos < 0)
        {
            throw std::out_of_range("out of bounds");
        }
        else
        {
            return array[pos];
        }
    }
    T& operator[](size_t pos) { /* TODO */ 
        return array[pos];
    }
    const T& operator[](size_t pos) const { /* TODO */ 
        return array[pos];
    }
    T& front() { /* TODO */ 
        return array[0];
    }
    const T& front() const { /* TODO */ 
        return array[0];
    }
    T& back() { /* TODO */ 
        return array[_size - 1];
    }
    const T& back() const { /* TODO */
        return array[_size -1];
    }

    void push_back(const T& value) { /* TODO */
        if (_capacity > _size)
        {
            array[_size] = value;
            _size += 1;
        }
        else
        {
            grow();
            array[_size] = value;
            _size += 1;
        }
    }
    void push_back(T&& value) { /* TODO */
        if (_capacity > _size)
        {
            array[_size] = std::move(value);
            _size += 1;
        }
        else
        {
            grow();
            array[_size] = std::move(value);
            _size += 1;
        }
    }
    void pop_back() { /* TODO */ 
        _size -= 1;
    }

    iterator insert(iterator pos, const T& value) { /* TODO */ 
        int  i = pos -begin();
        if (_capacity <= (_size + 1)) {
            grow();
        }
        _size += 1;
        //array[_size -1] = 0;
        for (int j = (_size - 1); j > i; j--) {
            array[j] = std::move(array[j -1]);
        }
        array[i] = value;
        iterator ret(&array[0]);
        ret += i;
        return ret;
    }
    iterator insert(iterator pos, T&& value) { /* TODO */
        int i = pos - begin();
        if (_capacity <= (_size + 1))
        {
            grow();
        }
        _size += 1;
        // array[_size -1] = 0;
        for (int j = (_size - 1); j > i; j--)
        {
            array[j] = std::move(array[j - 1]);
        }
        array[i] = std::move(value);
        iterator ret(&array[0]);
        ret += i;
        return ret;
    }
    iterator insert(iterator pos, size_t count, const T& value) { /* TODO */
        int i = pos - begin();
        int last = i + count - 1;
        while (_capacity <= (_size + count))
        {
            grow();
        }
        _size += count;
        // array[_size -1] = 0;
        for (int j = (_size - 1); j > last; j--)
        {
            array[j] = std::move(array[j - count]);
        }
        
        for (int z = i; z <= last; z++) {
            array[z] = value;
        }
        iterator ret(&array[0]);
        ret += i;
        return ret;
    }
    iterator erase(iterator pos) { /* TODO */
        int i = pos - begin();
        
        for (i; i < _size-1; i++) {
            array[i] = std::move(array[i + 1]);
        }
        _size -= 1;
        return pos;
    }
    iterator erase(iterator first, iterator last) { /* TODO */
        int j = last - begin();
        int i = first - begin();
        int diff = j -i;
        iterator pos = first;
        for (i; i < _size - diff; i++)
        {
            array[i] = std::move(array[i + diff]);

        }
        _size -= diff;
        return pos;
        

    }

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        // Points to some element in the vector (or nullptr)
        T* _ptr;

    public:
        iterator()
        { /* TODO */
            _ptr = nullptr;
        }
        explicit iterator(T* ptr)
        { /* TODO */
            _ptr = ptr;
        }

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;
        
        [[nodiscard]] reference operator*() const noexcept
        { /* TODO */
            return *_ptr;
        }

        [[nodiscard]] pointer operator->() const noexcept
        { /* TODO */
            return _ptr;
        }

        // Prefix Increment: ++a
        iterator &operator++() noexcept
        { /* TODO */
            _ptr++;
            return *this;
        }

        // Postfix Increment: a++
        iterator operator++(int) noexcept
        { /* TODO */
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        // Prefix Decrement: --a
        iterator &operator--() noexcept
        { /* TODO */
            _ptr--;
            return *this;
        }

        // Postfix Decrement: a--
        iterator operator--(int) noexcept
        { /* TODO */
            iterator temp = *this;
            --(*this);
            return temp;
        }

        iterator &operator+=(difference_type offset) noexcept
        { /* TODO */
            _ptr = _ptr + offset;
            return *this;
        }

        [[nodiscard]] iterator operator+(difference_type offset) const noexcept
        { /* TODO */
            iterator temp(_ptr + offset);
            return temp;
        }

        iterator &operator-=(difference_type offset) noexcept
        { /* TODO */
            _ptr = _ptr - offset;
            return *this;
        }

        [[nodiscard]] iterator operator-(difference_type offset) const noexcept
        { /* TODO */
            iterator temp1((_ptr - offset));
            return temp1;
        }

        /// @brief 
        /// @param rhs 
        /// @return 
        [[nodiscard]] difference_type operator-(const iterator &rhs) const noexcept
        { /* TODO */
            ptrdiff_t count = 0;
            T *rhsptr = rhs._ptr;
            count = _ptr - rhsptr;
            return count;
        }

        [[nodiscard]] reference operator[](difference_type offset) const noexcept
        { /* TODO */
            return *(_ptr + offset);
        }

        [[nodiscard]] bool operator==(const iterator &rhs) const noexcept
        { /* TODO */
            if (_ptr == rhs._ptr)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        [[nodiscard]] bool operator!=(const iterator &rhs) const noexcept
        { /* TODO */
            if (_ptr == rhs._ptr)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        [[nodiscard]] bool operator<(const iterator &rhs) const noexcept
        { /* TODO */
            if (_ptr < rhs._ptr)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        [[nodiscard]] bool operator>(const iterator &rhs) const noexcept
        { /* TODO */
            if (_ptr > rhs._ptr)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        [[nodiscard]] bool operator<=(const iterator &rhs) const noexcept
        { /* TODO */
            if (_ptr <= rhs._ptr)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        [[nodiscard]] bool operator>=(const iterator &rhs) const noexcept
        { /* TODO */
            if (_ptr >= rhs._ptr)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    };


    void clear() noexcept { /* TODO */ 
        for (int i = 0; i < _size; i++) {
            array[i] = 0;
        }
        _size = 0;
    }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept { /* TODO */ 
    return (iterator + offset);
}

#endif
