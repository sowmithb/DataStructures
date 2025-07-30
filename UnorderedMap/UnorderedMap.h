#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"



template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap {
    public:

    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode * _head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    template <typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap * _map;
        HashNode * _ptr;

        explicit basic_iterator(UnorderedMap const * map, HashNode *ptr) noexcept { /* TODO */ 
            _map = map;
            _ptr = ptr;
        }

    public:
        basic_iterator() { /* TODO */ 
            _map = nullptr;
            _ptr = nullptr;
        };

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;
        reference operator*() const { /* TODO */ 
            return _ptr->val;
        }
        pointer operator->() const { /* TODO */ 
            pointer temp = &(_ptr->val);
            return temp;
        }
        basic_iterator &operator++() { /* TODO */ 
            if(_ptr == nullptr) {
                return *this;
            }
            
            if(_ptr->next != nullptr) {
                _ptr = _ptr->next;
            }else{
                size_type currI = _map->bucket(_ptr->val.first) + 1;
                HashNode* temp = nullptr;
                while(temp == nullptr && currI < _map->bucket_count()) {
                    temp = _map->_buckets[currI];
                    currI++;
                }
                _ptr = temp;
            }
            return *this;
        }
        basic_iterator operator++(int) { /* TODO */ 
            basic_iterator temp(_map, _ptr);
            ++(*this);
            return temp;
        }
        bool operator==(const basic_iterator &other) const noexcept { /* TODO */
            if (_ptr == other._ptr)
            {
                return true;
            }
            return false;
        }
        bool operator!=(const basic_iterator &other) const noexcept { /* TODO */
            if (_ptr == other._ptr)
            {
                return false;
            }
            return true;
        }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const key_type, mapped_type>;
            using difference_type = ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

        private:
            friend class UnorderedMap<Key, T, Hash, key_equal>;
            using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

            HashNode * _node;

            explicit local_iterator( HashNode * node ) noexcept { /* TODO */ 
                _node = node;
            }

        public:
            local_iterator() { /* TODO */ 
                _node = nullptr;
            }

            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;
            reference operator*() const { /* TODO */ 
                return _node->val;
            }
            pointer operator->() const { /* TODO */
                pointer temp = &(_node->val);
                return temp;
            }
            local_iterator & operator++() { /* TODO */ 
                _node = _node->next;
                return *this;
            }
            local_iterator operator++(int) { /* TODO */ 
                HashNode* node = _node;
                local_iterator temp(node);
                ++(*this);
                return temp;
            }

            bool operator==(const local_iterator &other) const noexcept { /* TODO */ 
                if(_node == other._node) {
                    return true;
                }
                return false;
            }
            bool operator!=(const local_iterator &other) const noexcept { /* TODO */
                if (_node == other._node)
                {
                    return false;
                }
                return true;
            }
    };

