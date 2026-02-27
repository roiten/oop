#pragma once

#include <set>
#include <string>
#include <unordered_map>

using ListWords = std::set<std::string>;
using Dictionary = std::unordered_map<std::string, ListWords>;

std::string GetFileName(int argc, char* argv[]);

Dictionary LoadDictionary(const std::string& fileName);

void RunDictionary(Dictionary& dictionary, const std::string& fileName);