#include"lab4.h"

#include<sstream>
#include<iostream>
#include<vector>
#include <algorithm>
#include <deque>

using namespace std;

struct Page {
    int number = -1;

    string GetNumber() const {
        if (number < 0) {
            return "#";
        }
        return std::to_string(number);
    }
};

static const size_t PAGE_COUNT = 6;

class PageTableRandom : public vector<Page> {
public:
    PageTableRandom() {
        resize(PAGE_COUNT);
    }

    void Print() const {
        bool first = true;
        for (const Page &page: *this) {
            if (first) {
                first = false;
            } else {
                cout << " ";
            }
            cout << page.GetNumber();
        }
        cout << endl;
    }

    Page *GetFirstEmpty() {
        return GetPage(-1);
    }

    Page *GetPage(int number) {
        auto it = std::find_if(begin(), end(), [number](const Page &p) {
            return p.number == number;
        });

        if (it == end()) {
            return nullptr;
        }

        return &(*it);
    }

    Page &GetReplacingPage() {
        int num = uniform_rnd(0, PAGE_COUNT - 1);
        return operator[](num);
    }
};

void LRUAlgorithm();

int main(int argc, char *argv[]) {
    if (argc < 1) {
        return -1;
    }

    int algoNumber = stoi(argv[1]);
    if (algoNumber == 1) {
        cerr << "Not implemented yet" << endl;
    } else if (algoNumber == 2) {
        LRUAlgorithm();
    } else {
        return -2;
    }

    return 0;
}

void LRUAlgorithm() {
    PageTableRandom table;
    deque <size_t> list;

    string str;
    while (getline(cin, str)) {
        stringstream ss(str);

        bool read = true;
        if (!(ss >> read)) {
            cerr << "Error read r/m column. String:" << str << endl;
            return;
        }

        int pageNum = -1;
        if (!(ss >> pageNum) || pageNum < 0) {
            cerr << "Error read page column. String:" << str << endl;
            return;
        }

        if (auto empty = table.GetFirstEmpty()) {
            //Если есть пустое место
            empty->number = pageNum;
        } else {
            if (!table.GetPage(pageNum)) {
                //Вытесняем самый старый
                size_t lastPageNum = list.back();
                list.pop_back();

                //Вместо него - новый
                table.GetPage(lastPageNum)->number = pageNum;
            }
        }

        //Удаляем из очереди, если такой уже есть
        auto it = std::find(list.begin(), list.end(), pageNum);
        if (it != list.end()) {
            list.erase(it);
        }
        //Добавляем в голову последний
        list.push_front(pageNum);
        table.Print();
    }
}
