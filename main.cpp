#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

// ============================================================================
// 1. DOMAIN DATA CORE MODELS
// ============================================================================

struct Flight {
    int flightId;
    string origin;
    string destination;
    double price;
    int capacity;
    int bookedSeats;

    Flight() : flightId(0), origin(""), destination(""), price(0.0), capacity(0), bookedSeats(0) {}
    Flight(int id, string orig, string dest, double prc, int cap) 
        : flightId(id), origin(orig), destination(dest), price(prc), capacity(cap), bookedSeats(0) {}
};

struct Passenger {
    int passengerId;
    string name;
    int bookedFlightId;

    Passenger() : passengerId(0), name(""), bookedFlightId(0) {}
    Passenger(int id, string nm, int fId) : passengerId(id), name(nm), bookedFlightId(fId) {}
};

// ============================================================================
// 2. DATA STRUCTURE IMPLEMENTATIONS
// ============================================================================

// --- LINKED LIST ---
struct LLNode {
    Passenger data;
    LLNode* next;
    LLNode(Passenger p) : data(p), next(nullptr) {}
};

class LinkedList {
public:
    LLNode* head;
    LinkedList() : head(nullptr) {}

    void insert(Passenger p) {
        LLNode* newNode = new LLNode(p);
        if (!head) {
            head = newNode;
            return;
        }
        LLNode* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }

    bool remove(int pId) {
        if (!head) return false;
        if (head->data.passengerId == pId) {
            LLNode* temp = head;
            head = head->next;
            delete temp;
            return true;
        }
        LLNode* current = head;
        while (current->next && current->next->data.passengerId != pId) {
            current = current->next;
        }
        if (current->next) {
            LLNode* temp = current->next;
            current->next = current->next->next;
            delete temp;
            return true;
        }
        return false;
    }

    void display() {
        if (!head) {
            cout << "No records found.\n";
            return;
        }
        LLNode* temp = head;
        while (temp) {
            cout << "ID: " << temp->data.passengerId 
                 << " | Name: " << temp->data.name 
                 << " | Flight ID: " << temp->data.bookedFlightId << "\n";
            temp = temp->next;
        }
    }
};

// --- QUEUE (Waiting List) ---
struct QueueNode {
    Passenger data;
    QueueNode* next;
    QueueNode(Passenger p) : data(p), next(nullptr) {}
};

class Queue {
private:
    QueueNode* front;
    QueueNode* rear;
public:
    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(Passenger p) {
        QueueNode* newNode = new QueueNode(p);
        if (!rear) {
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        rear = newNode;
    }

    Passenger dequeue() {
        if (!front) return Passenger();
        QueueNode* temp = front;
        Passenger p = temp->data;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        return p;
    }

    bool isEmpty() { return front == nullptr; }

    void display() {
        if (isEmpty()) {
            cout << "Waiting list is empty.\n";
            return;
        }
        QueueNode* temp = front;
        while (temp) {
            cout << " [Passenger ID: " << temp->data.passengerId << " | Name: " << temp->data.name << "] -> ";
            temp = temp->next;
        }
        cout << "NULL\n";
    }
};

// --- STACK (Undo Actions Engine) ---
struct Action {
    string type; // "BOOK" or "CANCEL"
    Passenger passenger;
};

struct StackNode {
    Action data;
    StackNode* next;
    StackNode(Action a) : data(a), next(nullptr) {}
};

class Stack {
private:
    StackNode* topNode;
public:
    Stack() : topNode(nullptr) {}

    void push(Action a) {
        StackNode* newNode = new StackNode(a);
        newNode->next = topNode;
        topNode = newNode;
    }

    Action pop() {
        if (!topNode) return Action{"EMPTY", Passenger()};
        StackNode* temp = topNode;
        Action act = temp->data;
        topNode = topNode->next;
        delete temp;
        return act;
    }

