

import java.io.File;
import java.text.DecimalFormat;

/**
 * @author biowolp
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class LSMTrasher {
    String  iSeriesDir;
    int     iNumLSM;
    int     iNumTIF;
    int     iNumInfo;
    int     iNumPlanes;

    public LSMTrasher(String seriesDir) {
        iSeriesDir = seriesDir;

    }
    public void test1() {
        //println("TrashLSMs.test1, ");
        File lsmDir = new File(iSeriesDir + "/lsm");
        if (!lsmDir.exists()) {
            println("");
            println("###########################################");
            println("## No lsm dir found at, " + lsmDir);
            println("## delete request abandoned.");
            println("###########################################");
            return;

        }
        boolean errorLogFound = false;
        String [] lsmFileNames = lsmDir.list();
        boolean [] realLSM = new boolean[lsmFileNames.length];
        for (int i=0; i < lsmFileNames.length; i++) {
            if (lsmFileNames[i].equals("ERRORS.log")) errorLogFound = true;
            realLSM[i] = false;
            if (lsmFileNames[i].endsWith(".lsm")) {
                iNumLSM++;
                realLSM[i] = true;
            }
        }
        //println("test1, " + iNumLSM);
        String infoDirName = iSeriesDir + SLASH + INFO;
        //println("test1, " + infoDirName);

        File infoDir = new File(infoDirName);
        String [] infoFileNames = infoDir.list();
        boolean [] realINFO = new boolean[infoFileNames.length];
        for (int i=0; i < infoFileNames.length; i++) {
            realINFO[i] = false;
            if (infoFileNames[i].endsWith(".txt")) {
                iNumInfo++;
                realINFO[i] = true;
            }
        }
        //println("test1, " + iNumInfo);

        File tifDir = new File(iSeriesDir + SLASH + TIF);
        String [] tifFileNames = tifDir.list();
        boolean [] realTIF = new boolean[tifFileNames.length];
        for (int i=0; i < tifFileNames.length; i++) {
            realTIF[i] = false;
            if (tifFileNames[i].endsWith(".tif")) {
                iNumTIF++;
                realTIF[i] = true;
            }
        }
        //println("test1, " + iNumTIF);

        boolean trash = iNumLSM == iNumInfo;
        trash = trash & !errorLogFound;
        iNumPlanes = iNumTIF/iNumLSM;
        if (trash) trash = trash && iNumPlanes > MINPLANES;
        //println("test1, trash=" + trash);
        String xx = "";
        if (!trash) xx = "NOT ";
        println("");
        println("###########################################");
        println("## ");
        println("## " + xx + "deleting lsm files");
        println("## ");
        println("###########################################");

        if (!trash) return;

        for (int i=0; i < lsmFileNames.length; i++) {
            if (realLSM[i]) {
                File f = new File(iSeriesDir + SLASH + LSM + SLASH + lsmFileNames[i]);
                boolean x = false;
                x = f.delete();
                //f.deleteOnExit();
                //println(lsmFileNames[i] + CS + x);
            }
        }
        new File(iSeriesDir + SLASH + LSM).delete();



    }

    private static final int
         MINPLANES = 30
        ;

    private static final String
         LSM = "lsm"
        ,TIF = "tif"
        ,INFO = "info"
        ,SLASH = "/"
        ;

    public static void main(String[] args) {
        println("LSMTrasher.main, ");
    	String pathToSeries = args[0];
        int m = pathToSeries.lastIndexOf(SNRunner.SLASH);
        LSMTrasher trasher = new LSMTrasher(pathToSeries);
        trasher.test1();

    }

    private static void println(String s) {System.out.println(s);}
    private static void print(String s) {System.out.print(s);}
    private static final String CS = ", ";
    private static final DecimalFormat DF0 = new DecimalFormat("####");
    private static final DecimalFormat DF1 = new DecimalFormat("####.#");
    private static final DecimalFormat DF4 = new DecimalFormat("####.####");
    private static String fmt1(double d) {return DF1.format(d);}
    private static String fmt0(double d) {return DF1.format(d);}

}
