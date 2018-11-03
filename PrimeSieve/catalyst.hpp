/*
_________         __         .__                  __   
\_   ___ \_____ _/  |______  |  | ___.__. _______/  |_ 
/    \  \/\__  \\   __\__  \ |  |<   |  |/  ___/\   __\
\     \____/ __ \|  |  / __ \|  |_\___  |\___ \  |  |  
 \______  (____  /__| (____  /____/ ____/____  > |__|  
        \/     \/          \/     \/         \/        

https://github.com/pranav-srinivas-kumar/catalyst

Copyright (c) 2018 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <array>
#include <list>
#include <tuple>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <thread>
#include <functional>
#include <numeric>
#include <stdarg.h>
#include <memory>
#include <climits>
#include <typeinfo>
#include <type_traits>
#include <bitset>
//#include <any>
#include <random>
#include <cstring>
#include <regex>
#include <cctype>
// Expand Macros within macros like below
// Used to separate interface from implementation
#define EXPAND(x) x

// Boolean operators
#define is ==
#define isnt !=

// Lambda as a keyword
#define lambda [=]

// Variable and reference
#define var auto
#define ref var&

// Control flow operations
#define pass (void)0

// Python-style print
// End string class - used in print(..., end('!'))
class end {
public:
	template<typename Type>
	end(Type end_argument) {
		std::stringstream endstringstream;
		endstringstream << end_argument;
		end_ = endstringstream.str();
	}
	friend std::ostream& operator<<(std::ostream& os, const end& end_object);
	std::string end_;
};

std::ostream& operator<<(std::ostream& os, const end& end_object) {
	os << end_object.end_;
	return os;
}

// Separator class - used in print(..., sep(''))
class sep {
public:
	template<typename Type>
	sep(Type sep_argument) {
		std::stringstream sepstringstream;
		sepstringstream << sep_argument;
		sep_ = sepstringstream.str();
	}
	friend std::ostream& operator<<(std::ostream& os, const sep& sep_object);
	std::string sep_;
};

std::ostream& operator<<(std::ostream& os, const sep& sep_object) {
	os << sep_object.sep_;
	return os;
}

// Static variables to get print(...) to work
static std::string __print_end__ = "\n";
static std::string __print_sep__ = " ";
static bool __print_end_specified__ = false;
static bool __print_sep_specified__ = false;
static unsigned int __print_num_arguments__ = 0;
static unsigned int __print_num_arguments_so_far__ = 0;

// Recursive functions to set __print_end__ and __print_sep__ variables
void __set_sep_and_end_for_print__() {}
template<typename First, typename ...Rest>
void __set_sep_and_end_for_print__(First && first, Rest && ...rest) {
	if (std::is_same<decltype(first), end&&>::value) {
		std::stringstream endstream;
		endstream << std::forward<First>(first);
		__print_end__ = endstream.str();
		__print_end_specified__ = true;
	}
	else if (std::is_same<decltype(first), sep&&>::value) {
		std::stringstream sepstream;
		sepstream << std::forward<First>(first);
		__print_sep__ = sepstream.str();
		__print_sep_specified__ = true;
	}
	else
		__print_num_arguments__ += 1;

	__set_sep_and_end_for_print__(std::forward<Rest>(rest)...);
}

// Returns string to be printed by print(...)
std::string string_for_cout() { return ""; }
template<typename First, typename ...Rest>
std::string string_for_cout(First && first, Rest && ...rest) {
	std::string result = "";
	if (!std::is_same<decltype(first), end&&>::value and
		!std::is_same<decltype(first), sep&&>::value) {
		__print_num_arguments_so_far__ += 1;
		std::stringstream resultstream;

		resultstream << std::forward<First>(first);
		result += resultstream.str();
		if (__print_num_arguments_so_far__ < __print_num_arguments__)
			result += __print_sep__;
	}
	result += string_for_cout(std::forward<Rest>(rest)...);
	return result;
}
#define print_implementation(...) [=]() { \
	__set_sep_and_end_for_print__(__VA_ARGS__); \
	std::cout << string_for_cout(__VA_ARGS__); \
	std::cout << __print_end__; \
	__print_end__ = "\n"; \
	__print_sep__ = " "; \
	__print_num_arguments__ = 0; \
	__print_num_arguments_so_far__ = 0; \
}()
#define print(...) EXPAND(print_implementation(__VA_ARGS__))
#define sep sep
#define newline "", end("\n")

// Read input from command line
std::string input(std::string prompt) {
	print(prompt);
	std::string input;
	getline(std::cin, input);
	return input;
}
#define input input

// Geting current directory
#include <stdio.h>  /* defines FILENAME_MAX */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
static const std::string slash = "\\";
#include <direct.h>
#define GetCurrentWorkingDirectory _getcwd
#else
static const std::string slash = "/";
#include <unistd.h>
#define GetCurrentWorkingDirectory getcwd
#endif

// Python-style range
// Range with 1 argument - Return list [0, end)
template<typename Type>
std::vector<Type> range(Type end) {
	std::vector<Type> result;
	for (Type i = 0; i < end; i++)
		result.push_back(i);
	return result;
}

// Overloads when we want to return a vector of ints 
// but end is size_t to avoid the type conversion warning
std::vector<int> range(size_t end) {
	std::vector<int> result;
	for (int i = 0; i < end; i++)
		result.push_back(i);
	return result;
}

// Range with 2 arguments - Return list [start, end)
template<typename Type>
std::vector<Type> range(Type start, Type end) {
	std::vector<Type> result;
	for (Type i = start; i < end; i++)
		result.push_back(i);
	return result;
}

// Overloads when we want to return a vector of ints 
// but end is size_t to avoid the type conversion warning
std::vector<int> range(int start, size_t end) {
	std::vector<int> result;
	for (int i = start; i < end; i++)
		result.push_back(i);
	return result;
}

// Range with 3 arguments - Return list [start, end) 
// where each successive pair of items is 'step' distance apart
template<typename Type>
std::vector<Type> range(Type start, Type end, Type step) {
	if (step == 0)
		throw std::invalid_argument("Step must be greater than zero");
	std::vector<Type> result;
	if (step > 0) {
		for (Type i = start; i < end; i += step)
			result.push_back(i);
	}
	else {
		for (Type i = start; i > end; i += step)
			result.push_back(i);
	}
	return result;
}

// Overloads when we want to return a vector of ints 
// but end/step is size_t to avoid the type conversion warning
std::vector<int> range(int start, size_t end, size_t step) {
	if (step == 0)
		throw std::invalid_argument("Step must be greater than zero");
	std::vector<int> result;
	for (int i = start; i < end; i += static_cast<int>(step))
		result.push_back(i);
	return result;
}

// Python-style len(list)
template <typename Type>
size_t len(Type argument) {
	return argument.size();
}

// Enumerate like in Python
// TODO: Get this to work with std::maps and other containers
template <typename Type>
std::vector<std::pair<size_t, Type>> enumerate(std::vector<Type> list) {
	std::vector<std::pair<size_t, Type>> result;
	var position = 0;
	for (var iter = std::begin(list); iter != std::end(list); ++iter) {
		result.push_back({ position, *iter });
		position += 1;
	}
	return result;
}

template <typename Type>
std::vector<std::pair<size_t, Type>> enumerate(std::initializer_list<Type> list) {
	std::vector<std::pair<size_t, Type>> result;
	var position = 0;
	for (var iter = std::begin(list); iter != std::end(list); ++iter) {
		result.push_back({ position, *iter });
		position += 1;
	}
	return result;
}

std::fstream open(const char * filename, const char * mode) {
	/*
	'r'	open for reading (default)
	'w'	open for writing, truncating the file first
	'x'	open for exclusive creation, failing if the file already exists
	'a'	open for writing, appending to the end of the file if it exists
	'b'	binary mode
	't'	text mode (default)
	'+'	open a disk file for updating (reading and writing)
	'U'	universal newlines mode (deprecated)
	*/
	var result = std::fstream();
	if (mode == "r") {
		result = std::fstream(filename, std::ios::in);
	}
	else if (mode == "w") {
		result = std::fstream(filename, std::ios::out | std::ios::trunc);
	}
	else if (mode == "a") {
		result = std::fstream(filename, std::ios::out | std::ios::app);
	}
	else {
		result = std::fstream(filename, std::ios::in);
	}
	if (result.is_open())
		return result;
	else
		throw std::ios_base::failure("Unable to open file" + std::string(filename));
}
#define open open

