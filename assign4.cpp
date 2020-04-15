#include "MediaClientGui.cpp"
#include "MediaLibrary.hpp"
#include "Album.cpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Check_Browser.H>
#include <Fl/Fl_Tree_Item_Array.H>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdlib.h>
#include <sstream>
#include <thread>
#include <json/json.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>

#include <fstream>


using namespace std;

std::string cmd;
void run(){
   system(cmd.c_str());
}

/**
 * Copyright (c) 2020 Tim Lindquist, Kristina Capistrano
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ FLTK client UI for Music management.
 * This class extends the Gui component class MediaClientGui and demonstrates
 * sample control functions that respond to button clicks and menu item selects.
 * This software is meant to run on Linux and MacOS using g++.
 * Students may use this source code as the basis for their solution to
 * the programming projects in Ser321 to create a distributed music browser.
 * <p/>
 * This class  depends on the library curlpp to download a file or make
 * a URL request. 
 *
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @author Kristina Capistrano Kcapist1@asu.edu CIDSE - Software Engineering,
 *                       ASU at the Polytechnic campus
 * @file    assign4.cpp
 * @date    April, 2020
 **/

class MediaClient : public MediaClientGui {

public:
  std::string userId;
  std::string lastfmkey;
   
  std::thread * playThread;
  MediaLibrary * library;

   
/** ClickedX is one of the callbacks for GUI controls.
    * Callbacks need to be static functions. But, static functions
    * cannot directly access instance data. This program uses "userdata"
    * to get around that by passing the instance to the callback
    * function. The callback then accesses whatever GUI control object
    * that it needs for implementing its functionality.
    */
   static void ClickedX(Fl_Widget * w, void * userdata) {
      std::cout << "You clicked Exit" << std::endl;
      exit(1);
   }

   /**
    * Static search button callback method. 
    */
   static void SearchCallbackS(Fl_Widget*w, void*data) {
     
     MediaClient *o = (MediaClient*)data;
     o->buildTree();
      cout << "Search Clicked. You asked for a last.fm search of album: " <<
         o->albSrchInput->value() << " by artist: " <<
         o->artSrchInput->value() << endl;
      try{

	 //------------------editing
	//goal is to allow more than 1 word string album/artist name for search
	std::string url;
	 string _pre = "http://ws.audioscrobbler.com/2.0/?method=album.getinfo&artist=";
	 string _mid ="&album=";
	 string _post ="&format=json&api_key=";
	 string _albumname = o->albSrchInput->value();
	 string _artistname = o->artSrchInput->value();
	 string base  = _albumname;
	 string addstr = "+";
	 for ( int i = 0; i <base.length()-1; i++ ){
	   if ( base[i]==' '){
	     base.replace(i,1,addstr);
	   }

	 }
	 for ( int j = 0; j <_artistname.length()-1; j++ ){
	   if ( _artistname[j]==' '){
	     _artistname.replace(j,1,addstr);
	   }

	 }	 
	 url = _pre + _artistname +_mid + base + _post;
	 //------------------------editing 
         url = url + o->lastfmkey;
         cout << "sending request url: " << url << endl;
         std::ostringstream os;
         curlpp::Easy myRequest;
         myRequest.setOpt(new curlpp::options::WriteStream(&os));
         //curlpp::options::Url myUrl(std::string(url));
         myRequest.setOpt(new curlpp::options::Url(url.c_str()));
         myRequest.perform();
         std::string aString = os.str();
	 Album al(aString); //giving album constructor data from FM 
	 Track xd(aString);
	 
	 //Album xd(aString);
	 // std::ofstream outfile ("FMdata.json");
	 // outfile << aString << std::endl;
         std::cout << aString << std::endl;
	 
      }catch ( curlpp::LogicError & e ) {
         std::cout << e.what() << std::endl;
      }
      catch ( curlpp::RuntimeError & e ) {
         std::cout << e.what() << std::endl;
      }
   }

   // Static menu callback method
   static void TreeCallbackS(Fl_Widget*w, void*data) {
      MediaClient *o = (MediaClient*)data;
      o->TreeCallback(); //call the instance callback method
      
   }

   /**
    * TreeCallback is a callback for tree selections, deselections, expand or
    * collapse.
    */
   void TreeCallback() {
      // Find item that was clicked
      Fl_Tree_Item *item = (Fl_Tree_Item*)tree->item_clicked();
      cout << "Tree callback. Item selected: ";
      if ( item ) {
         cout << item->label();
      } else {
         cout << "none";
      }
      cout << endl;
      std::string aStr("unknown");
      std::string aTitle(item->label());
      switch ( tree->callback_reason() ) {  // reason callback was invoked
      case       FL_TREE_REASON_NONE: {aStr = "none"; break;}
      case     FL_TREE_REASON_OPENED: {aStr = "opened";break;}
      case     FL_TREE_REASON_CLOSED: {aStr = "closed"; break;}
      case   FL_TREE_REASON_SELECTED: {
         aStr = "selected";
	 Track md;
         if(library){
            cout << "trying to get: " << item->label() << endl;
            md = library->get(aTitle);
         }else{
            cout << "library entry not found" << endl;
            break;
         }
         cout << "media: "<< md.title << " " << md.album << " "
              << md.author << " " << md.rank << " " << md.duration
              << endl;
         trackInput->value(md.title.c_str());
         albumInput->value(md.album.c_str());
         authorInput->value(md.author.c_str());
         rankInput->value(md.rank.c_str());
         timeInput->value(md.duration.c_str());
	 

         break;
      }
      case FL_TREE_REASON_DESELECTED: {aStr = "deselected"; break;}
      default: {break;}
      }
      cout << "Callback reason: " << aStr.c_str() << endl;
   }

