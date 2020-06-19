#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <chrono>

using namespace std;

class Pair {
public:
    Pair(const char *key, int count) : key(key), count(count) {}
    const char *key;
    mutable int count;
};

struct CountComparator {
    typedef std::true_type is_transparent;

    bool operator()(const pair<string, int>& lhs, const int rhs) const {
        return lhs.second < rhs;
    }

    bool operator()(const int lhs, const pair<string, int>& rhs) const {
        return lhs < rhs.second;
    }

    bool operator()(const pair<string, int>& lhs, const pair<string, int>& rhs) const {
        return lhs.second < rhs.second;
    }
};

void check_file(ifstream &file) {
    if (!file) {
        cout << "Cannot open input file" << endl;
        exit(1);
    }
}

int get_file_size(ifstream &file) {
    file.seekg (0, ios::end);
    const int size = file.tellg();
    file.seekg (0, ios::beg);
    return size;
}

char* read_file_to_buffer(ifstream &file, int file_size) {
    char *Text = new char[file_size + 1];
    file.read(Text, file_size);
    file.close();
    Text[file_size] = '\0';
    return Text;
}

set<pair<string, int>, CountComparator> count_words_in(char *Text, int size) {
    map<string, int> word_counter;

    size_t word_length = 0;
    string word;
    for (int i = 0; i <= size; ++i) {
        if (isalpha(Text[i])) {
            word_length++;
            word += (char)tolower((int)Text[i]);
        } else {
            if (word_length == 0)
                continue;
            auto cur_word = word_counter.find(word);
            if (cur_word == word_counter.end()) {
                word_counter[word] = 1;
            } else {
                cur_word->second++;
            }
            word_length = 0;
            word = "";
        }
    }

    set<pair<string, int>, CountComparator> count_sorted;
    for(auto w : word_counter) {
        count_sorted.insert(w);
    }
    return count_sorted;
}

void show_res(set<pair<string, int>, CountComparator>& words, int file_size, int elapsed_time) {
    for (const auto& element : words)
        cout << element.first << " - " << element.second << endl;
    cout << "File size " << file_size << " B" << endl;
    cout << "Elapsed " << elapsed_time << " ms" << endl;
}

int main(int argc, char **argv) {
    ifstream input_file(argv[1]);
    check_file(input_file);

    const int file_size = get_file_size(input_file);
    char* Text = read_file_to_buffer(input_file, file_size);

    auto start = chrono::steady_clock::now();
    auto words_counts = count_words_in(Text, file_size);
    auto end = chrono::steady_clock::now();

    show_res(words_counts, file_size, chrono::duration_cast<chrono::milliseconds>(end - start).count());
    return 0;
}


