
#ifndef SSA_HH
#define SSA_HH

// A phi function is an SSA idiom to alert the register/location
// allocator that two singleton names should share a location, and
// provides a new output name to refer to that shared location.
//
// Phi functions are placed at the beginning of basic blocks that can
// be reached in multiple ways, providing unified access to names that
// might have different subscripts in each of the preceding basic
// blocks.
class phi_function
{
public:
    target m_output;
    vector<target> m_inputs;
};

class basic_block
{
public:

public:
    // The phi functions to open this basic block
    vector<phi_function> m_phi_functions;
    
    // The instructions in this basic block -- the last instruction
    // must be one of goto, branch, call, return
    vector<unique_ptr<Instruction>> m_instructions;
};

// Tag structure to attach a basic block to a node
struct basic_block_tag
{
    typedef boost::vertex_property_tag kind;
};

// Vertex property for a control flow graph
typedef boost::property<basic_block_tag, basic_block> vertex_prop;

// Edge property for a control flow graph
typedef boost::property<boost::edge_weight_t, bool> edge_prop;

// Define the control flow graph type
typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    vertex_prop,
    edge_prop
    > control_flow_graph;

// Define the control flow graph traits type
typedef boost:graph_traits<control_flow_graph> control_flow_traits;

class function
{
public:
    
public:
    // The ordered set of parameter targets for the function.
    //
    // As optimizations are performed, this set of targets may be
    // pruned is it turns out some parameters are dead.
    vector<target> m_parameter_targets;

    // The control flow graph for this function
    control_flow_graph m_control_flow;
};

// Static single assignment representation of a compilation unit
class comp_unit
{
public:
    friend class function;

public:
    void emit(const Mips &emitter);
    
public:
    // The targets for globals in this compilation unit
    vector<target> m_globals;

    // The functions that are present in this compilation unit
    vector<function> m_functions;

    // The vtables that are used in this compilation unit
    vector<unique_ptr<VTable>> m_vtables;
};

#endif
