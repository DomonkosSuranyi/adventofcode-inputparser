#include "aoc.h"
#include <iostream>

int main()
{
    auto mapper = [](const std::string& line) {
        return MapResult<uint32_t>::ok(std::stoi(line));
    };
    auto inputs = getAoCInput<uint32_t, decltype(mapper)>("input.txt", '\n', mapper);
    if(std::holds_alternative<AoCError>(inputs))
    {
        std::cout << std::get<AoCError>(inputs).toString() << std::endl;
    }
    else
    {
        for (const auto& num : std::get<std::vector<uint32_t>>(inputs))
        {
            std::cout << num << std::endl;
        }
    }

}
