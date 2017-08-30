#include <Rcpp.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>

using namespace std;
using namespace Rcpp;

#define USAGE "./format_checker [input file.el]\n"

// [[Rcpp::export]]
int format_checker(string file) {
	/*--- Check for .el extension ---*/
	if (file.c_str() == NULL) {
		cerr << "NULL argument passed\n";
		return -1;
	}
	
	size_t period;
	string fname(file);
	
	period = fname.find_last_of(".");
	if (period == fname.npos) {
		cerr << "ERROR: use a .el file extension\n";
		return -1;
	}

	if (fname.substr(period, fname.npos) != ".el") {
		cerr << "ERROR: use a .el file extension\n";
		return -1;
	}


	ifstream infile;
	map <string, int> edges;
	map <string, int> verts;

	/*----Open the file---*/
	infile.open(file.c_str());
	if (!infile.is_open()) {
		perror(file.c_str());
		return -1;
	}
	
	string line;
	int ln_num = 2, num_verts, num_edges;
	size_t tab;
	
	//process heading line
	getline(infile, line);
	stringstream ss(line);
	ss >> num_verts >> num_edges;


	//parse file by line by line
	while(getline(infile, line)) {
		string L1, L2;
		tab = line.find("\t");
		
		//check for no tab
		if (tab == line.npos) {
			cerr << "ERROR line " << ln_num << ": labels must be tab separated\n";
			infile.close();
			return -1;
		}
		L1 = line.substr(0,tab);
		L2 = line.substr(tab+1,line.npos);
		
		//check for multiple tabs on a line
		if (line.npos != line.find("\t",tab+1)) {
			cerr << "ERROR line " << ln_num << ": labels cannot contain tabs\n";
			infile.close();
			return -1;
		}
		
		//add labels to vertex list
		verts.insert(make_pair(L1, 0));
		verts.insert(make_pair(L2, 0));

		//label redundancy check
		pair<map<string,int>::iterator, bool> ret;
		ret = edges.insert(make_pair(L1+"\t"+L2, ln_num));
		if (!ret.second || !edges.insert(make_pair(L2+"\t"+L1, ln_num)).second) {
			cerr << "ERROR line " << ln_num << ": this edge is a duplicate of line " << ret.first->second << endl;
			infile.close();
			return -1;
		}
		ln_num++;
	}

	infile.close();

	if (ln_num != num_edges+2) {
		cerr << "ERROR: provided & detected number of edges do not match\n";
		return -1;
	}
	if ((int)verts.size() != num_verts) {
		cerr << "ERROR: provided & detected number of vertices do not match\n";
		return -1;
	}

	cout << fname << " is formatted correctly for use with cliqueR\n";
	return 0;
}