std::string read(std::fstream& file_stream) {
	std::string result = "";
	// Find size of file
	file_stream.seekg(0, std::ios::end);
	result.reserve(file_stream.tellg());
	file_stream.seekg(0, std::ios::beg);

	// single-pass input iterator that reads successive chars
	result.assign((std::istreambuf_iterator<char>(file_stream)),
		std::istreambuf_iterator<char>());
	return result;
}
#define read read

size_t write(std::fstream& file_stream, std::string contents) {
	file_stream << contents;
	return contents.length();
}
#define write write

void close(std::fstream& file_stream) {
	file_stream.close();
}
#define close close

bool del(const char * filename) {
	if (remove(filename) != 0)
		return false;
	else 
		return true;
}

// in for use in for expressions, e.g., for (var x in list)
#define in :

// In-place sort operation
// compute number of (variadic) macro arguments
// from http://groups.google.com/group/comp.std.c/browse_thread/thread/77ee8c8f92e4a3fb/346fc464319b1ee5?pli=1
#define PP_NARG(...) EXPAND( PP_NARG_(__VA_ARGS__, PP_RSEQ_N()) )
#define PP_NARG_(...) EXPAND( PP_ARG_N(__VA_ARGS__) )
#define PP_ARG_N(_1, _2, _3, N, ...) N
#define PP_RSEQ_N() 3, 2, 1, 0


// macro for exactly 2 arguments
#define SORT_1(list) std::sort(list.begin(), list.end()) 
// macro for exactly 3 arguments
#define SORT_2(list, comparison_function) std::sort(list.begin(), list.end(), comparison_function)

// macro selection by number of arguments
#define SORT_(N) SORT_##N
#define SORT_EVAL(N) SORT_(N)
#define sort(...) EXPAND( SORT_EVAL(EXPAND( PP_NARG(__VA_ARGS__) ))(__VA_ARGS__) )

// Sorted - returns a sorted list (copy) of the input list
template<typename Type>
std::vector<Type> sorted(std::initializer_list<Type> input_container) {
	std::vector<Type> result = input_container;
	sort(result);
	return result;
}

template <typename Type, class Lambda>
std::vector<Type> sorted(std::initializer_list<Type> input_container, Lambda sort_function) {
	std::vector<Type> result = input_container;
	sort(result, sort_function);
	return result;
}
#define sorted sorted

// Loop over multiple lists and zip 'em up
static size_t __smallest_zip_size__ = 0;

void __zip_set_smallest_size__() {}
template<typename First, typename ...Rest>
void __zip_set_smallest_size__(First && first, Rest && ...rest) {
	if (__smallest_zip_size__ == 0) {
		__smallest_zip_size__ = std::forward<First>(first).size();
	}
	else if (__smallest_zip_size__ > std::forward<First>(first).size())
		__smallest_zip_size__ = std::forward<First>(first).size();
	__zip_set_smallest_size__(std::forward<Rest>(rest)...);
}

// Zip operation
template<typename First, typename ...Rest>
auto zip(std::vector<First> first, std::vector<Rest> ...rest) {
	__zip_set_smallest_size__(first, rest...);
	std::vector<std::tuple<First, Rest...>> result;
	for(size_t i = 0; i < __smallest_zip_size__; i++)
		result.push_back(std::tuple<First, Rest...>({ first[i], rest[i]... }));
	__smallest_zip_size__ = 0;
	return result;
}

// reverse a list
template <typename Type>
Type reversed(Type list) {
	Type result = list;
	std::reverse(result.begin(), result.end());
	return result;
}

// set
template <typename Type>
std::set<Type> set(std::initializer_list<Type> list) {
	std::set<Type> result(list.begin(), list.end());
	return result;
}

// Contains utility functions
// Set contains given value
template <typename TValue>
bool contains(const std::set<TValue>& c, const TValue& x) {
	return std::find(std::begin(c), std::end(c), x) != std::end(c);
}

// Vector contains given value
template <typename TValue>
bool contains(const std::vector<TValue>& c, const TValue& x) {
	return std::find(std::begin(c), std::end(c), x) != std::end(c);
}

// Array contains given value
template <typename TValue, std::size_t N>
bool contains(const std::array<TValue, N>& c, const TValue& x) {
	return std::find(std::begin(c), std::end(c), x) != std::end(c);
}

// Map contains given key
template <typename TValue, typename... Ts>
bool contains(const std::map<Ts...>& c, const TValue& x) {
	return c.find(x) != std::end(c);
}

// String contains
bool contains(std::string input, std::string search_string, bool ignore_case = false) {
	bool result = false;
	if (ignore_case)
	{ // if case is to be ignored, convert both input and search string to all upper case
		std::transform(input.begin(), input.end(), input.begin(), ::toupper);
		std::transform(search_string.begin(), search_string.end(), search_string.begin(), ::toupper);
	}
	if (input.find(search_string) != std::string::npos)
	{ // search using find algorithm
		result = true;
	}
	return result;
}

// Python-style filter
// As the name suggests, filter creates a list of elements for 
// which a function returns true. Here is a short and concise example:
template <typename Lambda, class Iterable>
Iterable filter(Lambda filter_function, Iterable container) {
	Iterable result;
	std::copy_if(container.begin(), container.end(), std::back_inserter(result), filter_function);
	return result;
}

// Get current working directory
std::string getcwd(void) {
	char buffer[FILENAME_MAX];
	GetCurrentWorkingDirectory(buffer, FILENAME_MAX);
	std::string current_working_dir(buffer);
	return current_working_dir;
}

// Iterator shorthand
#define iter(iterable) std::begin(iterable)
#define next(iterator) (iterator = std::next(iterator))
#define prev(iterator) (iterator = std::prev(iterator))
#define deref(item) *item

// Check if two vectors are equal
template <typename Type>
bool equal(std::vector<Type> lhs, std::vector<Type> rhs) {
	if (lhs.size() != rhs.size())
		return false;
	else {
		for (size_t i = 0; i < lhs.size(); i++) {
			if (lhs[i] != rhs[i])
				return false;
		}
		return true;
	}
}

// More Algorithms
template <typename Type>
auto sum(Type iterable) {
	return std::accumulate(iterable.begin(), iterable.end(), 0);
}

// Specialization for a vector of bools
int sum(std::vector<bool> iterable) {
	int result = 0;
	for (size_t i = 0; i < iterable.size(); i++) {
		result += (iterable[i] == true ? 1 : 0);
	}
	return result;
}

// Time sleep
#define sleep(seconds) std::this_thread::sleep_for(std::chrono::milliseconds(seconds * 1000))

// String operations
size_t strlcpy(char *dst, const char *src, size_t destination_size) {
	size_t length = strlen(src);
	if (destination_size) {
		size_t bl = (length < destination_size - 1 ? length : destination_size - 1);
		((char*)memcpy(dst, src, bl))[bl] = 0;
	}
	return length;
}

