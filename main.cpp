#include <bits/stdc++.h>
using namespace std;

// User profile structure
struct UserProfile {
    string username;
    string name;
    string password;
    int age;
    string location;
    unordered_set<string> friends;
    vector<string> interests;
};


unordered_map<string, UserProfile> users; // Data structure to store user profiles

// Interests list
const unordered_map<int, string> interestsList = {
    {1, "Sports"},
    {2, "Music"},
    {3, "Reading"},
    {4, "Cooking"},
    {5, "Traveling"}
    // Add more interests as needed
};
// Function to manage user profiles
void manageUserProfiles();

// Function to calculate similarity score between two users based on interests
double calculateSimilarity(const UserProfile& user1, const UserProfile& user2);

// Function to recommend new friends to a user using collaborative filtering
void recommendFriends(const unordered_map<string, UserProfile>& users, const string& userName);

// Function to manage friendship connections
void manageFriendships();

// Function to find common friends between users using BFS
void findCommonFriendsBFS(const unordered_map<string, UserProfile>& users, const string& username1, const string& username2);

// Function to calculate the shortest path between users using DFS
void calculateShortestPathDFS(const unordered_map<string, UserProfile>& users, const string& username1, const string& username2);

// Function for sorting and searching user profiles
void sortAndSearchProfiles();

// Function for the main menu
void mainMenu();

// Function to load user profiles from file
void loadUserProfiles() {
    ifstream inputFile("user_profiles.txt");

    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            istringstream iss(line);
            string username;
            UserProfile user;

            if (iss >> username >> user.password >> user.name >> user.age >> user.location) {
                user.username = username; // Set the username field
                string interest;
                while (iss >> interest && interest != "FRIENDS:") {
                    user.interests.push_back(interest);
                }

                // After reading interests, the "FRIENDS:" keyword should be present
              /*  string friendsIndicator;
                iss >> friendsIndicator; // Read the next part after interests

                if (friendsIndicator == "FRIENDS:") {*/
                    string friendName;
                    while (iss >> friendName) {
                        user.friends.insert(friendName);
                    }


                users[username] = user;
            }
        }

        inputFile.close();
    }
}



// Function to save user profiles to file
void saveUserProfiles() {
    ofstream outputFile("user_profiles.txt");

    if (outputFile.is_open()) {
        for (const auto& pair : users) {
            const string& username = pair.first;
            const UserProfile& user = pair.second;

            outputFile << username << " " << user.password << " " << user.name << " " << user.age << " " << user.location << " ";

            for (const string& interest : user.interests) {
                outputFile << interest << " ";
            }

            // Check if the user has any friends before writing the "FRIENDS:" line
            if (!user.friends.empty()) {
                outputFile << "FRIENDS:";
                for (const string& friendName : user.friends) {
                    outputFile << " " << friendName;
                }
                outputFile << " "; // Add a space after the last friend to separate "FRIENDS:" from interests
            }

            outputFile << endl;
        }

        outputFile.close();
        cout << "User profiles saved successfully." << endl; // Print a success message
    } else {
        cout << "Error: Unable to open the file for writing." << endl; // Print an error message
    }
}


// Function to manage user profiles
void manageUserProfiles() {


    int choice;

    do {
        cout << "1. Log in" << endl;
        cout << "2. Create new account" << endl;
        cout << "3. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string username;
                string password;

                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                // Check if the user exists and the password is correct
                if (users.find(username) != users.end() && users[username].password == password) {
                    // Proceed to the next step of the program
                    // ...
                    // Here you can perform operations for the logged-in user
                    cout << "Logged in successfully as " << username << "." << endl;
                     mainMenu();
                    return;
                } else {
                    cout << "Invalid username or password. Please try again." << endl;
                }
                break;
            }
            case 2: {
                string username;
                string password;
                string name;
                int age;
                string location;

                cout << "Enter username: ";
                cin >> username;

                // Check if the username is already taken
                if (users.find(username) != users.end()) {
                    cout << "Username already taken. Please choose a different username." << endl;
                    break;
                }

                cout << "Enter password: ";
                cin >> password;
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter age: ";
                cin >> age;
                cout << "Enter location: ";
                cin.ignore();
                getline(cin, location);

                UserProfile newUser;
                newUser.username = username;
                newUser.name = name;
                newUser.password = password;
                newUser.age = age;
                newUser.location = location;

                cout << "Select your interests (enter numbers, separated by spaces):" << endl;
                    cout << "1. Sports" << endl;
                    cout << "2. Music" << endl;
                    cout << "3. Reading" << endl;
                    cout << "4. Cooking" << endl;
                    cout << "5. Traveling" << endl;
                    // Add more interests here

                    int numInterests;
                    cin >> numInterests;

                    for (int i = 0; i < numInterests; i++) {
                        int interestNumber;
                        cin >> interestNumber;

                        // Check if the selected interest number is valid
                        if (interestsList.count(interestNumber) > 0) {
                            newUser.interests.push_back(interestsList.at(interestNumber));
                        }
                    }

                    users[username] = newUser;

                    cout << "Account created successfully for " << username << "." << endl;


                    return;

            }
            case 3:
                cout << "Quitting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

        cout << endl;

    } while (choice != 3);
}


