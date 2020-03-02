package assign2;

import java.io.IOException;

import org.json.JSONException;

public class test {
	public static void main (String[] args) throws JSONException
	{
	
		try {
			writefile file = new writefile(getURL.geturl());
//			file.getalbumname();
//			file.getartistname();
//			file.gettrackinfo(1, "duration"); //num = track number and string param = name of key 
//			file.gettrackinfo(1, "name"); //num = track number and string param = name of key 
//
//			//file.gettrackname(1);
//			file.getrank(1);
			file.print();
			String[] data = {"@attr", "rank"};
			String rank =  file.get3rdlayer(1,data);
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
