#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <stack>
#include <fstream>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

// Forward declarations
class City;
class Trip;

// Structure to represent an attraction in a city
struct Attraction {
    string name;
    double rating;
    string type;
    
    Attraction(string n = "", double r = 0.0, string t = "") 
        : name(n), rating(r), type(t) {}
};

// Structure to represent a hotel in a city
struct Hotel {
    string name;
    double rating;
    int pricePerNight;
    
    Hotel(string n = "", double r = 0.0, int p = 0) 
        : name(n), rating(r), pricePerNight(p) {}
};

// City class representing a travel destination
class City {
public:
    string name;
    string country;
    double popularity;
    vector<Hotel> hotels;
    vector<Attraction> attractions;
    
    City(string n = "", string c = "", double p = 0.0) 
        : name(n), country(c), popularity(p) {}
    
    // Add hotel to city
    void addHotel(const Hotel& hotel) {
        hotels.push_back(hotel);
    }
    
    // Add attraction to city
    void addAttraction(const Attraction& attraction) {
        attractions.push_back(attraction);
    }
    
    // Display city information
    void displayInfo() const {
        cout << "\n=== " << name << ", " << country << " ===\n";
        cout << "Popularity Score: " << popularity << "\n";
        
        cout << "\nTop Hotels:\n";
        for (const auto& hotel : hotels) {
            cout << "  - " << hotel.name << " (Rating: " << hotel.rating 
                 << ", $" << hotel.pricePerNight << "/night)\n";
        }
        
        cout << "\nTop Attractions:\n";
        for (const auto& attraction : attractions) {
            cout << "  - " << attraction.name << " (" << attraction.type 
                 << ", Rating: " << attraction.rating << ")\n";
        }
    }
};

// Trip class representing a planned journey
class Trip {
public:
    string source;
    string destination;
    int distance;
    int cost;
    string date;
    string status; // "planned", "completed", "cancelled"
    
    Trip(string src = "", string dest = "", int dist = 0, int c = 0, string d = "", string s = "planned")
        : source(src), destination(dest), distance(dist), cost(c), date(d), status(s) {}
    
    void displayTrip() const {
        cout << source << " -> " << destination 
             << " (Distance: " << distance << "km, Cost: $" << cost 
             << ", Date: " << date << ", Status: " << status << ")\n";
    }
};

// Binary Search Tree Node for City storage
struct BSTNode {
    City city;
    BSTNode* left;
    BSTNode* right;
    
    BSTNode(const City& c) : city(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree for City management
class CityBST {
private:
    BSTNode* root;
    
    // Helper function for insertion
    BSTNode* insert(BSTNode* node, const City& city) {
        if (node == nullptr) {
            return new BSTNode(city);
        }
        
        if (city.name < node->city.name) {
            node->left = insert(node->left, city);
        } else if (city.name > node->city.name) {
            node->right = insert(node->right, city);
        }
        
        return node;
    }
    
    // Helper function for searching
    BSTNode* search(BSTNode* node, const string& cityName) {
        if (node == nullptr || node->city.name == cityName) {
            return node;
        }
        
        if (cityName < node->city.name) {
            return search(node->left, cityName);
        }
        
        return search(node->right, cityName);
    }
    
    // Helper function for in-order traversal
    void inOrder(BSTNode* node, vector<City>& cities) {
        if (node != nullptr) {
            inOrder(node->left, cities);
            cities.push_back(node->city);
            inOrder(node->right, cities);
        }
    }
    
public:
    CityBST() : root(nullptr) {}
    
    // Insert a city into BST
    void insertCity(const City& city) {
        root = insert(root, city);
    }
    
    // Search for a city by name
    City* findCity(const string& cityName) {
        BSTNode* result = search(root, cityName);
        return result ? &(result->city) : nullptr;
    }
    
    // Get all cities in sorted order (by name)
    vector<City> getAllCities() {
        vector<City> cities;
        inOrder(root, cities);
        return cities;
    }
};

// Heap structure for priority-based city recommendations
class CityHeap {
private:
    vector<City> heap;
    
    // Helper functions for heap operations
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }
    
    // Heapify up (for max heap based on popularity)
    void heapifyUp(int index) {
        if (index > 0 && heap[parent(index)].popularity < heap[index].popularity) {
            swap(heap[parent(index)], heap[index]);
            heapifyUp(parent(index));
        }
    }
    
    // Heapify down
    void heapifyDown(int index) {
        int maxIndex = index;
        int left = leftChild(index);
        int right = rightChild(index);
        
        if (left < heap.size() && heap[left].popularity > heap[maxIndex].popularity) {
            maxIndex = left;
        }
        
        if (right < heap.size() && heap[right].popularity > heap[maxIndex].popularity) {
            maxIndex = right;
        }
        
        if (index != maxIndex) {
            swap(heap[index], heap[maxIndex]);
            heapifyDown(maxIndex);
        }
    }
    
public:
    // Insert city into heap
    void insertCity(const City& city) {
        heap.push_back(city);
        heapifyUp(heap.size() - 1);
    }
    
    // Get most popular city (max element)
    City getTopDestination() {
        if (heap.empty()) {
            return City();
        }
        
        City top = heap[0];
        heap[0] = heap[heap.size() - 1];
        heap.pop_back();
        
        if (!heap.empty()) {
            heapifyDown(0);
        }
        
        return top;
    }
    
    // Check if heap is empty
    bool isEmpty() {
        return heap.empty();
    }
    
    // Get top N destinations without removing them
    vector<City> getTopNDestinations(int n) {
        vector<City> result;
        CityHeap tempHeap = *this; // Copy current heap
        
        for (int i = 0; i < n && !tempHeap.isEmpty(); i++) {
            result.push_back(tempHeap.getTopDestination());
        }
        
        return result;
    }
};

// Graph class for route planning using adjacency list
class RouteGraph {
private:
    unordered_map<string, vector<pair<string, int>>> adjList;
    
public:
    // Add an edge between two cities with distance
    void addRoute(const string& city1, const string& city2, int distance) {
        adjList[city1].push_back({city2, distance});
        adjList[city2].push_back({city1, distance}); // Undirected graph
    }
    
