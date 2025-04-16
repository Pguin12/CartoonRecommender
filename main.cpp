#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <queue>
#include <iomanip>

using namespace std;

class RecList {
    map<string, map<string, unordered_set<string>>> adjMatrix;

public:
    RecList() = default;

    static unordered_set<string> commonFeatures(set<string>& feature1, set<string>& feature2) {
        set<string> A(feature1.begin(), feature1.end());
        unordered_set<string> common;
        for (auto& feature : feature2) {
            if (A.count(feature) > 0) {
                common.insert(feature);
            }
        }
        return common;
    }

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

    void print() {
        for (auto& [from, row] : adjMatrix) {
            for (auto& [to, features] : row) {
                std::cout << from << " <-> " << to << ": ";
                for (auto& feature : features) {
                    std::cout << feature << ", ";
                }
                std::cout << "\n";
            }
        }
    }

    vector<string> Dijkstra(string start, string end) {
        // Implementation not provided in original code
        return {};
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

    static void returnBestMatch(set<string>& desired, const map<string, set<string>>& featureList) {
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
    static vector<Recommendation> getBestMatches(set<string>& desired, const map<string, set<string>>& featureList) {
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

    RecList recList;
    recList.makeMatrix(shows);

    set<string> desired = {"Sci-Fi", "Continuing", "Drama"};
    recList.returnBestMatch(desired, shows);

    // Example of using the new method
    auto recommendations = RecList::getBestMatches(desired, shows);
    for (int i = 0; i < min(3, (int)recommendations.size()); i++) {
        cout << i+1 << ". " << recommendations[i].name
     << " (Score: " << fixed << setprecision(2) << (recommendations[i].score * 100.0) << "%)\n";
    }

    return 0;
}