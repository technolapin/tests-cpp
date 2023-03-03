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

    inline static void set_index(uint32_t index, T val)
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
    inline static uint64_t mask = (1 << (Component<std::remove_cvref_t<Head>>::type_id)) | MaskBuilder<Tails...>::mask;
};


struct Entity
{

    const uint32_t index;
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
        using deref = std::remove_cvref<Head>::type;
        std::function<Out(Tail...)> foonext
            =  std::bind_front(foo, std::ref(Component<deref>::storage[index]));
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
    inline static uint32_t next_index = 0;
    std::vector<uint32_t> free_indexes = {};
    std::vector<Entity> entities;
    std::vector<System> systems;

    Entity & new_entity()
    {
        if (free_indexes.size() > 0)
        {
            entities.push_back(Entity{free_indexes.back(), 0});
            free_indexes.pop_back();
        }
        else
        {
            entities.push_back(Entity{next_index++, 0});
        }
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
    template<typename...Comp>
    using Function = void(Comp&...);
    
    template<typename... Comps>
    void
    add_system( Function<Comps&...> f)
    {
        add_system(std::function(f));
        
    }

   


    void
    run_systems()
    {
        for (const auto & sys: systems)
        {
            //    std::cout << "Mask Sys: " << sys.mask << std::endl;
            for (const auto & e: entities)
            {
                //  std::cout << "  Mask Ent: " << e.mask << std::endl;
                if ((e.mask & sys.mask) != sys.mask) continue;
                
                //std::cout << "    Entity n°" << e.index << std::endl;
                sys.functor(e.index);
            }
        }
    }
    
};



struct Pos  { double x,y,z;     };
struct Vel  { double x,y,z;     };    
struct Mass { double mass;      };
struct Name { std::string name; };

void
sys_pos_update(Pos & pos, Vel & vel)
{
    pos.x += vel.x;
    pos.y += vel.y;
    pos.z += vel.z;
    if (pos.y < 0)
    {
        pos.y = std::abs(pos.y);
        vel.y = -vel.y*0.99;
    }
}
void
sys_vel_update(Vel & vel, const Mass & mass)
{
    vel.y -= mass.mass*10.;
}

void
sys_print(const Pos & pos, const Name & name)
{
    std::cout << "Name: " << name.name << "   Pos: " << pos.x << " " << pos.y << " " << pos.z << std::endl;
}

int
main()
{
    World world;
    world.add_system(sys_pos_update);
    world.add_system(sys_vel_update);
    world.add_system(sys_print);

    world.new_entity()
        .with_component(Name{"Defo"})
        .with_component(Pos{0,10,0})
        .with_component(Mass{0.001});

    world.new_entity()
        .with_component(Name{"Teto"})
        .with_component(Pos{1,10,0})
        .with_component(Vel{0,0,0})
        .with_component(Mass{0.002});

    world.new_entity()
        .with_component(Name{"Momo"})
        .with_component(Pos{1,10,0})
        .with_component(Vel{0,0,0})
        .with_component(Mass{0.01});

    while(true)
    {
        world.run_systems();
    }

    return 0;
    
}