   // Static menu callback method
   static void Menu_ClickedS(Fl_Widget*w, void*data) {
      MediaClient *o = (MediaClient*)data;
      o->Menu_Clicked(); //call the instance callback method
   }

   // Menu selection instance method that has ccess to instance vars.
   void Menu_Clicked() {

      char picked[80];
      menubar->item_pathname(picked, sizeof(picked)-1);
      string selectPath(picked);
      cout << "Selected Menu Path: " << selectPath << endl;
      int select = genreChoice->value();
      cout << "Selected genre: " << ((select==0)?"rock":"blues") << endl;
      // Handle menu selections
      if(selectPath.compare("File/Save")==0){
         bool restSave = library->toJsonFile("mediacollection.json");
         cout << "Save not implemented" << endl;
      }else if(selectPath.compare("File/Restore")==0){
         cout << "Restore not implemented" << endl;
      }else if(selectPath.compare("File/Tree Refresh")==0){
         buildTree();
      }else if(selectPath.compare("File/Exit")==0){
	if(playThread && playThread->joinable()){
            playThread->join();
         }
         exit(0);
      }else if(selectPath.compare("Track/Add")==0){
     	Fl_Tree_Item *item = (Fl_Tree_Item*)tree->item_clicked();
	cout << "this is " << item->label()<< " found in parent "
	     << item->parent()->label();
         cout << "...track add being implemented" << endl;
      }else if(selectPath.compare("Track/Remove")==0){
         cout << "Remove not implemented" << endl;
      }else if (selectPath.compare("Album/Add")==0){
	cout << "STARTING AddAlbum FUNCTION" << endl;
     	library->initLibraryFromJsonFile("media.json");
	buildTree();
	bool currentlibrary = library->toJsonFile("music.json");
	if ( currentlibrary == true )
	  cout << "ADD ALBUM COMPLETE - ALBUM IN LIBRARY! " << endl;
	else cout<< " ALBUM NOT ADDED " << endl;
	
	
      }else if (selectPath.compare("Album/Remove")==0){//....IMPLEMENTING
	cout<< ".... REMOVE ALBUM FUNCTION START."<<endl;
       vector<string>albumss = library->getAlbumNames();

	
      }else if(selectPath.compare("Track/Play")==0){
         std::string unameres = exec("uname");
         std::string pwdPath = exec("pwd");
         pwdPath = pwdPath.substr(0,pwdPath.length()-1);
         std::cout << "OS type is: " << unameres << " curr.dir is: "
                   << pwdPath << std::endl;
         // This path is only valid on linux so we will have to check ostype
         std::stringstream streamLinux;
         streamLinux << "/usr/bin/vlc "
                     << pwdPath << "/MediaFiles/" << trackInput->value()
                     << ".mp3" ;
         std::string aStr("Linux");
         std::stringstream streamMac;
         streamMac << "/Applications/VLC.app/Contents/MacOS/VLC "
                   << pwdPath << "/MediaFiles/" << trackInput->value()
                   << ".mp3" ;
         cout << "mac command: " << streamMac.str() << endl;
         cout << "linux command: " << streamLinux.str() << endl;
         // start vlc to play the media file
         //limit the comparison to the length of Linux to remove new line char
         if(unameres.compare(0,aStr.length(),aStr)==0){
            string argLinux(streamLinux.str());
            cmd = argLinux;
            playThread = new std::thread(run);
         }else{
            string arg(streamMac.str());
            cmd = arg;
            playThread = new std::thread(run);
         }
      }
   }

   /**
    * a static method to remove spaces, tabs, new lines and returns from the
    * begining or end of a string.
    */
   static std::string& trimMe (std::string& str) {
      // right trim
      while (str.length() > 0 && (str[str.length()-1] == ' '  ||
                                  str[str.length()-1] == '\t' ||
                                  str[str.length()-1] == '\n' ||
                                  str[str.length()-1] == '\r')){
         str.erase(str.length ()-1, 1);
      }
      // left trim
      while (str.length () > 0 && (str[0] == ' ' || str[0] == '\t')){
         str.erase(0, 1);
      }
      return str;
   }

   /**
    * a method to execute a command line command and to return
    * the resulting string.
    */
   std::string exec(const char* cmd) {
      FILE* pipe = popen(cmd, "r");
      if (!pipe) return "ERROR";
      char buffer[128];
      std::string result = "";
      while(!feof(pipe)) {
         if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
      }
      pclose(pipe);
      return result;
   }

   void buildTree(){
      vector<string> result = library->getTitles();
      // cout << "\nserver has titles"<<endl;
      tree->clear();
      for(int i=0; i<result.size(); i++){
	//cout << " " << result[i];
         Track md = library->get(result[i]);
         //cout << md.title << " " << md.album << " " << md.author
         //     << " " << md.rank << endl;
         std::stringstream stream;
         stream << "Music"
                      << "/"
                      << md.album
                      << "/" << md.title;
         tree->add(stream.str().c_str());
      }
      cout << endl;
      tree->redraw();
   }



   MediaClient(const char * name = "kristina", const char * key = "myKey") : MediaClientGui(name) {
      searchButt->callback(SearchCallbackS, (void*)this);
      menubar->callback(Menu_ClickedS, (void*)this);
      tree->callback(TreeCallbackS, (void*)this);
      callback(ClickedX);
      lastfmkey = key;
      userId = "kristina.capistrano";
      library = new MediaLibrary();
      buildTree();
   }
};

int main(int argc, char * argv[]) {
   std::string developer = (argc>1)?argv[1]:"kristina.capistrano";
   std::string lastfmkey = (argc>2)?argv[2]:"lastfmkey";
   std::string windowTitle = developer + "'s Music Browser";
   MediaClient cm(windowTitle.c_str(),lastfmkey.c_str());
   return (Fl::run());
}