    bool isEmpty() { return topNode == nullptr; }
};

// --- BINARY SEARCH TREE (Flight Store) ---
struct BSTNode {
    Flight data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Flight f) : data(f), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* insertHelper(BSTNode* root, Flight f) {
        if (!root) return new BSTNode(f);
        if (f.flightId < root->data.flightId)
            root->left = insertHelper(root->left, f);
        else if (f.flightId > root->data.flightId)
            root->right = insertHelper(root->right, f);
        return root;
    }

    BSTNode* findMin(BSTNode* root) {
        while (root && root->left) root = root->left;
        return root;
    }

    BSTNode* deleteHelper(BSTNode* root, int id) {
        if (!root) return root;
        if (id < root->data.flightId)
            root->left = deleteHelper(root->left, id);
        else if (id > root->data.flightId)
            root->right = deleteHelper(root->right, id);
        else {
            if (!root->left) {
                BSTNode* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                BSTNode* temp = root->left;
                delete root;
                return temp;
            }
            BSTNode* temp = findMin(root->right);
            root->data = temp->data;
            root->right = deleteHelper(root->right, temp->data.flightId);
        }
        return root;
    }

    BSTNode* searchHelper(BSTNode* root, int id) {
        if (!root || root->data.flightId == id) return root;
        if (id < root->data.flightId) return searchHelper(root->left, id);
        return searchHelper(root->right, id);
    }

    void inorderHelper(BSTNode* root) {
        if (!root) return;
        inorderHelper(root->left);
        cout << "ID: " << root->data.flightId 
             << " | Route: " << root->data.origin << " -> " << root->data.destination 
             << " | Price: $" << root->data.price 
             << " | Seats: " << root->data.bookedSeats << "/" << root->data.capacity << "\n";
        inorderHelper(root->right);
    }

    void collectFlights(BSTNode* root, Flight arr[], int& count) {
        if (!root) return;
        collectFlights(root->left, arr, count);
        arr[count++] = root->data;
        collectFlights(root->right, arr, count);
    }

public:
    BSTNode* root;
    BST() : root(nullptr) {}

    void insert(Flight f) { root = insertHelper(root, f); }
    void remove(int id) { root = deleteHelper(root, id); }
    Flight* search(int id) {
        BSTNode* res = searchHelper(root, id);
        return res ? &(res->data) : nullptr;
    }
    void displayAll() { inorderHelper(root); }

    int getCount(BSTNode* node) {
        if (!node) return 0;
        return 1 + getCount(node->left) + getCount(node->right);
    }

    void populateArray(Flight arr[]) {
        int count = 0;
        collectFlights(root, arr, count);
    }
};

// --- GRAPH NETWORK (Routes Map) ---
class Graph {
private:
    static const int MAX_CITIES = 10;
    string cities[MAX_CITIES];
    double adjMatrix[MAX_CITIES][MAX_CITIES];
    int cityCount;

    int getCityIndex(string cityName) {
        for (int i = 0; i < cityCount; i++) {
            if (cities[i] == cityName) return i;
        }
        if (cityCount < MAX_CITIES) {
            cities[cityCount] = cityName;
            return cityCount++;
        }
        return -1;
    }

public:
    Graph() : cityCount(0) {
        for (int i = 0; i < MAX_CITIES; i++) {
            for (int j = 0; j < MAX_CITIES; j++) {
                adjMatrix[i][j] = (i == j) ? 0.0 : 1e9; // infinity setup
            }
        }
    }

    void addRoute(string src, string dest, double price) {
        int u = getCityIndex(src);
        int v = getCityIndex(dest);
        if (u != -1 && v != -1) {
            adjMatrix[u][v] = price;
        }
    }

    void displayNetwork() {
        cout << "\n=== Direct Route Connectivity Network ===\n";
        for (int i = 0; i < cityCount; i++) {
            cout << cities[i] << " directs to: \n";
            for (int j = 0; j < cityCount; j++) {
                if (adjMatrix[i][j] > 0 && adjMatrix[i][j] < 1e9) {
                    cout << "  -> " << cities[j] << " (Est. Base Ticket: $" << adjMatrix[i][j] << ")\n";
                }
            }
        }
    }

