package assign2;

import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;


import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.jayway.jsonpath.JsonPath;

public class writefile {
	public static String inputline;
	public static JSONObject myResponse;
	//public static JSONObject music;
	
	
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
		System.out.println("album name: " + albumname);
		return albumname;
		
	}
	
	public String getartistname() throws JSONException {
		JSONObject getalbum = new JSONObject(myResponse.getJSONObject("album").toString());	
		String artist = getalbum.getString("artist");
		System.out.println("artist name: " + artist);
		return artist;

	}
	public String getruntime(int x, String data) throws JSONException {
		JSONObject obj = new JSONObject(myResponse.getJSONObject("album").toString());	
		String prepath = "$.tracks.track[";
		String postpath = "].";
		String path = prepath + x + postpath + data; 
		//path - $.tracks.track[1].duration
        String value = JsonPath.read(obj.toString(), path); //value is the value pair of the key        
        System.out.println(data +": " + value); // test string 
        return value;
	}
	/*
	 * Below method is taken care of the method from above
	 * this is to avoid redundancy
	 */
//	public String gettrackname(int x) throws JSONException {
//		JSONObject obj = new JSONObject(myResponse.getJSONObject("album").toString());	
//        String trackname = JsonPath.read(obj.toString(), "$.tracks.track[1].name");         
//        System.out.println("track: " + trackname);
//        return trackname;
//	}
	public String getrank (int x) throws JSONException {
		JSONObject obj = new JSONObject(myResponse.getJSONObject("album").toString());	
        String rank = JsonPath.read(obj.toString(), "$.tracks.track[1].@attr.rank");         
        System.out.println("track: " + rank);
        return rank;
	}
	
	
	
	
//	public void getallruntime() throws JSONException, IOException{
//		JSONObject obj = new JSONObject(myResponse.getJSONObject("album").toString());	
//		JSONObject tracks = new JSONObject(obj.getJSONObject("tracks").toString());	
//		JSONArray track = tracks.getJSONArray("track"); //returns the array
//		int tracksize = track.length();
//		String prepath = "$.tracks.track[";
//		String postpath = "].duration";
//		String runtime;
//		JSONObject dobj = new JSONObject();
//		FileWriter file = new FileWriter("duration.json");
//       // Map<String, String> m = new LinkedHashMap<String, String>(tracksize); 
//		for (int i = 0 ; i < tracksize ; i++) {
//			String path = prepath + i + postpath;
//			//System.out.println(path);
//			runtime = JsonPath.read(obj.toString(),path.toString());         
//			dobj.put("runtime", runtime);
//			
//	        file.write(dobj.toString(4));// tostring will format properly, the param will give it space/indents
//
//			
//		}
//
//        file.flush();
//	}
	 

	
}
