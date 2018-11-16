import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.DecimalFormat;

import org.rhwlab.dbaccess.DBAccess;
import org.rhwlab.manifest.ManifestX;
import org.rhwlab.sulston.Measure;


public class ProcessAll {

	static long NOW;

	static long getProcessedMillis(long start) {
		NOW = System.currentTimeMillis();
		return (NOW - start);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		println("ProcessAll.main, ");
        ManifestX.reportAndUpdateManifest();
        DBAccess.cDBLocation = ManifestX.getManifestValue("DBLocation");
        String series = "";
		try {
			FileInputStream fis = new FileInputStream(args[0]);
			BufferedReader br = new BufferedReader(new InputStreamReader(fis));
			while (br.ready()) {
				series = br.readLine();
				if (series.length() < 2) continue;
				if (series.startsWith("#")) continue;
				long start = System.currentTimeMillis();
				String [] sa = new String[1];
				sa[0] = series;
				Measure1.main(sa);
				long now = System.currentTimeMillis();
				println("PROCESSALL, Measure1 completed in " + getProcessedMillis(start));
				sa[0] = series;
				RedExtractor1.main(sa);
				println("PROCESSALL, RedExtractor1 completed in " + getProcessedMillis(NOW));
				sa[0] = series;
				RedExcel2.main(sa);
				println("PROCESSALL, RedExcel2 completed in " + getProcessedMillis(NOW));
				sa[0] = series;
				Align1.main(sa);
				println("PROCESSALL, Align1 completed in " + getProcessedMillis(NOW));
				println("PROCESSALL, " + series + ", complete in " + getProcessedMillis(start));

			}
		} catch(IOException ioe) {
		ioe.printStackTrace();
		}

	}

	private static void println(String s) {System.out.println(s);}
    private static void print(String s) {System.out.print(s);}
    private static final String CS = ", ", C = ",";
    private static final String TAB = "\t";
    private static final DecimalFormat DF0 = new DecimalFormat("####");
    private static final DecimalFormat DF1 = new DecimalFormat("####.#");
    private static final DecimalFormat DF4 = new DecimalFormat("####.####");
    private static String fmt4(double d) {return DF4.format(d);}
    private static String fmt1(double d) {return DF1.format(d);}
    private static String fmt0(double d) {return DF0.format(d);}

}
