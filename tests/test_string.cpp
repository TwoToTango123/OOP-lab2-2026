#include <gtest/gtest.h>

#include <cstddef>
#include <cstring>
#include <sstream>
#include <utility>

#include "my_string.h"

TEST(StringCtor, DefaultIsEmpty) {
    String s;
    EXPECT_EQ(s.size(), 0u);
    EXPECT_EQ(s.capacity(), 1u);
    EXPECT_TRUE(s.empty());
    EXPECT_STREQ(s.c_str(), "");
}

TEST(StringCtor, FromCString) {
    String s("Hello");
    EXPECT_EQ(s.size(), 5u);
    EXPECT_STREQ(s.c_str(), "Hello");
    EXPECT_EQ(s[0], 'H');
    EXPECT_EQ(s[4], 'o');
}

TEST(StringCopy, CopyIsDeep) {
    String a("abc");
    String b(a);
    EXPECT_STREQ(b.c_str(), "abc");
    EXPECT_NE(a.c_str(), b.c_str());
    b[0] = 'X';
    EXPECT_STREQ(a.c_str(), "abc");
    EXPECT_STREQ(b.c_str(), "Xbc");
}

TEST(StringMove, MoveCtorEmptiesSource) {
    String a("Hello");
    const char* old_ptr = a.c_str();
    String b(std::move(a));
    EXPECT_EQ(b.c_str(), old_ptr);
    EXPECT_STREQ(b.c_str(), "Hello");
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.capacity(), 0u);
    EXPECT_STREQ(a.c_str(), "");
}

TEST(StringAssign, CopyAssign) {
    String a("first");
    String b("second");
    a = b;
    EXPECT_STREQ(a.c_str(), "second");
    EXPECT_STREQ(b.c_str(), "second");
    EXPECT_NE(a.c_str(), b.c_str());
}

TEST(StringAssign, MoveAssign) {
    String a("first");
    String b("second");
    a = std::move(b);
    EXPECT_STREQ(a.c_str(), "second");
    EXPECT_STREQ(b.c_str(), "");
}

TEST(StringAssign, SelfAssign) {
    String a("self");
    a = a;
    EXPECT_STREQ(a.c_str(), "self");
    EXPECT_EQ(a.size(), 4u);
}

TEST(StringPushBack, CapacityDoubles) {
    String s;
    s.push_back('a');
    EXPECT_EQ(s.capacity(), 1u);
    s.push_back('b');
    EXPECT_EQ(s.capacity(), 2u);
    s.push_back('c');
    EXPECT_EQ(s.capacity(), 4u);
    s.push_back('d');
    s.push_back('e');
    EXPECT_EQ(s.capacity(), 8u);
    EXPECT_STREQ(s.c_str(), "abcde");
}

TEST(StringPushBack, AfterMoveWorks) {
    String a("x");
    String b(std::move(a));
    a.push_back('y');
    EXPECT_STREQ(a.c_str(), "y");
    EXPECT_EQ(a.capacity(), 1u);
}

TEST(StringAppend, Basic) {
    String s("Hello");
    s.append(", World!");
    EXPECT_STREQ(s.c_str(), "Hello, World!");
    EXPECT_EQ(s.size(), 13u);
    EXPECT_GE(s.capacity(), s.size());
}

TEST(StringAppend, WithEmpty) {
    String s("abc");
    String empty;
    s.append(empty);
    EXPECT_STREQ(s.c_str(), "abc");
    empty.append(s);
    EXPECT_STREQ(empty.c_str(), "abc");
}

TEST(StringAppend, Self) {
    String s("ab");
    s.append(s);
    EXPECT_STREQ(s.c_str(), "abab");
}

TEST(StringFind, FoundAndNotFound) {
    String s("Hello, World!");
    EXPECT_EQ(s.find("World"), 7u);
    EXPECT_EQ(s.find("H"), 0u);
    EXPECT_EQ(s.find("xyz"), String::npos);
    EXPECT_EQ(s.find("Hello, World!!!"), String::npos);
    EXPECT_EQ(s.find(""), 0u);
}

TEST(StringPrint, WritesToStream) {
    String s("print me");
    std::ostringstream out;
    s.print(out);
    EXPECT_EQ(out.str(), "print me");
}

TEST(StringEqual, Compare) {
    EXPECT_TRUE(String("abc") == String("abc"));
    EXPECT_FALSE(String("abc") == String("abd"));
    EXPECT_FALSE(String("abc") == String("ab"));
    EXPECT_TRUE(String() == String(""));
}

TEST(WordHistogram, VariantExample) {
    String s("a bb a c a");
    String words[10];
    std::size_t counts[10];
    std::size_t n = 0;
    s.word_histogram(words, counts, n);
    ASSERT_EQ(n, 3u);
    EXPECT_STREQ(words[0].c_str(), "a");
    EXPECT_EQ(counts[0], 3u);
    EXPECT_STREQ(words[1].c_str(), "bb");
    EXPECT_EQ(counts[1], 1u);
    EXPECT_STREQ(words[2].c_str(), "c");
    EXPECT_EQ(counts[2], 1u);
}

TEST(WordHistogram, EmptyString) {
    String s;
    String words[1];
    std::size_t counts[1];
    std::size_t n = 99;
    s.word_histogram(words, counts, n);
    EXPECT_EQ(n, 0u);
}

TEST(WordHistogram, OnlyDelimiters) {
    String s("  ,.!  ?");
    String words[5];
    std::size_t counts[5];
    std::size_t n = 99;
    s.word_histogram(words, counts, n);
    EXPECT_EQ(n, 0u);
}

TEST(WordHistogram, SingleWord) {
    String s("hello");
    String words[3];
    std::size_t counts[3];
    std::size_t n = 0;
    s.word_histogram(words, counts, n);
    ASSERT_EQ(n, 1u);
    EXPECT_STREQ(words[0].c_str(), "hello");
    EXPECT_EQ(counts[0], 1u);
}

TEST(WordHistogram, Punctuation) {
    String s("Hi, hi! Hi... (hi)");
    String words[10];
    std::size_t counts[10];
    std::size_t n = 0;
    s.word_histogram(words, counts, n);
    ASSERT_EQ(n, 2u);
    EXPECT_STREQ(words[0].c_str(), "Hi");
    EXPECT_EQ(counts[0], 2u);
    EXPECT_STREQ(words[1].c_str(), "hi");
    EXPECT_EQ(counts[1], 2u);
}

TEST(WordHistogram, MaxWordsFitsBuffer) {
    String s("a b c d e");
    std::size_t max_words = s.size() / 2 + 1;
    String* words = new String[max_words];
    std::size_t* counts = new std::size_t[max_words];
    std::size_t n = 0;
    s.word_histogram(words, counts, n);
    EXPECT_EQ(n, 5u);
    EXPECT_STREQ(words[4].c_str(), "e");
    delete[] words;
    delete[] counts;
}
