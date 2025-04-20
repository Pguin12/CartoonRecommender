#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <map>
#include <set>
#include <sstream>
using namespace std;

#ifndef TV_LIST_H
#define TV_LIST_H

class TV_List {
    unordered_map<int, string> show_id;
    unordered_map<int, string> type_id;
    unordered_map<int, string> genre_id;
    unordered_map<int, string> status_id;

    unordered_multimap<string, string> gnr_to_show;
    unordered_multimap<string, string> show_to_gnr;

    void set_show_genre_list();
    void get_id(string my_file, unordered_map<int, string> &my_map);
public:
    TV_List();
    unordered_map<string, set<string>>& get_show_genre_list();
};



#endif //TV_LIST_H
