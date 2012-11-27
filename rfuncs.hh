
#ifndef RFUNCS_HH
#define RFUNCS_HH

template<class Container>
auto rbegin(Container &container) -> decltype(container.rbegin())
{
    return container.rbegin();
}

template<class Container>
auto rend(Container &container) -> decltype(container.rend())
{
    return container.rend();
}

#endif
