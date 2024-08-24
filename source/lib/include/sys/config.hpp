#ifndef __SYS_CONFIG_HPP__
#define __SYS_CONFIG_HPP__

#include <string>
#include <vector>

namespace config {

bool fail (void);

void init (std::string path);

template <typename type>
type get (std::string name);

template <>
bool get<bool> (std::string name);

template <>
std::string get<std::string> (std::string name);

template <>
int get<int> (std::string name);

template <>
double get<double> (std::string name);

template <>
std::vector<int>
get<std::vector<int>> (std::string name);

template <>
std::vector<double>
get<std::vector<double>> (std::string name);

template <>
std::vector<std::vector<int>>
get<std::vector<std::vector<int>>> (std::string name);

template <>
std::vector<std::vector<double>>
get<std::vector<std::vector<double>>> (std::string name);

}

#endif
