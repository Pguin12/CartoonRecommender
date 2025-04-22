#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
using namespace std;

#ifndef TV_LIST_H
#define TV_LIST_H

/*

class TV_List {
    unordered_map<int, string> show_id;
    unordered_map<int, string> type_id;
    unordered_map<int, string> genre_id;
    unordered_map<string, int> genretoid;
    unordered_map<int, string> status_id;
    unordered_map<string, unordered_set<int>> show_genre_list;

    void set_show_genre_list();
    void get_id(string my_file, unordered_map<int, string> &my_map);
public:
    TV_List();
    unordered_map<string, unordered_set<int>>& get_show_genre_list()
    {
        return show_genre_list;
    };
    string get_genre(int my_id) {
        return genre_id[my_id];
    }
    int get_genre_id(string name) {
        return genretoid[name];
    }
};

*/

class TV_List {
    unordered_map<int, string> show_id;
    unordered_map<int, string> type_id;
    unordered_map<int, string> genre_id;
    unordered_map<int, string> status_id;

    unordered_map<string, set<string>> show_genre_list;

    void set_show_genre_list();
    void get_id(string my_file, unordered_map<int, string> &my_map);
public:
    TV_List();
    unordered_map<string, set<string>>& get_show_genre_list()
    {
        // for (auto item : show_genre_list) {
        //     cout << item.first << endl;
        //     for (auto genres : item.second) {
        //         cout << " " << genres;
        //     }
        //     cout << endl;
        // }
        return show_genre_list;
    };
};

#endif //TV_LIST_H
