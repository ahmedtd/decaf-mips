
#include "ssa.hh"

comp_unit::comp_unit()
{
}

void comp_unit::emit(Mips &emitter)
{
    // Spit out each vtable
    for(const VTable &cur_vtable : m_vtables)
    {
        cur_vtable.emit(emitter);
    }

    // Spit out each global
    map<target, location> global_locations;
    unsigned int cur_global_offset = 0;
    for(const target &cur_global : m_globals)
    {
        // For now, don't optimize over global targets -- assign each a location
        global_locations[cur_global]
            = (location(location::global, cur_global_offset++));
        
    }

    // Iterate over each function
    for(const function &cur_function : m_functions)
    {
        
    }
}
