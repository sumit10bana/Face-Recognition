#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cmath>

#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

using namespace std;

class Descriptor
{
public:
	float			xi, yi;		// The location
	vector<double>	fv;			// The feature vector
//	string possibleName;
	Descriptor()
	{
	}

	Descriptor(float x, float y, vector<double> const& f)
	{
		xi = x;
		yi = y;
		fv = f;
	}
	void PrintFV(char *name[], FILE *file1)
	{
		FILE *file;
		file = fopen("data.txt", "a");
		//file1 = fopen("data1.txt","w");
		fprintf(file, "%s\n", name);
		fprintf(file1, "%s\n", name);
		printf("\n%d\n", fv.size());
		
		for(int i = 0; i < fv.size(); i++) {
				fprintf(file, "%f ", fv[i]);
				fprintf(file1, "%f ", fv[i]);
		}
		fprintf(file,"\n");
		fprintf(file1,"\n");
			fclose(file);
		//printf("\n");
	}
	string Recogn()
	{
		string possibleName;
		float val,diff,minval = 100000;
		fstream fs("data.txt",ios::in);
		while(fs.good()) {
			int i = 0;
			float sum = 0;
			string a;
			string b;
			getline(fs, a,'\n');
			getline(fs, b,'\n');
			if(a == "")break;
			stringstream ss;
			ss << b;
			while(ss >> val) {
				diff = (fabs(val - fv[i]));
				sum += diff*diff;
				if(sqrt(sum) > minval) break;
				i++;
			}
			sum = sqrt(sum);
			if(sum < minval) {
				minval = sum;
				possibleName = a;
			}
		}
		fs.close();
	//	cout << possibleName << " "<< minval<<  endl;
		return possibleName;
	}
	string Recogn1()
	{
		string possibleName;
		float val,diff,minval = 100000;
		fstream fs("data1.txt",ios::in);
		while(fs.good()) {
			int i = 0;
			float sum = 0;
			string a;
			string b;
			getline(fs, a,'\n');
			
			getline(fs, b,'\n');
			if(a == "")break;
			//cout << a << endl;
			stringstream ss;
			ss << b;
			while(ss >> val) {
				diff = (fabs(val - fv[i]));
				sum += diff*diff;
				if(sqrt(sum) > minval) break;
				i++;
			}
			sum = sqrt(sum);
			if(sum < minval) {
				minval = sum;
				possibleName = a;
			}
		}
		fs.close();
	//	cout << possibleName << " "<< minval<<  endl;
		return possibleName;
	}
	void PrintFV_offline(int ctr, int index)
	{
		FILE *file;
		file = fopen("offline.txt", "a");
		fprintf(file, "%d\n", ctr);
	//	printf("\n%d\n", fv.size());
		
		for(int i = 0; i < fv.size(); i++) {
				fprintf(file, "%f ", fv[i]);
		}
		fprintf(file,"\n");
			fclose(file);
		//printf("\n");
	}

};

#endif