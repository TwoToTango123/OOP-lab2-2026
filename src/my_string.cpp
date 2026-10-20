#include "my_string.h"

#include <cstring>
#include <utility>

String::String() : data_(new char[2]), size_(0), capacity_(1) {
    data_[0] = '\0';
}

String::String(const char* str) {
    if (str == nullptr) {
        str = "";
    }
    size_ = std::strlen(str);
    capacity_ = size_ > 0 ? size_ : 1;
    data_ = new char[capacity_ + 1];
    std::memcpy(data_, str, size_ + 1);
}

String::String(const String& other) : size_(other.size_) {
    capacity_ = size_ > 0 ? size_ : 1;
    data_ = new char[capacity_ + 1];
    if (other.data_ != nullptr) {
        std::memcpy(data_, other.data_, size_);
    }
    data_[size_] = '\0';
}

String::String(String&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

String::~String() {
    delete[] data_;
}

String& String::operator=(String other) {
    swap(other);
    return *this;
}

void String::swap(String& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

std::size_t String::size() const {
    return size_;
}

std::size_t String::capacity() const {
    return capacity_;
}

bool String::empty() const {
    return size_ == 0;
}

const char* String::c_str() const {
    if (data_ == nullptr) {
        return "";
    }
    return data_;
}

char& String::operator[](std::size_t i) {
    return data_[i];
}

const char& String::operator[](std::size_t i) const {
    return data_[i];
}

bool String::operator==(const String& other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] != other.data_[i]) {
            return false;
        }
    }
    return true;
}

void String::reserve(std::size_t new_cap) {
    if (new_cap <= capacity_) {
        return;
    }
    char* new_data = new char[new_cap + 1];
    if (data_ != nullptr) {
        std::memcpy(new_data, data_, size_);
    }
    new_data[size_] = '\0';
    delete[] data_;
    data_ = new_data;
    capacity_ = new_cap;
}

void String::push_back(char c) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_] = c;
    size_++;
    data_[size_] = '\0';
}

void String::append(const char* str) {
    if (str == nullptr) {
        return;
    }
    std::size_t len = std::strlen(str);
    if (len == 0) {
        return;
    }
    std::size_t new_cap = capacity_ == 0 ? 1 : capacity_;
    while (new_cap < size_ + len) {
        new_cap *= 2;
    }
    reserve(new_cap);
    std::memcpy(data_ + size_, str, len);
    size_ += len;
    data_[size_] = '\0';
}

void String::append(const String& other) {
    if (this == &other) {
        String copy(other);
        append(copy.c_str());
        return;
    }
    append(other.c_str());
}

void String::clear() {
    size_ = 0;
    if (data_ != nullptr) {
        data_[0] = '\0';
    }
}

std::size_t String::find(const char* str) const {
    if (str == nullptr) {
        return npos;
    }
    std::size_t len = std::strlen(str);
    if (len == 0) {
        return 0;
    }
    if (len > size_) {
        return npos;
    }
    for (std::size_t i = 0; i + len <= size_; ++i) {
        std::size_t j = 0;
        while (j < len && data_[i + j] == str[j]) {
            j++;
        }
        if (j == len) {
            return i;
        }
    }
    return npos;
}

void String::print(std::ostream& out) const {
    out << c_str();
}

static bool is_delim(char c) {
    const char* delims = " \t\n\r.,;:!?-()\"'";
    for (int i = 0; delims[i] != '\0'; ++i) {
        if (c == delims[i]) {
            return true;
        }
    }
    return false;
}

void String::word_histogram(String* words, std::size_t* counts, std::size_t& out_size) const {
    out_size = 0;
    std::size_t i = 0;
    while (i < size_) {
        while (i < size_ && is_delim(data_[i])) {
            i++;
        }
        if (i == size_) {
            break;
        }

        String word;
        while (i < size_ && !is_delim(data_[i])) {
            word.push_back(data_[i]);
            i++;
        }

        bool found = false;
        for (std::size_t k = 0; k < out_size; ++k) {
            if (words[k] == word) {
                counts[k]++;
                found = true;
                break;
            }
        }
        if (!found) {
            words[out_size] = std::move(word);
            counts[out_size] = 1;
            out_size++;
        }
    }
}

std::ostream& operator<<(std::ostream& out, const String& s) {
    s.print(out);
    return out;
}
