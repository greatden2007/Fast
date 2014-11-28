#pragma once
#include <vector>

using namespace std;
class ModelLoader
{
	void createArrays();
	vector<float> vertex = vector<float>();
	vector <int> flat = vector<int>();
public:
	ModelLoader();
	~ModelLoader();

	int *indexes;
	int indexes_counter;

	float *vertexes_out;
	int vertexs_counter;

	void read_from_file(const char *filename);
	
	vector<vector<float> > allVertexs = vector<vector<float> >();
	vector<vector<int> > allFlats = vector<vector<int> >();
};