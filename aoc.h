#pragma once

#include <utility>
#include <vector>
#include <string>
#include <variant>
#include <fstream>
#include <utility>

struct FileError
{
    FileError(const std::string& path) : path(path)
    {}

    std::string path;

    std::string toString()
    {
        return "The file " + path + " cannot be found.";
    }
};

struct MappingError
{
    std::string message;

    MappingError(const std::string& message) : message(message)
    {}

    std::string toString()
    {
        return message;
    }
};

struct AoCError {
    std::variant<FileError, MappingError> error;

    std::string toString()
    {
        if(std::holds_alternative<FileError>(error))
        {
            return std::get<FileError>(error).toString();
        }

        if(std::holds_alternative<MappingError>(error))
        {
            return std::get<MappingError>(error).toString();
        }

        return "Unknown type of AoCError";
    }
};

template<typename T>
class MapResult {
public:
    static MapResult<T> error(const std::string& msg)
    {
        return MapResult(MappingError(msg));
    }

    static MapResult<T> ok(T&& result)
    {
        return MapResult(std::move(result));
    }

    bool isOk() const
    {
        return std::holds_alternative<T>(inner_);
    }

    T unwrapOk()
    {
        return std::get<T>(inner_);
    }

    MappingError unwrapError()
    {
        return std::get<MappingError>(inner_);
    }

private:
    std::variant<T, MappingError> inner_;

    MapResult(MappingError&& error) : inner_(error)
    {}

    MapResult(T&& value) : inner_(std::forward<T>(value))
    {}
};

template<typename T, typename MAPPER>
std::variant<std::vector<T>, AoCError> getAoCInput(const char* filepath, const char separator, MAPPER mapper) {
    std::ifstream file(filepath);
    if (!file)
    {
        return AoCError { FileError(filepath) };
    }

    std::vector<T> ret;
    std::string line;

    while (getline(file, line, separator))
    {
        MapResult<T> result = mapper(line);
        if(result.isOk())
        {
            ret.push_back(result.unwrapOk());
        }
        else
        {
            return AoCError { result.unwrapError() };
        }
    }
    return ret;
}