// Function to calculate similarity score between two users based on interests
double calculateSimilarity(const UserProfile& user1, const UserProfile& user2) {
    unordered_set<string> interests1(user1.interests.begin(), user1.interests.end());
    unordered_set<string> interests2(user2.interests.begin(), user2.interests.end());

    int commonInterests = 0;
    for (const string& interest : interests1) {
        if (interests2.count(interest) > 0) {
            commonInterests++;
        }
    }

    double similarity = static_cast<double>(commonInterests) / sqrt(interests1.size() * interests2.size());

    return similarity;
}

// Helper function to trim leading and trailing whitespaces
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}

// Function to recommend new friends to a user using collaborative filtering
void recommendFriends(const string& userName) {
    // Find the user's profile
    auto it = users.find(userName);

    if (it != users.end()) {
        const UserProfile& user = it->second;

        unordered_set<string> existingFriends = user.friends;
        unordered_map<string, double> recommendationScores;

        // Calculate similarity scores with other users
        for (const auto& pair : users) {
            const UserProfile& otherUser = pair.second;

            // Skip the user's own profile and already connected users
            if (trim(otherUser.username) != trim(userName) && existingFriends.count(trim(otherUser.username)) == 0) {
                double similarity = calculateSimilarity(user, otherUser);
                recommendationScores[otherUser.username] = similarity;
            }
        }

        // Sort the recommendation scores in descending order
        vector<pair<string, double>> sortedRecommendations(recommendationScores.begin(), recommendationScores.end());
        sort(sortedRecommendations.begin(), sortedRecommendations.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
            return a.second > b.second;
        });

        // Display recommended friends
        if (!sortedRecommendations.empty()) {
            cout << "Recommended friends for " << userName << ":" << endl;
            for (const pair<string, double>& recommendation : sortedRecommendations) {
                if (recommendation.second > 0) {
                    cout << recommendation.first << " (Similarity: " << recommendation.second << ")" << endl;
                }
            }
        } else {
            cout << "No recommendations found for " << userName << "." << endl;
        }
    } else {
        cout << "User not found." << endl;
    }
}






// Function to manage friendship connections

void manageFriendships() {
    string username1, username2;
    int choice;

    do {
        cout << "1. Add friendship connection" << endl;
        cout << "2. Remove friendship connection" << endl;
        cout << "3. Print friend list" << endl;
        cout << "4. Recommend friends" << endl;
        cout << "5. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter username 1: ";
                cin >> username1;
                cout << "Enter username 2: ";
                cin >> username2;

                if (users.find(username1) != users.end() && users.find(username2) != users.end()) {
                    users[username1].friends.insert(username2);
                    users[username2].friends.insert(username1);

                    // Print a success message
                    cout << "Friendship connection added between " << username1 << " and " << username2 << "." << endl;
                } else {
                    cout << "One or both users not found." << endl;
                }
                break;
            }
            case 2: {
                cout << "Enter username 1: ";
                cin >> username1;
                cout << "Enter username 2: ";
                cin >> username2;

                if (users.find(username1) != users.end() && users.find(username2) != users.end()) {
                    users[username1].friends.erase(username2);
                    users[username2].friends.erase(username1);

                    // Print a success message
                    cout << "Friendship connection removed between " << username1 << " and " << username2 << "." << endl;
                } else {
                    cout << "One or both users not found." << endl;
                }
                break;
            }
            case 3: {
                cout << "Enter username: ";
                cin >> username1;

                if (users.find(username1) != users.end()) {
                    cout << "Friend list for " << username1 << ":" << endl;
                    for (const auto& friendName : users[username1].friends) {
                        cout << friendName << endl;
                    }
                } else {
                    cout << "User not found." << endl;
                }
                break;
            }
            case 4: {
                // Recommend friends
                string userName;
                cout << "Enter user name to recommend friends: ";
                cin >> userName;

                recommendFriends(userName);
                break;
            }
            case 5:
                cout << "Quitting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

        cout << endl;

    } while (choice != 5);
}


