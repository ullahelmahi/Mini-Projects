# ✈️ Smart Travel Planner

A C++ console-based travel assistant that helps users plan, manage, and explore trips between major cities using classic data structures and algorithms. It provides route optimization, hotel and attraction suggestions, and trip sorting — all in one interactive system.

---

## 🔍 Features

- 🗺️ **Plan a Trip**  
  Use Dijkstra’s algorithm to find the shortest route between cities and calculate estimated travel cost and distance.

- 🏨 **Search City Info**  
  Retrieve details about a city’s top hotels and attractions using a Binary Search Tree (BST).

- 🕓 **Travel History (Stack)**  
  Track completed trips, view in reverse chronological order, and even undo the last trip.

- 📅 **Manage Future Trips (Queue)**  
  Schedule upcoming trips and manage them easily with queue operations.

- 🌍 **Top Destination Suggestions (Heap)**  
  Discover the most popular cities to visit with a max-heap ranking system.

- 📊 **Sort Trips by Cost or Distance**  
  Use Quick Sort to sort by cost and Merge Sort to sort by distance.

- 🧭 **Display Available Routes**  
  Visualize all connected routes and distances.

- 💾 **Save to File**  
  Export your trip history and future plans to a `.txt` file.

---

## 🏗️ Tech Stack

- **Language:** C++  
- **Data Structures Used:**  
  - Binary Search Tree (BST)  
  - Max Heap  
  - Graph with Dijkstra’s Algorithm  
  - Stack (for history)  
  - Queue (for future trips)  
- **Algorithms:**  
  - Dijkstra’s Shortest Path  
  - Quick Sort (Cost)  
  - Merge Sort (Distance)

---

## 🚀 How to Run

1. Clone the repository or copy the source code into a file:
    ```bash
    g++ -o travelPlanner travelPlanner.cpp
    ./travelPlanner
    ```

2. Follow the menu instructions in the terminal.

---

## 🧪 Sample Use Cases

- Plan a trip from `Paris` to `Tokyo` and see the shortest path.
- Search `Rome` for hotels and attractions.
- View your most recent trips and undo the last one.
- See the top 3 most popular cities to visit.
- Sort all trips to find the cheapest or shortest one.

---

## 📌 Sample Cities Included

- Paris 🇫🇷  
- London 🇬🇧  
- Rome 🇮🇹  
- Tokyo 🇯🇵  
- New York 🇺🇸  

Each city includes:
- ✅ Top Hotels with Ratings and Prices
- ✅ Attractions with Ratings and Types

---

## 🔮 Future Enhancements

- User account creation and login
- Real-time integration with weather and booking APIs
- Visual GUI using Qt or SFML
- Database or JSON file persistence

---

## 📁 File Overview

| File | Description |
|------|-------------|
| `travelPlanner.cpp` | Main application source code |
| `travel_data.txt` | Auto-generated file with saved travel history and future trips |

---

## 📜 License

This project is open-source and available for educational purposes. Attribution appreciated if reused!

---

## 🙌 Acknowledgements

This project was developed as a data structure-based academic project to demonstrate practical usage of advanced concepts like graphs, heaps, and trees in real-world applications.

---

> 💬 For questions or suggestions, feel free to open an issue or submit a pull request.