    void findCheapestRoute(string src, string dest) {
        int uIdx = -1, vIdx = -1;
        for (int i = 0; i < cityCount; i++) {
            if (cities[i] == src) uIdx = i;
            if (cities[i] == dest) vIdx = i;
        }
        if (uIdx == -1 || vIdx == -1) {
            cout << "Source or Destination city outside current regional flight chart.\n";
            return;
        }

        double dist[MAX_CITIES];
        int parent[MAX_CITIES];
        bool visited[MAX_CITIES];

        for (int i = 0; i < cityCount; i++) {
            dist[i] = 1e9;
            parent[i] = -1;
            visited[i] = false;
        }

        dist[uIdx] = 0;

        for (int count = 0; count < cityCount - 1; count++) {
            double minPrice = 1e9;
            int u = -1;

            for (int v = 0; v < cityCount; v++) {
                if (!visited[v] && dist[v] <= minPrice) {
                    minPrice = dist[v];
                    u = v;
                }
            }

            if (u == -1) break;
            visited[u] = true;

            for (int v = 0; v < cityCount; v++) {
                if (!visited[v] && adjMatrix[u][v] < 1e9 && dist[u] + adjMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v];
                    parent[v] = u;
                }
            }
        }

        if (dist[vIdx] == 1e9) {
            cout << "No transit connection path available between these locations.\n";
            return;
        }

        cout << "Cheapest Transfer Flight Total Route Price: $" << dist[vIdx] << "\nPath Order: ";
        int curr = vIdx;
        string path = "";
        while (curr != -1) {
            path = cities[curr] + (path.empty() ? "" : " -> ") + path;
            curr = parent[curr];
        }
        cout << path << "\n";
    }
};

// ============================================================================
// 3. SELECTION SORT UTILITY ENGINE
// ============================================================================

void sortByPrice(Flight arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].price < arr[minIdx].price) {
                minIdx = j;
            }
        }
        Flight temp = arr[i];
        arr[i] = arr[minIdx];
        arr[minIdx] = temp;
    }
}

// ============================================================================
// 4. PERSISTENCE ENGINE (FILE HANDLING)
// ============================================================================

void saveDatabase(BST& flightTree, LinkedList& activeBookings) {
    ofstream fOut("flights.txt");
    int total = flightTree.getCount(flightTree.root);
    if (total > 0) {
        Flight* arr = new Flight[total];
        flightTree.populateArray(arr);
        for (int i = 0; i < total; i++) {
            fOut << arr[i].flightId << "," << arr[i].origin << "," << arr[i].destination << ","
                 << arr[i].price << "," << arr[i].capacity << "," << arr[i].bookedSeats << "\n";
        }
        delete[] arr;
    }
    fOut.close();

    ofstream bOut("bookings.txt");
    LLNode* temp = activeBookings.head;
    while (temp) {
        bOut << temp->data.passengerId << "," << temp->data.name << "," << temp->data.bookedFlightId << "\n";
        temp = temp->next;
    }
    bOut.close();
}

