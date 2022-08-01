// Read files and prints top k word by frequency

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>
#include <chrono>
#include <thread>
#include <boost/algorithm/string.hpp>
#include <atomic>
#include <array>



const size_t TOPK = 15;

using Counter = std::map<std::string, std::size_t>;
using Vector_string = std::vector<std::string>;

std::string tolower(const std::string &str);

void count_words(std::shared_ptr<Vector_string>, std::shared_ptr<Counter>&);

void print_topk(std::ostream& stream, const Counter&, const size_t k);

void reading_file(std::istream&, std::string&);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }
    long long count_all_words = 0;                                              // количество всех слов
    auto start = std::chrono::high_resolution_clock::now(); // для замера времени производительности

    const int count_threads = std::thread::hardware_concurrency();                    // количество потоков у системы





    std::string text_file;                                                      // текст файла
    for (int i = 1; i < argc; ++i) {
        std::ifstream input{argv[i]};
        if (!input.is_open()) {
            std::cerr << "Failed to open file " << argv[i] << '\n';
            return EXIT_FAILURE;
        }
       ;
        reading_file(input, text_file);
    }


    int text_len = text_file.length();

    int count_symbols_thread = text_len/count_threads;
    std::vector<std::stringstream*> vec_strstream(count_threads);
    for (int i = 0; i<count_threads; ++i) // ОШИБКА делит слова на части, нам нужны целые слова.
    {
        vec_strstream[i] = new std::stringstream();
        (*vec_strstream[i]) << text_file.substr(count_symbols_thread * i, count_symbols_thread);
    }


    std::vector<std::thread*> vector_threads(count_threads);
    std::vector<std::shared_ptr<Counter>> vector_Counter(count_threads);
    for (int i = 0; i<count_threads; ++i)
    {
        std::shared_ptr<Vector_string> shared_list_str;
        if (i == count_threads - 1)
        {
            std::string text_for_thread = text_file.substr(count_symbols_thread * i, text_len);
//            boost::split(shared_list_str, text_for_thread, boost::is_space());
            std::shared_ptr<Counter>  freq_dict_local;
            vector_Counter.push_back(freq_dict_local);
            std::thread thread_i(count_words,shared_list_str, std::ref(freq_dict_local));
            thread_i.detach();
            vector_threads.push_back(&thread_i);
            break;
        }

        std::string text_for_thread = text_file.substr(count_symbols_thread * i, count_symbols_thread * (i+1));
//        boost::split(shared_list_str, text_for_thread, boost::is_space());
        std::shared_ptr<Counter>  freq_dict_local;
        vector_Counter.push_back(freq_dict_local);
        std::thread thread_i(count_words, shared_list_str, std::ref(freq_dict_local));
        thread_i.detach();
        vector_threads.push_back(&thread_i);
    }
    Counter freq_dict;

    for(auto thread : vector_threads)thread->join();
    for (auto v_map : vector_Counter)
    {
        std::merge(freq_dict.begin(), freq_dict.end(),v_map->begin(), v_map->end(), std::inserter(freq_dict, freq_dict.begin()));
    }





    print_topk(std::cout, freq_dict, TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    auto time_count = static_cast<double>(elapsed_ms.count());
    time_count /= 1000;

    std::cout << "Elapsed time is " << time_count << " sec\n";
   std::cout << "Count all words " << count_all_words << std::endl;
}

void reading_file(std::istream& stream, std::string& text) {
    text = std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
}


std::string tolower(const std::string &str) {
    std::string lower_str;
    std::transform(std::cbegin(str), std::cend(str),
                   std::back_inserter(lower_str),
                   [](unsigned char ch) { return std::tolower(ch); });
    return lower_str;
};

void count_words(std::shared_ptr<Vector_string> vec, std::shared_ptr<Counter>& counter) {
    std::for_each(vec->cbegin(),
            vec->cend(),
            [counter](const std::string &s) { ++(*counter)[tolower(s)]; });
}

void print_topk(std::ostream& stream, const Counter& counter, const size_t k) {
    std::vector<Counter::const_iterator> words;
    words.reserve(counter.size());
    for (auto it = std::cbegin(counter); it != std::cend(counter); ++it) {
        words.push_back(it);
    }

    std::partial_sort(
            std::begin(words), std::begin(words) + k, std::end(words),
            [](auto lhs, auto &rhs) { return lhs->second > rhs->second; });

    std::for_each(
            std::begin(words), std::begin(words) + k,
            [&stream](const Counter::const_iterator &pair) {
                stream << std::setw(4) << pair->second << " " << pair->first
                       << '\n';
            });
}