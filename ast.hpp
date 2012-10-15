
#ifndef _H_ast
#define _H_ast

#include <ostream>
using std::ostream;
#include <map>
using std::map;
#include <string>
using std::string;

#include "location.hpp"

class Identifier;
class Decl;

class Node
{
public:
    Node(yyltype loc);
    Node();
    
    // void parent(const Node *p) const;
    // Node* parent() const;
    
    virtual bool scope_check(
        const map<const string, const Decl*> &current_scope
    );

public:
    const yyltype location;

protected:
    
    // The use of mutable here is almost certainly bad design. I really want to
    // have a more elegant design for the ast, but I'm not sure what I can do.
    // mutable Node *m_parent;
};

class Identifier : public Node 
{ 
public:
    Identifier(yyltype loc, const char *name);

public:
    const string name;
};

ostream& operator<<(std::ostream& out, const Identifier &id);


// This node class is designed to represent a portion of the tree that 
// encountered syntax errors during parsing. The partial completed tree
// is discarded along with the states being popped, and an instance of
// the Error class can stand in as the placeholder in the parse tree
// when your parser can continue after an error.
class Error : public Node
{
public:
    Error() : Node() {}
};

#endif
