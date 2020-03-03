package assign2;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.LinkedHashMap;
import java.util.Map;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.jayway.jsonpath.JsonPath;


public class writefile {
	public static String inputline;
	public static JSONObject myResponse;

	public writefile(String url) throws IOException, JSONException {
		URL obj = new URL(url);
		HttpURLConnection con = (HttpURLConnection)obj.openConnection();
		BufferedReader in = new BufferedReader (
				new InputStreamReader(con.getInputStream()));
		StringBuffer response = new StringBuffer();
		while((inputline = in.readLine()) != null){
			response.append(inputline);
		}
		in.close();
		try {
			myResponse = new JSONObject(response.toString());
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		  try (FileWriter file = new FileWriter("music.json")) {
	        	 
	            file.write(myResponse.toString(4));// tostring will format properly, the param will give it space/indents
	            file.flush();
	            
	        } catch (IOException e) {
	            e.printStackTrace();
	        }
		
	}
	public String getalbumname() throws JSONException {
		
		JSONObject getalbum = new JSONObject(myResponse.getJSONObject("album").toString());
		String albumname = getalbum.getString("name");
		return albumname;
		
	}
	
	public String getartistname() throws JSONException {
		JSONObject getalbum = new JSONObject(myResponse.getJSONObject("album").toString());	
		String artist = getalbum.getString("artist");
		return artist;

	}
	
	
	/**
	 * get in the nested json file 2nd layer to get information needed 
	 * @param x the index wanted from the track array
	 * @param data has to match the key name trying to get from the 2nd layer
	 * @return piece of data wanted
	 * @throws JSONException if json query did not work
	 */
	public String gettrackinfo(int x,  String data) throws JSONException {
		JSONObject obj = new JSONObject(myResponse.getJSONObject("album").toString());	
		String prepath = "$.tracks.track[";
		String postpath = "].";
		//path - $.tracks.track[1].duration
		String path = prepath + x + postpath + data; 		
        String value = JsonPath.read(obj.toString(), path); //value is the value pair of the key        
        return value;
	}
	/**
	 * This will go in to the last layer of the nested json album file from lastfm
	 * @param x the index wanted from the track array
	 * @param data array where index 1 matches the key name of data needed, 
	 * 			index 0 is the key_parent where key_wanted is located.
	 * @return
	 * @throws JSONException
	 */
	public String get3rdlayer(int x,  String[] data) throws JSONException {
		JSONObject obj = new JSONObject(myResponse.getJSONObject("album").toString());	
		String prepath = "$.tracks.track[";
		String postpath = "].";
		//path - $.tracks.track[1].duration
		String path = prepath + x + postpath + data[0] + "." + data[1]; 
		
        String value = JsonPath.read(obj.toString(), path); //value is the value pair of the key        
        return value;
	}
	

//	public String getrank (int x) throws JSONException {
//		JSONObject obj = new JSONObject(myResponse.getJSONObject("album").toString());	
//        String rank = JsonPath.read(obj.toString(), "$.tracks.track[1].@attr.rank");         
//        System.out.println("track: " + rank);
//        return rank;
//	}
	/**
	 * get summary
	 * @return
	 * @throws JSONException
	 */
	public String getsummary () throws JSONException {
		JSONObject obj = new JSONObject(myResponse.getJSONObject("album").toString());	
        String summary = JsonPath.read(obj.toString(), "$.wiki.summary");         
        return summary;
	}


	/**
	 * this method will create the new json file with the object wanted.
	 * @throws JSONException
	 * @throws FileNotFoundException
	 */
	public void print() throws JSONException, FileNotFoundException {
		String[] rank = {"@attr","rank"}; //string needed because we need to go the last layer 
		
        Map<String,String> o = new LinkedHashMap<String, String>(5); 
		JSONObject oo = new JSONObject();
        PrintWriter pw = new PrintWriter("newJSON.json"); 
        for ( int i = 0; i < this.size(); i++) {
        	o.put("album", this.getalbumname());
    		o.put("artist", this.getartistname());
    		o.put("track", this.gettrackinfo(i,"name"));
    		o.put("duration", this.gettrackinfo(i, "duration"));
    		o.put("rank", this.get3rdlayer(i, rank));
    		o.put("summary", this.getsummary());
    		oo.put((this.gettrackinfo(i, "name")),o); //get name of song to be the object name per song

        }
        pw.write(oo.toString(1)); 


        pw.flush(); 
        pw.close(); 
	}
	
	public int size() throws JSONException {
		JSONObject obj = new JSONObject(myResponse.getJSONObject("album").toString());
		JSONObject tracks = new JSONObject(obj.getJSONObject("tracks").toString());
		JSONArray trackarray = tracks.getJSONArray("track");		
		return trackarray.length();
	}
}
