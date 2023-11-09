module;

#include <string>
#include <stack>
#include <cassert>
#include <iostream>
#include <vector>
#include <cmath>

export module CalcCommandDispatcher_CoreCommands;

import CalcBackend_Stack;
import CalcUtilities;

using std::string;
using std::vector;

#define CLONE(X) X* cloneImp() const override { return new X { *this } }


