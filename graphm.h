/*
 * @file    graphm.h
 * @brief   This class represents a graph using an adjacency matrix. A second
 *          matrix is used to store the shortest path from each node to every
 *          other node. This matrix is populated by invoking findShortestPath()
 *          on a graph. Dijkstra's algorithm is used to determine the shortest
 *          paths.
 * @author  Brendan Sweeney, SID 1161836
 * @date    February 2, 2012
 */

#ifndef _GRAPHM_H
#define	_GRAPHM_H

#include <cstdlib>
#include "nodedata.h"

using namespace std;
const int NODELIMIT = 101;


class GraphM
{
public:

    GraphM();
    
    void buildGraph(ifstream& input);
    
    bool insertEdge(int source, int dest, int cost);
    
    bool removeEdge(int source, int dest);
    
    void findShortestPath(void);
    
    void displayAll(void);
    
    void display(int source, int dest);

private:

    struct TableType
    {
        bool visited;       // whether node has been visited
        int  dist;          // shortest distance from source known so far
        int  path;          // previous node in path of min dist
    }; // end struct TableType

    NodeData data[NODELIMIT];               // data for graph nodes information
    int         C[NODELIMIT][NODELIMIT];    // Cost array, the adjacency matrix
    int      size;                          // number of nodes in the graph
    TableType   T[NODELIMIT][NODELIMIT];    // stores visited, distance, path
    bool   pathed;                          // current shortest paths valid

    int findV(int source);

    void setW(int source, int v);

    void displayFrom(int source);

    void displayPath(int source, int dest);

    void pathDesc(int source, int dest);

}; // end class GraphM

#endif	/* _GRAPHM_H */
