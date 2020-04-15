#include "Track.hpp"
#include <iostream>
#include <stdlib.h>
#include <fstream>
using namespace std;
/**
 * Copyright 2020 Tim Lindquist and Kristina Capistrano
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
 * Purpose: MediaDescription is a class whose properties describe a single
 * media work -- song or video/clip.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @author Kristina Capistrano kcapist1@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version April 2020
 */
Track::Track(){
   title = "";
   author = "";
   album = "";
   rank = "";
   duration = "";
}

Track::Track(string aTitle, string anAuthor, string anAlbum, string rank, string duration) {
   title = aTitle;
   author = anAuthor;
   album = anAlbum;
   rank = rank;
   duration = duration;
}

Track::Track(const Json::Value& jsonObj){
   Json::Value::Members mbr = jsonObj.getMemberNames();
   for(vector<string>::const_iterator i = mbr.begin(); i!= mbr.end(); i++){
      //cout << *i << " " << endl;
      Json::Value jsonM = jsonObj[*i];
      if(*i=="title"){
         title = jsonM.asString();
      }else if(*i=="author"){
         author = jsonM.asString();
      }else if(*i=="album"){
         album = jsonM.asString();
      }else if(*i=="rank"){
         rank = jsonM.asString();
      }else if(*i=="duration"){
         duration = jsonM.asString();
      }
   }
}

Track::Track(string jsonString){
   cout << "......track(jsonstring) is being used here ..... "<< endl; 

   Json::Reader reader;
   Json::Value root;
   reader.parse(jsonString,root);
   album = root["album"]["name"].asString();
   author = root["album"]["artist"].asString();
   cout<<"Album: " << album  << endl;
   cout <<"arist: " << author << endl;
   
   ofstream _file;
   _file.open("media.json"); //open a json file to store only needed data
   Json::Value music;   
    
   const Json::Value& track = root["album"]["tracks"]["track"];
   for (int i = 0; i < track.size(); i++){
     duration  = track[i]["duration"].asString();
     int nduration = std::stoi(duration);
     int minute  = nduration / 60;
     int sec = nduration - (minute*60);
     string ssec = to_string(sec);
     if (sec == 6) { minute + 1; sec = 0;}
     if (sec<10){ ssec = "0"+ssec; }
     //cout << "time - " << minute << ":" << sec << endl;
     duration = (std::to_string(minute))+":"+ssec;
     title = track[i]["name"].asString();
     rank = track[i]["@attr"]["rank"].asString();
     music[title]["title"]=title;
     music[title]["author"]=author;
     music[title]["album"]=album;
     music[title]["rank"]=rank;
     music[title]["duration"]=duration;
     
    }//forloop
     Json::StyledWriter _writer;
     _file << _writer.write(music); //write to music.json file 
     _file.close(); //close file
     
}

Track::~Track() {
   //cout << "MediaDescription destructor.\n";
   title = "";
   author = "";
   album = "";
   rank = "";
   duration = "";
}

string Track::toJsonString(){
   string ret = "{}";
   Json::Value jsonLib;
   jsonLib["title"] = title;
   jsonLib["author"] = author;
   jsonLib["album"] = album;
   jsonLib["rank"] = rank;
   jsonLib["duration"] = duration;
   ret = jsonLib.toStyledString();
   return ret;
}

Json::Value Track::toJson(){
   //string ret = "{}";
   Json::Value jsonLib;
   jsonLib["title"] = title;
   jsonLib["author"] = author;
   jsonLib["album"] = album;
   jsonLib["rank"] = rank;
   jsonLib["duration"] = duration;
   return jsonLib;
}

void Track::setValues(string aTitle, string anAuthor,string anAlbum, string rank, string duration) {
   title = aTitle;
   author = anAuthor;
   album = anAlbum;
   rank = rank;
   duration = duration;
}

void Track::print(){
   cout << "media " << title << " author " << author << " album " << album 
        << " rank " << rank << " duration " << duration << "\n";
}


