#include <iostream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>

using namespace std;

class Vertex {
public:
	int ID;
	vector<int> adjacencyList;
	int inDegree;
	int outDegree;

	Vertex(int ID, int outDegree) {
		this->ID = ID;
		this->outDegree = outDegree;
		this->inDegree = 0;
	}

};

vector<int> merge(vector<int> V1, vector<int> V2) {
	bool merge = true;
	vector<int> result;
	for(int i = 0 ; i < V1.size(); i++) {
		result.push_back(V1[i]);
		if(V1[i] == V2.front() && merge) {
			for(int j = 1; j < V2.size(); j++) {
				result.push_back(V2[j]);
			}
			merge = false;
		}
	}
	return result;
}

int main(int argc, char const *argv[]) {

	string infile_name = argv[1];
	string outfile_name = argv[2];

	ifstream infile;
	infile.open(infile_name);

	ofstream outfile;
	outfile.open(outfile_name);

	int numOfVertices;
	infile >> numOfVertices;

	vector<Vertex*> graph;
	int numOfEdges = 0;


	while(numOfVertices != 0) {

		int vertexID, outDegree;
		infile >> vertexID >> outDegree;

		Vertex* vPtr = new Vertex(vertexID, outDegree);
		graph.push_back(vPtr);	
		numOfEdges += outDegree;

		while(outDegree != 0) {

			int adjacencyID;
			infile >> adjacencyID;

			vPtr->adjacencyList.push_back(adjacencyID);

			outDegree--;	
		}
		numOfVertices--;
	}

	for(int i = 0; i < graph.size(); i++) {
		for(int j = 0; j < graph[i]->adjacencyList.size(); j++) {
			graph[(graph[i]->adjacencyList[j])]->inDegree++;
			if(graph[(graph[i]->adjacencyList[j])]->inDegree > graph[(graph[i]->adjacencyList[j])]->outDegree) {	
				outfile << "no path";
				return 0;
			}
		}
	}

	vector<int> eulerianCircuit;

	int startingVertex;
	infile >> startingVertex;

	eulerianCircuit.push_back(startingVertex);
	while(graph[startingVertex]->outDegree > 0) {
		int nextVertex = graph[startingVertex]->adjacencyList.front();
		eulerianCircuit.push_back(nextVertex);
		graph[startingVertex]->adjacencyList.erase(graph[startingVertex]->adjacencyList.begin());
		graph[startingVertex]->outDegree--;
		startingVertex = nextVertex;
	}

	while(eulerianCircuit.size() <= numOfEdges) {

		vector<int> tour;

		for(int i : eulerianCircuit) {
			if(graph[i]->outDegree > 0) {
				startingVertex = i;
				break;
			}
		}

		tour.push_back(startingVertex);
		while(graph[startingVertex]->outDegree > 0) {
			int nextVertex = graph[startingVertex]->adjacencyList.front();
			tour.push_back(nextVertex);
			graph[startingVertex]->adjacencyList.erase(graph[startingVertex]->adjacencyList.begin());
			graph[startingVertex]->outDegree--;
			startingVertex = nextVertex;			
		}

		eulerianCircuit = merge(eulerianCircuit, tour);
	}

	for(int i : eulerianCircuit) {
		outfile << i << " ";
	}

	infile.close();
	outfile.close();

	return 0;
}