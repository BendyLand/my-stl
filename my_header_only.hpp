#pragma once

#include <string>
#include <exception>
#include <memory>

namespace my
{
    //todo
    class test
    {

    };
};

namespace my
{
    //todo: eventually replace std::exceptions with my own
    class exception
    {

    };
};

namespace my
{
    class string
    {
    private:
        // Fields
        std::string data;

    public:
        // Constructor, Copy/Move Constructors, Destructor
        string() : data("") {}
        string(const char* str) : data(str) {}
        string(const std::string& str) : data(str) {}
        string(const my::string& str) : data(str.data) {}
        string(my::string&& str) noexcept : data(std::move(str.data)) {}
        ~string() = default;

        // Operators
        inline operator std::string() const { return this->data; }
        inline string& operator+=(const string& other)
        {
            data += other.data;
            return *this;
        }

        // Methods
        inline const std::string str() const { return this->data; }
        inline std::string str() { return this->data; }
        inline bool contains(const std::string& substr) const
        {
            return this->data.find(substr) != std::string::npos;
        }
        inline string trim() const
        {
            size_t start = data.find_first_not_of(" \t\n\r");
            size_t end = data.find_last_not_of(" \t\n\r");
            return (start == std::string::npos) ? "" : data.substr(start, end - start + 1);
        }
        inline string to_upper() const
        {
            string result = data;
            std::transform(result.data.begin(), result.data.end(), result.data.begin(), ::toupper);
            return result;
        }
        inline string to_lower() const
        {
            string result = data;
            std::transform(result.data.begin(), result.data.end(), result.data.begin(), ::tolower);
            return result;
        }

        // Forward std::string methods
        inline size_t size() const { return data.size(); }
        inline bool empty() const { return data.empty(); }
        inline char& operator[](int index)
        {
            if (index >= 0) {
                if (index >= data.size()) throw std::out_of_range("Index out of bounds");
                return data[index];
            }
            do {
                index = data.size() + index;
            } while (index < 0);
            return data[index];
        }
        inline const char& operator[](int index) const
        {
            if (index >= 0) {
                if (index >= data.size()) throw std::out_of_range("Index out of bounds");
                return data[index];
            }
            do {
                index = data.size() + index;
            } while (index < 0);
            return data[index];
        }
    };
};

namespace my
{
    class regex
    {
    private:
        // Fields
        my::string pattern;

    public:
        // Constructor, Copy/Move Constructors, Destructor
        regex() : pattern("") {}
        regex(const my::string& p) : pattern(p) {}
        regex(const my::regex& r) : pattern(r.pattern) {}
        regex(my::regex&& r) noexcept : pattern(std::move(r.pattern)) {}
        ~regex() = default;

        // Methods
        //TODO
    };
};

namespace my
{
    template <typename T>
    class vector
    {
    private:
        size_t _capacity;
        size_t _size;
        std::unique_ptr<T[]> data;

    public:
        // Constructor, Copy/Move Constructors, Destructor
        vector() : data(nullptr), _size(0), _capacity(0) {}
        vector(const vector<T>& vec)
            : _capacity(vec._capacity), _size(vec._size), data(new T[vec._capacity])
        {
            for (size_t i = 0; i < _size; ++i) {
                data[i] = vec.data[i];
            }
        }
        vector(vector<T>&& vec) noexcept
            : data(std::move(vec.data)), _capacity(vec._capacity), _size(vec._size)
        {
            vec._capacity = 0;
            vec._size = 0;
        }
        vector(const std::initializer_list<T>& elements)
            : _size(elements.size()), _capacity(elements.size() * 2), data(std::make_unique<T[]>(_capacity))
        {
            size_t index = 0;
            for (const T& item : elements) {
                data[index++] = item;
            }
        }

        // Operators
        inline vector& operator=(const vector<T>& vec)
        {
            if (this != &vec) {
                vector temp(vec);  // Create a temporary copy
                std::swap(data, temp.data); 
                std::swap(_capacity, temp._capacity);
                std::swap(_size, temp._size);
            }
            return *this;
        }
        inline vector& operator=(vector<T>&& vec) noexcept
        {
            if (this != &vec) {
                data = std::move(vec.data);
                _capacity = vec._capacity;
                _size = vec._size;
                vec._capacity = 0;
                vec._size = 0;
            }
            return *this;
        }
        inline T& operator[](int index)
        {
            if (index >= 0) {
                if (index >= _size) throw std::out_of_range("Index out of bounds");
                return data[index];
            }
            do {
                index = _size + index;
            } while (index < 0);
            return data[index];
        }
        inline const T& operator[](int index) const
        {
            if (index >= 0) {
                if (index >= _size) throw std::out_of_range("Index out of bounds");
                return data[index];
            }
            do {
                index = _size + index;
            } while (index < 0);
            return data[index];
        }

        // Methods
        template <typename... Args>
        inline void emplace(Args&&... args)
        {
            if (_size == _capacity) resize();
            data[_size++] = T(std::forward<Args>(args)...);
        }
        inline void push(const T& value)
        {
            if (_size == _capacity) resize();
            data[_size++] = value;
        }
        inline void push(T&& value) 
        {
            if (_size == _capacity) resize();
            data[_size++] = std::move(value);
        }
        inline T pop()
        {
            if (_size == 0) throw std::length_error("Vector is empty.");
            T result = (*this)[-1];
            _size--;
            if (_capacity > _size * 2) resize(_capacity / 2);
            return result;
        }
        inline void resize(size_t new_cap = 0)
        {
            if (new_cap == 0) {
                new_cap = (_capacity == 0) ? 1 : _capacity * 2;
            }
            if (new_cap < _size) {
                throw std::runtime_error("New capacity is too small.");
            }
            auto newData = std::make_unique<T[]>(new_cap);
            for (size_t i = 0; i < _size; ++i) {
                newData[i] = std::move(data[i]);
            }
            data = std::move(newData);
            _capacity = new_cap;
        }
        inline size_t size() const { return _size; }
        inline size_t capacity() const { return _capacity; }
        T* begin() { return data.get(); }
        T* end() { return data.get() + _size; }
        const T* begin() const { return data.get(); }
        const T* end() const { return data.get() + _size; }
    };
}