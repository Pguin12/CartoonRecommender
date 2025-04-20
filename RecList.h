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
#ifndef RECLIST_H
#define RECLIST_H



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
    bool contains(string key) {
        return data.find(key) != data.end();
    }
};

class RecList {
    //map<string, map<string, unordered_set<string>>> adjMatrix;
    unordered_map<string, Info> adjList;
    set<string> listOfShows;
public:
    RecList() = default;

    void createList(unordered_map<string, set<string>>& featureList) {
        for (const auto& [item, features] : featureList) {
            vector<string> featureVec(features.begin(), features.end());
            for (const auto& one : featureVec) {
                for (const auto& two : featureVec) {
                    if (one == two) continue;
                    adjList[one].data[two].insert(item);
                    listOfShows.insert(item);
                }
            }
        }
    }

    void print() {
        std::set<std::string> printedPairs;

        for (auto& [show1, row] : adjList) {
            for (const auto& [show2, features] : row.getMap()) {
                // Ensure only one direction gets printed
                std::string key = (show1 < show2) ? show1 + "|" + show2 : show2 + "|" + show1;
                if (printedPairs.count(key)) continue;
                printedPairs.insert(key);

                std::cout << "Show " << show1 << " <-> Show " << show2 << ": (";

                bool first = true;
                for (const auto& feature : features) {
                    if (!first) std::cout << ", ";
                    std::cout << feature;
                    first = false;
                }

                std::cout << ")\n";
            }
        }
    }
    void printPath(vector<string>& path, unordered_map<string, unordered_map<string, unordered_set<string>>>& showGraph) {
        if (path.empty()) {
            cout << "No path exists." << endl;
            return;
        }

        cout << "Shortest path found:" << endl;
        for (size_t i = 0; i < path.size() - 1; i++) {
            string current = path[i];
            string next = path[i + 1];

            cout << current << " -> " << next << ": ";

            // Get the common features between the two shows (from the showGraph)
            const auto& commonFeatures = showGraph.at(current).at(next);

            if (!commonFeatures.empty()) {
                cout << "Common features: ";
                bool first = true;
                for (const auto& feature : commonFeatures) {
                    if (!first) cout << ", ";
                    cout << feature;
                    first = false;
                }
            } else {
                cout << "None";
            }
            cout << endl;
        }
    }


    void Dijkstra(string start, string end) {
        if ((!listOfShows.count(start)) && (!listOfShows.count(end))) {
            cout << "Show not found";
            return;
        }
        unordered_map<string, unordered_map<string, unordered_set<string>>> showGraph;

        for (auto& [feature, info] : adjList) {
            for (const auto& [otherFeature, shows] : info.getMap()) {
                for (const auto& show1 : shows) {
                    for (const auto& show2 : shows) {
                        if (show1 == show2) continue;
                        showGraph[show1][show2].insert(feature);
                    }
                }
            }
        }
        using P = pair<double, string>;
        priority_queue<P, vector<P>, greater<P>> pq;
        unordered_map<string,double> distance;
        unordered_map<string,string> previous;
        // initalize distance
        for (auto& node : listOfShows) { //uses name NOT map
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
            for (auto& [neighbor, features] : showGraph[current]) {
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
            return;
        }
        vector<string> path;
        for(string current = previous[end]; current != ""; current = previous[current]) {
            path.push_back(current);
        }
        path.push_back(end);
        reverse(path.begin(), path.end());
        printPath(path, showGraph);
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


#endif //RECLIST_H
