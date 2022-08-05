// Read files and prints top k word by frequency

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>
#include <thread>
#include <boost/algorithm/string.hpp>
#include <atomic>
#include <array>

#include "IOFile.h"
#include "util.h"


const size_t TOPK = 15;

using Counter = std::map<std::string, std::size_t>;
using Vector_string = std::vector<std::string>;


void calculations(int argc, char *argv[]){
    const int count_threads = std::thread::hardware_concurrency();                      // количество потоков у системы

    std::string text_file;
    for (int i = 1; i < argc; ++i) {
        std::ifstream input{argv[i]};
        if (!input.is_open()) {
            std::cerr << "Failed to open file " << argv[i] << '\n';
            exit(EXIT_FAILURE);
        };
        reading_file(input, text_file);
    }

    int text_len = text_file.length();

    int count_symbols_thread = text_len / count_threads;

    std::vector<std::stringstream *> vec_strstream(count_threads);

    for (int i = 0; i < count_threads; ++i) // ОШИБКА делит слова на части, нам нужны целые слова.
    {
        vec_strstream[i] = new std::stringstream();
        (*vec_strstream[i]) << text_file.substr(count_symbols_thread * i, count_symbols_thread);
    }


    std::vector<std::thread *> vector_threads(count_threads);
    std::vector<std::shared_ptr<Counter>> vector_Counter(count_threads);
    for (int i = 0; i < count_threads; ++i) {
        std::shared_ptr<Vector_string> shared_list_str;
        if (i == count_threads - 1) {
            std::string text_for_thread = text_file.substr(count_symbols_thread * i, text_len);
//            boost::split(shared_list_str, text_for_thread, boost::is_space());
            std::shared_ptr<Counter> freq_dict_local;
            vector_Counter.push_back(freq_dict_local);
            std::thread thread_i(count_words, shared_list_str, std::ref(freq_dict_local));
            thread_i.detach();
            vector_threads.push_back(&thread_i);
            break;
        }

        std::string text_for_thread = text_file.substr(count_symbols_thread * i, count_symbols_thread * (i + 1));
//        boost::split(shared_list_str, text_for_thread, boost::is_space());
        std::shared_ptr<Counter> freq_dict_local;
        vector_Counter.push_back(freq_dict_local);
        std::thread thread_i(count_words, shared_list_str, std::ref(freq_dict_local));
        thread_i.detach();
        vector_threads.push_back(&thread_i);
    }
    Counter freq_dict;

    for (auto thread: vector_threads)thread->join();
    for (auto v_map: vector_Counter) {
        std::merge(freq_dict.begin(), freq_dict.end(), v_map->begin(), v_map->end(),
                   std::inserter(freq_dict, freq_dict.begin()));
    }
    print_topk(std::cout, freq_dict, TOPK);
}

int main(int argc, char *argv[]) {
    check_arg(argc);
    auto elapsed_ms = time_measurement(calculations, argc, argv);
    auto time_count = static_cast<double>(elapsed_ms.count());
    time_count /= 1000;
    std::cout << "Elapsed time is " << time_count << " sec\n";
}





