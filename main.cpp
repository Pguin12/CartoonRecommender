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

using namespace std;

struct Info {
    unordered_map<string, unordered_set<string>> data;
    void insert(string key, unordered_set<string> val) {
        data.insert(make_pair(key, val));
    }
    unordered_map<string, unordered_set<string>> getMap() {
        return data;
    }
    int size() {
        return data.size();
    }
};

class RecList {
    //map<string, map<string, unordered_set<string>>> adjMatrix;
    unordered_map<string, Info> adjList;
public:
    RecList() = default;

    unordered_set<string> commonFeatures(set<string>& feature1, set<string>& feature2) {
        unordered_set<string> common;
        auto it1 = feature1.begin(), it2 = feature2.begin();
        while (it1 != feature1.end() && it2 != feature2.end()) {
            if (*it1 == *it2) {
                common.insert(*it1);
                ++it1; ++it2;
            } else if (*it1 < *it2) {
                ++it1;
            } else {
                ++it2;
            }
        }
        return common;
    }

    void createList(unordered_map<string, set<string>> featureList) {
        unordered_map<string, vector<string>> featureToItems;

        // Step 1: Build the inverted index: feature -> list of items
        for (const auto& [item, features] : featureList) {
            for (const string& feature : features) {
                featureToItems[feature].push_back(item);
            }
        }

        unordered_set<string> comparedPairs;

        // Step 2: For each item, find other items that share at least one feature
        for (const auto& [name1, features1] : featureList) {
            unordered_set<string> relatedItems;

            // Gather all items that share any feature with name1
            for (const string& feature : features1) {
                for (const string& related : featureToItems[feature]) {
                    if (related != name1) {
                        relatedItems.insert(related);
                    }
                }
            }

            // Step 3: Compare with those related items only
            for (const string& name2 : relatedItems) {
                // Avoid duplicate comparisons (e.g., both A-B and B-A)
                string key = name1 < name2 ? name1 + "|" + name2 : name2 + "|" + name1;
                if (comparedPairs.count(key)) continue;
                comparedPairs.insert(key);

                const auto& features2 = featureList[name2];
                unordered_set<string> shared = commonFeatures(
                    const_cast<set<string>&>(features1),
                    const_cast<set<string>&>(features2)
                );

                if (!shared.empty()) {
                    adjList[name1].insert(name2, shared);
                    adjList[name2].insert(name1, shared);
                }
            }
        }
    }

    void print() {
        for (auto& [from, row] : adjList) {
            for (auto& [to, features] : row.getMap()) {
                std::cout << from << " <-> " << to << ": ";
                for (const auto& feature : features) {
                    std::cout << feature << ", ";
                }
                std::cout << "\n";
            }
        }
    }

    vector<string> Dijkstra(string start, string end) {
        if (adjList.find(start) == adjList.end() || adjList.find(end) == adjList.end()) {
            cout << "Show not found";
            return {};
        }
        // pair {distance,node}
        priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> pq;
        // show -> distance
        unordered_map<string,double> distance;
        //prev node
        unordered_map<string,string> previous;
        // initalize distance
        for (auto& [node, _] : adjList) { //uses name NOT map
            distance[node] = numeric_limits<double>::infinity();
            previous[node] = "";
        }
        distance[start] = 0.0;
        pq.push({0.0, start});
        while (!pq.empty()) {
            auto [dist, current] = pq.top();
            pq.pop();
            if(current == end) {
                break;
            }
            if(dist > distance[current]) {
                continue;
            }
            for (auto& [neighbor, features] : adjList[current].getMap()) {
                double weight = 1.0/(features.size());
                double newDistance = distance[current] + weight;
                if (newDistance < distance[neighbor]) {
                    distance[neighbor] = newDistance;
                    previous[neighbor] = current;
                    pq.push({newDistance, neighbor});
                }
            }
        }
        if (distance[end]==numeric_limits<double>::infinity()) {
            cout<<"No path exists between " << start << " and " << end<<endl;
            return {};
        }
        vector<string> path;
        for(string current = previous[end]; current != ""; current = previous[current]) {
            path.push_back(current);
        }
        path.push_back(end);
        reverse(path.begin(), path.end());
        return path;
    }