void loadDatabase(BST& flightTree, LinkedList& activeBookings, Graph& travelGraph) {
    ifstream fIn("flights.txt");
    if (fIn.is_open()) {
        int id, cap, booked;
        string orig, dest;
        double prc;
        string line;
        while (getline(fIn, line)) {
            size_t p1 = line.find(',');
            size_t p2 = line.find(',', p1 + 1);
            size_t p3 = line.find(',', p2 + 1);
            size_t p4 = line.find(',', p3 + 1);
            size_t p5 = line.find(',', p4 + 1);

            if (p1 == string::npos) continue;

            id = stoi(line.substr(0, p1));
            orig = line.substr(p1 + 1, p2 - p1 - 1);
            dest = line.substr(p2 + 1, p3 - p2 - 1);
            prc = stod(line.substr(p3 + 1, p4 - p3 - 1));
            cap = stoi(line.substr(p4 + 1, p5 - p4 - 1));
            booked = stoi(line.substr(p5 + 1));

            Flight fl(id, orig, dest, prc, cap);
            fl.bookedSeats = booked;
            flightTree.insert(fl);
            travelGraph.addRoute(orig, dest, prc);
        }
        fIn.close();
    }

    ifstream bIn("bookings.txt");
    if (bIn.is_open()) {
        int pId, fId;
        string name;
        string line;
        while (getline(bIn, line)) {
            size_t p1 = line.find(',');
            size_t p2 = line.find(',', p1 + 1);
            if (p1 == string::npos) continue;

            pId = stoi(line.substr(0, p1));
            name = line.substr(p1 + 1, p2 - p1 - 1);
            fId = stoi(line.substr(p2 + 1));

            activeBookings.insert(Passenger(pId, name, fId));
        }
        bIn.close();
    }
}

// ============================================================================
// 5. MAIN SYSTEM COORDINATOR & INTERFACE
// ============================================================================

