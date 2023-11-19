
#include <graphviz/gvc.h>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>
#include "graph_operations.h"
#include <queue>
#include <unordered_map>
#include <stack> 

// Define a structure to store location information
struct LocationInfo {
    std::string location;
    double distance;
    double cost;

    LocationInfo(const std::string& loc, double dist = 500.0, double c = 0.0)
        : location(loc), distance(dist), cost(c) {}
};

// Define a graph as an adjacency list

class VillageMap {
public:
    VillageMap();
    void displayEconomicalPath(const std::vector<std::string>& economicalPath);
    void addLocation(const std::string& location);
    void addPath(const std::string& location1, const std::string& location2);
    void displayMap();
    std::vector<std::string> findShortestPath(const std::string& source, const std::string& destination);
    void displayPath(const std::vector<std::string>& shortestPath);
std::vector<std::string> findMostEconomicalPath(const std::string& source, const std::string& destination);
    void displayMapWithCosts();
    std::map<std::string, std::vector<LocationInfo>> mapGraph;
};

// Constructor: Initialize the village map
VillageMap::VillageMap() {
    mapGraph.clear();
}

// Add a location to the map
void VillageMap::addLocation(const std::string& location) {
    if (mapGraph.find(location) == mapGraph.end()) {
        mapGraph[location] = std::vector<LocationInfo>();
    }
}
void VillageMap::addPath(const std::string& location1, const std::string& location2) {
    double cost = 20.0 + (rand() / (RAND_MAX / (50.0 - 20.0))); // Generate a random cost between 20 and 50

    mapGraph[location1].push_back(LocationInfo(location2, 500.0, cost));
    mapGraph[location2].push_back(LocationInfo(location1, 500.0, cost));
}

// Display the village map graphically using Graphviz
void VillageMap::displayMap() {
    // Create a new graph context
Agraph_t* G = agopen(const_cast<char*>("G"), Agstrictdirected, nullptr);

    

    // Add nodes to the graph
    std::map<std::string, Agnode_t*> nodeMap;
    for (const auto& entry : mapGraph) {
        const std::string& location = entry.first;
        Agnode_t* node = agnode(G, (char*)location.c_str(), 1);
        nodeMap[location] = node;
    }

    // Add edges to the graph
    for (const auto& entry : mapGraph) {
        const std::string& location1 = entry.first;
        Agnode_t* node1 = nodeMap[location1];

        for (const LocationInfo& info : entry.second) {
            const std::string& location2 = info.location;
            Agedge_t* edge = agedge(G, node1, nodeMap[location2], nullptr, 1);
        }
    }

    // Initialize the Graphviz context
    GVC_t* gvc = gvContext();

    // Layout the graph
    gvLayout(gvc, G, "dot");

    // Render the graph to a file (e.g., PNG)
    gvRenderFilename(gvc, G, "png", "village_map.png");

    // Free resources
    gvFreeLayout(gvc, G);
    agclose(G);
    gvFreeContext(gvc);

    std::cout << "Graph visualization created successfully." << std::endl;
}

// Add a new method in the VillageMap class to find the shortest path
std::vector<std::string> VillageMap::findShortestPath(const std::string& source, const std::string& destination) {
    std::unordered_map<std::string, double> distance;
    std::unordered_map<std::string, std::string> previous;
    std::priority_queue<std::pair<double, std::string>, std::vector<std::pair<double, std::string>>, std::greater<std::pair<double, std::string>>> pq;

    // Initialize distances to all nodes as infinity
    for (const auto& entry : mapGraph) {
        distance[entry.first] = std::numeric_limits<double>::max();
    }

    // The distance from the source to itself is 0
    distance[source] = 0.0;
    pq.push({0.0, source});

    while (!pq.empty()) {
        double dist = pq.top().first;
        std::string current = pq.top().second;
        pq.pop();

        if (dist > distance[current]) {
            continue;
        }

        for (const LocationInfo& neighbor : mapGraph[current]) {
            double newDist = dist + neighbor.distance;
            if (newDist < distance[neighbor.location]) {
                distance[neighbor.location] = newDist;
                previous[neighbor.location] = current;
                pq.push({newDist, neighbor.location});
            }
        }
    }

    // Reconstruct the path
    std::vector<std::string> path;
std::string current = destination;

std::stack<std::string> reversedPath; // Create a stack to reverse the path

while (current != source) {
    reversedPath.push(current);
    current = previous[current];
}

reversedPath.push(source); // Add the source to the stack

// Transfer the reversed path to the vector
while (!reversedPath.empty()) {
    path.push_back(reversedPath.top());
    reversedPath.pop();
}

return path;
}

