#include "TV_List.h"
/*
void TV_List::set_show_genre_list()
{
    ifstream genres("TV-shows-data/genres.csv");
    string line;
    getline(genres, line); // skip headers
    int s = 0;
    while (getline(genres, line) && s < 5)
    {
        pair<string, string> row;
        stringstream ss(line);
        string cell;
        string cell2;

        getline(ss, cell, ',');
        getline(ss, cell2);

        row = make_pair(cell, cell2);

        int showid = stoi(row.first);
        int genreid = stoi(row.second);

        // insert genres for each id
        string show_name = show_id[showid];
        genretoid[genre_id[genreid]] = genreid;

        show_genre_list[show_name].insert(genreid);
        s++;
    }

}
void TV_List::get_id(string my_file, unordered_map<int,string>& my_map)
{
    ifstream file(my_file);
    string line;

    getline(file, line);    // skip headers
    int s = 0;
    while (getline(file, line) && s < 5)
    {
        pair<string, string> row;
        stringstream ss(line);
        string cell;
        string cell2;

        getline(ss, cell, ',');
        getline(ss, cell2);
        row = make_pair(cell, cell2);

        int id = stoi(row.first);
        my_map[id] = row.second;
        s++;
    }
}

TV_List::TV_List()
{
    ifstream shows("TV-shows-data/shows.csv");
    string line;
    cout << "hi!" << endl;
    getline(shows, line);   // skip headers
    int s = 0;
    while (getline(shows, line) && s < 5)
    {
        vector<string> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ','))
        {
            row.push_back(cell);
        }

        /* 13 items in each row
         * row = {
         *
         *  show id, name, number of seasons, show summary,
         *  adult (t/f), in production (t/f), original name, popularity,
         *  tagline, episode run time, type id, status id
         *
         *  }

        int id = stoi(row[0]);  // convert id to int
        show_id[id] = row[1];   // organize show by rows
        s++;
    }

    // get show type, genre, and show status ids to sort later
    get_id("TV-shows-data/types.csv", type_id);
    get_id("TV-shows-data/genre_types.csv", genre_id);
    get_id("TV-shows-data/status/csv", status_id);
    set_show_genre_list();
    cout << "done!" << endl;

}

*/

void TV_List::set_show_genre_list()
{
    ifstream genres("TV-shows-data/genres.csv");
    string line;

    getline(genres, line); // skip headers
    int s = 0;
    while (getline(genres, line) && s < 45000)
    {
        pair<string, string> row;
        stringstream ss(line);
        string cell;
        string cell2;

        getline(ss, cell, ',');
        getline(ss, cell2);

        row = make_pair(cell, cell2);

        int showid = stoi(row.first);
        int genreid = stoi(row.second);

        // insert genres for each id
        string show_name = show_id[showid];
        string show_genre = genre_id[genreid];

        show_genre_list[show_name].insert(show_genre);
        s++;
    }

}
void TV_List::get_id(string my_file, unordered_map<int,string>& my_map)
{
    ifstream file(my_file);
    string line;

    getline(file, line);    // skip headers
    int s = 0;
    while (getline(file, line) && s < 45000)
    {
        pair<string, string> row;
        stringstream ss(line);
        string cell;
        string cell2;

        getline(ss, cell, ',');
        getline(ss, cell2);
        row = make_pair(cell, cell2);

        int id = stoi(row.first);
        my_map[id] = row.second;
        s++;
    }
}

TV_List::TV_List()
{
    ifstream shows("TV-shows-data/shows.csv");
    string line;
    cout << "hi!" << endl;
    getline(shows, line);   // skip headers
    while (getline(shows, line))
    {
        vector<string> row;
        stringstream ss(line);
        string cell;
        int s = 0;
        while (getline(ss, cell, ',') && s < 45000)
        {
            row.push_back(cell);
        }

        /* 13 items in each row
         * row = {
         *
         *  show id, name, number of seasons, show summary,
         *  adult (t/f), in production (t/f), original name, popularity,
         *  tagline, episode run time, type id, status id
         *
         *  }
         */
        int id = stoi(row[0]);  // convert id to int
        show_id[id] = row[1];   // organize show by rows
        s++;
    }

    // get show type, genre, and show status ids to sort later
    get_id("TV-shows-data/types.csv", type_id);
    get_id("TV-shows-data/genre_types.csv", genre_id);
    get_id("TV-shows-data/status/csv", status_id);
    set_show_genre_list();
    cout << "done!" << endl;

}
