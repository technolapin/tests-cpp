#include <iostream>
#include <vector>
#include <array>

struct Luma
{
    unsigned char l;
};
struct RGB
{
    unsigned char r,g,b;
};
struct RGBA
{
    unsigned char r,g,b,a;
};

template<typename A, typename B>
A from(B & stuff)
{
    RGBA rgba = from<RGBA, B>(stuff);
    return from<A, RGBA>(rgba);
}

template<typename A, typename B>
B into(A & stuff)
{
    return from<A,B>(stuff);
}

template<>
RGB from<RGB, RGBA>(RGBA & pix)
{
    return {pix.r, pix.b, pix.g};
}
template<>
RGBA from<RGBA, RGB>(RGB & pix)
{
    return {pix.r, pix.b, pix.g, 255};
}
template<>
Luma from<Luma, RGBA>(RGBA & pix)
{
    return {(unsigned char) ((int(pix.r) + int(pix.g) + int(pix.b))/3)};
}
template<>
RGBA from<RGBA, Luma>(Luma & pix)
{
    return {pix.l,  pix.l, pix.l, 255};
}



std::ostream &
operator<<(std::ostream & os, Luma pix)
{
    return os << uint32_t(pix.l);
}

std::ostream &
operator<<(std::ostream & os, RGB pix)
{
    return os << uint32_t(pix.r) << " " << uint32_t(pix.g) << " " << uint32_t(pix.b);
}

std::ostream &
operator<<(std::ostream & os, RGBA pix)
{
    return os << uint32_t(pix.r) << " " << uint32_t(pix.g)
              << " " << uint32_t(pix.b) << " " << uint32_t(pix.a);
}



template<typename P, size_t W, size_t H>
struct Image
{
    std::array<std::array<P, H>, W> _pixels;
    
    Image(const P & base_pixel)
    {
        for (auto & col: _pixels)
        {
            for (auto & pix: col)
            {
                pix = base_pixel;
            }
        }
    }
    template<typename P2, size_t W2 = W, size_t H2 = H>
    Image(const Image<P2, W2, H2> & other)
    {
        for (auto & col: _pixels)
        {
            for (auto & pix: col)
            {
                pix = from<P, P2>(pix);
            }
        }
    }


    
};

// sbtimage => la lib

int
main()
{
    RGBA rgba = {105, 123, 34, 100};
    RGB rgb = {40, 50, 60};
    Luma luma = {200};

    std::cout << from<RGBA, RGB>(rgb) << std::endl;
    std::cout << from<RGBA, Luma>(luma)<< std::endl;
    std::cout << from<RGB, RGBA>(rgba)<< std::endl;
    std::cout << from<RGB, Luma>(luma)<< std::endl;
    std::cout << from<Luma, RGBA>(rgba)<< std::endl;
    std::cout << from<Luma, RGB>(rgb)<< std::endl;

    const auto img = Image<RGB, 16, 24>(rgb);
    
    const auto img2 = Image<Luma, 8, 8>(img);
    
    
    return 0;
}



