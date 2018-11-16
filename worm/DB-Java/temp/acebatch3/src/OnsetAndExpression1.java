import java.io.IOException;
import java.text.DecimalFormat;

import org.rhwlab.dbaccess.DBAccess;
import org.rhwlab.dbaccess.EmbryoXML;
import org.rhwlab.manifest.ManifestX;

public class OnsetAndExpression1 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// here I will pass in a series name
		// find the acetreeconfigfilepath and gene name
		// and pass those on to OnsetAndExpression.main
		ManifestX.reportAndUpdateManifest();
		DBAccess.cDBLocation = ManifestX.getManifestValue("DBLocation");
		String series = args[0];
		String[] sa = new String[2];
		try {
			EmbryoXML exml = new EmbryoXML(series);
			String annots = exml.iRecord[EmbryoXML.ANNOTS];
			String atconf = exml.iRecord[EmbryoXML.ATCONFIG];
			atconf = annots + "/dats/" + atconf;
			sa[0] = atconf;
			String gene = exml.iRecord[EmbryoXML.REDSIGNAL];
			sa[1] = gene;
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}

		println("OnsetAndExpression1.main, ");
		OnsetAndExpression.main(sa);

	}

	public static void println(String s) {
		System.out.println(s);
	}

	public static void print(String s) {
		System.out.print(s);
	}

	public static final String CS = ", ", C = ",", Q = "\"";
	public static final String TAB = "\t", NULL = "";
	public static final DecimalFormat DF0 = new DecimalFormat("####");
	public static final DecimalFormat DF1 = new DecimalFormat("####.#");
	public static final DecimalFormat DF2 = new DecimalFormat("####.##");
	public static final DecimalFormat DF4 = new DecimalFormat("####.####");
	public static final DecimalFormat DFE = new DecimalFormat("0.####E0");

	public static String fmt4(double d) {
		return DF4.format(d);
	}

	public static String fmt2(double d) {
		return DF2.format(d);
	}

	public static String fmt1(double d) {
		return DF1.format(d);
	}

	public static String fmt0(double d) {
		return DF0.format(d);
	}

	public static String fmtE(double d) {
		return DFE.format(d);
	}

}