    // Dijkstra's algorithm to find shortest path
    pair<int, vector<string>> findShortestPath(const string& source, const string& destination) {
        unordered_map<string, int> distances;
        unordered_map<string, string> previous;
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        
        // Initialize distances
        for (const auto& pair : adjList) {
            distances[pair.first] = INT_MAX;
        }
        distances[source] = 0;
        pq.push({0, source});
        
        while (!pq.empty()) {
            int currentDist = pq.top().first;
            string currentCity = pq.top().second;
            pq.pop();
            
            if (currentCity == destination) break;
            
            if (currentDist > distances[currentCity]) continue;
            
            for (const auto& neighbor : adjList[currentCity]) {
                string nextCity = neighbor.first;
                int weight = neighbor.second;
                int newDist = currentDist + weight;
                
                if (newDist < distances[nextCity]) {
                    distances[nextCity] = newDist;
                    previous[nextCity] = currentCity;
                    pq.push({newDist, nextCity});
                }
            }
        }
        
        // Reconstruct path
        vector<string> path;
        string current = destination;
        
        if (distances[destination] == INT_MAX) {
            return {-1, path}; // No path found
        }
        
        while (current != source) {
            path.push_back(current);
            current = previous[current];
        }
        path.push_back(source);
        reverse(path.begin(), path.end());
        
        return {distances[destination], path};
    }
    
    // Display all available routes
    void displayRoutes() {
        cout << "\n=== Available Routes ===\n";
        for (const auto& city : adjList) {
            cout << city.first << " connects to: ";
            for (const auto& neighbor : city.second) {
                cout << neighbor.first << "(" << neighbor.second << "km) ";
            }
            cout << "\n";
        }
    }
};

// Sorting algorithms for cities
class CitySorter {
public:
    // Quick Sort by cost (ascending)
    static void quickSortByCost(vector<Trip>& trips, int low, int high) {
        if (low < high) {
            int pi = partitionByCost(trips, low, high);
            quickSortByCost(trips, low, pi - 1);
            quickSortByCost(trips, pi + 1, high);
        }
    }
    
    // Merge Sort by distance (ascending)
    static void mergeSortByDistance(vector<Trip>& trips, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSortByDistance(trips, left, mid);
            mergeSortByDistance(trips, mid + 1, right);
            mergeByDistance(trips, left, mid, right);
        }
    }
    
