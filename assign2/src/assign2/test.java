package assign2;

import java.io.IOException;

import org.json.JSONException;

public class test {
	public static void main (String[] args) throws JSONException
	{
		getURL x = new getURL(); //has key and url 
	
		try {
			writefile file = new writefile(x.geturl());
//			file.getalbumname();
//			file.getartistname();
//			file.gettrackinfo(1, "duration"); //num = track number and string param = name of key 
//			file.gettrackinfo(1, "name"); //num = track number and string param = name of key 
//
//			//file.gettrackname(1);
//			file.getrank(1);
			file.print();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
