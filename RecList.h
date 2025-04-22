#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <queue>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include "TV_List.h"
#pragma once

using namespace std;

struct Info {
    //key: show, value: set of common features
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
    //big shoutout to emma coronado who helped solve our list i love her
    void createList(unordered_map<string, set<string>> featureList) {
        for (auto& [name1, feature1] : featureList) {
            for (auto& [name2, feature2] : featureList) {
                if (name1 < name2 && adjList[name1].size() < 500 && adjList[name2].size() < 500) {
                    unordered_set<string> shared = commonFeatures(feature1, feature2);
                    if (!shared.empty()) {
                        adjList[name1].insert(name2, shared);
                        adjList[name2].insert(name1, shared);
                    }
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
            cout << ((adjList.find(start) == adjList.end()) ?
                        "Start show not found: " + start :
                        "End show not found: " + end) << endl;
            return {};
        }
        if (start == end) return {start};
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
                double weight = (features.empty() ? numeric_limits<double>::max() : 1.0 / features.size());
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
        for (string current = end; current != ""; current = previous[current]) {
            path.push_back(current);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    double heuristic(const string& current, string& goal) {
        if (adjList.find(current) == adjList.end() || adjList.find(goal) == adjList.end())
            return 1.0; // Default: low similarity = high cost

        unordered_map<string, unordered_set<string>> currentMap = adjList.at(current).getMap();

        auto it = currentMap.find(goal);
        if (it == currentMap.end())
            return 1.0; // No direct connection to goal

        size_t sharedFeatures = it->second.size();
        return 1.0 / (1 + sharedFeatures); // More features = lower heuristic cost
    }

    vector<string> aStarSearch(string& start, string& goal) {
        unordered_map<string, double> gScore;
        unordered_map<string, double> fScore;
        unordered_map<string, string> cameFrom;

        auto cmp = [&](const string& a, const string& b) {
            return fScore[a] > fScore[b];
        };
        priority_queue<string, vector<string>, decltype(cmp)> openSet(cmp);

        for (auto& [node, _] : adjList) {
            gScore[node] = numeric_limits<double>::infinity();
            fScore[node] = numeric_limits<double>::infinity();
        }

        gScore[start] = 0;
        fScore[start] = heuristic(start, goal);

        openSet.push(start);

        while (!openSet.empty()) {
            string current = openSet.top();
            openSet.pop();

            if (current == goal) {
                vector<string> path;
                while (cameFrom.count(current)) {
                    path.push_back(current);
                    current = cameFrom[current];
                }
                path.push_back(start);
                reverse(path.begin(), path.end());
                return path;
            }

            unordered_map<string, unordered_set<string>> neighbors = adjList.at(current).getMap();
            for (const auto& [neighbor, features] : neighbors) {
                double tentativeG = gScore[current] + 1.0 / (1 + features.size());

                if (tentativeG < gScore[neighbor]) {
                    cameFrom[neighbor] = current;
                    gScore[neighbor] = tentativeG;
                    fScore[neighbor] = tentativeG + heuristic(neighbor, goal);
                    openSet.push(neighbor);
                }
            }
        }

        return {};
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
        Recommendation(string n, float s, vector<string> m)
    : name(n), score(s), matchingFeatures(std::move(m)) {}

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
            float score = desired.empty() ? 0.0f : static_cast<float>(matches.size()) / desired.size();

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
            float score = desired.empty() ? 0.0f : static_cast<float>(matches.size()) / desired.size();

            maxHeap.push(Recommendation(name, score, matches));
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
