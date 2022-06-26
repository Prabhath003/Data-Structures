#include <iostream>
#include <list>
#include <queue>
#include <cstdlib>
#include <algorithm>
#include <array>
#include <set>

// DECLARING THE NO. OF NODES
#define TOTAL_NODES 100

// CREATING KEY TO STORE THE NODES
struct key
{
    int ID;
    std::list<int> neighbours;
    char status;
};

// CLASS TO RUN THE SIMULATION OF INFECTION
class Node
{
    public:
        int NodeID;
        int timeStamp;
        std::string eventType;

        friend class Priority;
};

// CLASS TO PASS COMPARATOR OF THE PRIORITY QUEUE
class Priority
{
    public:
        int operator()(const Node &n1, const Node &n2)
        {
            return n1.timeStamp > n2.timeStamp;
        }
};

// STRUT TO STORE THE OVERALL STATUS AT AN INSTANT
struct Status
{
    int timestamp;
    std::set<int, std::less<int>> S;
    std::set<int, std::less<int>> R;
    std::set<int, std::less<int>> I;
};

int main()
{
    // RANDOMISING THE RANDOM OUTPUT
    std::srand(time(NULL));

    // CREATING AN ARRAY TO STORE NODES(ADJACENCY LIST FORM)
    std::array<key, TOTAL_NODES> nodes;
    for(unsigned i = 0; i < TOTAL_NODES; i++)
    {
        // ASSIGNING THE ID
        nodes[i].ID = i;
        // FORMING ITS NEIGHBOURS
        for(unsigned j = i + 1; j < TOTAL_NODES; j++)
        {
            // USING RANDOM NUMBER SIMILAR TO TOSSING A COIN
            bool coin = rand() % 2;
            if(coin == true) // TRUE = HEADS
            {
                nodes[i].neighbours.push_back(j);
                nodes[j].neighbours.push_back(i);
            }
        }
    }

    // SETS TO STRORE THE IDS OF SUSCEPTIBLE, INFECTED, RECOVERED PEOPLE
    std::set<int, std::less<int>> susceptible, infected, recovered;
    // INITIALLY ALL ARE SUSCEPTIBLE
    for(unsigned i = 0; i < TOTAL_NODES; i++)
        susceptible.insert(i);

    // CREATING A MIN-HEAP TO RUN THE SIMULATION USING TIMESTAMP AS PARAMETER
    std::priority_queue<Node, std::vector<Node>, Priority> Q;

    // PICKING A RANDOM PERSON
    int infec_starter = rand() % TOTAL_NODES;

    // ADDING HIM TO THE MIN-HEAP
    // TIME OF INFECTION
    Node temp;
    temp.NodeID = infec_starter;
    temp.eventType = "infection";
    temp.timeStamp = 0;
    Q.push(temp);

    // CREATING TIME OF RECOVERY
    int k = 0 + (rand() % 4) + 1;
    temp.NodeID = infec_starter;
    temp.timeStamp = k;
    temp.eventType = "recovery";
    Q.push(temp);

    // CREATING A RECORD TO STORE THE OVERALL STATUS AT EVERY INSANCE
    std::vector<Status> record;

    while(!Q.empty())
    {
        Node e = Q.top();
        Q.pop();

        // IF THE PERSON IS IN RECOVERY STATE
        if(e.eventType == "recovery")
        {
            bool exist;
            int pos;
            auto itr = std::find(infected.begin(), infected.end(), e.NodeID);
            // REMOVING FROM INFECTED SET
            if(itr != infected.end())
            {
                // CHECKING IF THE RECORD ALREADY CONTAINS THE TIMESTAMP
                exist = false;
                pos = 0;
                for(auto ts : record)
                {
                    if(ts.timestamp == e.timeStamp)
                    {
                        exist = true;
                        break;
                    }
                    pos++;
                }
                if(exist == false)
                {
                    Status st;
                    st.timestamp = e.timeStamp;
                    st.I = infected;
                    st.R = recovered;
                    st.S = susceptible;
                    record.push_back(st);
                }

                infected.erase(itr);
            }
            else
            {
                continue;
            }
            // ADDING TO RECOVERED SET 
            recovered.insert(e.NodeID);

            // UPDATING THE SAME THING IN THE RECORD
            itr = std::find(record[pos].I.begin(), record[pos].I.end(), e.NodeID);
            if(itr != record[pos].I.end())
            {
                record[pos].I.erase(itr);
                record[pos].R.insert(e.NodeID);
            }
        }
        // IF THE PERSON IS IN INFECTION STATE
        else if(e.eventType == "infection")
        {
            bool exist;
            int pos;
            // REMOVEING FROM SUSCEPTIBLE SET
            auto itr = std::find(susceptible.begin(), susceptible.end(), e.NodeID);
            if(itr != susceptible.end())
            {
                exist = false;
                pos = 0;
                // CHECKING IF THE RECORD ALREADY CONTAINS THE TIMESTAMP 
                for(auto ts : record)
                {
                    if(ts.timestamp == e.timeStamp)
                    {
                        exist = true;
                        break;
                    }
                    pos++;
                }
                if(exist == false)
                {
                    Status st;
                    st.timestamp = e.timeStamp;
                    st.I = infected;
                    st.R = recovered;
                    st.S = susceptible;
                    record.push_back(st);
                }

                susceptible.erase(itr);
            }
            else
            {
                continue;
            }
            // ADDING TO INFECTED SET
            infected.insert(e.NodeID);

            // UPDATING THE SAME THING IN THE RECORD
            itr = std::find(record[pos].S.begin(), record[pos].S.end(), e.NodeID);
            if(itr != record[pos].S.end())
            {
                record[pos].S.erase(itr);
                record[pos].I.insert(e.NodeID);
            }

            // TRYING TO INFECT THE NEIGHBOURS
            for(auto ID : nodes[e.NodeID].neighbours)
            {
                // CHECKING IF NEIGHBOURS ARE IN SUSCEPTIBLE SET
                auto itr = std::find(susceptible.begin(), susceptible.end(), ID);
                if(itr != susceptible.end())
                {
                    int j = 6;
                    // INFECTING A PERSON BY TOSSING A COIN 5 TIMES UNTIL HEAD COMES
                    for(unsigned i = 1; i <= 5; i++)
                    {
                        bool coin = rand() % 2;

                        if(coin == true)
                        {
                            j = i;
                            break;
                        }
                    }
                    if(j == 6) break;
                    // ADDING TO THE MIN-HEAP WITH THE TIMESTAMP OF INFECTION
                    temp.NodeID = ID;
                    temp.timeStamp = e.timeStamp + j;
                    temp.eventType = "infection";
                    Q.push(temp);

                    // ADDING TO THE MIN-HEAP WITH THE TIMESTAMP OF RECOVERY 
                    int k = e.timeStamp + j + (rand() % 4) + 1;
                    temp.NodeID = ID;
                    temp.timeStamp = k;
                    temp.eventType = "recovery";
                    Q.push(temp);
                }
            }
        }
    }

    // THE NUMBER OF SUSCEPTIBLE, INFECTED, RECOVERS AT GIVEN TIMESTAMP i
    std::cout << "SUSCEPTIBLE" << "\t" << "INFECTED" << "\t" << "RECOVERED" << "\t" << "TIMESTAMP(i)" << std::endl;
    for(auto itr : record)
    {
        std::cout << itr.S.size() << "\t \t" << itr.I.size() << "\t \t" << itr.R.size() << "\t \t" << itr.timestamp << std::endl;
    }

    //------------------------------------------------------------------------------------
    // DOING BFS ON NODE TO CALCULATE THEIR SHORTEST DISTANCE FROM THE INFEC_STARTER
    bool visited[TOTAL_NODES];
    for(int i = 0; i < TOTAL_NODES; i++)
        visited[i] = false;
    // ARRAY TO STORE THE SHORTEST DISTANCE
    int distance[TOTAL_NODES];
    distance[infec_starter] = 0;
    std::queue<int> _queue;

    // INITIALIZING THE START NODE
    visited[infec_starter] = true;
    _queue.push(infec_starter);

    int d = 0;
    // DOING BFS
    while(!_queue.empty())
    {
        int id = _queue.front();
        _queue.pop();
        d++;
        for (auto i = nodes[id].neighbours.begin(); i != nodes[id].neighbours.end(); i++)
        {
            if (!visited[*i])
            {
                visited[*i] = true;
                _queue.push(*i);
                distance[*i] = d;
            }
        }
    }
    //--------------------------------------------------------------------------------------------

    //COMPARING THE INSTANT AT WHICH A NODE GETS INFECTED EITH IT'S SHORTEST DISTANCE FROM THE START NODE INFEC_STARTER
    std::cout << std::endl << "CODE" << "\t" << "INSTANT OF INFECTION" << "\t" << "SHORTEST DISTANCE" << std::endl;
    for(unsigned k = 0; k < TOTAL_NODES; k++)
    {
        // FINDING THE FIRST INSTANT OF INFECTING
        int instant = -1;
        for(int i = 0; i < record.size(); i++)
        {
            for(auto j : record[i].I)
            {
                if(j == k)
                {
                    instant = i;
                    break;
                }
            }
            if(instant != -1)
                break;
        }
        char comparator;
        if(instant > distance[k])
            comparator = '>';
        else if(instant < distance[k])
            comparator = '<';
        else
            comparator = '=';

        std::cout << k << "\t \t" << instant << "\t" << comparator << "\t" << distance[k] << std::endl;
    }

    return 0;
}