#include "RecList.h"

unordered_set<string> RecList::commonFeatures(set<string> &feature1, set<string> &feature2) {
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

void RecList::createList(unordered_map<string, set<string>> featureList) {
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

vector<string> RecList::Dijkstra(string start, string end) {
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

double RecList::heuristic(const string &current, string &goal) {
    if (adjList.find(current) == adjList.end() || adjList.find(goal) == adjList.end())
        return 1.0; // Default: low similarity = high cost

    unordered_map<string, unordered_set<string>> currentMap = adjList.at(current).getMap();

    auto it = currentMap.find(goal);
    if (it == currentMap.end())
        return 1.0; // Not Connected to goal

    size_t sharedFeatures = it->second.size();
    return 1.0 / (1 + sharedFeatures); // Has more features
}

vector<string> RecList::aStarSearch(string &start, string &goal) {
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
            double tempG = gScore[current] + 1.0 / (1 + features.size());

            if (tempG < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tempG;
                fScore[neighbor] = tempG + heuristic(neighbor, goal);
                openSet.push(neighbor);
            }
        }
    }

    return {};
}

void RecList::printPath(const vector<string> &path) {
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

vector<RecList::Recommendation> RecList::getBestMatches(set<string> &desired,
    const unordered_map<string, set<string>> &featureList) {
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