std::string format(const std::string format_string, ...) {
	int final_n, n = ((int)format_string.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1) {
		formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
		strlcpy(&formatted[0], format_string.c_str(), format_string.length());
		va_start(ap, format_string);
		final_n = vsnprintf(&formatted[0], n, format_string.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return std::string(formatted.get());
}

// Python-style String slicing
std::string slice(std::string input_string, int start_index, int end_index = INT_MAX, int step = 1) {
	std::string result;
	if (end_index == INT_MAX) { // if end index is default, i.e., till end of string
		if (start_index < 0) { // if start index is negative, then start start_index characters from end of string
			for (int i = static_cast<int>(input_string.size()) + start_index; i < static_cast<int>(input_string.size()); i = i + step) {
				result += input_string[i];
			}
		}
		else { // start_index is not negative; iterate from start_index till end of string
			for (int i = start_index; i < static_cast<int>(input_string.size()); i = i + step) {
				result += input_string[i];
			}
		}
	}
	else { // end_index is not std::string::npos
		if (end_index < 0) { // if end_index is negative, start from start_index and go till (end_of_string + end_index)
			for (int i = start_index; i < static_cast<int>(input_string.size()) + end_index; i = i + step) {
				result += input_string[i];
			}
		}
		else { // if end_index is not negative either, then this is the trivial case
			for (int i = start_index; i < end_index; i = i + step) {
				result += input_string[i];
			}
		}
	}
	return result;
}
#define slice slice

// split string based on a delimiter string
// supports multi-character delimiter
// returns a vector of substrings after split
std::vector<std::string> split(std::string input_string, std::string delimiter, 
	std::vector<std::string> * result = new std::vector<std::string>()) {
	size_t counter_position = 0;
	size_t delimiter_position = input_string.find(delimiter); // check if delimiter is in input string
	if (delimiter_position != std::string::npos) { // if delimiter position is not end_of_string
		std::string split_string = input_string.substr(counter_position, delimiter_position);
		delimiter_position += delimiter.size();
		std::string split_remaining = input_string.erase(counter_position, delimiter_position);
		result->push_back(split_string);
		// Recursion! Recursion everywhere!
		std::vector<std::string> result_remaining = split(split_remaining, delimiter, result);
	}
	else { // delimiter not in input string. Just add entire input string to result vector
		result->push_back(input_string);
	}
	return *result;
}
#define split split

// Check if string startswith some character
bool starts_with(std::string input_string, char c, bool ignore_case = false) {
	bool result = false;
	if (input_string.size() > 0) {
		char first_character = input_string[0];
		if (ignore_case) {
			first_character = toupper(first_character);
			c = toupper(c);
		}
		if (first_character == c)
			result = true;
	}
	return result;
}

// Check if string startswith some substring
bool starts_with(std::string input_string, std::string starter, bool ignore_case = false) {
	bool result = false;
	if (input_string.size() > 0 && input_string.size() > starter.size()) {
		std::string input_copy = input_string;
		for (size_t i = 0; i < starter.size(); i++) {
			char input_character = input_string[i];
			char starter_character = starter[i];
			if (ignore_case) {
				input_character = toupper(input_character);
				starter_character = toupper(starter_character);
			}
			if (input_character == starter_character) {
				result = true;
				continue;
			}
			else {
				result = false;
				break;
			}
		}
	}
	return result;
}

// Check if string endswith character
bool ends_with(std::string input_string, char c, bool ignore_case = false) {
	bool result = false;
	if (input_string.size() > 0) {
		char last_character = input_string[input_string.size() - 1];
		if (ignore_case) {
			last_character = toupper(last_character);
			c = toupper(c);
		}
		if (last_character == c)
			result = true;
	}
	return result;
}

// Count number of times some search_string appears in input_string
size_t count(std::string input_string, std::string search_string, bool ignore_case = false) {
	size_t result = 0;
	if (contains(input_string, search_string, ignore_case)) {
		if (ignore_case) {
			std::transform(input_string.begin(), input_string.end(), input_string.begin(), ::toupper);
			std::transform(search_string.begin(), search_string.end(), search_string.begin(), ::toupper);
		}
		auto split_vector = split(input_string, search_string);
		result = split_vector.size() - 1;
	}
	return result;
}

// repeat input string for count number of times, optionally with a connector string
std::string repeat(std::string input_string, int count_value, std::string connector = "") {
	std::string result = "";
	while (count_value > 0) {
		result += input_string + connector;
		count_value--;
	}
	return result;
}

// returns copy of input string that is all upper case
std::string upper(std::string input_string) {
	std::string result;
	result.resize(input_string.size());
	std::transform(input_string.begin(), input_string.end(), result.begin(), ::toupper);
	return result;
}

// returns copy of input string that is all lower case
std::string lower(std::string input_string) {
	std::string result;
	result.resize(input_string.size());
	std::transform(input_string.begin(), input_string.end(), result.begin(), ::tolower);
	return result;
}

// find and replace substring in input string, optionally for a limited number of times
// by default, every occurrence of find_string is replaced by replace_string
std::string replace(std::string input_string, std::string find_string,
	std::string replace_string, int replace_count = -1) {
	std::string result = "";
	auto split_vector = split(input_string, find_string);
	auto split_size = split_vector.size();
	auto max_replace_count = split_size - 1;
	if (replace_count == -1) {
		for (auto &s : split_vector) {
			if (max_replace_count > 0)
				result += s + replace_string;
			else
				result += s;
			max_replace_count--;
		}
	}
	else {
		for (auto &s : split_vector) {
			if (replace_count > 0) {
				if (max_replace_count > 0)
					result += s + replace_string;
				else
					result += s;
				max_replace_count--;
			}
			else {
				if (max_replace_count > 0)
					result += s + find_string;
				else
					result += s;
				max_replace_count--;
			}
			replace_count--;
		}
	}
	return result;
}

// join a vector of strings into a single string
std::string join(std::vector<std::string> input, std::string connector = "") {
	std::string result;
	size_t max_connector_count = input.size() - 1;
	for (auto &s : input) {
		if (max_connector_count > 0)
			result += s + connector;
		else
			result += s;
		max_connector_count--;
	}
	return result;
}

// bulk find and replace sub-string in input string using translation table
std::string translate(std::string input_string,
	std::map<std::string, std::string> translation_table) {
	std::string result = input_string;
	for (auto &pair : translation_table)
		result = replace(result, pair.first, pair.second);
	return result;
}

// trim white spaces from the left end of an input string
std::string ltrim(std::string input_string) {
	std::string result = input_string;
	result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](int ch) {
		return !std::isspace(ch);
	}));
	return result;
}

// trim white spaces from right end of an input string
std::string rtrim(std::string input_string) {
	std::string result = input_string;
	result.erase(std::find_if(result.rbegin(), result.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(),
		result.end());
	return result;
}

// trim white spaces from either end of an input string
std::string trim(std::string input_string) {
	return ltrim(rtrim(input_string));
}

// Returns true if the two input strings are equal
bool equal(std::string lhs, std::string rhs) {
	return (lhs.compare(rhs) == 0);
}

// generic find function
// if sub-string is found, returns the index
// if sub-string is not found, returns std::string::npos
size_t find(std::string input_string, std::string search_string) {
	return input_string.find(search_string);
}

// finds first occurrence of sub-string in input string
size_t find_first(std::string input_string, std::string search_string) {
	return input_string.find_first_of(search_string);
}

// finds last occurrence of sub-string in input string
size_t find_last(std::string input_string, std::string search_string) {
	return input_string.find_last_of(search_string);
}

// performs std regex search and returns a vector of matched results
std::vector<std::string> find_regex(std::string input_string, std::string regex_string) {
	std::vector<std::string> results;
	std::smatch match_result;
	while (std::regex_search(input_string, match_result, std::regex(regex_string)))
	{
		for (auto &m : match_result)
			results.push_back(m);
		input_string = match_result.suffix();
	}
	return results;
}

// Python-style with expression
#define as ,
bool __with_result__ = false;

#define with2(expression, ...) \
	__with_result__ = [=]() { \
		try { \
			expression; \
			return true; \
		} \
		catch(...) { \
			return false; \
		} \
	}(); \
	{ \
		if (__with_result__ && !std::is_null_pointer<decltype(expression)>::value) \
			__VA_ARGS__ \
	}

#define with3(expression, target, ...) \
	__with_result__ = [=]() { \
		try { \
			var target = expression; \
			return true; \
		} \
		catch(...) { \
			return false; \
		} \
	}(); \
	{ \
		decltype(expression) target = decltype(expression)(); \
		if (__with_result__) \
			target = expression; \
		if (__with_result__ && !std::is_null_pointer<decltype(expression)>::value) \
			__VA_ARGS__ \
	}

#define GET_WITH_MACRO(_1,_2,_3,NAME,...) NAME
#define with(...) EXPAND(GET_WITH_MACRO(__VA_ARGS__, with3, with2)(__VA_ARGS__))

// Try-catch
#define raise throw
#define except catch

// Negations to common statements
#define unless(condition) if(!condition)
#define until(condition) while(!condition)

// Python-style round function
double round(double x, unsigned int digits) {
	if (digits > 0)
		return round(x*10.0, digits - 1) / 10.0;
	else 
		return round(x);
}

// Printer for std::tuples
template<std::size_t...> struct seq {};

template<std::size_t N, std::size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

template<std::size_t... Is>
struct gen_seq<0, Is...> : seq<Is...> {};

template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple(std::basic_ostream<Ch, Tr>& os, Tuple const& t, seq<Is...>) {
	using swallow = int[];
	(void)swallow {
		0, (void(os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), 0)...
	};
}

template<class Ch, class Tr, class... Args>
auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
-> std::basic_ostream<Ch, Tr>&
{
	os << "(";
	print_tuple(os, t, gen_seq<sizeof...(Args)>());
	return os << ")";
}

// Pretty print std::vector
template<typename T>
std::ostream & operator<<(std::ostream & os, std::vector<T> vec) {
	os << "[";
	if (vec.size() > 0) {
		std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<T>(os, ", "));
		os << vec.back();
	}
	os << "]";
	return os;
}

