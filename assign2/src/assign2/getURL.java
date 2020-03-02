package assign2;

public class getURL {
	private static String url = "http://ws.audioscrobbler.com/2.0/?method=album.getinfo&artist=Cher&album=Believe&api_key=c8eafe5514e3cd516c13658a6d9465a9&format=json";
	private static String apikey = "c8eafe5514e3cd516c13658a6d9465a9"; 
	
	public static String geturl () {
		return url;
	}
	public static String getkey() {
		return apikey;
	}
		
		
}
