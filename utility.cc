/* File: utiliy.cc
 * ---------------
 * Implementation of simple printing functions to report failures or
 * debugging information triggered by keys.
 */

#include <algorithm>
using std::find;
#include <string>
using std::string;
#include <map>
using std::map;
using std::begin;
using std::end;
#include <utility>
using std::get;

#include "utility.h"
#include <stdarg.h>
#include <string.h>

static map<string, bool> debug_keys;
static const int BufferSize = 2048;

void Failure(const char *format, ...)
{
  va_list args;
  char errbuf[BufferSize];
  
  va_start(args, format);
  vsprintf(errbuf, format, args);
  va_end(args);
  fflush(stdout);
  fprintf(stderr,"\n*** Failure: %s\n\n", errbuf);
  abort();
}


// int IndexOf(const string &key)
// {
//     auto pos = find(begin(debugKeys), end(debugKeys));
//     return (pos == end(debugKeys)) ? -1 : (pos - begin(debugKeys));
// }


bool IsDebugOn(const string &key)
{
    auto pos = debug_keys.find(key);
    return (pos == end(debug_keys)) ? false : get<1>(*pos);
}


void SetDebugForKey(const string &key, bool value)
{
    debug_keys[key] = value;
}



void PrintDebug(const string &key, const char *format, ...)
{
    va_list args;
    char buf[BufferSize];

    if (!IsDebugOn(key))
        return;
  
    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);
    printf("+++ (%s): %s%s",
           key.c_str(),
           buf,
           (buf[strlen(buf)-1] != '\n') ? "\n" : "");
}


void ParseCommandLine(int argc, char *argv[])
{
  if (argc == 1)
    return;
  
  if (strcmp(argv[1], "-d") != 0) { // first arg is not -d
    printf("Usage:   -d <debug-key-1> <debug-key-2> ... \n");
    exit(2);
  }

  for (int i = 2; i < argc; i++)
    SetDebugForKey(argv[i], true);
}

