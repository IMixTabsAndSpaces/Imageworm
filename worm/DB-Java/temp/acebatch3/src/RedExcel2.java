import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import org.rhwlab.analysis.EmbryoData;
import org.rhwlab.dbaccess.DBAccess;
import org.rhwlab.dbaccess.EmbryoXML;
import org.rhwlab.manifest.ManifestX;

public class RedExcel2 {

	// for compatibility with acebatch2, this function must return
	// an args array of dimension 5
	// where
	// args2[0] is the series name
	// args2[1] is "all"
	// args2[2] is csv file pathname
	// args2[3] is the AceTree config file to be used
	// args2[4] is the edited time points from the database
	// it appears that it was setup to handle three different cases
	// one where there was only one argument which must have been series name
	// if there is a second argument it must be something like "all"
	// if there is a third argument it must be the CD file pathname
	static String[] fleshOutArgs(String[] args) throws Exception {
		int nargs = args.length;
		String[] args2 = new String[5];
		for (int i = 0; i < nargs; i++)
			args2[i] = args[i];
		String series = args2[0];
		EmbryoXML exml = new EmbryoXML(series);
		if (nargs < 2)
			args2[1] = "all";
		if (nargs < 3) {
			String p = exml.iRecord[EmbryoXML.ANNOTS] + "/dats/";
			args2[2] = p + "CD" + series + ".csv";
		}
		if (nargs < 4) {
			String p = exml.iRecord[EmbryoXML.ANNOTS] + "/dats/";
			p += exml.iRecord[EmbryoXML.ATCONFIG];
			args2[3] = p;

		}
		if (nargs < 5) {
			String s = exml.iRecord[EmbryoXML.EDITEDTP];
			Character c = s.charAt(0);
			if (!Character.isDigit(c)) {
				s = exml.iRecord[EmbryoXML.TIMEPOINTS];
			}
			args2[4] = s;
		}
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
				EmbryoData.spreadsheet2(args);
			} catch (Exception e) {
				println("RedExcel2 CANNOT PROCESS: " + args[0]);
				e.printStackTrace();
			}
		}

	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// the arguments can have three forms
		// if the length > 1 then it must be series destination type
		// if the length = 1 then it could be
		// series or filename
		// where each line in the file would be of the form series type destination
		// type is one of: all, none, global, local, blot
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
						println("RedExcel2, " + s);
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
