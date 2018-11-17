import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.DecimalFormat;

import org.rhwlab.dbaccess.DBAccess;
import org.rhwlab.dbaccess.EmbryoXML;
import org.rhwlab.manifest.ManifestX;
import org.rhwlab.sulston.Measure;

public class Measure1 {

	// we must process the args provided to produce new args where
	// argsNew.length = 1
	// argsNew[0] is the AceTree config file for the series
	public static void mainX(String[] args) {
		// assuming one argument which is an acetree config file
		ManifestX.reportAndUpdateManifest();
		DBAccess.cDBLocation = ManifestX.getManifestValue("DBLocation");
		String[] args2 = new String[5];
		try {
			args2 = RedExcel2.fleshOutArgs(args);
		} catch (Exception e) {
			System.out.println("RedExcel2 CANNOT PROCESS: " + args[0]);
			e.printStackTrace();
			System.exit(0);
		}
		String[] args3 = new String[1];
		args3[0] = args2[3];
		Measure.main(args3);
	}

	static String getConfigPath(String series) {
		String p = null;
		try {
			EmbryoXML exml = new EmbryoXML(series);
			p = exml.iRecord[EmbryoXML.ANNOTS] + "/dats/";
			p += exml.iRecord[EmbryoXML.ATCONFIG];

		} catch (FileNotFoundException fnfe) {
			fnfe.printStackTrace();
		}
		return p;
	}

	// accepts either a file path containing series names
	// or a single series name
	public static void main(String[] args) {
		println("Measure1.main, ");
		ManifestX.reportAndUpdateManifest();
		DBAccess.cDBLocation = ManifestX.getManifestValue("DBLocation");
		File f = new File(args[0]);
		boolean bfile = f.exists();
		String series = "";
		if (!bfile) {
			args[0] = getConfigPath(args[0]);
			Measure.main(args);
		} else {
			try {
				FileInputStream fis = new FileInputStream(args[0]);
				BufferedReader br = new BufferedReader(new InputStreamReader(fis));
				while (br.ready()) {
					series = br.readLine();
					if (series.length() < 2)
						continue;
					if (series.startsWith("#"))
						continue;
					args[0] = getConfigPath(series);
					Measure.main(args);
				}
			} catch (IOException ioe) {
				ioe.printStackTrace();
			}
		}

	}

	private static void println(String s) {
		System.out.println(s);
	}

	private static void print(String s) {
		System.out.print(s);
	}

	private static final String CS = ", ", C = ",";
	private static final String TAB = "\t";
	private static final DecimalFormat DF0 = new DecimalFormat("####");
	private static final DecimalFormat DF1 = new DecimalFormat("####.#");
	private static final DecimalFormat DF4 = new DecimalFormat("####.####");

	private static String fmt4(double d) {
		return DF4.format(d);
	}

	private static String fmt1(double d) {
		return DF1.format(d);
	}

	private static String fmt0(double d) {
		return DF0.format(d);
	}

}
