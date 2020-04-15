#include "Album.hpp"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
/**
 * Copyright 2020 Kristina Capistrano
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Purpose: to initiate an album object.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Kristina Capistrano - kcapist1@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version April 2020
 */

Album::Album(){
  string albumName= "";
  string author= "";
  vector<string> tracks={""};
  string image = "" ;
  string runTime = "";
  string summary = "";
}
Album::~Album(){
  albumName= "";
  author= "";
  tracks={""};
  image = "" ;
  runTime = "";
  summary = "";
}
Album::Album(string album, string author, vector<string> tracks, string image,
	     string runtime, string summary) {
  albumName = album;
  author = author;
  tracks = tracks;
  image = image;
  runTime = runtime ;
  summary = summary;
}
/*
 * 
 * @param string from parsed json file of FM 
*/
Album::Album(string jsonString){

  Json::Reader reader;
  Json::Value root;
  reader.parse(jsonString,root);
  albumName = root["album"]["name"].asString();
  author = root["album"]["artist"].asString();
  const Json::Value& _img = root["album"]["image"]; //image
  image = _img[3]["#text"].asString(); //extra large image 
  //  const char* img = str.c_str();  //image 
  cout<<"Album: " << albumName << endl;
  cout <<"arist: " << author << endl;
   
  ofstream _file;
  _file.open("testfile.json"); //open a json file to store only needed data
  Json::Value music;   
  const Json::Value& allsongs = root["album"]["tracks"]["track"];
  cout <<"size of album " << allsongs.size() << endl;
  int tduration;
  for (int i = 0; i < allsongs.size(); i++){
    runTime  = allsongs[i]["duration"].asString();//get duration from file
    int nduration = std::stoi(runTime);

    string song =  allsongs[i]["name"].asString();
    cout << "song being pushed to vector<string>... " << song << endl;
    tracks.push_back(song); //tracks is recording properly
    music[albumName]["tracks"][i] = tracks.at(i);//creating array of songs
    tduration += nduration; //adding time together - working
     
  }//forloop
  for ( auto i : tracks) cout << i << endl;
  music[albumName]["artist"]=author;
  music[albumName]["summary"] = root["album"]["wiki"]["summary"].asString();
  runTime = std::to_string(tduration);
  music[albumName]["run time"] = runTime;
  music[albumName]["image"] = image;
  Json::StyledWriter _writer;
  _file << _writer.write(music); //write to testfile.json file 
  _file.close(); //close file

  
     
}
