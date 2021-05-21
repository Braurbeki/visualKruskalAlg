#include <iostream>
#include "Point.h"
#include<vector>
#include<map>

using namespace std;

class Kruskal
{
private:
	int numV;
	multimap<int, pair<Point, Point>> graph;
	multimap<int, pair<Point, Point>> resTree;
	int *parent;

	void initGraph()
	{
		graph.clear();
		srand(time(NULL));
		numV = 4 + rand() % 6;
		initParent();
		char name = 'A';
		Point point, previousPoint, prePastPoint;
		for (int i = 0; i < numV; i++, name++)
		{
			if (i > 1)
			{
				prePastPoint = previousPoint;
			}
			if (i != 0)
			{
				previousPoint = point;
			}	
			point.name = name;
			point.algName = i + 1;
			int weight = rand() % 12 + 1;

			point.x = 5 + rand() % 760 + weight;
			point.y = 5 + rand() % 700 + weight;

			if (i == 0) { continue; }
			graph.insert(pair<int, pair<Point, Point>>{weight, { previousPoint, point }});
			if (i > 2)
			{
				graph.insert(pair<int, pair<Point, Point>>{weight, { prePastPoint, point }});
			}
			
		}
	}

	void buildTree()
	{
		resTree.clear();
		int firstP = 0, secondP = 0;
		for (auto it = graph.begin(); it != graph.end(); it++)
		{
			if (it->second.first.algName > 0 && it->second.second.algName > 0)
			{
				firstP = find_set(it->second.first.algName);
				secondP = find_set(it->second.second.algName);
				
				if (firstP != secondP)
				{
					union_sets(firstP, secondP);
					resTree.insert(pair<int, pair<Point, Point>>{it->first, { it->second.first, it->second.second }});
				}
			}
			
		}
	}


	void initParent()
	{
		parent = new int[numV + 1];
		
		for (int i = 0; i <= numV; i++)
		{
			parent[i] = i;
		}
	}


	int find_set(int v) {
		if (v == parent[v])
			return v;
		return find_set(parent[v]);
	}

	void union_sets(int a, int b) {
		parent[b] = a;
	}
public:
	Kruskal()
	{
		reload();
	}

	multimap<int, pair<Point, Point>> getGraph()
	{
		return graph;
	}

	void reload()
	{
		initGraph();
		buildTree();
	}

	multimap<int, pair<Point, Point>> getTree()
	{
		return resTree;
	}
};