/*
 * @file    graphm.cpp
 * @brief   This class represents a graph using an adjacency matrix. A second
 *          matrix is used to store the shortest path from each node to every
 *          other node. This matrix is populated by invoking findShortestPath()
 *          on a graph. Dijkstra's algorithm is used to determine the shortest
 *          paths.
 * @author  Brendan Sweeney, SID 1161836
 * @date    February 2, 2012
 */

#include <climits>
#include <string>
#include "graphm.h"

using namespace std;


/**---------------------- Default Constructor ---------------------------------
 * Creates an empty graph and initializes matrixes to sane values.
 * @pre None.
 * @post An empty graph exists and none of the matrix nodes contain garbage
 *       values.
 */
GraphM::GraphM() : size(0), pathed(false)
{
    for (int i = 0; i < NODELIMIT; ++i)
    {
        for (int j = 0; j < NODELIMIT; ++j)
        {
            C[i][j] = INT_MAX;          // empty adjacency matrix
            T[i][j].visited = false;    // all nodes unvisited
            T[i][j].dist = INT_MAX;     // min known distance is infinity
            T[i][j].path = 0;           // no known paths
        } // end for (int j = 0)
    } // end for (int i = 0)
} // end Constructor

/**---------------------- buildGraph() ----------------------------------------
 * Builds a graph from data in an ifstream. The input file must be formatted so
 * that the first line contains only the number of nodes. The next lines should
 * contain text descriptions of the nodes, one per line, for as many lines as
 * were specified by the integer on the first line. All remaining lines should
 * contain three integers separated by white space. These represent edges. The
 * first is the starting node, the second the destination node, and the third
 * is the cost of the edge. Input it terminated when an edge line begins with a
 * 0.
 * @param input  The stream from which to read a graph structure. Must be
 *               formatted as described above.
 * @pre The ifstream is readable and contains a valid graph description.
 * @post This graph will represent the data from input.
 */
void GraphM::buildGraph(ifstream& input)
{
    int    nodeCount, source, dest, cost;   // containers for validation
    string description;

    input >> nodeCount;         // expect positive int for size

    if (nodeCount > 0 && nodeCount < NODELIMIT)     // valid number of nodes
    {
        input.get();            // clear end of line
        size = nodeCount;       // number of nodes to expect from file

        for (int i = 1; i <= size; ++i)
        {
            getline(input, description);    // each line, one per node, should
            data[i] = description;          //  contain description of node
            C[i][i] = 0;                    // node distance to self is zero
        } // end for (int i = 1)

        input >> source >> dest >> cost;    // get first edge

        while(source != 0)  // check for termination of input
        {
            input.get();    // clear end of line
            
            if (!insertEdge(source, dest, cost))    // insert failed
            {
                cerr << "ERROR: Could not insert edge (" << source << ", " <<
                        dest << ") with cost of " << cost << endl;
            } // end if (!insertEdge(source, dest, cost))

            input >> source >> dest >> cost;        // get next edge
        } // end while(source != 0)
    } // end if (nodeCount >= 0)
} // end buildGraph(ifstream&)

/**---------------------- insertEdge() ----------------------------------------
 * Inserts a single edge into the graph between two existing nodes.
 * @param source  The node from which to start the edge.
 * @param dest  The adjacent node at which to end the edge.
 * @param cost  The cost of the edge between the nodes.
 * @pre source and dest are within the limits of the adjacency matrix; cost is
 *      positive.
 * @post The edge now exists in this graph. Any shortest paths found are no
 *       longer valid.
 * @return true if the input was valid and the edge could be added; false,
 *         otherwise.
 */
bool GraphM::insertEdge(int source, int dest, int cost)
{
    bool success = (source > 0 && source <= size && cost > 0 &&     // validate
                    dest > 0 && dest <= size && source != dest);    //  input

    if (success)    // input is within matrix bounds
    {
        C[source][dest] = cost;         // update cell with cost
        pathed = false;
    } // end if (success)

    return success;
} // end insertEdge(int, int, int)

/**---------------------- removeEdge() ----------------------------------------
 * Removes a single existing edge from the graph.
 * @param source  The node from which the edge starts.
 * @param dest  The adjacent node at which the edge ends.
 * @pre The specified edge exists within this graph.
 * @post The edge is removed from this graph. Any shortest paths found are no
 *       longer valid.
 * @return true if the input was valid and the edge is now not in the graph;
 *         false, otherwise.
 */
bool GraphM::removeEdge(int source, int dest)
{
    bool success = (source > 0 && source <= size &&         // validate input
                    dest > 0 && dest <=size && source != dest);

    if (success)    // input is within matrix bounds
    {
        C[source][dest] = INT_MAX;      // update cell to infinity
        pathed = false;
    } // end if (success)

    return success;
} // end removeEdge(int, int)

/**---------------------- findShortestPath() ----------------------------------
 * Uses Dijkstra's Algorithm to find the shortes paths from every node to every
 * other node. A path matrix is used to store path descriptions.
 * @pre The graph is not empty.
 * @post All shortest paths are represented in the path matrix. A flag is set
 *       to indicate the matrix is valid.
 */
void GraphM::findShortestPath(void)
{
    int numVisits, v;

    if (!pathed)
    {
        for (int i = 1; i <= size; ++i)
        {
            for (int j = 1; j <= size; ++j)
            {
                T[i][j].dist = INT_MAX;
                T[i][j].path = 0;
                T[i][j].visited = false;
            } // end for (int j = 1)
        } // end for (int i = 1)

        for (int source = 1; source <= size; ++source)
        {
            numVisits = 0;
            T[source][source].dist = 0;

            while(numVisits < size)
            {
                v = findV(source);
                T[source][v].visited = true;
                ++numVisits;
                setW(source, v);
            } // end while(numVisits < size)
        } // end for (int source = 1)
    } // end if (!pathed)

    pathed = true;
} // end findShortestPath()

