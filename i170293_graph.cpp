#include"i170293_graph.h"
#include"string.h"
#include"string"
int main(){
	
	Graph <string> G(25);
	G.MakeGraph();
	//G.Print();

	/*G.AddCity("London");
	
	G.AddEdge("Lahore","London",60);
	
	G.DropCity("London");
	G.Print();
	G.DeleteEdge("Islamabad","Murree");*/

    //G.SingleSourceShortestPath("Islamabad");
	G.SourceToDestination("Multan","Islamabad");
	/*G.AllToDestination("Hassan Abdal");*/


	return 0;
}