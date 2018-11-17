

import java.io.File;
import java.text.DecimalFormat;

public class DirSwapper {

    String      iPath;

    public DirSwapper() {
        println("DirSwapper, ");

    }

    public DirSwapper(String tifPath) {
        iPath = tifPath;
    }

    public void swap() {
        println("swap, ");
        String path = iPath; //
        File tif = new File(path + "tif");
        File tifR = new File(path + "tifR");
        File tifx = new File(path + "tifx");
        tif.renameTo(tifx);
        tifR.renameTo(tif);
        tifx.renameTo(tifR);

    }

    /**
     * @param args
     */
    public static void main(String[] args) {
    	if (args.length < 1) {
    		println("SNRunner.main, must have one argument, the path to a series");
    		System.exit(0);
    	}
    	String pathToSeries = args[0];
        int m = pathToSeries.lastIndexOf(SNRunner.SLASH);
        String topDir = pathToSeries.substring(0, m);
        String seriesName = pathToSeries.substring(m+1);
        String imageDir =topDir + SNRunner.SLASH + seriesName + SNRunner.SLASH;
        (new DirSwapper(imageDir)).swap();
    }

    private static void println(String s) {System.out.println(s);}
    private static void print(String s) {System.out.print(s);}
    private static final String CS = ", ";
    private static final String TAB = "\t";
    private static final DecimalFormat DF0 = new DecimalFormat("####");
    private static final DecimalFormat DF1 = new DecimalFormat("####.#");
    private static final DecimalFormat DF4 = new DecimalFormat("####.####");
    private static String fmt4(double d) {return DF4.format(d);}
    private static String fmt1(double d) {return DF1.format(d);}
    private static String fmt0(double d) {return DF0.format(d);}
}
