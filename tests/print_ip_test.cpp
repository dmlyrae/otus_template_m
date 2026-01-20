#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <tuple>
#include <list>
#include <vector>
#include <cstdint>
#include <iostream>
#include "print_ip.h"

// Вспомогательная функция для перехвата вывода
class OutputCapture {
public:
    OutputCapture() {
        old_cout = std::cout.rdbuf(buffer.rdbuf());
    }
    
    ~OutputCapture() {
        std::cout.rdbuf(old_cout);
    }
    
    std::string getOutput() {
        return buffer.str();
    }
    
    void clear() {
        buffer.str("");
        buffer.clear();
    }
    
private:
    std::ostringstream buffer;
    std::streambuf* old_cout;
};

// Простые тесты для целочисленных типов
TEST(PrintIpTest, Int8) {
    OutputCapture capture;
    print_ip(int8_t{-1});
    std::string output = capture.getOutput();
    // -1 в беззнаковом виде = 255
    EXPECT_EQ(output, "255\n");
}

TEST(PrintIpTest, Int16) {
    OutputCapture capture;
    print_ip(int16_t{0});
    std::string output = capture.getOutput();
    EXPECT_EQ(output, "0.0\n");
}

TEST(PrintIpTest, Int32) {
    OutputCapture capture;
    print_ip(int32_t{2130706433});
    std::string output = capture.getOutput();
    // 2130706433 = 0x7F000001 = 127.0.0.1
    EXPECT_EQ(output, "127.0.0.1\n");
}

// Тест для строк
TEST(PrintIpTest, String) {
    OutputCapture capture;
    print_ip(std::string{"Hello, World!"});
    std::string output = capture.getOutput();
    EXPECT_EQ(output, "Hello, World!\n");
}

// Тест для tuple
TEST(PrintIpTest, Tuple) {
    OutputCapture capture;
    print_ip(std::make_tuple(123, 456, 789, 0));
    std::string output = capture.getOutput();
    EXPECT_EQ(output, "123.456.789.0\n");
}
