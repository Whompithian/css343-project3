/*
 * @file
 * @brief   This class represents a graph using an adjacency list. Each node in
 *          the graph is represented by an index in the list. The elements of
 *          the list include a list of nodes to which the indicated node is
 *          adjacent. The graph does not keep track of its size.
 * @author  Brendan Sweeney, SID 1161836
 * @date    February 2, 2012
 */

#ifndef _GRAPHL_H
#define	_GRAPHL_H

#include <cstdlib>
#include <iomanip>
#include "nodedata.h"

using namespace std;
const int GRAPHNODELIMIT = 101;


struct EdgeNode;                // forward reference for the compiler
struct GraphNode
{
    EdgeNode* edgeHead;         // head of the list of edges
    NodeData  data;             // information about the node
    bool      visited;          // used during recursive searches
}; // end GraphNode

struct EdgeNode
{
    int       adjGraphNode;     // subscript of the adjacent graph node
    EdgeNode* nextEdge;         // next edge in list, if it exists
}; // end EdgeNode

class GraphL
{
public:

    GraphL();

    virtual ~GraphL();
    
    void buildGraph(ifstream& input);

    void depthFirstSearch(void);

    void displayGraph(void) const;

private:

    GraphNode* adjList[GRAPHNODELIMIT];     // adjacency list of nodes

    bool insertEdge(int source, int dest, int size);

    void dfs(int v);

}; // end GraphL

#endif	/* _GRAPHL_H */
