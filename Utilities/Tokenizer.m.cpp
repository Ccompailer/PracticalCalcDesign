module;

#include <sstream>
#include <string>
#include <vector>
#include <istream>
#include <compare>
#include <iterator>
#include <ranges>
#include <iostream>
#include <algorithm>

export module CalcUtilities:Tokenizer;

using std::string;
using std::istream_iterator;
using std::back_inserter;
using std::vector;
using std::istream;

namespace ranges = std::ranges;
namespace views = std::views;

export class Tokenizer
{

};
