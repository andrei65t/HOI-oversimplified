#pragma once
#include <stdexcept>
#include <string>

class AccessDeniedException : public std::runtime_error {
public:
    explicit AccessDeniedException(const std::string& msg)
        : std::runtime_error(msg) {}
};
class FactoryBuildException : public std::runtime_error {
    public:
        explicit FactoryBuildException(const std::string& msg)
            : std::runtime_error(msg) {}
    };
    
