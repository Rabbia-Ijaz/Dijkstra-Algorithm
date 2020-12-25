#include<iostream>
#include<string.h>
#include<string>
#include<cstring>
#include<sstream>
#include<fstream>
using namespace std;
template <class T>
class GraphNode          //Graph Node
{
public:
	string adjacentvertex;
	int weight;
	int distance;
	bool visited;
	GraphNode<T>* next;
	GraphNode()
	{
		adjacentvertex="-";
		weight=0;
		distance=10000;
		visited=false;
		next=NULL;
	}
};
template <class T>
class GraphLinkedList     //Graph Linked List
{
public:
	GraphNode<T>* head;
	int size;

	int numItems;
	GraphLinkedList(int ignored=0)
	{
		head=NULL;
		size=ignored;
		numItems=0;
	}
	void insert(string vertex, int wei)
	{
		if(head==NULL)
		{
			head=new GraphNode<T>();
			head->adjacentvertex=vertex;
			head->weight=wei;
			head->next=NULL;
		}
		else
		{
			GraphNode<T>* temp=head;
			GraphNode<T>* newNode=new GraphNode<T>();
			newNode->adjacentvertex=vertex;
			newNode->weight=wei;
			while(temp->next)
			{
				temp=temp->next;
			}
			temp->next=newNode;
			newNode->next=NULL;
			numItems++;
		}
	}

	bool isEmpty ()
	{
		return(head==NULL);
	}
	void display()
	{
		if(isEmpty())
		{
			return ;
		}	
		GraphNode<T>* temp;
		temp=head;
		while(temp->next)
		{
			cout<<temp->adjacentvertex<< " "<< temp->weight <<" --- ";
			temp=temp->next;
		}
		cout<<temp->adjacentvertex<< " "<< temp->weight;
	}

	void deleteNode(string city)
	{
		
		GraphNode<T>* curr=head;
		GraphNode<T>* prev=NULL;
		int currIndex=1;
		while(curr && curr->adjacentvertex!=city)
		{
			prev=curr;
			curr=curr->next;
			currIndex++;
		}
		if(curr)
		{
			if(prev)
			{
				prev->next=curr->next;
				delete curr;
			}
			else
			{
				head=curr->next;
				delete curr;
			}
		}
	}
	void destroy()
	{
		GraphNode<T>* curr=head;
		GraphNode<T> *nextNode=NULL;
		while(curr!=NULL)
		{
			nextNode=curr->next;
			delete curr;
			curr=nextNode;
		}
	}
	~GraphLinkedList()
	{ 
		destroy();
	}
};

template <class T>
class Graph                //Graph
{
public:
	int maxsize;			//max no of vertices
	int gsize;				//current no of vertices
	GraphLinkedList<T>* AdjList;
	Graph(int s=0)
	{
		maxsize=s;					//max no of vertices
		gsize=0;					//current no of vertices
		AdjList=new GraphLinkedList<T> [maxsize];
		for(int i=0;i<maxsize;++i)
		{
			AdjList[i].head=NULL;
		}
	}
	void MakeGraph()   //adding data from file
	{
		string* cityNames=new string[maxsize];
		ifstream infile;
		infile.open("GraphData.csv");
		if(infile.bad())
		{
			cout<<"can't open file";
			exit(-1);
		}
		else
		{   cout<<"*****File Okay******" <<endl;
			string temp;
			getline(infile,temp,',');
			int i=0;
			for( ;i<maxsize-1; ++i)
			{
				getline(infile,cityNames[i],',');
				AdjList[i].insert(cityNames[i],0);
			}
			getline(infile,cityNames[i],'\n');
			AdjList[i].insert(cityNames[i],0);
			for(int j=0;j<maxsize && infile.good();++j)
			{
				string temp2;
				getline(infile,temp,',');
				int k=0;
				for(;k<maxsize-1 ;++k)
				{
					getline(infile,temp2,','); 
					if(temp2 != "0" )
					{
						AdjList[j].insert(cityNames[k],atoi(temp2.c_str()));
					}  
				}
				getline(infile,temp2,'\n'); 
				if(temp2 != "0")
				{
					AdjList[j].insert(cityNames[k],atoi(temp2.c_str()));
				} 
			}
		}
		infile.close();
		
		Print();

	}
	void AddCity(string cityName)  //Adding city
	{
		maxsize++;
		Graph <T> newGraph(maxsize);

		int i=0;
		for(;i<maxsize-1; ++i)
		{
			newGraph.AdjList[i]=AdjList[i];
		}
		newGraph.AdjList[i].insert(cityName,0);
		AdjList=newGraph.AdjList;
		cout<<"City *"<< cityName<<"* is Added"<<endl;
		
	}
	void AddedgeNode(string city1,string city2,int wei)
	{
		int found_index=-1;
		for(int i=0;i<maxsize;i++)
		{
			if(AdjList[i].head->adjacentvertex==city1)
			{
				found_index=i;
			}
		}
		if(found_index)
		{
			AdjList[found_index].insert(city2,wei);
			
		}
		else
		{
			return;
		}
		//Print();
	}

