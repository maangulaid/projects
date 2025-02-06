#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <list>
#include <string>
#include <algorithm>
using namespace std;




//flight between cities
//destin = destination
struct Flight {
    string destin; 
    int cost;           // Cost of the flight
    int time;           // Flight duration in minutes
    Flight(string dest, int c, int t) : destin(dest), cost(c), time(t) {}
};

// Class to represent the graph of cities and flights
class FlightGraph 
{
private:
    unordered_map<string, list<Flight>> adjList; // Adjacency list to store cities and their flights
    unordered_map<string, bool> visited;        // Tracks if a city has been visited

public:
    // Adds a flight between two cities (bidirectional)
    void addFlight(const string& origin, const string& destin, int cost, int time)
     {
        adjList[origin].emplace_back(destin, cost, time);
        adjList[destin].emplace_back(origin, cost, time); // Flights are bidirectional
        visited[origin] = false;
        visited[destin] = false;
    }

    // this function block will find all possible flight
    //use  iterative backtracking
    void findAllFlights(const string& origin, const string& destin, bool sortByTime, ofstream& output)
     {
        // Check if either city does not exist in the graph
        if (adjList.find(origin) == adjList.end() || adjList.find(destin) == adjList.end())
         {
            output << "ATTENTION: THERE IS NO POSSIBLE FLIGHT FOUND FOR " << origin << " -> " << destin << ".\n";
            return;
        }

        // Stack to track the curr path and associated cost/time
        // utilizing a stack. The stack will serve to track your progress during the search,
        stack<pair<vector<string>, pair<int, int>>> pathStack;
        vector<pair<vector<string>, pair<int, int>>> validPaths;


        // let Initialize the stack first
        pathStack.push({{origin}, {0, 0}});



        // Iterative backtracking
        while (!pathStack.empty()) 
        {
            //current city =top(stack)
            auto curr = pathStack.top(); 
            pathStack.pop();

            vector<string> currPath = curr.first; 


            // Last city in the current path
            string currCity = currPath.back();    
            int currCost = curr.second.first;    
            int currTime = curr.second.second;    




            // If the destin city is read let store the path
            if (currCity == destin) 
            {
                validPaths.push_back({currPath, {currCost, currTime}});
                continue; 
            }




            // neighboring
            for (const Flight& flight : adjList[currCity]) 
            {
                
                if (find(currPath.begin(), currPath.end(), flight.destin) == currPath.end()) 
                {
                    vector<string> newPath = currPath;        
                    newPath.push_back(flight.destin);       
                    pathStack.push({newPath, {currCost + flight.cost, currTime + flight.time}}); 

                }
            }
        }

        // Handle case where no valid paths were found
        if (validPaths.empty()) {
            output << "ATTENTION: NO FLIGHT PLAN IS FOUND " << origin << " -> " << destin << ".\n";
            return;
        }





        // Sort the paths time or cost
        sort(validPaths.begin(), validPaths.end(), [sortByTime](const auto& a, const auto& b) 
        {
            return sortByTime ? a.second.second < b.second.second : a.second.first < b.second.first;
        });

        // Output the top three paths (or fewer if less exist)
        output << "Flight from " << origin << " to " << destin << " (" << (sortByTime ? "Time" : "Cost") << "):\n";
        for (size_t i = 0; i < validPaths.size() && i < 3; ++i) 
        {
            const auto& path = validPaths[i];
            output << "Path " << (i + 1) << ": ";
            for (size_t j = 0; j < path.first.size(); ++j)
            {
                output << path.first[j];
                if (j != path.first.size() - 1) output << " -> ";
            }
            output << " Time: " << path.second.second << " Cost: " << path.second.first << "\n";
        }
    }
};

int main(int argc, char* argv[]) 
{
    // requested to execute using a command-line arguments
    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " <flight_data_file> <requested_flights_file> <output_file>\n";
        return 1;
    }

    string flightDataFile = argv[1];
    string requestedFlightsFile = argv[2];
    string outputFile = argv[3];

    FlightGraph flightGraph;

    // Open and parse the flight data file
    ifstream flightData(flightDataFile);
    if (!flightData) 
    {
        cerr << "ERROR: THERE IS ISSUE OPENING THE DATA_FILE.\n";
        return 1;
    }

    string line;
    getline(flightData, line);
    int numberOfFlights = stoi(line); 

    
    // Parse flight data (origin, destin, cost, time)
    while (getline(flightData, line)) 
    {
        stringstream ss(line);
        string origin, destin;
        int cost, time;
        getline(ss, origin, '|');
        getline(ss, destin, '|');
        ss >> cost;
        ss.ignore(1, '|');
        ss >> time;
        flightGraph.addFlight(origin, destin, cost, time);
    }
    flightData.close();

    // Open and parse the requested flights file
    //correct error if can not be opened
    ifstream requestedFlights(requestedFlightsFile);
    if (!requestedFlights)
     {
        cerr << "ERROR: THERE IS ISSUE OPENING THE FLIGHT_FILE.\n";
        return 1;
    }

    ofstream output(outputFile);
    if (!output)
     {
        cerr << "ERROR: THERE IS ISSUE OPENING THE OUTPUT FILE.\n";
        return 1;
    }

    getline(requestedFlights, line);

    int numberOfRequests = stoi(line); 

    




    while (getline(requestedFlights, line))
     {
        stringstream ss(line);
        string origin, destin;
        char sortBy;
        getline(ss, origin, '|');
        getline(ss, destin, '|');
        ss >> sortBy;

        bool sortByTime = (sortBy == 'T'); 
        flightGraph.findAllFlights(origin, destin, sortByTime, output);
        output << "\n";
    }


    // Close all file streams
    requestedFlights.close();
    output.close();

    return 0;
}