/**---------------------- findV() ---------------------------------------------
 * Finds a vector to visit for the shortest path routine.
 * @param source  The node from which to search for an adjacent node to visit.
 * @pre This graph is not empty.
 * @post None.
 * @return The index of the node that should be visited next.
 */
int GraphM::findV(int source)
{
    int v = 0;

    for (int i = 1; i <= size; ++i)
    {
        if (!T[source][i].visited)
        {
            if (T[source][i].dist < T[source][v].dist)
            {
                v = i;
            } // end if (T[source][i].dist < T[source][v].dist)
        } // end if (!T[source][i].visited...)
    } // end for (int i = 1)

    return v;
} // end findV(int, int&)

/**---------------------- setW() --- ------------------------------------------
 * Sets the current shortest path information on all nodes adjacent to the
 * visited node.
 * @param source  The node from which a path is being determined.
 * @pre v has been found and is the correct node to visit.
 * @post The path matrix is updated with the shortest distance currently known
 *       for all nodes adjacent to v.
 */
void GraphM::setW(int source, int v)
{
    for (int w = 1; w <= size; ++w)
    {
        if (!T[source][w].visited && C[v][w] < INT_MAX &&
             T[source][v].dist < INT_MAX)
        {
            if (T[source][w].dist > (T[source][v].dist + C[v][w]))
            {
                T[source][w].dist = T[source][v].dist + C[v][w];
                T[source][w].path = v;
            } // end if (T[source][w].dist > (T[source][v].dist + C[v][w]))
        } // end if (!T[source][w].visited...)
    } // end for (int w = 1)
} // end setW(int, int, int)

/**---------------------- displayAll() ----------------------------------------
 * Prints out a list of all nodes and their adjacencies. Requires that shortest
 * paths have been found. If this is not the case, findShortestPath() is
 * invoked.
 * @pre The graph is not empty.
 * @post If paths were not valid, they have been updated.
 */
void GraphM::displayAll(void)
{
    if (!pathed)
    {
        findShortestPath();
    } // end if (!pathed)

    cout.width(26);
    cout << left << "Description";
    cout.width(11);
    cout << "From node";
    cout.width(9);
    cout << "To node";
    cout.width(12);
    cout << "Dijkstra's";
    cout.width();
    cout << "Path" << endl;

    for (int source = 1; source <= size; ++source)
    {
        displayFrom(source);
    } // end for (int i = 1)

    cout << endl;
} // end displayAll()

/**---------------------- displayFrom() ---------------------------------------
 * Displays all the nodes that have paths from a specified node. Requires that
 * shortest paths have been found.
 * @param sourse  The node from which to display paths.
 * @pre The graph is not empty. The path matrix is valid.
 * @post None.
 */
void GraphM::displayFrom(int source)
{
    cout.width(32);
    cout << left << data[source] << endl;

    for (int dest = 1; dest <= size; ++dest)
    {
        if (dest != source)
        {
            cout.width(35);
            cout << right << source;
            cout.width(5);
            cout << dest;
            cout.width(14);

            if (T[source][dest].dist == INT_MAX)
            {
                cout << "----" << endl;
            }
            else
            {
                cout << T[source][dest].dist;
                cout.width();
                cout << "    ";
                displayPath(source, dest);
                cout << dest << endl;
            } // end if (T[source][dest].dist == INT_MAX)
        } // end if (dest != source)
    } // end for (int dest = 1)
} // end displayFrom(int)

/**---------------------- displayPath() ---------------------------------------
 * Displays the full path and distance between two nodes. Requires that
 * shortest paths have been found.
 * @param source  The node from which to start the path.
 * @param dest  The node at which to end the path.
 * @pre This graph is not empty.
 * @post None.
 */
void GraphM::displayPath(int source, int dest)
{
    if (!pathed)
    {
        findShortestPath();
    } // end if (!pathed)

    if (T[source][dest].path != 0)
    {
        displayPath(source, T[source][dest].path);
        cout << T[source][dest].path << ' ';
    } // end if (T[source][dest].path != 0)
} // end displayPath(int, int)

/**---------------------- display() 0------------------------------------------
 * Displays the full path and distance between two specified nodes. Requires
 * that shortest paths have been found.
 * @param source  The node from which to start the path.
 * @param dest  The node at which to end the path.
 * @pre This graph is not empty.
 * @post If paths were not valid, they have been updated.
 */
void GraphM::display(int source, int dest)
{
    if (!pathed)
    {
        if (T[source][dest].dist < INT_MAX)
        {
            cout.width(4);
            cout << right << source;
            cout.width(8);
            cout << dest;
            cout.width(8);
            cout << T[source][dest].dist;
            cout << "        ";
            displayPath(source, dest);
            cout << source << endl << data[dest] << endl;
            pathDesc(source, dest);
        }
        else
        {
            cout << "No path from " << source << " to " << dest << '.' << endl;
        } // end if (T[source][dest].dist < INT_MAX)
    } // end if (!pathed)

    cout << endl;
} // end display(int, int)

/**---------------------- pathDesc() ------------------------------------------
 * Displays the description of a path.
 * @param source  The node from which to display a path description.
 * @param dest  The node at which to end the path.
 * @pre Shortest paths have been found.
 * @post None.
 */
void GraphM::pathDesc(int source, int dest)
{
    if (T[source][dest].path != 0)
    {
        pathDesc(source, T[source][dest].path);
        cout << data[dest] << endl;
    } // end if (T[source][dest].path != 0)
}
