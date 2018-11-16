import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import org.rhwlab.dbaccess.DBAccess;
import org.rhwlab.manifest.ManifestX;
import org.rhwlab.sulston.SeriesSulstonizer;

public class Align1 {

	static void processSeries(String series) {
		String[] args = new String[1];
		args[0] = series;
		String[] args2 = new String[5];
		try {
			args2 = RedExcel2.fleshOutArgs(args);
		} catch (Exception e) {
			System.out.println("RedExcel2 CANNOT PROCESS: " + args[0]);
			e.printStackTrace();
			System.exit(0);
		}
		String[] args3 = new String[2];
		args3[0] = args2[3];
		args3[1] = args2[4];
		SeriesSulstonizer.main(args3);

	}

	// must process args using database
	// such that the args passed to SeriesSulstonizer are
	// args[0] is the AceTree config file for the series
	// args[1] is the number of edited time points
	public static void main(String[] args) {
		ManifestX.reportAndUpdateManifest();
		DBAccess.cDBLocation = ManifestX.getManifestValue("DBLocation");
		String arg = args[0];
		File f = new File(arg);
		if (f.exists()) {
			try {
				FileInputStream fis = new FileInputStream(f);
				BufferedReader br = new BufferedReader(new InputStreamReader(fis));
				while (br.ready()) {
					String s = br.readLine();
					if (s.length() < 2)
						continue;
					if (s.startsWith("#"))
						continue;
					processSeries(s);
				}
			} catch (IOException ioe) {
				ioe.printStackTrace();
				System.exit(0);
			}
		} else {
			processSeries(args[0]);
		}

		/*
		 * String [] args2 = new String[5]; try { args2 = RedExcel2.fleshOutArgs(args);
		 * } catch(Exception e) { System.out.println("RedExcel2 CANNOT PROCESS: " +
		 * args[0]); e.printStackTrace(); System.exit(0); } String [] args3 = new
		 * String[2]; args3[0] = args2[3]; args3[1] = args2[4];
		 * SeriesSulstonizer.main(args3);
		 */
	}

}
