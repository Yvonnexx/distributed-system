#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<string>
#include<map>
#include<stdio.h>
#include<time.h>
#include<algorithm>

using namespace std;

class ListNode;
class GraphNode{
    public:
        int data;   
        int degree;
        ListNode* head; //link to the next node
        ListNode* tail;
        map<int,double> credit;
	GraphNode(int x);
	~GraphNode(){};
	GraphNode();
};

class ListNode{
    public:
        int data;
        ListNode* next;
        ListNode();
	~ListNode(){};
        ListNode(int x);
};

GraphNode::GraphNode(){
    head = NULL;
    tail = head;
    credit[0] = 1.0;
    degree = 0;
}

GraphNode::GraphNode(int x):data(x){
    head = NULL;
    tail = head;
    credit[0] = 1.0;
    degree = 0;
}

ListNode::ListNode(){
    next = NULL;
}

ListNode::ListNode(int x):data(x){
    next = NULL;
}

class Graph{
    public:
        int V;
        GraphNode* vertex_array;
        Graph(int v);
        void addEdge(int src, int dest);
};

Graph::Graph(int v){
    vertex_array = new GraphNode[v];
    for(int i = 0; i < v; i++) {
        vertex_array[i].data = i;
    }
}

void Graph::addEdge(int src, int dest){
    ListNode* lNode = new ListNode(dest);
    ListNode* lNode2 = new ListNode(src);
    (this->vertex_array[src].degree)++; (this->vertex_array[dest].degree)++;
    if(this->vertex_array[src].head == NULL) {
        this->vertex_array[src].head = lNode;
        this->vertex_array[src].tail = lNode;
    } else {
        this->vertex_array[src].tail->next = lNode;
        this->vertex_array[src].tail = this->vertex_array[src].tail->next;
    }
    if(this->vertex_array[dest].head == NULL) {
        this->vertex_array[dest].head = lNode2;
        this->vertex_array[dest].tail = lNode2;
    } else {
        this->vertex_array[dest].tail->next = lNode2;
        this->vertex_array[dest].tail = this->vertex_array[dest].tail->next;
    }
}

int main(int argc, char** argv){
	int id1, id2;
	vector<int> it;
	ifstream infile1(argv[1]);
	clock_t start,end;
	start = clock();
	while(infile1 >> id1 >>id2 ){
		it.push_back(id1);	
		it.push_back(id2);
	}
	int max = *(std::max_element(it.begin(), it.end()));	
    //adding edge to the graph
	int V = max+1;
	Graph* graph = new Graph(V);
	ifstream infile2(argv[1]);
    while(infile2>> id1 >> id2){
        graph->addEdge(id1,id2);
    }
    end = clock();
    cout<<"time to read input file ="<< (double)(end-start)/CLOCKS_PER_SEC<<" sec." <<endl;
    cout<<"finish creating the graph"<<endl;
	//updating credits for all the nodes
	int T = atoi(argv[3]);
	ofstream tempfile;
	tempfile.open("temp.txt");
	for(int t=1;t<=T;t++){
		clock_t  start1, end1;
		start1 = clock();
		for(int v =0; v<V; v++){
			if(graph->vertex_array[v].degree == 0) {
				continue;
			}
			ListNode* ln = graph->vertex_array[v].head;
			while(ln){
				graph->vertex_array[v].credit[t] += graph->vertex_array[ln->data].credit[t-1] / double(graph->vertex_array[ln->data].degree);
				ln = ln->next;
			}
			tempfile<<graph->vertex_array[v].credit[t]<<"\t"<<endl;
		}
		end1 = clock();
		cout<<"round" << t <<"="<< (double)(end1-start1)/CLOCKS_PER_SEC<<" sec," <<endl;
	}
	tempfile.close();
	remove("temp.txt");
	FILE *myfile;
	myfile = fopen(argv[2],"w");
	for(int v=0; v<V; v++){
		if(graph->vertex_array[v].degree > 0){
			fprintf(myfile,"%d\t%d\t",graph->vertex_array[v].data,graph->vertex_array[v].degree);
			for(int t=1;t<=T;t++)
				fprintf(myfile,"%6f\t",graph->vertex_array[v].credit[t]);
			fprintf(myfile,"\n");
		}
	}
	fclose(myfile);
	return 0;
}
