#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "Track.hpp"

/**
 *@author Kristina Capistrano 
 *@version 4.4.20
 *Ser321 Principles of Distributed Software Systems 
 */

using namespace std;
  
  /*serialize and deserialize JSON - constructor will initiate a json fil
   *constructor to initialize an object from a Json::Value
   */
void Track::jsonreadwrite(){
 
    ifstream ifs("FMdata.json");
    Json::Reader reader;//read from fm json file 
    Json::Value obj ;
    reader.parse(ifs, obj);

    ofstream _file;
    _file.open("media.json"); //open a json file to store only needed data
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
      music[title]["rank"]=rank;
      music[title]["time"]=time;
      music[title]["genre"]="pop";
      music[title]["typeExt"]="mp3";
      music[title]["summary"] = obj["album"]["wiki"]["summary"].asString();

     
    }//forloop
     Json::StyledWriter _writer;
     _file << _writer.write(music); //write to music.json file 
     _file.close(); //close file 

  };//method jsonreadwrite


/**
 * Get image to use for the music player - will xlarge copy
 */
const char* Track::getImage(){
    ifstream ifs("FMdata.json");
    Json::Reader reader;//read from fm json file 
    Json::Value obj ;
    reader.parse(ifs, obj);
    
    const Json::Value& _img = obj["album"]["image"];
    std::string  str = _img[3]["#text"].asString();
    const char* img = str.c_str();
    cout <<"loading image const char* .... " <<  img << endl;
    return img;
  }
 
  