// Specialization for a vector of bools
std::ostream & operator<<(std::ostream & os, std::vector<bool> vec) {
	os << "[";
	if (vec.size() > 0) {
		for (size_t i = 0; i < vec.size() - 1; i++) {
			os << (vec[i] == true? "true" : "false");
			os << ", ";
		}
		os << (vec[vec.size() - 1] == true ? "true" : "false");
	}
	os << "]";
	return os;
}


// Pretty print std::map
template <typename Key, typename Value>
std::ostream& operator<<(std::ostream & os, std::map<Key, Value> map) {
	for (auto kvpair in map) {
		os << kvpair.first << " : " << kvpair.second << std::endl;
	}
	return os;
}

// Make a set out of a list of arguments
template<typename First>
std::set<First> make_set() {
	return {}; 
}

template<typename First>
std::set<First> make_set(First && first) {
	return std::set<First>(first); 
}

template<typename First, typename ...Rest>
std::set<First> make_set(First && first, Rest && ...rest) {
	std::set<First> result = { first, rest... };
	return result;
} 

// STL containers and container operations
template<typename First>
std::vector<First> make_list() {
	return {}; 
}

template<typename First>
std::vector<First> make_list(First && first) {
	return std::vector<First>(first); 
}

template<typename First, typename ...Rest>
std::vector<First> make_list(First && first, Rest && ...rest) {
	std::vector<First> result = { first, rest... };
	return result;
} 

// Specialization for character arrays - returns a vector of std::strings
template<typename ...Rest>
std::vector<std::string> make_list(const char first[], Rest && ...rest) {
	std::vector<std::string> result = { first, rest... };
	return result;
}

// Specialization for vector of vectors
template<typename Type, typename ...Rest>
std::vector<std::vector<Type>> make_list(std::vector<Type> first, Rest && ...rest) {
	std::vector<std::vector<Type>> result = { first, rest... };
	return result;
}

template<typename Type, typename ...Rest>
std::vector<std::vector<Type>> make_list(std::initializer_list<Type> first, Rest && ...rest) {
	std::vector<std::vector<Type>> result = { first, rest... };
	return result;
}

// Specialization for std::string - Return list of characters in string
std::vector<std::string> make_list(std::string input_string) {
	std::vector<std::string> result;
	for (size_t i = 0; i < input_string.size(); i++)
		result.push_back(std::string(1, input_string[i]));
	return result;
}

template<typename Type>
Type index(std::vector<Type> container, size_t index, Type default_value = Type()) {
	Type result;
	try {
		if (index < container.size())
			result = container[index];
		else
			result = default_value;
	}
	catch (...) {
		result = default_value;
	}
	return result;
}

// Specialization for std::string input
char index(std::string container, size_t index, char default_value = '\0') {
	char result;
	try {
		if (index < container.size())
			result = container[index];
		else
			result = default_value;
	}
	catch (...) {
		result = default_value;
	}
	return result;
}

template<typename Key, typename Value>
Value index(std::map<Key, Value> container, Key index, Value default_value = Value()) {
	Value result;
	try {
		if (contains(container, index))
			result = container[index];
		else
			result = default_value;
	}
	catch (...) {
		result = default_value;
	}
	return result;
}

// Specialization for std::string where the index is provided as const char *
template<typename Value>
Value index(std::map<std::string, Value> container, const char * index, Value default_value = Value()) {
	Value result;
	try {
		if (contains(container, index))
			result = container[index];
		else
			result = default_value;
	}
	catch (...) {
		result = default_value;
	}
	return result;
}

#define append push_back

template <typename Type>
std::string make_string(Type argument) {
	std::stringstream result; 
	result << argument; 
	return result.str();
}

template<typename First, typename Second>
std::pair<First, Second> make_pair(First first, Second second) {
	return std::make_pair(first, second);
}

// Dictionary creator:
// Handle dict(1 argument)
template<typename Key, typename Value>
auto make_dictionary(std::pair<Key, Value>&& first) {
	std::map<decltype(first.first), decltype(first.second)> result({ first });
	return result;
}

// Handle dict(2 or more arguments)
template<typename Key, typename Value, typename ...Rest>
auto make_dictionary(std::pair<Key, Value>&& first, Rest...rest) {
	std::map<decltype(first.first), decltype(first.second)> result({ first, rest... });
	return result;
}

// More dict constructors
template<typename Key, typename Value>
auto make_dictionary(std::vector<std::tuple<Key, Value>> list_of_pairs) {
	std::map<Key, Value> result;
	for (var item : list_of_pairs)
		upsert(result, std::get<0>(item), std::get<1>(item));
	return result;
}

template<typename Key, typename Value>
auto make_dictionary(std::initializer_list<std::tuple<Key, Value>> list_of_pairs) {
	std::map<Key, Value> result;
	for (var item : list_of_pairs)
		upsert(result, std::get<0>(item), std::get<1>(item));
	return result;
}

#define key first
#define val second

// Upsert into a dictionary
template <typename Key, typename Value>
bool upsert(std::map<Key, Value>& dictionary, Key k, Value v) {
	try {
		if (contains(dictionary, k)) {
			// Key exists. Simply update
			dictionary[k] = v;
		}
		else {
			dictionary.insert(std::make_pair(k, v));
		}
		return true;
	}
	catch (std::exception& e) {
		print(e.what());
		return false;
	}
}

// Get a list of all the keys in a dictionary
template <typename Key, typename Value>
std::vector<Key> keys(std::map<Key, Value> dictionary) {
	std::vector<Key> result;
	for (auto kvpair in dictionary)
		result.push_back(kvpair.first);
	return result;
}

// Get a list of all the values in a dictionary
template <typename Key, typename Value>
std::vector<Value> values(std::map<Key, Value> dictionary) {
	std::vector<Value> result;
	for (auto kvpair in dictionary)
		result.push_back(kvpair.second);
	return result;
}

// Delete a key from a dictionary
template <typename Key, typename Value>
bool del(std::map<Key, Value>& dictionary, Key k) {
	try {
		dictionary.erase(k);
		return true;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return false;
	}
}

template <typename Value>
bool del(std::map<std::string, Value>& dictionary, const char * k) {
	try {
		dictionary.erase(k);
		return true;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return false;
	}
}

// Python-style map
// Map applies a function to all the items in an input list.
template <class Lambda, class Type>
std::vector<Type> map(Lambda map_function, const std::vector<Type> &container) {
	std::vector<Type> result;
	result.reserve(container.size());
	for (auto &v : container) {
		result.push_back(map_function(v));
	}
	return result;
}

// Run lambda function for each element in input list
template <class Lambda, class Type>
void foreach(Lambda foreach_function, const std::vector<Type> &container) {
	for (auto v in container)
		foreach_function(v);
}

// This is just abuse.. My attempt at Python-style list comprehension in C++
#define where if