	void AddEdge(string city1,string city2,int wei) //Adding new Edge
	{
		cout<<"Edge between *"<<city1<<"* and *" <<city2<<"* is Added"<<endl;
		AddedgeNode(city1, city2,wei);
		AddedgeNode(city2, city1,wei);
	}
		
	void DropCity(string name)  //deleting city
	{
		bool found=false;
		int index=-1;
		for(int i=0;i<maxsize;++i){
			if(AdjList[i].head->adjacentvertex==name){
				index=i;
				found=true;
				break;
			}
		}
		if(found)
		{
			AdjList[index].destroy();
			GraphLinkedList<T> *temp=new GraphLinkedList<T> [maxsize-1];
			for(int i=0,j=0;i<maxsize;++i){
				if(i!=index){
					temp[j]=AdjList[i];
					++j;
				}
			}
			AdjList=temp;
			maxsize--;
			for(int i=0;i<maxsize;++i)
			{
				AdjList[i].deleteNode(name);
			}
			cout<<"City *"<<name<<"* is Deleted"<<endl;
		}
		else
		{
			cout<<"City not found"<<endl;
		}
		
	}
	void DeleteEdge(string city1,string city2)  //deleting edge
	{
		bool found=false;
		DeleteEdgeNode(found,city1,city2);
		DeleteEdgeNode(found,city2,city1);
		if(found)
			cout<<"Edge between city *"<<city1<<"* and *"<<city2<<"* is Deleted"<<endl;
	}
	void DeleteEdgeNode(bool & found,string city1,string city2)
	{
		int count=0;
		for(int i=0;i<maxsize;++i)
		{
			if(AdjList[i].head->adjacentvertex==city1)
			{
				AdjList[i].deleteNode(city2);
				
				found=true;
				count++;
			}
		}
		if(count==0)
			cout<<" City 1 *"<<city1<<"* Not Found"<<endl;
	}
	void SingleSourceShortestPath(string city)
	{   
		bool F=false;
		for(int i=0;i<maxsize;++i)
		{
			if(AdjList[i].head->adjacentvertex==city)
			{
				F=true;
				break;
			}
		}
		if(F)
		{
			cout<<endl;
			cout<<"* shortest distance from "<< city <<" to every city*"<<endl;
			bool found=false;
			int index;
			int count=maxsize-1;
			GraphNode<T> *allCities= new GraphNode<T>[maxsize];
			GraphNode<T> *ParentCities= new GraphNode<T>[maxsize];
			string* VisitedCities=new string[maxsize];
			int vIndex=0;
			for(int i=0;i<maxsize;++i)
			{
				allCities[i].adjacentvertex= AdjList[i].head->adjacentvertex;
				allCities[i].distance=10000;
			}
			index=returnindex(city);
			allCities[index].distance=0;
			allCities[index].visited=true;
			ParentCities[index].adjacentvertex="SOURCE";
			VisitedCities[vIndex]=allCities[index].adjacentvertex;
			VisitedCities[vIndex]+= "  ";
			VisitedCities[vIndex]= VisitedCities[vIndex]+ Num2Str(allCities[index].distance);
			VisitedCities[vIndex]+=" --- ";
			VisitedCities[vIndex]+=ParentCities[index].adjacentvertex;
			vIndex++;
			GraphNode<T> *curr=new GraphNode<T>;
			while(count!=0)
			{
				curr=AdjList[index].head->next;
				while(curr!=NULL)
				{
					if(allCities[returnindex(curr->adjacentvertex)].distance > curr->weight+ allCities[index].distance && allCities[returnindex(curr->adjacentvertex)].visited==false) //distance
					{
						allCities[returnindex(curr->adjacentvertex)].distance=curr->weight+ allCities[index].distance;
						ParentCities[returnindex(curr->adjacentvertex)].adjacentvertex=AdjList[index].head->adjacentvertex;
					}
				
					curr=curr->next;
				}
				if(PartofGraph(FindMinimum(allCities)) ==true)
				{
				   index= returnindex(FindMinimum(allCities));
				   VisitedCities[vIndex]=allCities[index].adjacentvertex;
				   VisitedCities[vIndex]+= "  ";
				   VisitedCities[vIndex]= VisitedCities[vIndex]+ Num2Str(allCities[index].distance);
				   VisitedCities[vIndex]+=" --- ";
				   VisitedCities[vIndex]+=ParentCities[index].adjacentvertex;
				   vIndex++;
				}
				else
				{
					cout<<"GRAPH IS DISCONNECTED"<<endl;
					break;
				}
				allCities[index].visited=true;
				count--;
			}
			for(int i=0;i<vIndex;++i)
			{
				cout<<VisitedCities[i]<< endl;
			}
			delete [] allCities;
			delete [] ParentCities;
			delete [] VisitedCities;
		}
		if (F)
		{
			cout<<endl;
			cout<<"*Second shortest distance from "<< city <<" to every city*"<<endl;
			bool found=false;
			int index;
			int count=maxsize-1;
			GraphNode<T> *allCities= new GraphNode<T>[maxsize];
			GraphNode<T> *ParentCities= new GraphNode<T>[maxsize];
			string* VisitedCities=new string[maxsize];
			int vIndex=0;
			for(int i=0;i<maxsize;++i)
			{
				allCities[i].adjacentvertex= AdjList[i].head->adjacentvertex;
				allCities[i].distance=-1;
			}
			index=returnindex(city);
			allCities[index].distance=0;
			allCities[index].visited=true;
			ParentCities[index].adjacentvertex="SOURCE";
			VisitedCities[vIndex]=allCities[index].adjacentvertex;
			VisitedCities[vIndex]+= "  ";
			VisitedCities[vIndex]= VisitedCities[vIndex]+ Num2Str(allCities[index].distance);
			VisitedCities[vIndex]+=" --- ";
			VisitedCities[vIndex]+=ParentCities[index].adjacentvertex;
			vIndex++;
			GraphNode<T> *curr=new GraphNode<T>;
			while(count!=0)
			{
				curr=AdjList[index].head->next;
				while(curr!=NULL)
				{
					if(allCities[returnindex(curr->adjacentvertex)].distance < curr->weight+ allCities[index].distance && allCities[returnindex(curr->adjacentvertex)].visited==false) //distance
					{
						allCities[returnindex(curr->adjacentvertex)].distance=curr->weight+ allCities[index].distance;
						ParentCities[returnindex(curr->adjacentvertex)].adjacentvertex=AdjList[index].head->adjacentvertex;
					}
				
					curr=curr->next;
				}
				if(PartofGraph(FindMaximum(allCities)) ==true)
				{
				   index= returnindex(FindMaximum(allCities));
				   VisitedCities[vIndex]=allCities[index].adjacentvertex;
				   VisitedCities[vIndex]+= "  ";
				   VisitedCities[vIndex]= VisitedCities[vIndex]+ Num2Str(allCities[index].distance);
				   VisitedCities[vIndex]+=" --- ";
				   VisitedCities[vIndex]+=ParentCities[index].adjacentvertex;
				   vIndex++;
				}
				else
				{
					cout<<"GRAPH IS DISCONNECTED"<<endl;
					break;
				}
				allCities[index].visited=true;
				count--;
			}
			for(int i=0;i<vIndex;++i)
			{
				cout<<VisitedCities[i]<< endl;
			}
			delete [] allCities;
			delete [] ParentCities;
			delete [] VisitedCities;
		}

		else
		{
			cout<<"City not found"<<endl;
		}
	}

