#include "TV_List.h"
void TV_List::set_show_genre_list()
{
    ifstream genres("TV-shows-data/genres.csv");
    string line;

    getline(genres, line); // skip headers
    while (getline(genres, line))
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
        string name = show_id[showid];
        string genre = genre_id[genreid];

        gnr_to_show.insert({genre, name});
        show_to_gnr.insert({name, genre});
    }
    for(pair<string, string> item : show_to_gnr) {
        cout << item.first << " " << item.second << endl;
    }

}
void TV_List::get_id(string my_file, unordered_map<int,string>& my_map)
{
    ifstream file(my_file);
    string line;

    getline(file, line);    // skip headers
    while(getline(file, line))
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

    }
}
TV_List::TV_List()
{
    ifstream shows("TV-shows-data/shows.csv");
    string line;

    getline(shows, line);   // skip headers
    while (getline(shows, line))
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
         */

        int id = stoi(row[0]);  // convert id to int
        show_id[id] = row[1];   // organize show by rows
    }

    // get show type, genre, and show status ids to sort later
    get_id("TV-shows-data/types.csv", type_id);
    get_id("TV-shows-data/genre_types.csv", genre_id);
    get_id("TV-shows-data/status.csv", status_id);
    set_show_genre_list();

}