// Function to find common friends between users using BFS
void findCommonFriendsBFS( const string& username1, const string& username2) {
    if (users.find(username1) != users.end() && users.find(username2) != users.end()) {
        unordered_set<string> commonFriends;

        // Perform BFS from username1 to find common friends
        unordered_set<string> visited;
        queue<string> q;
        q.push(username1);
        visited.insert(username1);

        while (!q.empty()) {
            string currUser = q.front();
            q.pop();
            if(currUser==username2)break;

            // Check if the current user is a friend of username2
            if (users.at(username2).friends.count(currUser) > 0) {
                commonFriends.insert(currUser);
            }

            // Enqueue unvisited friends
            for (const auto& friendName : users.at(currUser).friends) {
                if (visited.count(friendName) == 0) {
                    q.push(friendName);
                    visited.insert(friendName);
                }
            }
        }

        // Print common friends
        cout << "Common friends between " << username1 << " and " << username2 << ":" << endl;
        if (commonFriends.empty()) {
            cout << "No common friends found." << endl;
        } else {
            for (const auto& friendName : commonFriends) {
                cout << friendName << endl;
            }
        }
    } else {
        cout << "One or both users not found." << endl;
    }
}

// Function to calculate the shortest path between users using DFS
void calculateShortestPathDFS( const string& username1, const string& username2) {
    if (users.find(username1) != users.end() && users.find(username2) != users.end()) {
        unordered_map<string, string> path;
        path[username1] = username1;

        // Perform DFS to find the shortest path
        stack<string> s;
        s.push(username1);

        while (!s.empty()) {
            string currUser = s.top();
            s.pop();

            // Check if the current user is the target user
            if (currUser == username2) {
                break;
            }

            // Explore unvisited friends
            for (const auto& friendName : users.at(currUser).friends) {
                if (path.count(friendName) == 0) {
                    s.push(friendName);
                    path[friendName] = currUser;
                }
            }
        }

        // Reconstruct the shortest path
        if (path.find(username2) != path.end()) {
            cout << "Shortest path from " << username1 << " to " << username2 << ": ";
            string currUser = username2;
            string shortestPath = currUser;

            while (currUser != username1) {
                currUser = path[currUser];
                shortestPath = currUser + " -> " + shortestPath;
            }

            cout << shortestPath << endl;
        } else {
            cout << "No path found between " << username1 << " and " << username2 << "." << endl;
        }
    } else {
        cout << "One or both users not found." << endl;
    }
}

void analyzeNetwork() {

    string username1, username2;
    int choice;

    do {
        cout << "1. Find common friends between users (BFS)" << endl;
        cout << "2. Calculate shortest path between users (DFS)" << endl;
        cout << "3. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter username 1: ";
                cin >> username1;
                cout << "Enter username 2: ";
                cin >> username2;

                findCommonFriendsBFS(username1, username2);
                break;
            }
            case 2: {
                cout << "Enter username 1: ";
                cin >> username1;
                cout << "Enter username 2: ";
                cin >> username2;

                calculateShortestPathDFS(username1, username2);
                break;
            }
            case 3:
                cout << "Quitting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

        cout << endl;

    } while (choice != 3);
}

// Comparator function for sorting user profiles by name
bool compareByName(const UserProfile& u1, const UserProfile& u2) {
    return u1.name < u2.name;
}

// Comparator function for sorting user profiles by age
bool compareByAge(const UserProfile& u1, const UserProfile& u2) {
    return u1.age < u2.age;
}

// Function to perform binary search on sorted user profiles by name
vector<int> binarySearchByName(const vector<UserProfile>& users, const string& name) {
    vector<int> indices;

    int left = 0;
    int right = users.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (users[mid].name == name) {
            indices.push_back(mid);
            // Check for additional matches on both sides
            int leftPtr = mid - 1;
            int rightPtr = mid + 1;

            while (leftPtr >= left && users[leftPtr].name == name) {
                indices.push_back(leftPtr);
                leftPtr--;
            }

            while (rightPtr <= right && users[rightPtr].name == name) {
                indices.push_back(rightPtr);
                rightPtr++;
            }

            break;
        } else if (users[mid].name < name) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return indices;
}

// Function to perform binary search on sorted user profiles by age
vector<int> binarySearchByAge(const vector<UserProfile>& users, int age) {
    vector<int> indices;

    int left = 0;
    int right = users.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (users[mid].age == age) {
            indices.push_back(mid);
            // Check for additional matches on both sides
            int leftPtr = mid - 1;
            int rightPtr = mid + 1;

            while (leftPtr >= left && users[leftPtr].age == age) {
                indices.push_back(leftPtr);
                leftPtr--;
            }

            while (rightPtr <= right && users[rightPtr].age == age) {
                indices.push_back(rightPtr);
                rightPtr++;
            }

            break;
        } else if (users[mid].age < age) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return indices;
}

