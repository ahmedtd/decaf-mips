
#ifndef SCOPE_HH
#define SCOPE_HH

class VarDecl;
class ClassDecl;
class InterfaceDecl;
class FnDecl;

#include <map>
using std::map;

class scope
{
public:
    map<string, const VarDecl*> vardecls;
    map<string, const ClassDecl*> classdecls;
    map<string, const InterfaceDecl*> ifacedecls;
    map<string, const FnDecl*> fndecls;

    map<string, Location> var_locations;
    map<string, int> var_this_offsets;

    string nearest_top_label;
    string nearest_bot_label;
};

#endif
