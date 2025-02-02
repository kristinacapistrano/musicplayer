#include <string>
#include <cmath>
#include <json/json.h>

using namespace std;

/**
 * Copyright 2020 Tim Lindquist, Kristina Capistrano
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
 * Purpose: Track is a class whose properties describe a single
 * media work -- song or video/clip.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @author Kristina Capistrano kcapist1@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version April 2020
 */
class Track {
protected:
public:

  string title;
  string author;
  string album;
  string rank;
  string duration;

  Track();
  Track(string aTitle, string anAuthor,
	string anAlbum, string rank, string duration);
  Track(const Json::Value& jsonObj);
  Track(string jsonString);
  ~Track();
  string toJsonString();
  Json::Value toJson();
  // void fromJson(Json::Value json);
  void setValues(string aTitle, string anAuthor,
		 string anAlbum, string rank, string duration);
  void print();
};
