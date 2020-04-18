#include <iostream>
#include <stdlib.h>
#include <fstream>
using namespace std;

class Album{
public:
  string albumName;
  string author;
  vector<string> tracks;
  string image;
  string runTime;
  string summary;
  Album();
  Album(string albumName,string author,vector<string> tracks,string image,
	string runTime, string summary);
  Album(const Json::Value& jsonObj);
  Json::Value initAlbum(string jsonstring);
};
