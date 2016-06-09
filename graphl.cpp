/*
 * @file    graphl.cpp
 * @brief   This class represents a graph using an adjacency list. Each node in
 *          the graph is represented by an index in the list. The elements of
 *          the list include a list of nodes to which the indicated node is
 *          adjacent. The graph does not keep track of its size.
 * @author  Brendan Sweeney, SID 1161836
 * @date    February 2, 2012
 */

#include "graphl.h"


/**---------------------- Default Constructor ---------------------------------
 * Creates an empty graph and sets all pointers to NULL.
 * @pre None.
 * @post An empty graph exists. All pointers are NULL.
 */
GraphL::GraphL()
{
    for (int i = 0; i < GRAPHNODELIMIT; ++i)
    {
        adjList[i] = NULL;
    } // end for (int i = 0)
} // end constructor

/**---------------------- Destructor ------------------------------------------
 * Deletes all dynamically allocated memory and sets remaining pointers to
 * NULL.
 * @pre None.
 * @post All memory is freed and any pointers are NULL.
 */
GraphL::~GraphL()
{
    EdgeNode* delPtr;

    for (int i = 1; i < GRAPHNODELIMIT && adjList[i] != NULL; ++i)
    {
        delPtr = adjList[i]->edgeHead;

        while(delPtr != NULL)
        {
            adjList[i]->edgeHead = adjList[i]->edgeHead->nextEdge;
            delete delPtr;
            delPtr = adjList[i]->edgeHead;
        } // end while(delPtr != NULL)
    } // end for (int i = 1)

    delPtr = NULL;
} // end destructor

/**---------------------- buildGraph() ----------------------------------------
 * Constructs a graph from an input file.
 * @param input  The file from which to read the data for all ndoes.
 * @pre The ifstream input can be read.
 * @post This graph represents the graph described in input.
 */
void GraphL::buildGraph(ifstream& input)
{
    int    nodeCount, source, dest;         // containers for validation
    string description;

    input >> nodeCount;         // expect positive int for size

    if (nodeCount > 0 && nodeCount < GRAPHNODELIMIT)    // valid number nodes
    {
        input.get();            // clear end of line

        for (int i = 1; i <= nodeCount; ++i)
        {
            getline(input, description);    // each line, one per node, should
            adjList[i] = new GraphNode();   //  contain description of node
            adjList[i]->data = description;
        } // end for (int i = 1)

        input >> source >> dest;            // get first edge

        while(source != 0)  // check for termination of input
        {
            input.get();    // clear end of line

            if (!insertEdge(source, dest, nodeCount))   // insert failed
            {
                cerr << "ERROR: Could not insert edge (" << source << ", " <<
                        dest << ")" << endl;
            } // end if (!insertEdge(source, dest, cost))

            input >> source >> dest;            // get next edge
        } // end while(source != 0)
    } // end if (nodeCount >= 0)
} // end buildGraph(ifstream&)

/**---------------------- insertEdge() ----------------------------------------
 * Inserts a single edge into this graph.
 * @param source  The starting node.
 * @param dest  The ending ndoe.
 * @param size  The number of nodes in the graph.
 * @pre All input are within range of the adjacency list.
 * @post The specified adge has been inserted.
 * @return true if the input was valid; false, otherwise.
 */
bool GraphL::insertEdge(int source, int dest, int size)
{
    bool success = (source > 0 && source <= size &&     // validate input
                    dest > 0 && dest <= size && source != dest);

    if (success)    // input is within list bounds
    {
        EdgeNode* newPtr = new EdgeNode;        // for pointer redirection
        newPtr->adjGraphNode = dest;            // initialize new node
        newPtr->nextEdge = adjList[source]->edgeHead;
        adjList[source]->edgeHead = newPtr;     // insert at list head
        newPtr = NULL;                          // paranoia
    } // end if (success)

    return success;
} // end insertEdge(int, int, int)

/**---------------------- depthFirstSearch() ----------------------------------
 * Lists the nodes in depth-first-search order.
 * @pre None.
 * @post None.
 */
void GraphL::depthFirstSearch()
{   // initialize graph for search
    for (int i = 1; i < GRAPHNODELIMIT && adjList[i] != NULL; ++i)
    {
        adjList[i]->visited = false;
    } // end for (int i = 1)

    cout << endl << "Depth-first ordering: ";

    for (int v = 1; v < GRAPHNODELIMIT && adjList[v] != NULL; ++v)
    {
        if (!adjList[v]->visited)
        {
            dfs(v);
        } // end if (!adjList[v]->visited)
    } // end for (int v = 1)
    
    cout << endl << endl;
} // end depthFirstSearch()

/**---------------------- dfs() -----------------------------------------------
 * Helper for depthFirstSearch().
 * @param v  The node to start searching from.
 * @pre v is the proper node to search from.
 * @post All nodes from v have been visited.
 */
void GraphL::dfs(int v)
{
    EdgeNode* visit = adjList[v]->edgeHead;
    
    adjList[v]->visited = true;
    cout << v << ' ';
    
    while(visit != NULL)
    {
        if (!adjList[visit->adjGraphNode]->visited)
        {
            dfs(visit->adjGraphNode);
        } // end if (!adjList[visit->adjGraphNode]->visited)

        visit = visit->nextEdge;
    } // end while(visit != NULL)
} // end dfs(int)

/**---------------------- displayGraph() --------------------------------------
 * Prints out the nodes and their edges.
 * @pre None.
 * @post None.
 */
void GraphL::displayGraph() const
{
    EdgeNode* cur;      // for edge list traversal

    cout << endl << "Graph:" << endl;

    for (int i = 1; i < GRAPHNODELIMIT && adjList[i] != NULL; ++i)
    {
        cout << "Node " << setw(4) << i << "        "
             << adjList[i]->data << endl;
        
        cur = adjList[i]->edgeHead;

        while(cur != NULL)
        {
            cout << "  edge " << i << ' ' << cur->adjGraphNode << endl;
            cur = cur->nextEdge;
        } // end while(cur != NULL)
    } // end for (int i = 1)

    cout << endl;
} // end displayGraph()