private:

    size_type _bucket(size_t code) const { /* TODO */ 
        return _range_hash(code, _bucket_count);
    }
    size_type _bucket(const Key & key) const { /* TODO */ 
        return _bucket(_hash(key));
    }
    size_type _bucket(const value_type & val) const { /* TODO */ 
        return _bucket(val.first);
    }

    HashNode*& _find(size_type code, size_type bucket, const Key & key) { /* TODO */ 
        HashNode** temp = &_buckets[bucket];
        while(*temp != nullptr) {
            if(_equal((*temp)->val.first, key)) {
                return *temp;
            }
            temp = &((*temp)->next);
        }
        return *temp;
    }

    HashNode*& _find(const Key & key) { /* TODO */ 
        return _find(_hash(key), _range_hash(_hash(key), _bucket_count), key);
    }

    HashNode * _insert_into_bucket(size_type bucket, value_type && value) { /* TODO */ 
        HashNode* next = _buckets[bucket];
        HashNode *node = new HashNode(std::move(value), next);
        _buckets[bucket] = node;
        _size++;

        if (_head == nullptr )
        {
            _head = node;
        }
        else if (_range_hash(_hash(_head->val.first), _bucket_count) >= bucket) {
            _head = node;
        }

            return node;
    }

    void _move_content(UnorderedMap & src, UnorderedMap & dst) { /* TODO */ 
        

        for(int i = 0; i < src.bucket_count(); i++) {
            dst._buckets[i] = src._buckets[i];
            src._buckets[i] = nullptr;
        }

        dst._size = src._size;
        src._size = 0;
        dst._head = src._head;
        dst._hash = src._hash;
        dst._equal = src._equal;
        dst._bucket_count = src._bucket_count;
        src._head = nullptr;
    }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash { }, const key_equal & equal = key_equal { }) {
         /* TODO */ 
         _bucket_count = next_greater_prime(bucket_count);
         _hash = hash;
         _equal = equal;
         _size = 0;
         _buckets = new HashNode*[_bucket_count]{};
         _head = nullptr;
    }

    ~UnorderedMap() { /* TODO */ 
        clear();
        delete [] _buckets;
        _buckets = nullptr;
        _bucket_count = 0;

    }

    UnorderedMap(const UnorderedMap & other) { /* TODO */
        _bucket_count = other._bucket_count;
        _hash = other._hash;
        _equal = other._equal;
        _size = 0;
        _buckets = new HashNode *[_bucket_count] {};
        _head = nullptr;

        for(auto i = other.cbegin(); i != other.cend(); i++) {
            insert(*i);
        }
    }

    UnorderedMap(UnorderedMap && other) { /* TODO */
        _buckets = _buckets = new HashNode *[other._bucket_count] {};
        _move_content(other, *this);
    }

    UnorderedMap & operator=(const UnorderedMap & other) { /* TODO */
        clear();
        delete[] _buckets;
        _bucket_count = other._bucket_count;
        _hash = other._hash;
        _equal = other._equal;
        _size = 0;
        _buckets = new HashNode *[_bucket_count] {};
        _head = nullptr;

        for (auto i = other.cbegin(); i != other.cend(); i++)
        {
            insert(i._ptr->val);
        }
        return *this;
    }

    UnorderedMap & operator=(UnorderedMap && other) { /* TODO */ 
        clear();
        delete [] _buckets;
        _buckets = new HashNode* [other._bucket_count]{};
        _move_content(other, *this);
        return *this;
    }

    void clear() noexcept { /* TODO */
        HashNode *temp1;
        HashNode *temp2;

        for (int i = 0; i < _bucket_count; i++){
            temp1 = _buckets[i];
            temp2 = temp1;
            while(temp1 != nullptr && temp2 != nullptr) {
                temp1 = temp1->next;
                delete temp2;
                temp2 = temp1;
            }
            _buckets[i] = nullptr;
        }
        _size = 0;

    }

    size_type size() const noexcept { /* TODO */ return _size;}

    bool empty() const noexcept { /* TODO */ 
        return _size == 0;
    }

    size_type bucket_count() const noexcept { /* TODO */ return _bucket_count;}

    iterator begin() { /* TODO */ 
        return iterator(this, _head);
    }
    iterator end() { /* TODO */ 
        return iterator();
    }

    const_iterator cbegin() const { /* TODO */
        return const_iterator(this, _head);
    };
    const_iterator cend() const { /* TODO */
        return const_iterator();
    };

    local_iterator begin(size_type n) { /* TODO */ 
        return local_iterator(_buckets[n]);
    }
    local_iterator end(size_type n) { /* TODO */ 
        return local_iterator();
    }

    size_type bucket_size(size_type n) { /* TODO */ 
        HashNode* temp = _buckets[n];
        size_type count = 0;
        while(temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    float load_factor() const { /* TODO */ 
        return (_size * 1.0) / _bucket_count;
    }

    size_type bucket(const Key & key) const { /* TODO */ 
        return _bucket(key);
    }

    std::pair<iterator, bool> insert(value_type && value) { /* TODO */ 
        size_type indx = _bucket(value);
        HashNode*& currNode = _find(value.first);
        
        bool insert = false;
        iterator pos;

        if(currNode == nullptr) {
            HashNode* newNode = _insert_into_bucket(indx, std::move(value));
            pos = iterator(this, newNode);
            insert = true;
        }else{
            pos = iterator(this, currNode);
            insert = false;
        }

        return std::make_pair(pos, insert);
    }

    std::pair<iterator, bool> insert(const value_type & value) { /* TODO */ 

        return insert(value_type(value));
    }

    iterator find(const Key & key) { /* TODO */ 
        return iterator(this, _find(key));
    }

    T& operator[](const Key & key) { /* TODO */ 
        HashNode*& currP = _find(key);
        if(currP == nullptr) {
            auto temp = insert(value_type(key, T()));
            return (*(temp.first)).second;
        }
        return currP->val.second;
    }

    iterator erase(iterator pos) { /* TODO */
        HashNode *&curr = _find(pos._ptr->val.first);
        iterator prev = pos;
        curr = curr->next;
        pos++;
        if (_equal(prev._ptr->val.first, _head->val.first))
        {
            _head = pos._ptr;
        }
        _size--;
        delete prev._ptr;

        return pos;
    }

    size_type erase(const Key & key) { /* TODO */ 
        HashNode*& curr = _find(key);

        if(curr == nullptr) {
            return 0;
        }else{
            iterator pos = iterator(this, curr);
            erase(pos);
            return 1;
        }
    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        while(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}
