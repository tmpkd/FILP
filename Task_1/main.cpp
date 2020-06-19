#include <iostream>
#include <fstream>
#include <set>
#include <chrono>

using namespace std;


class Pair {
public:
    Pair(const char *word, int count) : word(word), count(count) {}
    const char *word;
    mutable int count;
};

struct WordsComparator {
    typedef std::true_type is_transparent;

    bool operator()(const Pair& l, const char *r) const {
        return strcmp(l.word, r) < 0;
    }

    bool operator()(const char *l, const Pair& r) const {
        return strcmp(l, r.word) < 0;
    }

    bool operator()(const Pair& l, const Pair& r) const {
        return strcmp(l.word, r.word) < 0;
    }
};

struct CountComparator {
    typedef std::true_type is_transparent;

    bool operator()(const Pair& l, const int r) const {
        return l.count < r;
    }

    bool operator()(const int l, const Pair& r) const {
        return l < r.count;
    }

    bool operator()(const Pair& l, const Pair& r) const {
        return l.count < r.count;
    }
};

void check_file(ifstream &file) {
    if (!file) {
        cout << "Cannot open input file" << endl;
        exit(1);
    }
}

int get_file_size(ifstream &file) {
    file.seekg(0, ios::end);
    const int size = file.tellg();
    file.seekg(0, ios::beg);
    return size;
}

char* read_file_to_buffer(ifstream &file, int file_size) {
    char *Text = new char[file_size + 1];
    file.read(Text, file_size);
    file.close();
    Text[file_size] = '\0';
    return Text;
}

multiset<Pair, CountComparator> count_words_in(char *Text, int size) {
    set<Pair, WordsComparator> comp_by_letters;

    size_t word_length = 0;
    char *word = (char*)malloc(word_length);
    for (int i = 0; i <= size; i++) {
        if (isalpha(Text[i])) {
            word_length++;
            word = (char*)realloc(word, word_length);
            word[word_length - 1] = (char)tolower((int)Text[i]);
        } else {
            if (word_length == 0)
                continue;
            auto cur_word = comp_by_letters.find(word);
            if (cur_word == comp_by_letters.end()) {
                comp_by_letters.insert(Pair(word, 1));
            } else {
                cur_word->count++;
            }
            word_length = 0;
            word = (char*)malloc(word_length);
        }
    }

    multiset<Pair, CountComparator> comp_by_count;
    for(auto w : comp_by_letters)
        comp_by_count.insert(w);

    return comp_by_count;
}

void show_res(multiset<Pair, CountComparator>& words, int file_size, int elapsed_time) {
    for (auto element : words)
        cout << element.word << " - " << element.count << endl;
    cout << "File size " << file_size << " B" << endl;
    cout << "Elapsed " << elapsed_time << " ms" << endl;
}

int main(int argc, char **argv) {
    ifstream input_file(argv[1]);
    check_file(input_file);

    const int file_size = get_file_size(input_file);
    char *Text = read_file_to_buffer(input_file, file_size);

    auto start = chrono::steady_clock::now();
    auto words_counts = count_words_in(Text, file_size);
    auto end = chrono::steady_clock::now();

    show_res(words_counts, file_size, chrono::duration_cast<chrono::milliseconds>(end - start).count());
    return 0;
}


