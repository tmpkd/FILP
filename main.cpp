#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int main()
{
    char bookName[] = "book.txt";
    countWords(bookName);
}

void countWords(char* bookName) {
    int size = getFileSize(bookName);
    char* text = extractTextFromFile(bookName, size);
    map <char*, int, StringComparer> counter;
    int wordLength = 0;
    int i;
    for (i = 0; i < size && text[i] > 0; i++) {

        if (isalpha(text[i])) {
            wordLength++;
        }
        else {
            if (wordLength != 0) {
                counter[getSubStr(text, i - wordLength, wordLength)] += 1;
                wordLength = 0;
            }
        }
    }
    if (wordLength != 0)
        counter[getSubStr(text, i - wordLength, wordLength)] += 1;


    for (map<char*, int, StringComparer>::iterator iter = counter.begin(); iter != counter.end(); iter++) {
        cout << iter->first << " : " << iter->second << '\n';
    }
}

struct StringComparer
{
    bool operator()(char const* a, char const* b) const
    {
        return strcmp(a, b) < 0;
    }
}

int getFileSize(char* fileName) {
    ifstream file(fileName, ios::binary | ios::ate);
    int size = file.tellg();
    file.close();
    return size;
}

char* extractTextFromFile(char* fileName, int size) {
    ifstream file;
    file.open(fileName);
    char* textBuffer = new char[size + 1];
    file.read(textBuffer, size);
    textBuffer[size] = '\0';
    file.close();
    return textBuffer;
}

char* getSubStr(char* text, int start, int length) {
    char* word = new char[length + 1];
    memcpy(word, text + start, length);
    word[length] = '\0';
    return word;
}