	void SourceToDestination(string source, string destination)
	{
		bool F=false;
		bool F2=false;
		for(int i=0;i<maxsize;++i)
		{
			if(AdjList[i].head->adjacentvertex==source)
			{
				F=true;
			}
			if(AdjList[i].head->adjacentvertex==destination)
			{
				F2=true;
			}
			if(F && F2)
				break;

		}
		if(F && F2)
		{
			cout<<"*Shortest Path from "<<source<<" to "<<destination<<"*"<<endl;
			if(source==destination)
			{
				cout<< "Source and Destination are same"<<endl;
				return;
			}
			int iteration=0;
			bool found=false;
			int index;
			int count=maxsize-1;
			GraphNode<T> *allCities= new GraphNode<T>[maxsize];
			GraphNode<T> *ParentCities= new GraphNode<T>[maxsize];
			string* SourToDest=new string[maxsize];
			for(int i=0;i<maxsize;++i)
			{
				allCities[i].adjacentvertex= AdjList[i].head->adjacentvertex;
				allCities[i].distance=10000;
			}
			index=returnindex(source);
			allCities[index].distance=0;
			allCities[index].visited=true;
			ParentCities[index].adjacentvertex="SOURCE";
			GraphNode<T> *curr=new GraphNode<T>;
			while(count!=0)
			{
				curr=AdjList[index].head->next;
				while(curr!=NULL)
				{
					if(allCities[returnindex(curr->adjacentvertex)].distance > curr->weight+ allCities[index].distance && allCities[returnindex(curr->adjacentvertex)].visited==false) //distance
					{
						allCities[returnindex(curr->adjacentvertex)].distance=curr->weight+ allCities[index].distance;
						ParentCities[returnindex(curr->adjacentvertex)].adjacentvertex=AdjList[index].head->adjacentvertex;
						if(allCities[returnindex(curr->adjacentvertex)].adjacentvertex==destination)
							found=true;
					}
				
					curr=curr->next;
				}
				if(PartofGraph(FindMinimum(allCities)) ==true)
				{
				   index= returnindex(FindMinimum(allCities));
				}
				else
				{
					cout<<"GRAPH IS DISCONNECTED"<<endl;
					break;
				}
				allCities[index].visited=true;
				count--;
			}
			if(!found)
			{
				cout<<"Source and Destination are not connected"<<endl;
				return;
			}
			index=returnindex(destination);
			int j=0;
			bool foundcity=false;
			for( int i=0;i<maxsize && !foundcity;++i,++j)
			{
				if(allCities[index].adjacentvertex==source)
					foundcity=true;
				SourToDest[i]= allCities[index].adjacentvertex;
				index= returnindex(ParentCities[index].adjacentvertex);
			
			}
			for(j=j-1;j>0;--j)
			{
				cout<<SourToDest[j]<<" ";
				cout<< allCities[returnindex(SourToDest[j])].distance<<" ----- ";
			}
			cout<<SourToDest[0]<<" ";
			cout<< allCities[returnindex(SourToDest[0])].distance<<endl;
		}
		cout<<endl<<endl;
		if(F & F2)
		{
			cout<<"*Second Shortest Path from "<<source<<" to "<<destination<<"*"<<endl;
			if(source==destination)
			{
				cout<< "Source and Destination are same"<<endl;
				return;
			}
			int iteration=0;
			bool found=false;
			int index;
			int count=maxsize-1;
			GraphNode<T> *allCities= new GraphNode<T>[maxsize];
			GraphNode<T> *ParentCities= new GraphNode<T>[maxsize];
			string* SourToDest=new string[maxsize];
			for(int i=0;i<maxsize;++i)
			{
				allCities[i].adjacentvertex= AdjList[i].head->adjacentvertex;
				allCities[i].distance=10000;
			}
			index=returnindex(source);
			allCities[index].distance=0;
			allCities[index].visited=true;
			ParentCities[index].adjacentvertex="SOURCE";
			GraphNode<T> *curr=new GraphNode<T>;
			while(count!=0)
			{
				curr=AdjList[index].head->next;
				while(curr!=NULL)
				{
					if(allCities[returnindex(curr->adjacentvertex)].visited==false) //distance
					{
						allCities[returnindex(curr->adjacentvertex)].distance=curr->weight+ allCities[index].distance;
						ParentCities[returnindex(curr->adjacentvertex)].adjacentvertex=AdjList[index].head->adjacentvertex;
						if(allCities[returnindex(curr->adjacentvertex)].adjacentvertex==destination)
							found=true;
					}
				
					curr=curr->next;
				}
				if(PartofGraph(FindMinimum(allCities)) ==true)
				{
				   index= returnindex(FindMinimum(allCities));
				}
				else
				{
					cout<<"GRAPH IS DISCONNECTED"<<endl;
					break;
				}
				allCities[index].visited=true;
				count--;
			}
			if(!found)
			{
				cout<<"Source and Destination are not connected"<<endl;
				return;
			}
			index=returnindex(destination);
			int j=0;
			bool foundcity=false;
			for( int i=0;i<maxsize && !foundcity;++i,++j)
			{
				if(allCities[index].adjacentvertex==source)
					foundcity=true;
				SourToDest[i]= allCities[index].adjacentvertex;
				index= returnindex(ParentCities[index].adjacentvertex);
			
			}
			for(j=j-1;j>0;--j)
			{
				cout<<SourToDest[j]<<" ";
				cout<< allCities[returnindex(SourToDest[j])].distance<<" ----- ";
			}
			cout<<SourToDest[0]<<" ";
			cout<< allCities[returnindex(SourToDest[0])].distance<<endl;
		}

		else
		{
			cout<<"City not found"<<endl;
		}

	}
	 