    // Sort cities by popularity (descending)
    static void sortCitiesByPopularity(vector<City>& cities) {
        sort(cities.begin(), cities.end(), [](const City& a, const City& b) {
            return a.popularity > b.popularity;
        });
    }

private:
    // Partition function for Quick Sort
    static int partitionByCost(vector<Trip>& trips, int low, int high) {
        int pivot = trips[high].cost;
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (trips[j].cost <= pivot) {
                i++;
                swap(trips[i], trips[j]);
            }
        }
        swap(trips[i + 1], trips[high]);
        return i + 1;
    }
    
    // Merge function for Merge Sort
    static void mergeByDistance(vector<Trip>& trips, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        vector<Trip> leftArr(n1), rightArr(n2);
        
        for (int i = 0; i < n1; i++)
            leftArr[i] = trips[left + i];
        for (int j = 0; j < n2; j++)
            rightArr[j] = trips[mid + 1 + j];
        
        int i = 0, j = 0, k = left;
        
        while (i < n1 && j < n2) {
            if (leftArr[i].distance <= rightArr[j].distance) {
                trips[k] = leftArr[i];
                i++;
            } else {
                trips[k] = rightArr[j];
                j++;
            }
            k++;
        }
        
        while (i < n1) {
            trips[k] = leftArr[i];
            i++; k++;
        }
        
        while (j < n2) {
            trips[k] = rightArr[j];
            j++; k++;
        }
    }
};

// Main Travel Planner Manager class
class TravelPlannerManager {
private:
    RouteGraph routeGraph;
    CityBST cityDatabase;
    CityHeap popularDestinations;
    stack<Trip> travelHistory;
    queue<Trip> futureTrips;
    vector<Trip> allTrips;

public:
    // Constructor - Initialize with sample data
    TravelPlannerManager() {
        initializeSampleData();
    }
    
    // Initialize sample cities and routes
    void initializeSampleData() {
        // Create sample cities
        City paris("Paris", "France", 9.5);
        paris.addHotel(Hotel("Hotel Ritz", 4.8, 500));
        paris.addHotel(Hotel("Ibis Budget", 4.2, 80));
        paris.addAttraction(Attraction("Eiffel Tower", 4.9, "Monument"));
        paris.addAttraction(Attraction("Louvre Museum", 4.7, "Museum"));
        
        City london("London", "UK", 9.2);
        london.addHotel(Hotel("The Savoy", 4.9, 600));
        london.addHotel(Hotel("Premier Inn", 4.3, 90));
        london.addAttraction(Attraction("Big Ben", 4.6, "Monument"));
        london.addAttraction(Attraction("British Museum", 4.8, "Museum"));
        
        City rome("Rome", "Italy", 8.8);
        rome.addHotel(Hotel("Hotel Eden", 4.7, 400));
        rome.addHotel(Hotel("B&B Rome", 4.1, 70));
        rome.addAttraction(Attraction("Colosseum", 4.9, "Historical"));
        rome.addAttraction(Attraction("Vatican City", 4.8, "Religious"));
        
        City tokyo("Tokyo", "Japan", 9.0);
        tokyo.addHotel(Hotel("Park Hyatt", 4.8, 450));
        tokyo.addHotel(Hotel("Capsule Hotel", 4.0, 40));
        tokyo.addAttraction(Attraction("Tokyo Tower", 4.5, "Monument"));
        tokyo.addAttraction(Attraction("Senso-ji Temple", 4.6, "Religious"));
        
        City newyork("New York", "USA", 9.3);
        newyork.addHotel(Hotel("The Plaza", 4.6, 550));
        newyork.addHotel(Hotel("YMCA", 3.8, 100));
        newyork.addAttraction(Attraction("Statue of Liberty", 4.7, "Monument"));
        newyork.addAttraction(Attraction("Central Park", 4.8, "Park"));
        
        // Add cities to BST and Heap
        cityDatabase.insertCity(paris);
        cityDatabase.insertCity(london);
        cityDatabase.insertCity(rome);
        cityDatabase.insertCity(tokyo);
        cityDatabase.insertCity(newyork);
        
        popularDestinations.insertCity(paris);
        popularDestinations.insertCity(london);
        popularDestinations.insertCity(rome);
        popularDestinations.insertCity(tokyo);
        popularDestinations.insertCity(newyork);
        
        // Add routes between cities (distance in km)
        routeGraph.addRoute("Paris", "London", 344);
        routeGraph.addRoute("Paris", "Rome", 1105);
        routeGraph.addRoute("London", "Rome", 1435);
        routeGraph.addRoute("Paris", "New York", 5837);
        routeGraph.addRoute("London", "New York", 5585);
        routeGraph.addRoute("Rome", "New York", 6905);
        routeGraph.addRoute("Tokyo", "New York", 10838);
        routeGraph.addRoute("Paris", "Tokyo", 9714);
        routeGraph.addRoute("London", "Tokyo", 9560);
        
        // Add some sample completed trips to history
        travelHistory.push(Trip("Paris", "London", 344, 200, "2024-01-15", "completed"));
        travelHistory.push(Trip("London", "Rome", 1435, 350, "2024-02-20", "completed"));
        
        // Add sample future trips
        futureTrips.push(Trip("Rome", "Tokyo", 9714, 800, "2024-08-15", "planned"));
        futureTrips.push(Trip("Tokyo", "New York", 10838, 1200, "2024-09-01", "planned"));
    }
    
