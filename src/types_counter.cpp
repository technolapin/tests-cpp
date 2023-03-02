#include <iostream>
#include <vector>

static uint32_t next_type_id = 0;

template<typename T>
struct TypeCounter
{
    inline static uint32_t type_id = (next_type_id++);
};


int
main(void)
{
    std::cout << TypeCounter<char>::type_id << std::endl;
    std::cout << TypeCounter<char>::type_id << std::endl;
    std::cout << TypeCounter<uint32_t>::type_id << std::endl;
    std::cout << TypeCounter<std::vector<char>>::type_id << std::endl;
    std::cout << TypeCounter<char>::type_id << std::endl;
    std::cout << TypeCounter<uint32_t>::type_id << std::endl;

    return 0;
}