	void AllToDestination(string destination)
	{
		bool F=false;
		for(int i=0;i<maxsize;++i)
		{
			if(AdjList[i].head->adjacentvertex==destination)
			{
				F=true;
				break;
			}
		}
		if(F)
		{
			cout<<endl<<endl;
			cout<<"* Every city to "<<destination<<" * "<<endl;
			string source;
			for(int i=0;i<maxsize;++i)
			{
				cout<<"_________________________________________________________________"<<endl;
				source=AdjList[i].head->adjacentvertex;
				SourceToDestination(source,destination);
				cout<<endl;
			}
		}
		else
		{
			cout<<"City not found"<<endl;
		}
	}
	int returnindex(string city)
	{
		for(int i=0;i<maxsize;++i)
		{
			if(AdjList[i].head->adjacentvertex==city)
				return i;
		}
	}
	string Num2Str(int t)
	{
		stringstream s;
		s<<t;
		return s.str();
	}
	string FindMinimum(GraphNode<T> *cityArray)
	{
		int minimum=10000;
		string temp;
		for(int i=0;i<maxsize;++i)
		{
			if(cityArray[i].visited==false && cityArray[i].distance < minimum)
			  {
				 minimum=cityArray[i].distance;
				 temp=cityArray[i].adjacentvertex;
			 }

		}
		return temp;
	}

