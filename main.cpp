#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <iomanip>
#include <unordered_map>
#include "TV_List.h"
#include "RecList.h"
#include "RecList2.h"
#include <chrono>
using namespace std;


//comment hell main
int main() {
    auto start = chrono::high_resolution_clock::now();
    TV_List ShowList;
    unordered_map<string, set<string>> shows = ShowList.get_show_genre_list();

/*
    map<string, set<string>> shows = {{"Breaking Bad", {"Drama", "2008", "Ended", "Crime", "Critically Acclaimed"}},
        {"The Office", {"Comedy", "2005", "Ended", "Mockumentary", "Workplace"}},
        {"Stranger Things", {"Sci-Fi", "2016", "Continuing", "Thriller", "80s Nostalgia"}},
        {"Avatar: The Last Airbender", {"Animated", "2005", "Ended", "Adventure", "Fantasy"}},
        {"Friends", {"Comedy", "1994", "Ended", "Sitcom", "New York"}},
        {"The Mandalorian", {"Sci-Fi", "2019", "Continuing", "Star Wars", "Action"}},
        {"The Crown", {"Drama", "2016", "Continuing", "Historical", "British Royalty"}},
        {"Rick and Morty", {"Animated", "2013", "Continuing", "Sci-Fi", "Dark Humor"}},
        {"Game of Thrones", {"Fantasy", "2011", "Ended", "Drama", "Epic"}},
        {"Wednesday", {"Mystery", "2022", "Continuing", "Supernatural", "Teen"}}
    };
*/
    RecList recList;
    recList.createList(shows);
    /*
    //recList.makeMatrix(shows);
    cout << "read!" << endl;
    string startShow = "Chernobyl";
    string endShow = "Avatar: The Last Airbender";
    cout << "Finding path from '" << startShow << "' to '" << endShow << "':" << endl;
    vector<string> ah = recList.Dijkstra(startShow, endShow);
    recList.printPath(ah);
    */



    set<string> desired = {"Sci-Fi"};
    cout << "read!" << endl;
    auto recommendations = RecList::getBestMatches(desired, shows);
    cout << "read!" << endl;
    for (int i = 0; i < min(3, (int)recommendations.size()); i++) {
        cout << i+1 << ". " << recommendations[i].name
     << " (Score: " << fixed << setprecision(2) << (recommendations[i].score * 100.0) << "%)\n";
    }
    cout << "read!" << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    cout << duration.count() << " seconds" << endl;

    return 0;
}
