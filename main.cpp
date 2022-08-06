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
//#include <boost/algorithm/string.hpp>
#include <atomic>
#include <array>
#include <iomanip>
#include "IOFile.h"
#include "util.h"


const size_t TOPK = 10;

using Counter = std::map<std::string, std::size_t>;


void calculations(int argc, char *argv[]){
    const unsigned int count_threads = 4;// std::thread::hardware_concurrency();

    std::string text_file;
    for (int i = 1; i < argc; ++i) {
        std::ifstream input{argv[i]};
        if (!input.is_open()) {
            std::cerr << "Failed to open file " << argv[i] << '\n';
            exit(EXIT_FAILURE);
        };
        reading_file(input, text_file);
    }

    std::vector<Counter> counters(count_threads);
    std::vector<std::stringstream> vec_strstream(count_threads);
    std::vector<std::thread*> vec_thread(count_threads);
    size_t text_len = text_file.length();
    size_t count_symbols_thread = text_len / count_threads;
    size_t counter_text = 0;
    size_t old_counter = 0;
    bool spliting = false;

    for(int thr = 0; thr < count_threads; ++thr) {
        for (size_t i = std::min(counter_text + count_symbols_thread - 1,text_len - 1); i < text_len; ++i) {
            if (!spliting){
                i = std::min(counter_text + count_symbols_thread - 1,text_len - 1);
            }
            if ((i - old_counter) >= count_symbols_thread || i == text_len - 1){
                spliting = true;
            }
            char val = text_file[i];
            counter_text++;
            if (spliting && (text_len - 1  == i || val == ' ' || val == '\t' || val == '\n') ) {
                vec_strstream[thr] << text_file.substr(old_counter, i - old_counter);
                old_counter = i;
                spliting = false;
                break;
            }
        }
        auto thread_i = new std::thread(count_words, std::ref(vec_strstream[thr]), std::ref(counters[thr]));
        vec_thread[thr] = thread_i;
    }

    Counter v_map;
    for(int thr = 0; thr < count_threads; ++thr) {
        vec_thread[thr]->join();
        auto thr_map = counters[thr];
        for (auto it = thr_map.begin(); it != thr_map.end(); ++it){
            v_map[it->first]+=it->second;
        }
    }


    for(int thr = 0; thr < count_threads; ++thr) {
        delete vec_thread[thr];
    }
    print_topk(std::cout, v_map, TOPK);
}

int main(int argc, char *argv[]) {
    check_arg(argc);
    auto elapsed_ms = time_measurement(calculations, argc, argv);
    auto time_count = static_cast<double>(elapsed_ms.count());
    time_count /= 1000;
    std::cout << "Elapsed time is " << time_count << " sec\n";
}