// Modify the displayMap method to show distances and costs
void VillageMap::displayMapWithCosts() {
    // Create a new graph context
    Agraph_t* G = agopen("G", Agstrictdirected, nullptr);

    // Add nodes to the graph
    std::map<std::string, Agnode_t*> nodeMap;
    for (const auto& entry : mapGraph) {
        const std::string& location = entry.first;
        Agnode_t* node = agnode(G, (char*)location.c_str(), 1);
        nodeMap[location] = node;
    }

    // Add edges to the graph, including distances and costs as labels
    for (const auto& entry : mapGraph) {
        const std::string& location1 = entry.first;
        Agnode_t* node1 = nodeMap[location1];

        for (const LocationInfo& info : entry.second) {
            const std::string& location2 = info.location;
            Agedge_t* edge = agedge(G, node1, nodeMap[location2], nullptr, 1);

            // Add labels for distances and costs
            char label[100];
            snprintf(label, sizeof(label), "Dist: %.1fm\nCost: rs.%.2f", info.distance, info.cost);
            agsafeset(edge, const_cast<char*>("label"), label, const_cast<char*>(""));
        }
    }

    // Initialize the Graphviz context
    GVC_t* gvc = gvContext();

    // Layout the graph
    gvLayout(gvc, G, "dot");

    // Render the graph to a file (e.g., PNG)
    gvRenderFilename(gvc, G, "png", "village_map_with_costs.png");

    // Free resources
    gvFreeLayout(gvc, G);
    agclose(G);
    gvFreeContext(gvc);

    std::cout << "Graph visualization with costs created successfully." << std::endl;
}
// Modify the displayEconomicalPath method to highlight the nodes and edges in the most economical path
// Modify the displayEconomicalPath method to highlight the nodes and edges in the most economical path
void VillageMap::displayEconomicalPath(const std::vector<std::string>& economicalPath) {
    GVC_t* gvc = gvContext();
    Agraph_t* g = agopen("G", Agdirected, nullptr);

    double totalCost = 0.0; // Initialize total cost

    // Add nodes to the graph
    for (const auto& [location, _] : mapGraph) {
        Agnode_t* node = agnode(g, const_cast<char*>(location.c_str()), true);
        agsafeset(node, const_cast<char*>("label"), const_cast<char*>(location.c_str()), const_cast<char*>(""));
    }

    // Add edges to the graph
    for (const auto& [location, connections] : mapGraph) {
        for (const auto& connection : connections) {
            Agnode_t* node1 = agnode(g, const_cast<char*>(location.c_str()), false);
            Agnode_t* node2 = agnode(g, const_cast<char*>(connection.location.c_str()), false);
            Agedge_t* edge = agedge(g, node1, node2, nullptr, true);
            double edgeCost = connection.cost;

            // Set the label of the edge to display the rounded-off cost
            char label[32];
            snprintf(label, sizeof(label), "%.2f", edgeCost); // Round off to 2 decimal places
            agsafeset(edge, const_cast<char*>("label"), label, const_cast<char*>(""));

            totalCost += edgeCost; // Add the cost of this edge to the total cost
        }
    }

    // Highlight the nodes and edges in the most economical path
    for (size_t i = 0; i < economicalPath.size() - 1; i++) {
        Agnode_t* node1 = agnode(g, const_cast<char*>(economicalPath[i].c_str()), false);
        Agnode_t* node2 = agnode(g, const_cast<char*>(economicalPath[i + 1].c_str()), false);
        Agedge_t* edge = agedge(g, node1, node2, nullptr, false);
        agsafeset(edge, const_cast<char*>("color"), const_cast<char*>("blue"), const_cast<char*>(""));
        agsafeset(node1, const_cast<char*>("color"), const_cast<char*>("blue"), const_cast<char*>(""));
        agsafeset(node2, const_cast<char*>("color"), const_cast<char*>("blue"), const_cast<char*>(""));
    }

    // // Set the graph label to display the total cost
    // char totalCostLabel[64];
    // snprintf(totalCostLabel, sizeof(totalCostLabel), "Total Cost: %.2f", totalCost);
    // agsafeset(g, const_cast<char*>("label"), totalCostLabel, const_cast<char*>(""));

    // Render the graph using Graphviz
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", "economical_path.png");
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
}


