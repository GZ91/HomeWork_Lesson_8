#include "util.h"

std::string tolower(const std::string &str) {
    std::string lower_str;
    std::transform(std::cbegin(str), std::cend(str), std::back_inserter(lower_str),
                   [](unsigned char ch) { return std::tolower(ch); });
    return lower_str;
};

//void count_words(std::shared_ptr<std::vector<std::string>> vec, std::shared_ptr<std::map<std::string, std::size_t>> &counter) {
//    std::for_each(vec->cbegin(), vec->cend(), [counter](const std::string &s) { ++(*counter)[tolower(s)]; });
//}
void count_words(std::stringstream& stream, std::map<std::string, std::size_t>& counter) {
    std::for_each(std::istream_iterator<std::string>(stream),
                  std::istream_iterator<std::string>(),
                  [&counter](const std::string &s) { ++counter[tolower(s)]; });
}

void print_topk(std::ostream& stream, const std::map<std::string, std::size_t>& counter, const size_t k) {
    std::vector<std::map<std::string, std::size_t>::const_iterator> words;
    words.reserve(counter.size());
    for (auto it = std::cbegin(counter); it != std::cend(counter); ++it) {
        words.push_back(it);
    }

    std::partial_sort(
            std::begin(words), std::begin(words) + k, std::end(words),
            [](auto lhs, auto &rhs) { return lhs->second > rhs->second; });

    std::for_each(
            std::begin(words), std::begin(words) + k,
            [&stream](const std::map<std::string, std::size_t>::const_iterator &pair) {
                stream << std::setw(4) << pair->second << " " << pair->first
                       << '\n';
            });
}

void check_arg(int argc){
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        exit(EXIT_FAILURE);
    }
}

std::chrono::milliseconds time_measurement(void (*method)(int argc, char *argv[]),int argc, char *argv[]){
    auto start = std::chrono::high_resolution_clock::now();
    method(argc, argv);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}