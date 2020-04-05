#include <iostream>
#include <stdlib.h>
#include <fstream>

//Created file so far that has music.json as name of txt file
//next objective is to read info from the url given  - this has been started
//line above is done
//next objective is to get only nessecary data
using namespace std;

class Track {


public:

  
  /*serialize and deserialize JSON*/
  void jsonreadwrite(){
 
    ifstream ifs("FMdata.json");
    Json::Reader reader;//read from fm json file 
    Json::Value obj ;
    reader.parse(ifs, obj);

    ofstream _file;
    _file.open("music.json"); //create a json file to store only needed data
    Json::Value music;

    string album = obj["album"]["name"].asString();
    string artist = obj["album"]["artist"].asString();
    cout<<"Album: " << album  << endl;
    cout << "Artist: " << artist << endl << endl;
    const Json::Value& track = obj["album"]["tracks"]["track"];

    for (int i = 0; i < track.size(); i++){
      string time  = track[i]["duration"].asString();
      int nduration = std::stoi(time);
      int minute  = nduration / 60;
      int sec = nduration - (minute*60);
      string ssec = to_string(sec);
      if (sec == 6) { minute + 1; sec = 0;}
      if (sec<10){ ssec = "0"+ssec; }
      //cout << "time - " << minute << ":" << sec << endl;
      time = (std::to_string(minute))+":"+ssec;
      string title = track[i]["name"].asString();
      string rank = track[i]["@attr"]["rank"].asString();
      cout << "title: " << title<< " and duration: " << time
	   <<" and rank: " << rank  << endl;
      music[title]["title"]=title;
      music[title]["author"]=artist;
      music[title]["album"]=album;



    }//forloop
     Json::StyledWriter _writer;
     _file << _writer.write(music);
    _file.close();

  }//method 

};//class 
  