// Modify the displayMap method to highlight the nodes and edges in the shortest path
void VillageMap::displayPath(const std::vector<std::string>& shortestPath) {
    GVC_t* gvc = gvContext();
    Agraph_t* g = agopen("G", Agdirected, nullptr);

    // Add nodes to the graph
    for (const auto& [location, _] : mapGraph) {
        Agnode_t* node = agnode(g, const_cast<char*>(location.c_str()), true);
        agsafeset(node, const_cast<char*>("label"), const_cast<char*>(location.c_str()), const_cast<char*>(""));
    }

    // Add edges to the graph
    for (const auto& [location, connections] : mapGraph) {
        for (const auto& connection : connections) {
            Agnode_t* node1 = agnode(g, const_cast<char*>(location.c_str()), false);
            Agnode_t* node2 = agnode(g, const_cast<char*>(connection.location.c_str()), false);
            agedge(g, node1, node2, nullptr, true);
        }
    }

    // Highlight the nodes and edges in the shortest path
    for (size_t i = 0; i < shortestPath.size() - 1; i++) {
        Agnode_t* node1 = agnode(g, const_cast<char*>(shortestPath[i].c_str()), false);
        Agnode_t* node2 = agnode(g, const_cast<char*>(shortestPath[i + 1].c_str()), false);
        Agedge_t* edge = agedge(g, node1, node2, nullptr, false);
        agsafeset(edge, const_cast<char*>("color"), const_cast<char*>("red"), const_cast<char*>(""));
        agsafeset(node1, const_cast<char*>("color"), const_cast<char*>("red"), const_cast<char*>(""));
        agsafeset(node2, const_cast<char*>("color"), const_cast<char*>("red"), const_cast<char*>(""));
    }

    // Render the graph using Graphviz
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", "shortest_path.png");
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
}