    void printPath(const vector<string>& path) {
        if (path.empty()) {
            return;
        }
        cout << "Shortest path found:" << endl;
        for (size_t i = 0; i < path.size() -1 ; i++) {
            string current = path[i];
            string next = path[i+1];
            cout << current << " -> " << next << ": ";
            cout << "Common features: ";
            const Info& currentInfo = adjList[current];
            const Info& nextInfo = adjList[next];

            const auto& currentMap = currentInfo.data;
            const auto& nextMap = nextInfo.data;

            unordered_set<string> common;

            for (const auto& [key1, set1] : currentMap) {
                auto it = nextMap.find(key1);
                if (it != nextMap.end()) {
                    const unordered_set<string>& set2 = it->second;
                    for (const auto& item : set1) {
                        if (set2.count(item)) {
                            common.insert(item);
                        }
                    }
                }
            }

            if (!common.empty()) {
                for (const auto& feature : common) {
                    cout << feature << ", ";
                }
            } else {
                cout << "None";
            }
            cout << endl;
        }
    }
    static auto highestScore(set<string>& desired, set<string> featureList) {
        vector<string> result;
        std::set_intersection(
            desired.begin(), desired.end(),
            featureList.begin(), featureList.end(),
            std::inserter(result, result.begin())
        );
        return result;
    }

    // Structure to represent a recommendation with its score and matching features
    struct Recommendation {
        string name;
        float score;
        vector<string> matchingFeatures;

        // Define comparison for max heap (greatest score first)
        bool operator<(const Recommendation& other) const {
            return score < other.score; // For max heap (priority queue uses less comparison)
        }
    };

    static void returnBestMatch(set<string>& desired, const unordered_map<string, set<string>>& featureList) {
        // Create a priority queue (max heap) for recommendations
        priority_queue<Recommendation> maxHeap;

        cout << "You are looking for: " << endl;
        for (auto& feature : desired) {
            cout << feature << "\n";
        }
        cout << endl;

        // Calculate scores and build the max heap
        for (auto& [name, features] : featureList) {
            vector<string> matches = highestScore(desired, features);
            float score = static_cast<float>(matches.size()) / static_cast<float>(desired.size());

            // Add to max heap
            maxHeap.push({name, score, matches});
        }

        // Print results in order of highest score first (extracted from max heap)
        cout << "Results in order of relevance:" << endl;
        while (!maxHeap.empty()) {
            auto recommendation = maxHeap.top();
            maxHeap.pop();

            cout << recommendation.name << ": "
                 << fixed << setprecision(2) << (recommendation.score * 100.0) << "%" << endl;
            for (auto& feature : recommendation.matchingFeatures) {
                cout << "- " << feature << "\n";
            }
            cout << endl;
        }
    }

    // New method to get recommendations using max heap without printing
    static vector<Recommendation> getBestMatches(set<string>& desired, const unordered_map<string, set<string>>& featureList) {
        // Create a priority queue (max heap) for recommendations
        priority_queue<Recommendation> maxHeap;

        // Calculate scores and build the max heap
        for (auto& [name, features] : featureList) {
            vector<string> matches = highestScore(desired, features);
            float score = static_cast<float>(matches.size()) / static_cast<float>(desired.size());

            // Add to max heap
            maxHeap.push({name, score, matches});
        }

        // Extract results in order
        vector<Recommendation> results;
        while (!maxHeap.empty()) {
            results.push_back(maxHeap.top());
            maxHeap.pop();
        }

        return results;
    }
};

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
    set<string> desired = {"Documentary"};
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
