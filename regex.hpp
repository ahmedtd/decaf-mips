
#ifndef REGEX_HEADER
#define REGEX_HEADER

#include <intializer_list>

class char_union
{
public:
    char_union(std::intializer_list<char> init_list);

    void match(istream &in);

private:
    std::unordered_set<char> match_set;
};

char_union::char_union(std::initializer<char> init_list)
    :
    match_set(init_list)
{
}

void char_union::match(istream &in, stringstream &putback)
{
    char next_char;
    while(in >> next_char)
    {
        next_char
    }
}

#endif
