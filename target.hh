
#ifndef TARGET_HH
#define TARGET_HH

class target
{
public:
    enum class value_type { lvalue, rvalue };
    enum class location_type { parameter, local, global };

public:

    // Constructors
    target();
    target(const string &name,
           unsigned int subscript,
           const value_type &value_class,
           const location_type &location_class,
           const vector<unsigned int> &indirection_offsets);

    // Virtual destructor
    virtual ~target();

    // Possibly some methods to generate loads/stores with transparent
    // indirection

    // Accessors
    const string& name() const;
    const unsigned int subscript() const;
    const value_type& value_class() const;
    const location_type& location_class() const;
    const vector<unsigned int> indirection_offsets() const;

private:
    
    // Base name of the target -- "tmp" for temporaries and the variable name
    // otherwise.
    string m_name;
    
    // The subscript used to label unique occurrences of the same base label
    unsigned int m_subscript;
    
    // Whether or not this target is being used in an lvalue or an rvalue
    // context
    value_type m_value_class;

    // What sort of accessing mode this target will use.
    //
    // location_type::parameter: This target may appear in a parameter register
    // or in the activation record
    //
    // location_type::local: This target is local to the function, and may live
    // in a temp register or on the stack
    //
    // location_type::global: This target is a global, and will live at some
    // offset from the global pointer.
    location_type m_location_class;
    
    // If and how this target is indirected.
    //
    // If this vector is empty, then this target is a direct target.  If this
    // vector has elements, then the length of the vector determines how many
    // layers of indirection are used to access the target, and each element is
    // an offset to apply at the corresponding level.
    vector<unsigned int> m_indirection_offsets;
};

bool have_common_base(const target &left, const target &right);

#endif
