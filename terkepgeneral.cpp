#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

const int XX=400;
const int YY=300;

int main() {
	int X=XX;
	int Y=YY;
	ofstream f("terkep.txt");
	vector<vector<int> > t(X,vector<int>(Y,0));
	int K=7;
	for (int l=0;l<K;l++) {
		for (int i=0;i<t.size();i++) {
			for (int j=0;j<t[i].size();j++){
				t[i][j]+=rand()%(2<<(K-l))-(2<<(K+1-l));
			}
		}
		for (int k=0;k<K-l;k++)
		for (int i=0;i<t.size();i++) {
			for (int j=0;j<t[i].size();j++) {
				t[i][j]=(4*t[i][j]+t[i][(j+Y-1)%Y]+t[i][(j+1)%Y]+t[(i+X-1)%X][j]+t[(i+1)%X][j])/8;
			}
		}
	}
/*
	for (int i=0;i<t.size();i++) {
		t[i][0]=0;		
		t[i][Y-1]=0;
	}
	for (int j=0;j<t[0].size();j++) {
		t[0][j]=0;
		t[X-1][j]=0;
	}
	*/
	int min, max;
	min=max=t[0][0];
	for (int i=0;i<t.size();i++) {
		for (int j=0;j<t[i].size();j++) {
			if (t[i][j]<min) min=t[i][j];
			if (t[i][j]>max) max=t[i][j];
		}
	}
	for (int i=0;i<t.size();i++) {
		for (int j=0;j<t[i].size();j++) {
			t[i][j]=(t[i][j]-min)/float(max-min)*250-125;
		}
	}
	
	
	f << X << " " << Y << endl;
	for (int i=0;i<t.size();i++) {
		for (int j=0;j<t[i].size();j++) {
			f << t[i][j] << " ";
		}
	
		f << endl;
	}
}