    // Plan a new trip between two cities
    void planTrip() {
        string source, destination, date;
        
        cout << "\n=== Plan New Trip ===\n";
        cout << "Enter source city: ";
        cin.ignore();
        getline(cin, source);
        cout << "Enter destination city: ";
        getline(cin, destination);
        cout << "Enter travel date (YYYY-MM-DD): ";
        getline(cin, date);
        
        // Find shortest path using Dijkstra's algorithm
        auto result = routeGraph.findShortestPath(source, destination);
        
        if (result.first == -1) {
            cout << "No route found between " << source << " and " << destination << "\n";
            return;
        }
        
        int distance = result.first;
        vector<string> path = result.second;
        int estimatedCost = distance * 0.1; // $0.10 per km
        
        cout << "\n--- Route Found ---\n";
        cout << "Path: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << "\nTotal Distance: " << distance << " km\n";
        cout << "Estimated Cost: $" << estimatedCost << "\n";
        
        // Create and add trip to future trips queue
        Trip newTrip(source, destination, distance, estimatedCost, date, "planned");
        futureTrips.push(newTrip);
        allTrips.push_back(newTrip);
        
        cout << "Trip added to your future trips!\n";
    }
    
    // View travel history using stack
    void viewTravelHistory() {
        cout << "\n=== Travel History ===\n";
        
        if (travelHistory.empty()) {
            cout << "No travel history available.\n";
            return;
        }
        
        stack<Trip> tempStack = travelHistory; // Copy to preserve original
        
        cout << "Recent trips (most recent first):\n";
        while (!tempStack.empty()) {
            tempStack.top().displayTrip();
            tempStack.pop();
        }
        
        // Option to undo last trip
        cout << "\nWould you like to undo the last trip? (y/n): ";
        char choice;
        cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            if (!travelHistory.empty()) {
                Trip lastTrip = travelHistory.top();
                travelHistory.pop();
                cout << "Undone trip: ";
                lastTrip.displayTrip();
            }
        }
    }
    
    // Manage future trips using queue
    void manageFutureTrips() {
        cout << "\n=== Future Trips Management ===\n";
        cout << "1. View upcoming trips\n";
        cout << "2. Cancel next trip\n";
        cout << "3. Mark next trip as completed\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                if (futureTrips.empty()) {
                    cout << "No upcoming trips planned.\n";
                    return;
                }
                
                queue<Trip> tempQueue = futureTrips;
                cout << "\nUpcoming trips:\n";
                while (!tempQueue.empty()) {
                    tempQueue.front().displayTrip();
                    tempQueue.pop();
                }
                break;
            }
            
            case 2: {
                if (futureTrips.empty()) {
                    cout << "No trips to cancel.\n";
                    return;
                }
                
                Trip cancelledTrip = futureTrips.front();
                futureTrips.pop();
                cout << "Cancelled trip: ";
                cancelledTrip.displayTrip();
                break;
            }
            
            case 3: {
                if (futureTrips.empty()) {
                    cout << "No trips to complete.\n";
                    return;
                }
                
                Trip completedTrip = futureTrips.front();
                futureTrips.pop();
                completedTrip.status = "completed";
                
                // Add to travel history
                travelHistory.push(completedTrip);
                
                cout << "Completed trip: ";
                completedTrip.displayTrip();
                break;
            }
            
            default:
                cout << "Invalid choice.\n";
        }
    }
    
    // Search city information using BST
    void searchCityInfo() {
        cout << "\n=== City Information Search ===\n";
        cout << "Enter city name: ";
        
        string cityName;
        cin.ignore();
        getline(cin, cityName);
        
        City* city = cityDatabase.findCity(cityName);
        
        if (city) {
            city->displayInfo();
        } else {
            cout << "City '" << cityName << "' not found in database.\n";
            
            // Show available cities
            cout << "\nAvailable cities:\n";
            vector<City> allCities = cityDatabase.getAllCities();
            for (const auto& c : allCities) {
                cout << "- " << c.name << ", " << c.country << "\n";
            }
        }
    }
    
    // Get top destination recommendations using heap
    void getTopDestinations() {
        cout << "\n=== Top Destination Recommendations ===\n";
        cout << "How many top destinations would you like to see? ";
        
        int n;
        cin >> n;
        
        vector<City> topDestinations = popularDestinations.getTopNDestinations(n);
        
        cout << "\nTop " << n << " destinations by popularity:\n";
        for (size_t i = 0; i < topDestinations.size(); i++) {
            cout << (i + 1) << ". " << topDestinations[i].name 
                 << " (Popularity: " << topDestinations[i].popularity << ")\n";
        }
    }
    
    // Sort and display trips
    void sortTrips() {
        if (allTrips.empty()) {
            cout << "\nNo trips available to sort.\n";
            return;
        }
        
        cout << "\n=== Sort Trips ===\n";
        cout << "1. Sort by Cost (Quick Sort)\n";
        cout << "2. Sort by Distance (Merge Sort)\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        vector<Trip> sortedTrips = allTrips; // Copy for sorting
        
        switch (choice) {
            case 1:
                CitySorter::quickSortByCost(sortedTrips, 0, sortedTrips.size() - 1);
                cout << "\nTrips sorted by cost (ascending):\n";
                break;
                
            case 2:
                CitySorter::mergeSortByDistance(sortedTrips, 0, sortedTrips.size() - 1);
                cout << "\nTrips sorted by distance (ascending):\n";
                break;
                
            default:
                cout << "Invalid choice.\n";
                return;
        }
        
        for (const auto& trip : sortedTrips) {
            trip.displayTrip();
        }
    }
    
    // Display all available routes
    void displayRoutes() {
        routeGraph.displayRoutes();
    }
    
    // Save data to file
    void saveDataToFile() {
        ofstream file("travel_data.txt");
        
        if (!file.is_open()) {
            cout << "Error: Could not create save file.\n";
            return;
        }
        
        file << "=== TRAVEL HISTORY ===\n";
        stack<Trip> tempStack = travelHistory;
        while (!tempStack.empty()) {
            Trip trip = tempStack.top();
            file << trip.source << "," << trip.destination << "," 
                 << trip.distance << "," << trip.cost << "," 
                 << trip.date << "," << trip.status << "\n";
            tempStack.pop();
        }
        
        file << "\n=== FUTURE TRIPS ===\n";
        queue<Trip> tempQueue = futureTrips;
        while (!tempQueue.empty()) {
            Trip trip = tempQueue.front();
            file << trip.source << "," << trip.destination << "," 
                 << trip.distance << "," << trip.cost << "," 
                 << trip.date << "," << trip.status << "\n";
            tempQueue.pop();
        }
        
        file.close();
        cout << "Data saved to travel_data.txt successfully!\n";
    }
    
    // Display main menu
    void displayMenu() {
        cout << "\n" << string(50, '=') << "\n";
        cout << "        SMART TRAVEL PLANNER\n";
        cout << string(50, '=') << "\n";
        cout << "1. Plan a Trip (Graph + Dijkstra's)\n";
        cout << "2. View Travel History (Stack)\n";
        cout << "3. Manage Future Trips (Queue)\n";
        cout << "4. Search City Information (BST)\n";
        cout << "5. Get Top Destinations (Heap)\n";
        cout << "6. Sort Trips (Sorting Algorithms)\n";
        cout << "7. Display Available Routes\n";
        cout << "8. Save Data to File\n";
        cout << "9. Exit\n";
        cout << string(50, '=') << "\n";
        cout << "Enter your choice: ";
    }
    
    // Main program loop
    void run() {
        int choice;
        
        do {
            displayMenu();
            cin >> choice;
            
            switch (choice) {
                case 1: planTrip(); break;
                case 2: viewTravelHistory(); break;
                case 3: manageFutureTrips(); break;
                case 4: searchCityInfo(); break;
                case 5: getTopDestinations(); break;
                case 6: sortTrips(); break;
                case 7: displayRoutes(); break;
                case 8: saveDataToFile(); break;
                case 9: cout << "Thank you for using Smart Travel Planner!\n"; break;
                default: cout << "Invalid choice. Please try again.\n";
            }
            
            if (choice != 9) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
            
        } while (choice != 9);
    }
};

// Main function
int main() {
    cout << "Welcome to Smart Travel Planner!\n";
    cout << "Initializing system with sample data...\n";
    
    TravelPlannerManager planner;
    planner.run();
    
    return 0;
}