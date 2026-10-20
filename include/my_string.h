#pragma once

#include <cstddef>
#include <iostream>

class String {
public:
    static constexpr std::size_t npos = (std::size_t)-1;

    String();
    String(const char* str);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    String& operator=(String other);
    void swap(String& other) noexcept;

    std::size_t size() const;
    std::size_t capacity() const;
    bool empty() const;
    const char* c_str() const;

    char& operator[](std::size_t i);
    const char& operator[](std::size_t i) const;
    bool operator==(const String& other) const;

    void push_back(char c);
    void append(const char* str);
    void append(const String& other);
    void clear();
    std::size_t find(const char* str) const;
    void print(std::ostream& out) const;

    void word_histogram(String* words, std::size_t* counts, std::size_t& out_size) const;

private:
    void reserve(std::size_t new_cap);

    char* data_;
    std::size_t size_;
    std::size_t capacity_;
};

std::ostream& operator<<(std::ostream& out, const String& s);
