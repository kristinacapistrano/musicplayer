#include "MediaLibrary.hpp"
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * Copyright 2020 Tim Lindquist, 
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
 * Purpose: MediaLibrary is a class defining the interface between clients
 * and the server. The server implementation of MediaLibrary
 * provides storage for description of multiple media works -- song or video/clip.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version January 2020
 */

MediaLibrary::MediaLibrary(){
   initLibraryFromJsonFile("media.json");
}

MediaLibrary::~MediaLibrary() {
   //cout << "MediaLibrary destructor.\n";
   media.clear();
}

bool MediaLibrary::initLibraryFromJsonFile(string jsonFileName){
   bool ret = false;
   Json::Reader reader;
   Json::Value root;
   std::ifstream json(jsonFileName.c_str(), std::ifstream::binary);
   bool parseSuccess = reader.parse(json,root,false);
   if(parseSuccess){
      Json::Value::Members mbr = root.getMemberNames();
      for(vector<string>::const_iterator i = mbr.begin(); i!= mbr.end(); i++){
	//cout << *i << " " << endl; returns the title name of each obj
         Json::Value jsonMedia = root[*i];
         Track * aDesc = new Track(jsonMedia);
         media[*i] = *aDesc;
         cout << "adding ";
         aDesc->print();
      }
      ret = true;
   }
   return ret;
}

bool MediaLibrary::toJsonFile(string jsonFileName){
   bool ret = false;
   Json::Value jsonLib;
   for(std::map<string,Track>::iterator i = media.begin();
                                                         i!= media.end(); i++){
      string key = i->first;
      cout << key << " " << endl;
      Track aMedia = media[key];
      Json::Value jsonMedia = aMedia.toJson();
      jsonLib[key] = jsonMedia;
   }
   Json::StyledStreamWriter ssw("  ");
   std::ofstream jsonOutFile(jsonFileName.c_str(), std::ofstream::binary);
   ssw.write(jsonOutFile, jsonLib);
   return true;
}

Track MediaLibrary::get(string aTitle){
   //cout << "get: " << aTitle << endl;
   Track aMedia = media[aTitle];
   return aMedia;
}

vector<string> MediaLibrary::getTitles(){
  //cout << "getTitles: " << endl;
  vector<string> myVec;
  for(map<string,Track>::iterator it = media.begin();
      it != media.end();++it){
    myVec.push_back(it->first);
    //cout << it->first << "\n";
  }
  return myVec;
}

/*
 *This will get all albums  names from the library. 
 *@return vector<string> of album names in the library 
 */
vector<string> MediaLibrary::getAlbumNames(){
  cout << "\n...GETTING ALBUM NAMES " << endl;

  vector<string>tracks = getTitles();  
  vector<string>albums;
  //get the album names from each object 
  for ( int i = 0; i < tracks.size(); i++ ) {
    Track md = get(tracks[i]);
    albums.push_back(md.album);//push the first album to vector
  }
  std::sort(albums.begin(),albums.end());
  albums.erase(std::unique(albums.begin(), albums.end()), albums.end());
  for ( auto i : albums ) cout <<i << endl ;
  cout << "\n...complete getAlbumNames " << endl;
  return albums;
}

void MediaLibrary::removeTrack(string songtitle){
  vector<string>songs = getTitles();
  vector<string>::iterator result = find(songs.begin(),songs.end(),songtitle);
  if (result == songs.end()) cout << "Song cannot be found" << endl;
  else songs.erase(result);
  for (auto i : songs) cout << "updated..." << i << endl;
  
  ofstream _file;
  _file.open("music.json"); //open a json file to store only needed data
  Json::Value music;
  string title;
  for ( int i = 0; i < songs.size(); i++ ) {
    title = songs[i];
    Track md = get(songs[i]);
    music[title]["title"]=songs[i];
    music[title]["author"]=md.author;
    music[title]["album"]=md.album;
    music[title]["rank"]=md.rank;
    music[title]["time"]=md.duration;
   
  }
  Json::StyledWriter _writer;
  _file << _writer.write(music); //write to music.json file 
  _file.close(); //close file 
}

