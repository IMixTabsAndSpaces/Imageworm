import org.rhwlab.analysis.RedBkgComp7;
import org.rhwlab.manifest.ManifestX;


public class GreenExtractor1 {

	/**
	 * @param args
	 */
    public static void main(String[] args) {
		ManifestX.reportAndUpdateManifest();
        RedBkgComp7.cSignalIsGreen = true;
        String endtime = RedExtractor1.makeConfigPathFromName(args);
        // if that is all that was specified we need to provide
        // the endtime as obtained from the database
        if (args.length == 1) {
        	String config = args[0];
        	args = new String[2];
        	args[0] = config;
        	args[1] = endtime;
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