// Function for sorting and searching user profiles
void sortAndSearchProfiles() {
   vector<UserProfile> sortedUsers;
    sortedUsers.reserve(users.size()); // Reserve memory for efficiency

    // Extract UserProfile objects from the users map using a lambda function
    transform(users.begin(), users.end(), back_inserter(sortedUsers),
              [](const pair<const string, UserProfile>& userPair) { return userPair.second; });

    int choice;

    do {
        cout << "1. Sort user profiles by name" << endl;
        cout << "2. Sort user profiles by age" << endl;
        cout << "3. Search user profile by name" << endl;
        cout << "4. Search user profile by age" << endl;
        cout << "5. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
              // Sort user profiles by name
                sort(sortedUsers.begin(), sortedUsers.end(), compareByName);
                cout << "User profiles sorted by name:" << endl;
                for (const UserProfile& user : sortedUsers) {
                    cout << "Username: " << user.username << ", Name: " << user.name << ", Age: " << user.age << ", Location: " << user.location << endl;
                    cout << "Interests: ";
                    for (const string& interest : user.interests) {
                        cout << interest << " ";
                    }
                    cout << endl;
                    cout << "Friends: ";
                    for (const string& friendName : user.friends) {
                        cout << friendName << " ";
                    }
                    cout << endl << endl;
                }
                break;
            }
            case 2: {
              // Sort user profiles by age
                sort(sortedUsers.begin(), sortedUsers.end(), compareByAge);
                cout << "User profiles sorted by age:" << endl;
                for (const UserProfile& user : sortedUsers) {
                    cout << "Username: " << user.username << ", Name: " << user.name << ", Age: " << user.age << ", Location: " << user.location << endl;
                    cout << "Interests: ";
                    for (const string& interest : user.interests) {
                        cout << interest << " ";
                    }
                    cout << endl;
                    cout << "Friends: ";
                    for (const string& friendName : user.friends) {
                        cout << friendName << " ";
                    }
                    cout << endl << endl;
                }
                break;
            }
            case 3: {
                // Search user profile by name
                string searchName;
                cout << "Enter name to search: ";
                cin >> searchName;

                // Binary search on sorted user profiles by name
                vector<int> indices = binarySearchByName(sortedUsers, searchName);
                if (!indices.empty()) {
                    cout << "User(s) found with name '" << searchName << "':" << endl;
                    for (int index : indices) {
                        cout << "Name: " << sortedUsers[index].name << ", Age: " << sortedUsers[index].age << ", Location: " << sortedUsers[index].location << endl;
                    }
                } else {
                    cout << "User not found." << endl;
                }
                break;
            }
            case 4: {
                // Search user profile by age
                int searchAge;
                cout << "Enter age to search: ";
                cin >> searchAge;

                // Binary search on sorted user profiles by age
                vector<int> indices = binarySearchByAge(sortedUsers, searchAge);
                if (!indices.empty()) {
                    cout << "User(s) found with age '" << searchAge << "':" << endl;
                    for (int index : indices) {
                        cout << "Name: " << sortedUsers[index].name << ", Age: " <<sortedUsers[index].age << ", Location: " << sortedUsers[index].location << endl;
                    }
                } else {
                    cout << "User not found." << endl;
                }
                break;
            }
            case 5:
                cout << "Quitting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

        cout << endl;

    } while (choice != 5);
}


// Function for the main menu
void mainMenu() {
    int choice;

    do {
        cout << "---------------------------" << endl;
        cout << "       SOCIAL NETWORK       " << endl;
        cout << "---------------------------" << endl;
        cout << "1. Manage User Profiles" << endl;
        cout << "2. Manage Friendships" << endl;
        cout << "3. Analyze Network" << endl;
        cout << "4. Sort and Search Profiles" << endl;
        cout << "5. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                manageUserProfiles();
                break;
            case 2:
                manageFriendships();
                break;
            case 3:
                analyzeNetwork();
                break;
            case 4:
                sortAndSearchProfiles();
                break;
            case 5:
                cout << "Quitting..." << endl;
                // Call the saveUserProfiles() function before quitting the program

                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

        cout << endl;

    } while (choice != 5);
}

int main() {

   loadUserProfiles();
    mainMenu();
    saveUserProfiles();

    return 0;
}
