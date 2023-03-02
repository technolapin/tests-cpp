#include <iostream>
#include <vector>
#include <functional>
#include <type_traits>
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

template<typename ...>
struct MaskBuilder;

template<>
struct MaskBuilder<>
{
    inline static uint64_t mask = 0;
};


template<typename Head, typename... Tails>
struct MaskBuilder<Head, Tails...>
{
    inline static uint64_t mask = (1 << (Component<Head>::type_id)) | MaskBuilder<Tails...>::mask;
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


template<typename Out, typename... Args>
struct Currier{};
    
template<typename Out, typename Head, typename... Tail>
struct Currier<Out, Head, Tail...>
{
    static inline Out
    curry(std::function<Out(Head, Tail...)> foo, uint32_t index)
    {
        using deref = std::remove_reference<Head>::type;
        std::function<Out(Tail...)> foonext =  std::bind_front(foo, std::ref(Component<deref>::storage[index]));
        return Currier<Out,Tail...>::curry(foonext, index);
    }
};
template<typename Out>
struct Currier<Out>
{
    static inline Out
    curry(std::function<Out(void)> foo, [[maybe_unused]] uint32_t index)
    {
        return foo();
    }
};


struct System
{
    std::function<void(uint32_t)> functor;
    uint64_t mask;
};

struct World
{
    std::vector<Entity> entities;
    std::vector<System> systems;

    Entity & new_entity()
    {
        entities.push_back(Entity{});
        return entities.back();
    }

    template<typename... Comps>
    void
    add_system(std::function<void(Comps&...)> f)
    {
        std::function<void(uint32_t)> fun = [f](uint32_t index)
        {
            Currier<void, Comps&...>::curry(f, index);
        };
        systems.push_back(System{fun, MaskBuilder<Comps...>::mask});
    }
    
   


    void
    run_systems()
    {
        for (const auto & sys: systems)
        {
            std::cout << "Mask Sys: " << sys.mask << std::endl;
            for (const auto & e: entities)
            {
                std::cout << "  Mask Ent: " << e.mask << std::endl;
                if ((e.mask & sys.mask) != sys.mask) continue;
                
                std::cout << "    Entity n°" << e.index << std::endl;
                sys.functor(e.index);
            }
        }
    }
    
};

void
fooo(uint32_t index)
{
    auto & c = Component<char>::storage[index];
    const auto & s = Component<std::string>::storage[index];
    const auto & n = Component<int>::storage[index];

    c = s[n%s.size()];
}


int
test(int a, int b, int c)
{
    return a+b+c;
}


    
// void
// curry()
// {
//     [](T a, T b)
//     {
//         f(a,b,c)
//             };
// }

int
main()
{

    std::function<void(uint32_t)> sys_incr = [](uint32_t index)
    {
        Component<int>::storage[index]++;
    };
    std::function<void(int&)> sys_incr2 = [](int& val)
    {
        std::cout << "number = " << val << std::endl;
        val +=1;
    };

    std::function<void(char&,std::string&,int&)> sys_txt = [](char & c, std::string & s, int & n)
    {

        c = s[n%s.size()];
    };

    World world;
    world.new_entity()
        .with_component(3)
        .with_component('a')
        .with_component(std::string("Texte"));
    world.new_entity()
        .with_component('b');
    world.new_entity()
        .with_component(33);
    world.new_entity()
        .with_component(43)
        .with_component('w');
    world.new_entity()
        .with_component('b')
        .with_component(5);

//    world.add_system<int>(sys_incr);
    //   world.add_system<char, std::string, int>(fooo);



    world.add_system(sys_incr2);
    world.add_system(sys_txt);
    for (const auto & i: Component<int>::storage)
    {
        std::cout << i << " ";
    }std::cout << std::endl;

    for (uint32_t i = 0; i < 10; ++i)
    {
        world.run_systems();
       for (auto &c : Component<char>::storage)
        {
            std::cout << c<< " ";
        } std::cout << std::endl;

    }

    for (const auto & i: Component<int>::storage)
    {
        std::cout << i << " ";
    }std::cout << std::endl;
    // std::cout << "TEST " << test(1)(1)(1) << std::endl;
    /*
    for (uint32_t i = 0; i < 4; ++i)
    {
        std::cout << Component<int>::storage[i] << "*3 = "
                  << Currier<int, &int, &int, &int>::curry(test, i) << std::endl;
    }
    */
    //  std::cout << "TEST " << Component<int&>::type_id << std::endl;
    return 0;
    
}
