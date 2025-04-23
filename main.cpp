#include <algorithm>
#include <iostream>
#include <vector>
#include <winsock2.h>
#include <string>
#include <set>
#include <iomanip>
#include <unordered_map>
#include "TV_List.h"
#include "RecList.h"
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <unistd.h>
using namespace std;

void menu () {
    TV_List ShowList;
    unordered_map<string, set<string>> shows = ShowList.get_show_genre_list();
    RecList recList;
    recList.createList(shows);
    while (true) {
        int choice = 0;
        cout << "Type 1 for two show searchup, or 2 for filtering, 3 to exit" << endl;;
        cin >> choice;
        if (choice == 1) {
            string show1;
            string show2;
            int search;
            cout << "Select your two shows! (Enter 1 show)" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, show1);
            cout << "Show two!" << endl;
            getline(cin, show2);
            cout << "Type 1 to use Dijskstra or 2 for A star!" << endl;
            cin >> search;
            if (search == 1) {
                vector<string> p = recList.Dijkstra(show1, show2);
                recList.printPath(p);
            }
            else if (search == 2) {
                vector<string> p = recList.aStarSearch(show1, show2);
                recList.printPath(p);
            }
        }
        if (choice == 2) {
            set<string> desired;

            cout << "Choose your genres (press any non-number to finish)\n";
            vector<string> genres = {
                "Romance", "Kids", "Comedy", "Crime", "Musical", "War & Politics",
                "Sci-Fi & Fantasy", "Soap", "Western", "History", "Family", "Drama",
                "News", "Music", "Animation", "Action & Adventure", "Documentary",
                "Reality", "Mystery", "Talk"
            };

            for (size_t i = 0; i < genres.size(); ++i) {
                cout << (i + 1) << ". " << genres[i] << endl;
            }

            while (true) {
                if (!desired.empty()) {
                    cout << "You have chosen: ";
                    for (const auto& g : desired) {
                        cout << g << ", ";
                    }
                    cout << endl;
                }

                cout << "Enter a number to add a genre, or any letter to get recommendations: ";
                string x;
                cin >> x;

                if (all_of(x.begin(), x.end(), ::isdigit)) {
                    int choice = stoi(x);
                    if (choice >= 1 && choice <= 20) {
                        desired.insert(genres[choice - 1]); // fix index
                    } else {
                        cout << "Please choose a number between 1 and 20!" << endl;
                    }
                } else {
                    break;
                }
            }
            auto recommendations = RecList::getBestMatches(desired, shows);
            cout << "\nTop recommendations:\n";
            for (int i = 0; i < min(10, (int)recommendations.size()); ++i) {
                cout << i + 1 << ". " << recommendations[i].name
                     << " (Score: " << fixed << setprecision(2)
                     << (recommendations[i].score * 100.0) << "%)\n";
            }
        }
        else if (choice == 3) {
            break;
        }
    }

}

int main() {
    menu();
    return 0;
}