	string FindMaximum(GraphNode<T> *cityArray)
	{
		int maximum=-1;
		string temp;
		for(int i=0;i<maxsize;++i)
		{
			if(cityArray[i].visited==false && cityArray[i].distance >maximum)
			  {
				 maximum=cityArray[i].distance;
				 temp=cityArray[i].adjacentvertex;
			 }

		}
		return temp;
	}

	string Find2ndMinimum(GraphNode<T> *cityArray)
	{
		int minimum=10000,minimum2=10000;
		string temp;
		for(int i=0;i<maxsize;++i)
		{
			if(cityArray[i].visited==false && cityArray[i].distance < minimum)
			  {
				 minimum=cityArray[i].distance;
				 temp=cityArray[i].adjacentvertex;
			 }

		}

		for(int i=0;i<maxsize;++i)
		{
			if(cityArray[i].visited==false && cityArray[i].distance < minimum2 && cityArray[i].distance > minimum)
			  {
				 minimum2=cityArray[i].distance;
				 temp=cityArray[i].adjacentvertex;
			 }

		}

		return temp;
	}

	void Print()
	{
		for(int i=0;i<maxsize;++i)
		{
			AdjList[i].display();
			cout<<endl;
		}
		cout<<"*********************************"<<endl;
	}
	bool PartofGraph(string city)
	{
		for(int i=0;i<maxsize;++i)
		{
			if(AdjList[i].head->adjacentvertex==city)
				return true;
		}
		return false;
	}
	~Graph()
	{

	}
};
//int main(){
//	
//	Graph <string> G(25);
//	G.MakeGraph();
//	G.Print();
//
//	G.AddCity("London");
//	G.AddEdge("Lahore","Peshawar",60);
//	G.DropCity("Quetta");
//	G.DeleteEdge("Islamabad","Murree");
//
//    G.SingleSourceShortestPath("Multan");
//	G.SourceToDestination("Multan","Islamabad");
//	G.AllToDestination("Hassan Abdal");
//
//
//	return 0;
//}