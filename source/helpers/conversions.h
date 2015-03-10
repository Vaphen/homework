#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_

#include <sstream>

namespace std {
    template<typename T>
    static std::string to_string(T inputValue) {
        std::ostringstream outputStream;
        outputStream << inputValue;
        return outputStream.str();
    }

    static unsigned int stoi(std::string inputValue) {
        std::stringstream outputStream;
        unsigned int result;
        outputStream << inputValue;
        outputStream >> result;
        return result;
    }

    template<typename T>
    static unsigned int atoi(T inputValue) {
        return std::stoi(inputValue);
    }
}

#endif // CONVERSIONS_H_

