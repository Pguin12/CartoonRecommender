//
// Created by tiffh on 4/18/2025.
//
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <set>
#include <sstream>
using namespace std;

#ifndef TV_LIST_H
#define TV_LIST_H

class TV_List {
    map<int, string> show_id;
    map<int, string> type_id;
    map<int, string> genre_id;
    map<int, string> status_id;

    map<string, set<string>> show_genre_list;

    void set_show_genre_list();
    void get_id(string my_file, map<int, string> &my_map);
public:
    TV_List();
    map<string, set<string>>& get_show_genre_list()
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
