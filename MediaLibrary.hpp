#include "Track.hpp"
#include "Album.hpp"

#include <string>
#include <map>
#include <vector>
#include <json/json.h>

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
 * provides storage for description of  multiple media works -- song or video/clip.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version January 2020
 */
class MediaLibrary {
protected:
   std::map<std::string, Track> media;
   std::map<std::string, Album> mediaAlbum;

public:
  MediaLibrary();
  ~MediaLibrary();

  bool initLibraryFromJsonFile(string jsonFileName);
  bool toJsonFile(string jsonFileName);
  string serviceInfo();
  Track get(string aTitle);
  std::vector<string> getTitles();
  std::vector<string> getAlbumNames();
  void getAlbum(string fmresult);
  void removeTrack(string song);
};
