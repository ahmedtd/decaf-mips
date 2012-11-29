
#ifndef MAPPER_HH
#define MAPPER_HH

// A mapper takes a Static Single Assignment compilation unit and annotates
// every target with a concrete location.  The compilation unit object has a map
// of global targets to concrete positions and each function within it has a map
// of local targets to concrete positions.
class mapper
{
    
};

// A dumb-as-rocks mapper -- gives every target a concrete location, and makes phi functions
class simple_mapper
    : public mapper
{
};

#endif
