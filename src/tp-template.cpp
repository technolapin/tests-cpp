#include <iostream>
#include <vector>
#include <tuple>
static uint32_t next_type_id = 0;

template<typename T>
struct Component
{
    inline static uint32_t type_id = (next_type_id++);
    inline static bool is_component = true;
    inline static std::vector<T> storage = {};

    static void set_index(uint32_t index, T val)
    {
        if (storage.size() <= index)
        {
            storage.resize(index+1, val);
        }
        else
        {
            storage[index] = val;
        }
    }
};

struct Name
{
    std::string name;
};

struct Entity
{
    inline static uint32_t next_index = 0;

    const uint32_t index = next_index++;
    uint64_t mask;


    template<typename T>
    Entity& with_component(T val)
    {
        Component<T>::set_index(index, val);
        mask |= 1 << Component<T>::type_id;
        return *this;
    }
};


struct World
{
    std::vector<Entity> entities;

    
};



int
main()
{

    std::cout << Component<char>::type_id << std::endl;
    std::cout << Component<Name>::type_id << std::endl;
    std::cout << Component<uint32_t>::type_id << std::endl;
    std::cout << Component<char>::type_id << std::endl;
    Component<char>::storage.push_back('u');
    Component<char>::storage.push_back('w');
    Component<char>::storage.push_back('u');

    Entity e;
    e.with_component(3)
        .with_component('a')
        .with_component('b');
    
    for (auto &c : Component<char>::storage)
    {
        std::cout << c;
    }
    std::cout << std::endl;

    
    return 0;
    
}