int main() {
    BST flightTree;
    LinkedList activeBookings;
    Graph regionalNetwork;
    Stack undoStack;
    Queue waitingList;

    // Load configurations from disk
    loadDatabase(flightTree, activeBookings, regionalNetwork);

    // Dynamic verification check for fresh runs
    if (!flightTree.search(101)) {
        flightTree.insert(Flight(101, "NewYork", "London", 450.0, 2));
        flightTree.insert(Flight(102, "London", "Dubai", 320.0, 5));
        flightTree.insert(Flight(103, "NewYork", "Dubai", 800.0, 10));
        regionalNetwork.addRoute("NewYork", "London", 450.0);
        regionalNetwork.addRoute("London", "Dubai", 320.0);
        regionalNetwork.addRoute("NewYork", "Dubai", 800.0);
    }

    int choice = 0;
    while (choice != 9) {
        cout << "\n============================================\n";
        cout << "       AIRLINE RESERVATION SYSTEM (DSA)     \n";
        cout << "============================================\n";
        cout << "1. Add New Flight (Admin)\n";
        cout << "2. View Scheduled Flights (Inorder Sorted)\n";
        cout << "3. Sort and Display Flights by Price\n";
        cout << "4. Book Flight Ticket\n";
        cout << "5. Cancel Active Reservation\n";
        cout << "6. View Passenger Booking Ledger\n";
        cout << "7. Compute Cheapest Route Vector (Dijkstra Graph)\n";
        cout << "8. Undo Last Operational State Change\n";
        cout << "9. Safe Terminal Shutdown & Save Data\n";
        cout << "Enter selection index: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            string discard;
            cin >> discard;
            cout << "Invalid terminal parameter type.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int id, cap; string src, dst; double cost;
                cout << "Set Numerical Flight ID: "; cin >> id;
                if (flightTree.search(id)) {
                    cout << "Operation aborted: Flight ID collision registered.\n";
                    break;
                }
                cout << "Departure Airport City: "; cin >> src;
                cout << "Destination Airport City: "; cin >> dst;
                cout << "Base Price Standard Fare ($): "; cin >> cost;
                cout << "Total Seating Array Capacity Limit: "; cin >> cap;

                Flight newFl(id, src, dst, cost, cap);
                flightTree.insert(newFl);
                regionalNetwork.addRoute(src, dst, cost);
                cout << "Flight registered into BST and Graph Matrix successfully.\n";
                break;
            }
            case 2:
                cout << "\n--- Displaying Inorder Traversal Metrics via BST Structure ---\n";
                flightTree.displayAll();
                break;

            case 3: {
                int total = flightTree.getCount(flightTree.root);
                if (total == 0) {
                    cout << "No active configurations.\n";
                    break;
                }
                Flight* localArr = new Flight[total];
                flightTree.populateArray(localArr);
                sortByPrice(localArr, total);
                cout << "\n--- Scheduled Flight List Sorted by Price (Selection Sort) ---\n";
                for (int i = 0; i < total; i++) {
                    cout << "$" << localArr[i].price << " -> Flight " << localArr[i].flightId 
                         << " [" << localArr[i].origin << " to " << localArr[i].destination << "]\n";
                }
                delete[] localArr;
                break;
            }
            case 4: {
                int fId, pId; string pName;
                cout << "Enter Target Flight ID: "; cin >> fId;
                Flight* target = flightTree.search(fId);
                if (!target) {
                    cout << "Flight location footprint not discovered.\n";
                    break;
                }
                cout << "Enter Unique Passenger Reference ID: "; cin >> pId;
                cout << "Passenger Name Identifier (SingleWord): "; cin >> pName;

                Passenger p(pId, pName, fId);
                if (target->bookedSeats < target->capacity) {
                    target->bookedSeats++;
                    activeBookings.insert(p);
                    undoStack.push(Action{"BOOK", p});
                    cout << "Seat confirmation verified. Ledger assigned successfully.\n";
                } else {
                    cout << "Target flight operational capacity reached. Route queued to FIFO Waiting List.\n";
                    waitingList.enqueue(p);
                }
                break;
            }
            case 5: {
                int pId;
                cout << "Enter Passenger ID to process cancellation sequence: "; cin >> pId;
                
                LLNode* temp = activeBookings.head;
                Passenger foundPass;
                bool match = false;
                while (temp) {
                    if (temp->data.passengerId == pId) {
                        foundPass = temp->data;
                        match = true;
                        break;
                    }
                    temp = temp->next;
                }

                if (match && activeBookings.remove(pId)) {
                    Flight* f = flightTree.search(foundPass.bookedFlightId);
                    if (f) f->bookedSeats--;
                    undoStack.push(Action{"CANCEL", foundPass});
                    cout << "Cancellation processed cleanly.\n";

                    if (!waitingList.isEmpty()) {
                        Passenger nextInLine = waitingList.dequeue();
                        if (f && f->bookedSeats < f->capacity) {
                            f->bookedSeats++;
                            activeBookings.insert(nextInLine);
                            cout << "Waiting queue passenger '" << nextInLine.name << "' assigned open slot.\n";
                        }
                    }
                } else {
                    cout << "Passenger index tracking node missing.\n";
                }
                break;
            }
            case 6:
                cout << "\n--- Processing Dynamic Linked List Ledger History ---\n";
                activeBookings.display();
                break;

            case 7: {
                string sCity, dCity;
                cout << "Origin Vector Node: "; cin >> sCity;
                cout << "Destination Target Node: "; cin >> dCity;
                regionalNetwork.findCheapestRoute(sCity, dCity);
                break;
            }
            case 8: {
                if (undoStack.isEmpty()) {
                    cout << "Undo history buffer cleared or empty.\n";
                    break;
                }
                Action lastAct = undoStack.pop();
                if (lastAct.type == "BOOK") {
                    activeBookings.remove(lastAct.passenger.passengerId);
                    Flight* f = flightTree.search(lastAct.passenger.bookedFlightId);
                    if (f) f->bookedSeats--;
                    cout << "Reversed recent booking for Passenger ID: " << lastAct.passenger.passengerId << "\n";
                } else if (lastAct.type == "CANCEL") {
                    Flight* f = flightTree.search(lastAct.passenger.bookedFlightId);
                    if (f && f->bookedSeats < f->capacity) {
                        f->bookedSeats++;
                        activeBookings.insert(lastAct.passenger);
                        cout << "Reversed recent cancellation. Ticket active for ID: " << lastAct.passenger.passengerId << "\n";
                    } else {
                        cout << "Rollback failed: Flight fully booked. Passenger must join waitlist.\n";
                    }
                }
                break;
            }
            case 9:
                saveDatabase(flightTree, activeBookings);
                cout << "System states serialized to flat text files. Safe environment shutdown complete.\n";
                break;
            default:
                cout << "Invalid index code option selection selection.\n";
        }
    }
    return 0;
}