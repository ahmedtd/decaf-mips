/* File: ast.cc
 * ------------
 */

#include <stdexcept>
using std::logic_error;

#include "ast.hpp"
#include "ast_type.hpp"
#include "ast_decl.hpp"

Node::Node(yyltype loc)
    : location(loc)//,
      // m_parent(nullptr)
{
}

Node::Node()
    : location()//,
      // m_parent(nullptr)
{
}

// void Node::parent(Node *p)
// {
//     m_parent = p;
// }

// Node* Node::parent()
// {
//     return m_parent;
// }

bool Node::scope_check(
    const scope &exterior_scope)
{
    // Checking the scope doesn't have meaning for all nodes in the AST
    
    throw logic_error("Node::scope_check called on a Node for which it "
                      "doesn't have meaning\n");

    return false;
}
	 
Identifier::Identifier(yyltype loc, const char *n)
    : Node(loc),
      name(n)
{
}

ostream& operator<<(ostream& out, const Identifier &id)
{
    return out << id.name;
}

