import java.io.File;

import org.rhwlab.analysis.RedBkgComp7;
import org.rhwlab.dbaccess.DBAccess;
import org.rhwlab.dbaccess.EmbryoXML;
import org.rhwlab.manifest.ManifestX;


public class RedExtractor1 {

	// this sub returns a String representing the edited time points
	// and it guarantees that arg[0] is a config filepath
	public static String makeConfigPathFromName(String [] args) {

		String series = args[0];
		File f = new File(series);
		if (f.exists()) {
			series = f.getName();
			int k = series.indexOf(".");
			series = series.substring(0, k);

		}

		EmbryoXML exml = null;
		try {
        	exml = new EmbryoXML(series);
        } catch(Exception e) {
        	e.printStackTrace();
        	System.exit(0);
        }
        String p = exml.iRecord[EmbryoXML.ANNOTS] + "/dats/";
        p += exml.iRecord[EmbryoXML.ATCONFIG];
        args[0] = p;
        return exml.iRecord[EmbryoXML.EDITEDTP];
	}

    /**
     * @param args
     * this version is to take a file with series lines
     */
    public static void main(String[] args) {
		ManifestX.reportAndUpdateManifest();
        DBAccess.cDBLocation = ManifestX.getManifestValue("DBLocation");
        RedBkgComp7.cSignalIsGreen = false;
        // according to the rules of acebatch,
        // args[0] could be either a series name or a config file pathname
        // if it is a series name, we must convert it to a config file
        String endtime = makeConfigPathFromName(args);
        // if that is all that was specified we need to provide
        // the endtime as obtained from the database
        if (args.length == 1) {
        	String config = args[0];
        	args = new String[2];
        	args[0] = config;
        	args[1] = "599";
        } else {
        	// the only acebatch alternative to a single arg is 7 args
        	// in that case I only need to swap start and end time positions
        	// and understand that the seventh arg will be ignored
        	// that being a location for the nuc files
        	String start = args[1];
        	args[2] = args[1];
        	args[2] = start;
        }

        RedBkgComp7.main(args);

    }
    private static void println(String s) {System.out.println(s);}
    private static final String CS = ", ", C = ",";

}
