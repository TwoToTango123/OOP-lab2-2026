#include <cstddef>
#include <iostream>
#include <utility>

#include "my_string.h"

void print_histogram(const String& text) {
    std::size_t max_words = text.size() / 2 + 1;
    String* words = new String[max_words];
    std::size_t* counts = new std::size_t[max_words];
    std::size_t n = 0;

    text.word_histogram(words, counts, n);

    std::cout << "Текст: \"" << text << "\"\n";
    std::cout << "Разных слов: " << n << '\n';
    for (std::size_t i = 0; i < n; ++i) {
        std::cout << "  " << words[i] << ": " << counts[i] << '\n';
    }

    delete[] words;
    delete[] counts;
}

int main() {
    String s1;
    String s2("Hello");
    String s3(s2);
    String s4(std::move(s3));

    s1 = s2;
    s1.append(", World!");
    s1.print(std::cout);
    std::cout << '\n';

    std::size_t pos = s1.find("World");
    std::cout << pos << std::endl;

    std::cout << "s3 после перемещения: \"" << s3 << "\", size = " << s3.size() << '\n';
    std::cout << "s4: \"" << s4 << "\"\n\n";

    print_histogram(String("a bb a c a"));
    std::cout << '\n';

    std::cout << "Введите строку: ";
    String input;
    char c;
    while (std::cin.get(c) && c != '\n') {
        input.push_back(c);
    }
    print_histogram(input);

    return 0;
}