// Pick that takes just two arguments
#pragma warning(disable: 4715)
#define pick2(transform, constraint) [=]() { \
	int count = 0; \
	constraint \
	{ \
		std::remove_const<std::remove_reference<decltype(transform)>::type>::type __temp__ = transform; \
		if (count == 0) \
		{ \
			count += 1; \
			std::vector<std::remove_const<std::remove_reference<decltype(transform)>::type>::type> result; \
			constraint \
				result.push_back(transform); \
			return result; \
		} \
		std::vector<std::remove_const<std::remove_reference<decltype(transform)>::type>::type> result; \
		return result; \
	} \
	throw std::runtime_error("pick constraint never satisfied"); \
}()

// Pick that takes 3 arguments
#define pick3(type, transform, constraint) [=]() { \
	int count = 0; \
	constraint \
	{ \
		type __temp__ = transform; \
		if (count == 0) \
		{ \
			count += 1; \
			std::vector<type> result; \
			constraint \
				result.push_back(transform); \
			return result; \
		} \
		std::vector<type> result; \
		return result; \
	} \
	return std::vector<type>();\
}()

#define GET_PICK_MACRO(_1,_2,_3,NAME,...) NAME
#define pick(...) EXPAND(GET_PICK_MACRO(__VA_ARGS__, pick3, pick2)(__VA_ARGS__))

// Introspection
template <class T>
std::string
__type__() {
	typedef typename std::remove_reference<T>::type TR;
	std::string r = typeid(TR).name();
	if (std::is_const<TR>::value)
		r += " const";
	if (std::is_volatile<TR>::value)
		r += " volatile";
	if (std::is_lvalue_reference<T>::value)
		r += "&";
	else if (std::is_rvalue_reference<T>::value)
		r += "&&";
	return r;
}
#define type(variable) __type__<decltype(variable)>()

