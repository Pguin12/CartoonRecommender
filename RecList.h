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

    unordered_set<string> commonFeatures(set<string>& feature1, set<string>& feature2);
    //big shoutout to emma coronado who helped solve our list i love her
    void createList(unordered_map<string, set<string>> featureList);
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

    vector<string> Dijkstra(string start, string end);

    //meant for A*
    double heuristic(const string& current, string& goal);

    vector<string> aStarSearch(string& start, string& goal);

    void printPath(const vector<string>& path);

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

    static vector<Recommendation> getBestMatches(set<string>& desired, const unordered_map<string, set<string>>& featureList);
    
};
