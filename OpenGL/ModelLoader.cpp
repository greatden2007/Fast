#include "ModelLoader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <time.h>
using namespace std;

ModelLoader::ModelLoader() {

}

ModelLoader::~ModelLoader() {

}

void ModelLoader::read_from_file(const char *filename) {
	clock_t time_0 = clock();
	int i = 0;
	ifstream inStream(filename);
	string line;

	std::regex rgx1("\\s(\\d+)");
	std::smatch match1;

	regex rgx2(" (.+?)\\s(.+?)\\s(.+?)$");
	smatch match2;

	while (getline(inStream, line)) {

		// faces
		if (line[0] == *"f") {

			flat.clear();
			while (std::regex_search(line, match1, rgx1))
			{
				flat.push_back(::atoi(match1[0].str().c_str()));
				line = match1.suffix().str();
			}
			allFlats.push_back(flat);
		}

		else if (line[0] == *"v" && line[1] == *" ") {
			i = 0;
			while (regex_search(line, match2, rgx2)) {
				vertex.clear();
				vertex.push_back(::atof(match2[1].str().c_str()));
				vertex.push_back(::atof(match2[2].str().c_str()));
				vertex.push_back(::atof(match2[3].str().c_str()));
				line = match2.suffix().str();
				allVertexs.push_back(vertex);
			}
		}
	}
	createArrays();
	cout << "time to open model: " << clock() - time_0 << "ms";
}

void ModelLoader::createArrays() {
	indexes_counter = (int)allFlats.size() * 3;
	indexes = new int[indexes_counter];
	int counter = 0;
	for (int i = 0; i < allFlats.size(); i++) {
		indexes[counter++] = allFlats[i][0]-1;
		indexes[counter++] = allFlats[i][1]-1;
		indexes[counter++] = allFlats[i][2]-1;
	}
	

	vertexs_counter = (int)allVertexs.size() * 3;
	vertexes_out = new float[vertexs_counter];
	counter = 0;
	for (int i = 0; i < allVertexs.size(); i++) {
		vertexes_out[counter++] = allVertexs[i][0];
		vertexes_out[counter++] = allVertexs[i][1];
		vertexes_out[counter++] = allVertexs[i][2];
	}
}