// Check if a container is a subset of another container
template<typename Type>
bool is_subset(const std::set<Type>& lhs, const std::set<Type>& rhs) {
	return std::includes(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

// Check if a container is a superset of another container
template<typename Type>
bool is_superset(const std::set<Type>& lhs, const std::set<Type>& rhs) {
	return std::includes(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

// Get union of two std::set sets
template<typename Type>
std::set<Type> get_union(const std::set<Type>& lhs, const std::set<Type>& rhs) {
	std::set<Type> result = lhs;
	result.insert(rhs.begin(), rhs.end());
	return result;
}

// Get intersection of two std::set sets
template<typename Type>
std::set<Type> get_intersection(const std::set<Type>& lhs, const std::set<Type>& rhs) {
	std::set<Type> result;
	std::set_intersection(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
		std::inserter(result, result.begin()));
	return result;
}

// Get intersection of two std::set sets
template<typename Type>
std::set<Type> get_difference(const std::set<Type>& lhs, const std::set<Type>& rhs) {
	std::set<Type> result;
	std::set_difference(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
		std::inserter(result, result.begin()));
	return result;
}

// Get intersection of two std::set sets
template<typename Type>
std::set<Type> get_symmetric_difference(const std::set<Type>& lhs, const std::set<Type>& rhs) {
	std::set<Type> result;
	std::set_symmetric_difference(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
		std::inserter(result, result.begin()));
	return result;
}
// More List Operations

// Return true if all of the elements of input list are true
template<typename Type>
bool all(std::vector<Type> input_list) {
	if (input_list.size() == 0)
		return true;
	else
		return std::all_of(input_list.begin(), input_list.end(), [=](var i) { return static_cast<bool>(i) == true; });
}

// Return true if any of the elements in the input list is true
template<typename Type>
bool any(std::vector<Type> input_list) {
	if (input_list.size() == 0)
		return false;
	else
		return std::any_of(input_list.begin(), input_list.end(), [=](var i) { return static_cast<bool>(i) == true; });
}

// Get union of two std::vector lists
template<typename Type>
std::vector<Type> get_union(std::vector<Type> lhs, std::vector<Type> rhs) {
	std::vector<Type> result(lhs.size() + rhs.size());
	sort(lhs);
	sort(rhs);
	var it = std::set_union(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), result.begin());
	result.resize(it - result.begin());
	return result;
}

// Intersection function
// Get intersection of two std::vector lists
template<typename Type>
std::vector<Type> get_intersection(std::vector<Type> lhs, std::vector<Type> rhs) {
	std::vector<Type> result(lhs.size() + rhs.size());
	sort(lhs);
	sort(rhs);
	var it = std::set_intersection(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), result.begin());
	result.resize(it - result.begin());
	return result;
}

// Check if a std::vector is a subset of another std::vector
template<typename Type>
bool is_subset(std::vector<Type> lhs, std::vector<Type> rhs) {
	sort(lhs);
	sort(rhs);
	return std::includes(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

// Check if a std::vector is a superset of another std::vector
template<typename Type>
bool is_superset(std::vector<Type> lhs, std::vector<Type> rhs) {
	sort(lhs);
	sort(rhs);
	return std::includes(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

// Get intersection of two std::set sets
template<typename Type>
std::vector<Type> get_difference(std::vector<Type> lhs, std::vector<Type> rhs) {
	std::vector<Type> result(lhs.size() + rhs.size());
	sort(lhs);
	sort(rhs);
	var it = std::set_difference(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), result.begin());
	result.resize(it - result.begin());
	return result;
}

// Get intersection of two std::set sets
template<typename Type>
std::vector<Type> get_symmetric_difference(std::vector<Type> lhs, std::vector<Type> rhs) {
	std::vector<Type> result(lhs.size() + rhs.size());
	sort(lhs);
	sort(rhs);
	var it = std::set_symmetric_difference(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), result.begin());
	result.resize(it - result.begin());
	return result;
}

// Bit vector using std::bitset
#define bitvec(size) std::bitset<size>()

#define pair std::pair
#define linked_list std::list
#define list std::vector
#define string std::string
#define dictionary std::map
#define set std::set
#define tuple std::tuple
#define make_tuple(...) std::make_tuple(__VA_ARGS__)
#define pack make_tuple
#define unpack(packed_tuple, ...) \
	std::tie(__VA_ARGS__) = packed_tuple	

// Class related macros
#define class class
#define getter(member) public: decltype(member) get_##member() const { return member; } private:
#define setter(member) public: void set_##member(const decltype(member)& __value__) { member = __value__; } private:
#define gettersetter(member) getter(member) setter(member)
#define class_member4(datatype, name, getsetarg1, getsetarg2) private: datatype name; getsetarg1(name) getsetarg2(name)
#define class_member3(datatype, name, getsetarg) private: datatype name; getsetarg(name) 
#define class_member2(datatype, name) private: datatype name;
#define GET_CLASS_MEMBER_MACRO(_1, _2, _3, _4, NAME,...) NAME
#define member_variable(...) EXPAND(GET_CLASS_MEMBER_MACRO(__VA_ARGS__, \
	class_member4, class_member3, class_member2)(__VA_ARGS__))

#define default_constructor(classname) public: classname()
#define copy_constructor(classname) public: classname(const classname& other)
#define copy_assignment_operator(classname) public: classname& operator=(const classname& other)
#define move_constructor(classname) public: classname(classname&& other)
#define move_assignment_operator(classname) public: classname& operator=(classname&& other)
#define destructor(classname) public: ~classname()
#define default_member_functions(classname) \
	default_constructor(classname) = default;\
	copy_constructor(classname) = default;\
	copy_assignment_operator(classname) = default;\
	move_constructor(classname) = default;\
	move_assignment_operator(classname) = default;\
	destructor(classname) = default;
#define constructor(classname, ...) public: classname(__VA_ARGS__)

#define returns ,
#define member_function_implementation(function_signature, returntype) public: returntype function_signature
#define member_function(...) EXPAND(member_function_implementation(__VA_ARGS__))

#define inherits_from :
#define self *this
#define function [=]

// Statistics Functions
// Helper to determine whether there's a const_iterator for T.
template<typename T>
struct has_const_iterator {
private:
	template<typename C> static char test(typename C::const_iterator*);
	template<typename C> static int  test(...);
public:
	enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

template <typename Container>
typename std::enable_if<has_const_iterator<Container>::value, double>::type
mean(Container iterable) {
	double result = 0.0;
	if (len(iterable) > 0) { // prevent division by zero
		result = sum(iterable) / static_cast<double>(len(iterable));
	}
	return result;
}

template <typename Container>
typename std::enable_if<has_const_iterator<Container>::value, double>::type
harmonic_mean(Container iterable) {
	double reciprocal_sum = 0.0, result = 0.0;
	for (size_t i = 0; i < iterable.size(); i++) {
		reciprocal_sum += (1.0 / iterable[i]);
	}
	if (len(iterable) > 0 && reciprocal_sum > 0) { // prevent division by zero
		result = static_cast<double>(len(iterable)) / reciprocal_sum;
	}
	return result;
}

template <typename Container>
typename std::enable_if<has_const_iterator<Container>::value, double>::type
median(Container iterable) {
	sort(iterable);
	double result = 0.0;
	if (iterable.size() > 0) {
		if (iterable.size() % 2 == 0) {
			result = (iterable[(iterable.size() - 1) / 2] + iterable[iterable.size() / 2]) / 2.0;
		}
		else {
			result = iterable[(iterable.size() - 1) / 2];
		}
	}
	return result;
}

template <typename Container>
typename std::enable_if<has_const_iterator<Container>::value, double>::type
median_low(Container iterable) {
	sort(iterable);
	double result = 0.0;
	if (iterable.size() > 0) {
		if (iterable.size() % 2 == 0) {
			result = iterable[(iterable.size() - 1) / 2];
		}
		else {
			result = iterable[(iterable.size() - 1) / 2];
		}
	}
	return result;
}

template <typename Container>
typename std::enable_if<has_const_iterator<Container>::value, double>::type
median_high(Container iterable) {
	sort(iterable);
	double result = 0.0;
	if (iterable.size() > 0) {
		if (iterable.size() % 2 == 0) {
			result = iterable[iterable.size() / 2];
		}
		else {
			result = iterable[(iterable.size() - 1) / 2];
		}
	}
	return result;
}

// Mode only works on std::vector for now.
template <typename Type>
Type mode(std::vector<Type> iterable) {
	dictionary<Type, int> count_map;
	if (iterable.size() > 0) {
		for (size_t i = 0; i < iterable.size(); i++) {
			if (contains(count_map, iterable[i])) {
				upsert(count_map, iterable[i], count_map[iterable[i]] + 1);
			}
			else {
				upsert(count_map, iterable[i], 0);
			}
		}
	}

	Type result = Type();
	int max_count = 0;
	for (auto kvpair in count_map) {
		if (kvpair.second > max_count) {
			max_count = kvpair.second;
			result = kvpair.first;
		}
	}
	return result;
}

template <typename Container>
typename std::enable_if<has_const_iterator<Container>::value, double>::type
pvariance(Container iterable, double mu = -1) { // mean is an optional parameter
	double result = 0.0;
	if (iterable.size() > 0) {
		double iterable_mean = mu == -1 ? mean(iterable) : mu;
		double temp = 0.0;
		for (double iter : iterable) {
			temp += (iter - iterable_mean) * (iter - iterable_mean);
		}
		result = temp / iterable.size();
	}
	return result;
}

template <typename Container>
typename std::enable_if<has_const_iterator<Container>::value, double>::type
pstdev(Container iterable, double mu = -1) {
	return sqrt(pvariance(iterable, mu));
}

template <typename Container>
typename std::enable_if<has_const_iterator<Container>::value, double>::type
variance(Container iterable, double xbar = -1) {
	double result = 0.0;
	if (iterable.size() > 0) {
		double iterable_mean = xbar == -1 ? mean(iterable) : xbar;
		double temp = 0.0;
		for (double iter : iterable) {
			temp += (iter - iterable_mean) * (iter - iterable_mean);
		}
		result = temp / (iterable.size() - 1);
	}
	return result;
}

template <typename Container>
typename std::enable_if<has_const_iterator<Container>::value, double>::type
stdev(Container iterable, double xbar = -1) {
	return sqrt(variance(iterable, xbar));
}

// Random number generation helper functions
static std::random_device __random__device__; // obtain a random number from hardware
static std::mt19937_64 __mt19937_engine__(__random__device__()); // seed the generator

// short, int, long, long long, unsigned short, unsigned int, unsigned long, or unsigned long long
template <typename Type>
Type get_random_int(Type lower_bound, Type upper_bound) {
	std::uniform_int_distribution<> distribution(lower_bound, upper_bound); // define the range
	return distribution(__mt19937_engine__);
}

// float, double, or long double
template <typename Type>
Type get_random_real(Type lower_bound, Type upper_bound) {
	std::uniform_real_distribution<> distribution(lower_bound, upper_bound); // define the range
	return distribution(__mt19937_engine__);
}

template <typename Type>
Type choice(std::vector<Type> iterable) {
	return iterable[get_random_int(0, (int)iterable.size() - 1)];
}
// Specialization for std::string
string choice(string iterable) {
	return string(1, iterable[get_random_int(0, (int)iterable.size() - 1)]);
}

// Big integer type
class bigint {
private:
	// list of digits
	// I could try to be clever and use bit fields with c-style structs
	// but I'm lazy now and so this is how I'm going to represent the number
	list<int> value;

public:
	bigint() {}

	~bigint() {
		value.clear();
	}

	bigint(const bigint& other) {
		for (size_t i = 0; i < other.value.size(); i++) {
			value.push_back(other.value[i]);
		}
	}

	bigint& operator=(const bigint& other) {
		if (&other == this)
			return self;
		value = other.value;
		return self;
	}

	template <typename Type>
	bigint(Type input_value) {
		bool negative = false;
		if (input_value < 0) {
			negative = true;
			input_value *= -1;
		}
		int quotient = 0;
		int remainder = 0;
		while (input_value > 10) {
			remainder = static_cast<unsigned long long int>(input_value) % 10;
			value.push_back(remainder);
			input_value = input_value / 10;
		}
		if (input_value == 10) {
			value.push_back(0);
			value.push_back(1);
		}
		else {
			value.push_back(static_cast<int>(input_value));
		}
		
		// Handle negative numbers
		if (negative)
			value[value.size() - 1] *= -1;
	}

	bigint(const char * input_value) {
		string input_value_as_string(input_value);
		replace(input_value_as_string, ",", "");
		replace(input_value_as_string, ", ", "");
		replace(input_value_as_string, "\n", "");
		for (var c in reversed(input_value_as_string)) {
			if (c == '-') {
				value[value.size() - 1] *= -1;
			}
			else {
				std::stringstream digitstream;
				digitstream << c;
				int digit;
				digitstream >> digit;
				value.push_back(digit);
			}
		}
	}

	bigint(string input_value) {
		replace(input_value, ",", "");
		replace(input_value, ", ", "");
		replace(input_value, "\n", "");
		for (var c in reversed(input_value)) {
			if (c == '-') {
				value[value.size() - 1] *= -1;
			}
			else {
				std::stringstream digitstream;
				digitstream << c;
				int digit;
				digitstream >> digit;
				value.push_back(digit);
			}
		}
	}

	template <typename Type>
	bigint(list<Type> input_value) {
		value.clear();
		for (size_t i = 0; i < input_value.size(); i++) {
			value.push_back(static_cast<int>(input_value[i]));
		}
	}

	template <typename Type>
	bigint operator+(Type other) {
		bigint lhs(self);
		bigint rhs(other);
		bigint result;

		int carry = 0;
		list<int> lhs_value = value;
		list<int> rhs_value = bigint(rhs).value;

		if (lhs_value.size() == 0) {
			if (rhs.value.size() > 0)
				return rhs;
			else
				return bigint(0);
		}
		if (rhs_value.size() == 0) {
			if (value.size() > 0)
				return self;
			else
				return bigint(0);
		}

		typedef enum { POSITIVE, NEGATIVE } sign;
		sign result_sign = POSITIVE, lhs_sign = POSITIVE, rhs_sign = POSITIVE;

		if (lhs.value[lhs.value.size() - 1] < 0)
			lhs_sign = NEGATIVE;

		if (rhs.value[rhs.value.size() - 1] < 0)
			rhs_sign = NEGATIVE;

		if (lhs_sign == NEGATIVE and rhs_sign == POSITIVE) {
			bigint lhs_mod = lhs * -1;
			if (lhs_mod > rhs)
				result_sign = NEGATIVE;
		}

		if (lhs_sign == POSITIVE and rhs_sign == NEGATIVE) {
			bigint rhs_mod = rhs * -1;
			if (rhs_mod > lhs)
				result_sign = NEGATIVE;
		}

		if (lhs_sign == NEGATIVE and rhs_sign == NEGATIVE)
			result_sign = NEGATIVE;

		if (lhs_sign == POSITIVE and rhs_sign == POSITIVE) {
			var max_digits = lhs_value.size() > rhs_value.size() ? lhs_value.size() : rhs_value.size();
			for (size_t i = 0; i < max_digits; i++) {
				if (i < lhs_value.size() and i < rhs_value.size()) {
					// i is digit index in both lhs and rhs
					var lhs_digit = lhs_value[i];
					var rhs_digit = rhs_value[i];
					var sum = lhs_digit + rhs_digit + carry;
					if (sum < 10) {
						// sum is between [0, 9]
						result.value.push_back(sum);
						carry = 0;
					}
					else {
						// sum is greater than 9
						result.value.push_back(sum % 10); // save remainder
						carry = sum / 10; // carry over quotient
					}
				}
				else if (i < rhs_value.size() || i < lhs_value.size()) {
					var digit = (rhs_value.size() > i) ? rhs_value[i] : lhs_value[i];
					var sum = digit + carry;
					if (sum < 10) {
						// sum is between [0, 9]
						result.value.push_back(sum);
						carry = 0;
					}
					else {
						// sum is greater than 9
						result.value.push_back(sum % 10); // save remainder
						carry = sum / 10; // carry over quotient
					}
				}
			}
			if (carry > 0)
				result.value.push_back(carry);
		}
		else if (lhs_sign == POSITIVE and rhs_sign == NEGATIVE and result_sign == POSITIVE) {
			// lhs > rhs and rhs is negative
			// this implements subtraction
			var max_digits = lhs_value.size();
			for (size_t i = 0; i < max_digits; i++) {
				if (i < lhs_value.size() and i < rhs_value.size()) {
					// i is digit index in both lhs and rhs
					var lhs_digit = lhs_value[i];
					var rhs_digit = rhs_value[i];

					if (rhs_digit < 0)
						rhs_digit *= -1;

					if (lhs_digit >= rhs_digit) {
						if (rhs_digit < 0) 
							result.value.push_back(lhs_digit + rhs_digit);
						else
							result.value.push_back(lhs_digit - rhs_digit);
					}
					else {
						// implement carry over logic
						carry += 10;

						size_t j = i;
						while ((j + 1) < lhs_value.size()) {
							lhs_value[j + 1] -= 1;
							if (lhs_value[j + 1] == -1) {
								lhs_value[j + 1] = 9;
							}
							else
								break;
							j += 1;
						}

						rhs_digit *= -1; // Make rhs digit negative
						var sum = lhs_digit + carry + rhs_digit;
						if (sum < 10) {
							// sum is between [0, 9]
							result.value.push_back(sum);
							carry = 0;
						}
						else {
							// sum is greater than 9
							result.value.push_back(sum % 10); // save remainder
							carry = sum / 10; // carry over quotient
						}
					}
				}
				else if (i >= rhs_value.size()) {
					var digit = lhs_value[i];
					var sum = digit + carry;
					if (sum < 10) {
						// sum is between [0, 9]
						result.value.push_back(sum);
						carry = 0;
					}
					else {
						// sum is greater than 9
						result.value.push_back(sum % 10); // save remainder
						carry = sum / 10; // carry over quotient
					}
				}
			}

		}
		else if (lhs_sign == POSITIVE and rhs_sign == NEGATIVE and result_sign == NEGATIVE) {
			// return -(lhs + (-rhs)) where lhs and rhs are positive numbers
			var negation_lhs = self; negation_lhs.value[negation_lhs.value.size() - 1] *= -1;
			var negation_rhs = rhs; negation_rhs.value[negation_rhs.value.size() - 1] *= -1;
			result = negation_rhs + negation_lhs;
			result = result.remove_zero_padding();
			result.value[result.value.size() - 1] *= -1;
		}
		else if (lhs_sign == NEGATIVE and rhs_sign == POSITIVE and result_sign == NEGATIVE) {
			// return -(lhs + (-rhs)) where lhs and rhs are positive numbers
			var negation_lhs = self; negation_lhs.value[negation_lhs.value.size() - 1] *= -1;
			var negation_rhs = rhs; negation_rhs.value[negation_rhs.value.size() - 1] *= -1;
			result = negation_rhs + negation_lhs;
			result = result.remove_zero_padding();
			result.value[result.value.size() - 1] *= -1;
		}
		else if (lhs_sign == NEGATIVE and rhs_sign == POSITIVE and result_sign == POSITIVE) {
			// return (-lhs + rhs) = (rhs - lhs) where rhs > lhs and lhs and rhs are positive numbers
			result = rhs + self;
		}
		else if (lhs_sign == NEGATIVE and rhs_sign == NEGATIVE) {
			// return -(lhs + rhs) where lhs and rhs are positive numbers
			var negation_lhs = self; negation_lhs.value[negation_lhs.value.size() - 1] *= -1;
			var negation_rhs = rhs; negation_rhs.value[negation_rhs.value.size() - 1] *= -1;
			result = negation_rhs + negation_lhs;
			result = result.remove_zero_padding();
			result.value[result.value.size() - 1] *= -1;
		}
		if (result.value.size() == 0)
			result.value.push_back(0);
		return result;
	}

	template <typename Type>
	bigint& operator+=(Type rhs) {
		self = self + bigint(rhs);
		return self;
	}

	template <typename Type>
	bigint operator-(Type other) {
		bigint rhs(other);
		rhs.value[rhs.value.size() - 1] *= -1;
		return (self + rhs);
	}

	template <typename Type>
	bigint& operator-=(Type rhs) {
		self = self - bigint(rhs);
		return self;
	}

	template <typename Type>
	bigint operator*(Type rhs) {
		return self * bigint(rhs);
	}

	bigint operator*(bigint rhs) {
		bool lhs_is_negative = is_negative();
		bool rhs_is_negative = rhs.is_negative();
		bool result_is_negative = false;
		
		if (lhs_is_negative && rhs_is_negative)
			result_is_negative = false;
		else if (
			(lhs_is_negative && !rhs_is_negative) ||
			(!lhs_is_negative && rhs_is_negative))
			result_is_negative = true;
		else if (!lhs_is_negative && !rhs_is_negative)
			result_is_negative = false;

		list<int> lhs_value = value;
		list<int> rhs_value = rhs.value;

		if (lhs_value.size() > 0) {
			if (lhs_value[lhs_value.size() - 1] < 0)
				lhs_value[lhs_value.size() - 1] *= -1;
		}

		if (rhs_value.size() > 0) {
			if (rhs_value[rhs_value.size() - 1] < 0)
				rhs_value[rhs_value.size() - 1] *= -1;
		}

		// Reserve size = lhs.digits + rhs.digits
		list<int> result_value((lhs_value.size() + rhs_value.size()), 0);

		for (size_t j = 0; j < rhs_value.size(); j++) {
			int rhs_digit = rhs_value[j]; // get next digit in RHS value
			list<int> intermediate_value((lhs_value.size() + rhs_value.size()), 0);

			int carry = 0;
			for (size_t i = 0; i < value.size(); i++) {
				int digit_product = (rhs_digit * lhs_value[i]) + carry;
				carry = 0; // carry is now used up. zero before recalculation of carry
				if (digit_product >= 0 and digit_product < 10) {
					intermediate_value[i + j] = digit_product;
				}
				else if (digit_product >= 10) {
					int quotient = digit_product / 10;
					int remainder = digit_product % 10;
					intermediate_value[i + j] = remainder;
					carry += quotient;
				}
			}
			if (carry > 0) {
				intermediate_value[value.size() + j] += carry;
			}
			result_value = (bigint(result_value) + bigint(intermediate_value)).value;
		}
		bigint result(result_value);
		result = result.remove_zero_padding();
		if (result_is_negative && result.value.size() > 0) {
			result.value[result.value.size() - 1] *= -1;
		}
		return result;
	}

	template <typename Type>
	bigint& operator*=(Type rhs) {
		self = self * bigint(rhs);
		return self;
	}

	template <typename Type>
	bool operator==(const Type& other) const {
		return (remove_zero_padding().value == bigint(other).remove_zero_padding().value);
	}

	template <typename Type>
	bool operator!=(const Type& other) const {
		return !(self == bigint(other));
	}

	template <typename Type>
	bool operator>(const Type& other) const {
		bigint lhs = remove_zero_padding();
		bigint rhs(other);
		rhs = rhs.remove_zero_padding();

		if (lhs.is_positive() and rhs.is_negative())
			return true;
		else if (lhs.is_negative() and rhs.is_positive())
			return false;
		else if (lhs.is_negative() and rhs.is_negative() and lhs.value.size() < rhs.value.size())
			return true;
		else if (lhs.is_negative() and rhs.is_negative() and lhs.value.size() > rhs.value.size())
			return false;

		// LHS and RHS are positive and LHS has more digits, LHS is greater
		if (lhs.is_positive() and rhs.is_positive() and lhs.value.size() > rhs.value.size()) {
			return true;
		}
		else {
			// LHS and RHS are positive and LHS has fewer digits, LHS is not greater
			if (lhs.is_positive() and rhs.is_positive() and lhs.value.size() < rhs.value.size()) {
				return false;
			}
			// LHS and RHS have same number of digits
			else {
				// LHS and RHS are equal in value
				if (lhs.value == rhs.value) {
					return false;
				}
				else {
					// If both numbers are negative, 
					// if abs(lhs) > abs(rhs) return false
					if (lhs.is_negative() && rhs.is_negative()) {
						if (lhs.value[lhs.value.size() - 1] > rhs.value[rhs.value.size() - 1]) {
							return true;
						}
						else if (lhs.value[lhs.value.size() - 1] < rhs.value[rhs.value.size() - 1]) {
							return false;
						}
						else if (lhs.value[lhs.value.size() - 1] == rhs.value[rhs.value.size() - 1]) {
							// Start with most significant digit
							// Check every digit until you find the victor
							for (size_t i = lhs.value.size() - 1; i > 0; --i) {
								if (lhs.value[i - 1] < rhs.value[i - 1]) {
									return true;
								}
								else if (lhs.value[i - 1] > rhs.value[i - 1]) {
									return false;
								}
							}
							return false;
						}
					}
					else {
						// Start with most significant digit
						// Check every digit until you find the victor
						for (size_t i = lhs.value.size(); i > 0; --i) {
							if (lhs.value[i - 1] > rhs.value[i - 1]) {
								return true;
							}
							else if (lhs.value[i - 1] < rhs.value[i - 1]) {
								return false;
							}
						}
						return false;
					}
				}
			}
		}
	}

	template <typename Type>
	bool operator>=(const Type& other) const {
		bigint rhs(other);
		return (self == rhs || self > rhs);
	}

	template <typename Type>
	bool operator<(const Type& other) const {
		bigint rhs(other);
		return !(self >= rhs);
	}

	template <typename Type>
	bool operator<=(const Type& other) const {
		bigint rhs(other);
		return (self == rhs || self < rhs);
	}

	template <typename Type>
	bigint operator/(Type other) {
		bigint divisor(other);

		bool dividend_is_negative = is_negative();
		bool divisor_is_negative = divisor.is_negative();
		bool result_is_negative = false;

		if (dividend_is_negative && divisor_is_negative)
			result_is_negative = false;
		else if (
			(dividend_is_negative && !divisor_is_negative) ||
			(!dividend_is_negative && divisor_is_negative))
			result_is_negative = true;
		else if (!dividend_is_negative && !divisor_is_negative)
			result_is_negative = false;

		if (dividend_is_negative) {
			value[value.size() - 1] *= -1;
		}

		if (divisor_is_negative) {
			divisor.value[divisor.value.size() - 1] *= -1;
		}

		if (self < divisor) {
			if (dividend_is_negative) {
				value[value.size() - 1] *= -1;
			}
			return bigint(0);
		}
		else if (self == divisor) {
			if (dividend_is_negative) {
				value[value.size() - 1] *= -1;
			}
			if (result_is_negative)
				return bigint(-1);
			else
				return bigint(1);
		}
		else {
			// Division by subtraction
			// Yeah... I could do better than this
			bigint dividend = self;
			bigint quotient(0);
			while (dividend >= divisor and dividend > 0) {
				dividend -= divisor;
				quotient += 1;
				dividend = dividend.remove_zero_padding();
			}
			if (result_is_negative)
				quotient *= -1;

			if (dividend_is_negative) {
				value[value.size() - 1] *= -1;
			}
			return quotient;
		}
	}

	template <typename Type>
	bigint operator%(Type other) {

		/*
		The binary / operator yields the quotient, and the binary % operator yields 
		the remainder from the division of the first expression by the second. If 
		the second operand of / or % is zero the behavior is undefined. For integral 
		operands the / operator yields the algebraic quotient with any fractional part 
		discarded; if the quotient a/b is representable in the type of the result, 
		(a/b)*b + a%b is equal to a.
		*/
		bigint dividend(self);
		bigint divisor(other);
		return dividend - ((dividend / divisor) * divisor);
	}

	string to_string() const {
		string result = "";
		bool nonzero_started = false;

		list<int> this_value = value;
		if (is_negative())
			this_value[this_value.size() - 1] *= -1;

		for (var i = this_value.size(); i > 0; --i) {
			if (this_value[i - 1] > 0) {
				nonzero_started = true;
				result += make_string(this_value[i - 1]);
			}
			else if (nonzero_started || this_value[i - 1] < 0) {
				result += make_string(this_value[i - 1]);
			}
		}
		if (result == "")
			result = "0";
		else if (is_negative())
			result = "-" + result;
		return result;
	}
	 
	size_t digits() const {
		return remove_zero_padding().value.size();
	}

private:

	bool is_negative() const {
		if (value.size() > 0) {
			if (value[value.size() - 1] < 0)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	bool is_positive() const {
		return !is_negative();
	}

	list<int> get_value() const {
		return reversed(value);
	}

	bigint remove_zero_padding() const {
		bigint result;
		bool nonzero_started = false;
		for (var i = value.size(); i > 0; --i) {
			if (value[i - 1] > 0) {
				nonzero_started = true;
				result.value.push_back(value[i - 1]);
			}
			else if (nonzero_started || value[i - 1] < 0) {
				result.value.push_back(value[i - 1]);
				if (!nonzero_started)
					nonzero_started = true;
			}
		}
		result.value = reversed(result.value);
		if (result.value.size() == 0)
			result.value = { 0 };
		return result;
	}

	friend std::ostream& operator<<(std::ostream& out, const bigint &big_integer);

};

std::ostream& operator<<(std::ostream& out, const bigint &big_integer) {
	out << big_integer.to_string();
	return out;
}

#define measure_execution_time(unit, ...) \
	[=]() {\
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();\
		__VA_ARGS__;\
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();\
		return std::chrono::duration_cast<std::chrono::unit>(end - start).count();\
	}()


// numpy-style ndarray class
template <typename Type>
class ndarray {
public:
	ndarray() : size_(0), data_(nullptr), shape_({}) {}

	ndarray(std::initializer_list<size_t> shape) {
		shape_ = shape;
		size_ = 1;
		for (size_t i = 0; i < shape_.size(); i++)
			size_ *= shape_[i];
		data_ = (Type *)malloc(size_ * sizeof(Type));
	}

	~ndarray() {
		free(data_);
		data_ = nullptr;
		shape_.clear();
	}

private:
	Type * data_;
	size_t size_;
	list<size_t> shape_;
};

#define shape(first, ...) std::initializer_list<size_t>{first, __VA_ARGS__}