// Search for and display the shortest path between source and destination
void searchAndDisplayShortestPath(VillageMap& village, const std::string& source, const std::string& destination) {
    std::vector<std::string> shortestPath = village.findShortestPath(source, destination);

    std::cout << "Shortest Path from " << source << " to " << destination << ":" << std::endl;
    for (const std::string& location : shortestPath) {
        std::cout << location;
        if (location != destination) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
    village.displayPath(shortestPath);
}
//Search and find most economical path

std::vector<std::string> VillageMap::findMostEconomicalPath(const std::string& source, const std::string& destination) {
    std::unordered_map<std::string, double> distance;
    std::unordered_map<std::string, double> cost;
    std::unordered_map<std::string, std::string> previous;
    std::priority_queue<std::pair<std::pair<double, double>, std::string>, std::vector<std::pair<std::pair<double, double>, std::string>>, std::greater<std::pair<std::pair<double, double>, std::string>>> pq;

    // Initialize distances and costs to all nodes as infinity
    for (const auto& entry : mapGraph) {
        distance[entry.first] = std::numeric_limits<double>::max();
        cost[entry.first] = std::numeric_limits<double>::max();
    }

    // The distance and cost from the source to itself are 0
    distance[source] = 0.0;
    cost[source] = 0.0;
    pq.push({{0.0, 0.0}, source});

    while (!pq.empty()) {
        double dist = pq.top().first.first;
        double c = pq.top().first.second;
        std::string current = pq.top().second;
        pq.pop();

        if (dist > distance[current] || (dist == distance[current] && c > cost[current])) {
            continue;
        }

        for (const LocationInfo& neighbor : mapGraph[current]) {
            double newDist = dist + neighbor.distance;
            double newCost = c + neighbor.cost;
            if (newDist < distance[neighbor.location] || (newDist == distance[neighbor.location] && newCost < cost[neighbor.location])) {
                distance[neighbor.location] = newDist;
                cost[neighbor.location] = newCost;
                previous[neighbor.location] = current;
                pq.push({{newDist, newCost}, neighbor.location});
            }
        }
    }

    // Reconstruct the path
    std::vector<std::string> path;
    std::string current = destination;

    std::stack<std::string> reversedPath; // Create a stack to reverse the path

    while (current != source) {
        reversedPath.push(current);
        current = previous[current];
    }

    reversedPath.push(source); // Add the source to the stack

    // Transfer the reversed path to the vector
    while (!reversedPath.empty()) {
        path.push_back(reversedPath.top());
        reversedPath.pop();
    }

    return path;
}


void loadGraph1() {
    // Create a village map
    VillageMap village;

    // Add locations
   // Add locations
    village.addLocation("Noida City Centre");
    village.addLocation("Noida Golf Course");
    village.addLocation("Botanical Garden");
    village.addLocation("Noida Sector 18");
    village.addLocation("Noida Sector 16");
    village.addLocation("Noida Sector 15");
    village.addLocation("New Ashok Nagar");
    village.addLocation("Mayur Vihar Extension");
    village.addLocation("Mayur Vihar-I");
    village.addLocation("Akshardham");
    village.addLocation("Yamuna Bank");
    village.addLocation("Indraprastha");
    village.addLocation("Pragati Maidan");
    village.addLocation("Mandi House");
    village.addLocation("Barakhamba Road");
    village.addLocation("Rajiv Chowk");
    village.addLocation("R K Ashram Marg");
    village.addLocation("Jhandewalan");
    village.addLocation("Karol Bagh");
    village.addLocation("Rajendra Place");
    village.addLocation("Patel Nagar");
    village.addLocation("Shadipur");
    village.addLocation("Kirti Nagar");
    village.addLocation("Moti Nagar");
    village.addLocation("Ramesh Nagar");
    village.addLocation("Rajouri Garden");
    village.addLocation("Tagore Garden");
    village.addLocation("Subhash Nagar");
    village.addLocation("Janakpuri East");
    village.addLocation("Janakpuri West");
    village.addLocation("Uttam Nagar East");
    village.addLocation("Uttam Nagar West");
    village.addLocation("Nawada");
    village.addLocation("Dwarka Mor");
    village.addLocation("Dwarka");
    village.addLocation("Dwarka Sector 14");
    village.addLocation("Dwarka Sector 13");
    village.addLocation("Dwarka Sector 12");
    village.addLocation("Dwarka Sector 11");
    village.addLocation("Dwarka Sector 10");
    village.addLocation("Dwarka Sector 9");
    village.addLocation("Dwarka Sector 8");
    village.addLocation("Dwarka Sector 21");
    // Add paths between locations with a default distance of 500 meters
    // Chain-like connections
    village.addPath("Noida City Centre", "Noida Golf Course");
    village.addPath("Noida Golf Course", "Botanical Garden");
    village.addPath("Botanical Garden", "Noida Sector 18");
    village.addPath("Noida Sector 18", "Noida Sector 16");
    village.addPath("Noida Sector 16", "Noida Sector 15");
    village.addPath("Noida Sector 15", "New Ashok Nagar");
    village.addPath("New Ashok Nagar", "Mayur Vihar Extension");
    village.addPath("Mayur Vihar Extension", "Mayur Vihar-I");
    village.addPath("Mayur Vihar-I", "Akshardham");
    village.addPath("Akshardham", "Yamuna Bank");
    village.addPath("Yamuna Bank", "Indraprastha");
    village.addPath("Indraprastha", "Pragati Maidan");
    village.addPath("Pragati Maidan", "Mandi House");
    village.addPath("Mandi House", "Barakhamba Road");
    village.addPath("Barakhamba Road", "Rajiv Chowk");
    village.addPath("Rajiv Chowk", "R K Ashram Marg");
    village.addPath("R K Ashram Marg", "Jhandewalan");
    village.addPath("Jhandewalan", "Karol Bagh");
    village.addPath("Karol Bagh", "Rajendra Place");
    village.addPath("Rajendra Place", "Patel Nagar");
    village.addPath("Patel Nagar", "Shadipur");
    village.addPath("Shadipur", "Kirti Nagar");
    village.addPath("Kirti Nagar", "Moti Nagar");
    village.addPath("Moti Nagar", "Ramesh Nagar");
    village.addPath("Ramesh Nagar", "Rajouri Garden");
    village.addPath("Rajouri Garden", "Tagore Garden");
    village.addPath("Tagore Garden", "Subhash Nagar");
    village.addPath("Subhash Nagar", "Janakpuri East");
    village.addPath("Janakpuri East", "Janakpuri West");
    village.addPath("Janakpuri West", "Uttam Nagar East");
    village.addPath("Uttam Nagar East", "Uttam Nagar West");
    village.addPath("Uttam Nagar West", "Nawada");
    village.addPath("Nawada", "Dwarka Mor");
    village.addPath("Dwarka Mor", "Dwarka");
    village.addPath("Dwarka", "Dwarka Sector 14");
    village.addPath("Dwarka Sector 14", "Dwarka Sector 13");
    village.addPath("Dwarka Sector 13", "Dwarka Sector 12");
    village.addPath("Dwarka Sector 12", "Dwarka Sector 11");
    village.addPath("Dwarka Sector 11", "Dwarka Sector 10");
    village.addPath("Dwarka Sector 10", "Dwarka Sector 9");
    village.addPath("Dwarka Sector 9", "Dwarka Sector 8");
    village.addPath("Dwarka Sector 8", "Dwarka Sector 21");
    
    // Connections to Yamuna Bank
    village.addPath("Yamuna Bank", "Laxmi Nagar");
    village.addPath("Laxmi Nagar", "Nirman Vihar");
    village.addPath("Nirman Vihar", "Preet Vihar");
    village.addPath("Preet Vihar", "Karkarduma");
    village.addPath("Karkarduma", "Anand Vihar ISBT");
    village.addPath("Anand Vihar ISBT", "Kaushambi");
    village.addPath("Kaushambi", "Vaishali");
    
    // Connections to Kirti Nagar
    village.addPath("Kirti Nagar", "Satguru Ramsingh Marg");
    village.addPath("Satguru Ramsingh Marg", "Ashok Park Main");
    village.addPath("Ashok Park Main", "Punjabi Bagh East");
    village.addPath("Punjabi Bagh East", "Shivaji Park");
    village.addPath("Shivaji Park", "Madipur");
    village.addPath("Madipur", "Paschim Vihar East");
    village.addPath("Paschim Vihar East", "Paschim Vihar West");
    village.addPath("Paschim Vihar West", "Peera Garhi");
    village.addPath("Peera Garhi", "Udyog Nagar");
    village.addPath("Udyog Nagar", "Surajmal Stadium");
    village.addPath("Surajmal Stadium", "Nangloi");
    village.addPath("Nangloi", "Nangloi Railway station");
    village.addPath("Nangloi Railway station", "Rajdhani Park");
    village.addPath("Rajdhani Park", "Mundka");

    // Display the village map graphically
    village.displayMap();

    std::cout << "Graph loaded successfully in village_map.png" << std::endl;
    
 
    // ... (Rest of the main function as before)

    // Example: Find and display the shortest path between two locations

    // std::string source, destination;
    // std::cout<<"Enter the source stations: ";
    // std::cin>>source;
    // std::cout<<"Enter the destination stations: ";
    // std::cin>>destination;
    searchAndDisplayShortestPath(village, "Botanical Garden", "Rajiv Chowk");
    
    std::cout<<"done!!";
}

void loadGraph2() {
    // Create a village map
    VillageMap village;

    // Add locations
   // Add locations
    village.addLocation("Noida City Centre");
    village.addLocation("Noida Golf Course");
    village.addLocation("Botanical Garden");
    village.addLocation("Noida Sector 18");
    village.addLocation("Noida Sector 16");
    village.addLocation("Noida Sector 15");
    village.addLocation("New Ashok Nagar");
    village.addLocation("Mayur Vihar Extension");
    village.addLocation("Mayur Vihar-I");
    village.addLocation("Akshardham");
    village.addLocation("Yamuna Bank");
    village.addLocation("Indraprastha");
    village.addLocation("Pragati Maidan");
    village.addLocation("Mandi House");
    village.addLocation("Barakhamba Road");
    village.addLocation("Rajiv Chowk");
    village.addLocation("R K Ashram Marg");
    village.addLocation("Jhandewalan");
    village.addLocation("Karol Bagh");
    village.addLocation("Rajendra Place");
    village.addLocation("Patel Nagar");
    village.addLocation("Shadipur");
    village.addLocation("Kirti Nagar");
    village.addLocation("Moti Nagar");
    village.addLocation("Ramesh Nagar");
    village.addLocation("Rajouri Garden");
    village.addLocation("Tagore Garden");
    village.addLocation("Subhash Nagar");
    village.addLocation("Janakpuri East");
    village.addLocation("Janakpuri West");
    village.addLocation("Uttam Nagar East");
    village.addLocation("Uttam Nagar West");
    village.addLocation("Nawada");
    village.addLocation("Dwarka Mor");
    village.addLocation("Dwarka");
    village.addLocation("Dwarka Sector 14");
    village.addLocation("Dwarka Sector 13");
    village.addLocation("Dwarka Sector 12");
    village.addLocation("Dwarka Sector 11");
    village.addLocation("Dwarka Sector 10");
    village.addLocation("Dwarka Sector 9");
    village.addLocation("Dwarka Sector 8");
    village.addLocation("Dwarka Sector 21");
    // Add paths between locations with a default distance of 500 meters
    // Chain-like connections
    village.addPath("Noida City Centre", "Noida Golf Course");
    village.addPath("Noida Golf Course", "Botanical Garden");
    village.addPath("Botanical Garden", "Noida Sector 18");
    village.addPath("Noida Sector 18", "Noida Sector 16");
    village.addPath("Noida Sector 16", "Noida Sector 15");
    village.addPath("Noida Sector 15", "New Ashok Nagar");
    village.addPath("New Ashok Nagar", "Mayur Vihar Extension");
    village.addPath("Mayur Vihar Extension", "Mayur Vihar-I");
    village.addPath("Mayur Vihar-I", "Akshardham");
    village.addPath("Akshardham", "Yamuna Bank");
    village.addPath("Yamuna Bank", "Indraprastha");
    village.addPath("Indraprastha", "Pragati Maidan");
    village.addPath("Pragati Maidan", "Mandi House");
    village.addPath("Mandi House", "Barakhamba Road");
    village.addPath("Barakhamba Road", "Rajiv Chowk");
    village.addPath("Rajiv Chowk", "R K Ashram Marg");
    village.addPath("R K Ashram Marg", "Jhandewalan");
    village.addPath("Jhandewalan", "Karol Bagh");
    village.addPath("Karol Bagh", "Rajendra Place");
    village.addPath("Rajendra Place", "Patel Nagar");
    village.addPath("Patel Nagar", "Shadipur");
    village.addPath("Shadipur", "Kirti Nagar");
    village.addPath("Kirti Nagar", "Moti Nagar");
    village.addPath("Moti Nagar", "Ramesh Nagar");
    village.addPath("Ramesh Nagar", "Rajouri Garden");
    village.addPath("Rajouri Garden", "Tagore Garden");
    village.addPath("Tagore Garden", "Subhash Nagar");
    village.addPath("Subhash Nagar", "Janakpuri East");
    village.addPath("Janakpuri East", "Janakpuri West");
    village.addPath("Janakpuri West", "Uttam Nagar East");
    village.addPath("Uttam Nagar East", "Uttam Nagar West");
    village.addPath("Uttam Nagar West", "Nawada");
    village.addPath("Nawada", "Dwarka Mor");
    village.addPath("Dwarka Mor", "Dwarka");
    village.addPath("Dwarka", "Dwarka Sector 14");
    village.addPath("Dwarka Sector 14", "Dwarka Sector 13");
    village.addPath("Dwarka Sector 13", "Dwarka Sector 12");
    village.addPath("Dwarka Sector 12", "Dwarka Sector 11");
    village.addPath("Dwarka Sector 11", "Dwarka Sector 10");
    village.addPath("Dwarka Sector 10", "Dwarka Sector 9");
    village.addPath("Dwarka Sector 9", "Dwarka Sector 8");
    village.addPath("Dwarka Sector 8", "Dwarka Sector 21");
    
    // Connections to Yamuna Bank
    village.addPath("Yamuna Bank", "Laxmi Nagar");
    village.addPath("Laxmi Nagar", "Nirman Vihar");
    village.addPath("Nirman Vihar", "Preet Vihar");
    village.addPath("Preet Vihar", "Karkarduma");
    village.addPath("Karkarduma", "Anand Vihar ISBT");
    village.addPath("Anand Vihar ISBT", "Kaushambi");
    village.addPath("Kaushambi", "Vaishali");
    
    // Connections to Kirti Nagar
    village.addPath("Kirti Nagar", "Satguru Ramsingh Marg");
    village.addPath("Satguru Ramsingh Marg", "Ashok Park Main");
    village.addPath("Ashok Park Main", "Punjabi Bagh East");
    village.addPath("Punjabi Bagh East", "Shivaji Park");
    village.addPath("Shivaji Park", "Madipur");
    village.addPath("Madipur", "Paschim Vihar East");
    village.addPath("Paschim Vihar East", "Paschim Vihar West");
    village.addPath("Paschim Vihar West", "Peera Garhi");
    village.addPath("Peera Garhi", "Udyog Nagar");
    village.addPath("Udyog Nagar", "Surajmal Stadium");
    village.addPath("Surajmal Stadium", "Nangloi");
    village.addPath("Nangloi", "Nangloi Railway station");
    village.addPath("Nangloi Railway station", "Rajdhani Park");
    village.addPath("Rajdhani Park", "Mundka");

    // Display the village map graphically
     village.displayMapWithCosts();

    std::cout << "Graph loaded successfully in village_map.png" << std::endl;
        std::vector<std::string> economicalPath = village.findMostEconomicalPath("Botanical Garden", "Rajiv Chowk");

     village.displayEconomicalPath(economicalPath);

    // ... (Rest of the main function as before)

    // Example: Find and display the shortest path between two locations

    // std::string source, destination;
    // std::cout<<"Enter the source stations: ";
    // std::cin>>source;
    // std::cout<<"Enter the destination stations: ";
    // std::cin>>destination;
    
    
    std::cout<<"done!!";
}
