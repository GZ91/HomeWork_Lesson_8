#pragma once
#include <thread>
#include <map>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <string>

std::string tolower(const std::string &str);

void count_words(std::stringstream& stream, std::map<std::string, std::size_t> &counter);
void print_topk(std::ostream& stream, const std::map<std::string, std::size_t>& counter, const size_t k);
void check_arg(int);
std::chrono::milliseconds time_measurement(void (*)(int argc, char *argv[]),int argc, char *argv[]);
void reading_file(std::istream &, std::string &);