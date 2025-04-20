#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <queue>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include "TV_List.h"
#include "RecList.h"
using namespace std;


//comment hell main
int main() {

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
    //recList.makeMatrix(shows);
    cout << "read!" << endl;
    string startShow = "Friends";
    string endShow = "The Mandalorian";
    cout << "Finding path from '" << startShow << "' to '" << endShow << "':" << endl;
    recList.Dijkstra(startShow, endShow);


/*
    set<string> desired = {"Sci-Fi"};
    cout << "read!" << endl;
    recList.returnBestMatch(desired, shows);
    cout << "read!" << endl;
    auto recommendations = RecList::getBestMatches(desired, shows);
    cout << "read!" << endl;
    for (int i = 0; i < min(3, (int)recommendations.size()); i++) {
        cout << i+1 << ". " << recommendations[i].name
     << " (Score: " << fixed << setprecision(2) << (recommendations[i].score * 100.0) << "%)\n";
    }
    cout << "read!" << endl;


    /*
    cout << "\n=== Finding paths between shows using Dijkstra's algorithm ===\n" << endl;

    string startShow = "Breaking Bad";
    string endShow = "Rick and Morty";
    cout << "Finding path from '" << startShow << "' to '" << endShow << "':" << endl;
    vector<string> path = recList.Dijkstra(startShow, endShow);
    recList.printPath(path);
    cout << "read!" << endl;

    cout << "\n----------------------------------------\n" << endl;

    startShow = "Friends";
    endShow = "The Mandalorian";
    cout << "Finding path from '" << startShow << "' to '" << endShow << "':" << endl;
    path = recList.Dijkstra(startShow, endShow);
    recList.printPath(path);
    cout << "read!" << endl;
    */

    return 0;
}

/*
void makeMatrix(map<string, set<string>> featureList) {
    for (auto& [name1, feature1] : featureList) {
        for (auto& [name2, feature2] : featureList) {
            if (name1 < name2) {
                unordered_set<string> shared = commonFeatures(feature1, feature2);
                if (!shared.empty()) {
                    adjMatrix[name1][name2] = shared;
                    adjMatrix[name2][name1] = shared;
                }
            }
        }
    }
}
*/
