#pragma once
#include <exception>
#include <string>
#include <format>

class IndexOutOfRangeException : public std::exception {
private:
    std::string message_;
public:
    IndexOutOfRangeException() : message_("Index out of range") {}
    IndexOutOfRangeException(const std::string& msg) : message_(msg) {}
    IndexOutOfRangeException(const std::string& container, const std::string& method, const std::string& issue) 
        : message_(std::format("Index out of range in {} in method {} \nissue: {}", container, method, issue)) {
            
        } //stringstream
    IndexOutOfRangeException(const std::string& container, const std::string& method, const std::string& issue, const size_t index, const size_t item) 
        : message_(std::format("Index out of range in {} in method {} \nissue: {} \n index:{} tried_endex:{}", container, method, issue, index, item)) {
            
        } //stringstream
    IndexOutOfRangeException(const char* msg) : message_(msg ? msg : "Unknown index error") {}

    const char* what() const noexcept override {return message_.c_str();}
};

class InvalidArgumentException : public std::exception {
private:
    std::string message_;
public:
    InvalidArgumentException() : message_("Invalid argument") {}
    InvalidArgumentException(const std::string& msg) : message_(msg) {}
    InvalidArgumentException(const std::string& container, const std::string& method, const std::string& issue) 
        : message_(std::format("Invalid argument in {} in method {} \nissue: {}", container, method, issue)) {}
    InvalidArgumentException(const std::string& container, const std::string& method, const std::string& issue, const size_t index, const size_t item) 
        : message_(std::format("Invalid argument in {} in method {} \nissue: {} \n index:{} tried_endex:{}", container, method, issue, index, item)) {}
    InvalidArgumentException(const char* msg) : message_(msg ? msg : "Unknown invalid argument") {}

    const char* what() const noexcept override {return message_.c_str();}
};


class EndOfStreamException : public std::exception {
private:
    std::string message_;
public:
    EndOfStreamException() : message_("Stream reached end.") {}
    EndOfStreamException(const std::string& msg) : message_(msg) {}
    EndOfStreamException(const std::string& container, const std::string& method, const std::string& issue) 
        : message_(std::format("Stream reached end in {} in method {} \nissue: {}", container, method, issue)) {}
    EndOfStreamException(const std::string& container, const std::string& method, const std::string& issue, const size_t index, const size_t item) 
        : message_(std::format("Stream reached end in {} in method {} \nissue: {} \n index:{} tried_endex:{}", container, method, issue, index, item)) {}
    EndOfStreamException(const char* msg) : message_(msg ? msg : "Unknown invalid argument") {}

    const char* what() const noexcept override {return message_.c_str();}
};