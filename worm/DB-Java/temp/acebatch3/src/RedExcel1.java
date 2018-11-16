import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import org.rhwlab.analysis.EmbryoData;
import org.rhwlab.dbaccess.DBAccess;
import org.rhwlab.dbaccess.EmbryoXML;
import org.rhwlab.manifest.ManifestX;
import org.rhwlab.snight.Config;

public class RedExcel1 {

	private static String[] fleshOutArgs(String[] args) throws Exception {
		int nargs = args.length;
		String[] args2 = new String[5];
		for (int i = 0; i < nargs; i++)
			args2[i] = args[i];
		String series = args2[0];
		EmbryoXML exml = new EmbryoXML(series);
		if (nargs < 2)
			args2[1] = Config.REDCHOICE[3];
		if (nargs < 3) {
			String p = exml.iRecord[EmbryoXML.ANNOTS] + "/dats/";
			args2[2] = p + "S" + series + ".csv";
		}
		// now append the acetree config file path
		// and the edited time points
		String p = exml.iRecord[EmbryoXML.ANNOTS] + "/dats/";
		p += exml.iRecord[EmbryoXML.ATCONFIG];
		args2[3] = p;
		// now add the number of edited time points

		String s = exml.iRecord[EmbryoXML.EDITEDTP];
		Character c = s.charAt(0);
		if (!Character.isDigit(c)) {
			s = exml.iRecord[EmbryoXML.TIMEPOINTS];
		}
		args2[4] = s;
		return args2;
	}

	private static void processOneLine(String[] args) {
		boolean bfile = false;
		boolean bok = true;
		if (args.length == 1) {
			File f = new File(args[0]);
			bfile = f.exists();
		}
		if (args.length > 1 || !bfile) {
			// this is a request for output based on a single series
			try {
				String[] args2 = fleshOutArgs(args);
				args = args2;
				EmbryoData.spreadsheet1(args);
			} catch (Exception e) {
				println("RedExcel1 cannot process: " + args);
			}
		}

	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// the arguments can have three forms
		// if the length > 1 then it must be series type destination
		// if the length = 1 then it could be
		// series or filename
		// where each line in the file would be of the form series type destination
		// if it is series or if any line has only one item that is a series name
		// and we default everything else
		ManifestX.reportAndUpdateManifest();
		DBAccess.cDBLocation = ManifestX.getManifestValue("DBLocation");
		boolean bfile = false;
		String[] args2 = null;
		if (args.length == 1) {
			File f = new File(args[0]);
			bfile = f.exists();
		}
		if (args.length > 1 || !bfile) {
			// this is a request for output based on a single series
			processOneLine(args);
		} else {
			try {
				FileInputStream fis = new FileInputStream(args[0]);
				BufferedReader br = new BufferedReader(new InputStreamReader(fis));
				while (br.ready()) {
					String s = br.readLine();
					if (s.length() > 2) {
						if (s.charAt(0) == '#')
							continue;
						println("RedExcel1, " + s);
						args2 = s.split("\\s+");
						processOneLine(args2);
					}
				}
			} catch (IOException ioe) {
				ioe.printStackTrace();
			}

		}

	}

	private static void println(String s) {
		System.out.println(s);
	}

	private static final String CS = ", ", C = ",";

}
