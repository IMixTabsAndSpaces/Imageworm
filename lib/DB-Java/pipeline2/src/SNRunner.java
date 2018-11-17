

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.util.Vector;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.rhwlab.dbaccess.EmbryoXML;
import org.rhwlab.manifest.ManifestX;
import org.rhwlab.snight.Config;
import org.rhwlab.snight.NucZipper;


/**
 * @author biowolp
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class SNRunner implements CmdInterface {

    public boolean iStarryNiteRunning;

    String  iTopDir;
    String  iSeriesName;
    String  iTempParams;
    String  iStarryNite;
    String  iSampleImageName;
    String  iSNSampleImageName;
    String  iFileOpenPath;
    String  iParametersFileName;
    int     iStartingIndex;
    int     iNucFileCount;
    int     iEndCode;
    String  iWorkingDir;
    String  iFinalParams;
    boolean iSwap;
    int		iMaxTimePoint;
    double	iXy_res;
    double	iZ_res;
    int		iPlaneEnd;

    public static String       cDBLocation;


    public SNRunner(String topDir, String seriesName, String inputParamFilePath, boolean swap) {
    	ManifestX.reportAndUpdateManifest();
    	iStarryNiteRunning = false;
        iSwap = swap;
        //iStarryNite = "/nfs/waterston/tools2/StarryNite-090205";
        iStarryNite = ManifestX.getManifestValue("StarryNite");
        //iStarryNite = "./aout";
        iParametersFileName = inputParamFilePath; //"standard-parameters";
        iSeriesName = seriesName;
        iTopDir = topDir;
        iWorkingDir = iTopDir + "/" + iSeriesName + "/temp";
        new File(iWorkingDir).mkdir();
        iTempParams = iWorkingDir + "/tempParams";
        iMaxTimePoint = Integer.parseInt(DBEntryMaker.timePointsEstimate(iTopDir + SLASH + iSeriesName));
        getSampleImageName();
        if (iSampleImageName.length() == 0) {
            println("SNRunner, no images found");
        } else {
            println("SNRunner, sample image, " + iSampleImageName);
        }
        iEndCode = -1;
        //iSampleImageName = iTopDir + "/" + iSeriesName + "/tif/" + iSeriesName + "_L1-t001-p01.tif";

    }

    private void getSampleImageName() {
        String tif = "/tif";
        if (iSwap) tif = "/tifR";
        String tifDir = iTopDir + SLASH + iSeriesName + tif;
        //println("getSampleImageName, " + tifDir);
        File tifDirFile = new File(tifDir);
        String [] names = tifDirFile.list();
        for (int i=0; i < names.length; i++) {
            String name = names[i];
            //println("getSampleImageName, " + name);
            if (name.endsWith(".tif")) {
                iSNSampleImageName = iTopDir + SLASH + iSeriesName + tif + "/" + name;
                iSampleImageName = iTopDir + SLASH + iSeriesName + "/tif/" + name;
                break;

            }
        }
    }

    public void runStarryNite() {

        if (iStarryNiteRunning) return;
        File f = new File(iTempParams);
        //if (!f.exists()) createTempParamsFile();
        String series = iSeriesName;
        if (series.length() == 0) {
            println("PLEASE ENTER Series name");
            return;
        }
        File sn = new File(iStarryNite);
        File im = new File(iSNSampleImageName);
        //File pf = new File(iParametersFileName.getText());
        File pf = new File(iTempParams);
        boolean b = sn.exists();
        b &= im.exists();
        b &= pf.exists();
        if (!b) {
            if (!sn.exists()) {
                println("executable file not found " + sn);
                return;
            }
            if (!im.exists()) {
                println("image file not found " + im);
                return;
            }
            if (!pf.exists()) {
                println("parameters file not found " + pf);
                return;
            }
            return;
        }
        parseParametersFile(pf);
        String s = iSNSampleImageName;
        int k = s.lastIndexOf("-");
        s = s.substring(0, k);
        k = s.lastIndexOf("-");
        s = s.substring(0, k);
        println(s);

        StringBuffer sb = new StringBuffer();
        String snLoc = iStarryNite;
        File fx = new File(snLoc);

        sb.append(snLoc + SP);
        //sb.append(iStarryNite.getText() + SP);
        sb.append(s + SP);
        sb.append(". ");
        sb.append(iTempParams);
        //sb.append(iParametersFileName.getText());
        String command = sb.toString();
        println(command);
        //command = iStarryNite;
        /*
        try {
            execItTest(sb.toString());
        } catch(Exception e) {
            e.printStackTrace();
            return;
        }
        */
        try {
            iStarryNiteRunning = true;
            println("runStarryNite, " + command);
            CmdThread cmdT = CmdThread.exec(this, command, STARRYNITE, new File(iWorkingDir));
            System.out.println(" running..\n" + sb.toString());
        } catch (IOException ex) {
            println("runStarryNite, got IOException");
        	processNewError(ex.getMessage(), STARRYNITE);
        }

        boolean waiter = true;
        while (waiter) {
            try {Thread.sleep(1);} catch(Exception e){e.printStackTrace();}
            waiter = iStarryNiteRunning;
        }

    }

    public void finishUp() {
        println("finishUp, " + iEndCode);
        String s = iSampleImageName;
        iFinalParams = iWorkingDir + "/" + iSeriesName + "-parameters";
        int k = s.lastIndexOf("-");
        s = s.substring(0, k);
        k = s.lastIndexOf("-");
        s = s.substring(0, k);
        println(s);
        try {
            ChannelCopy.transferCopy(iTempParams, iFinalParams);
        } catch(Exception e) {
            e.printStackTrace();
        }

        if (iEndCode == 20 || iEndCode == 0) {
            processNucleiFiles(iWorkingDir + "/" + iSeriesName + ".zip", iFinalParams, iSampleImageName);
        } else {
            println("bad outcome processing");
        }
        createConfigFile(iSeriesName, s);
        moveNucAndConfigFiles();

    }

    void parseParametersFile(File pf) {
    	try {
    		FileInputStream fis = new FileInputStream(pf);
    		BufferedReader br = new BufferedReader(new InputStreamReader(fis));
    		while (br.ready()) {
    			String s = br.readLine();
    			String [] sa = s.split(" ");
    			if (sa[0].startsWith("plane_end")) {
    				iPlaneEnd = Integer.parseInt(sa[1]);
    			} else if (sa[0].startsWith("xy_res")) {
    				iXy_res = Double.parseDouble(sa[1]);
    			} else if (sa[0].startsWith("z_res")) {
    				iZ_res = Double.parseDouble(sa[1]);
    			}
    		}
    		br.close();

    	} catch(IOException ioe) {
    		ioe.printStackTrace();
    	}
    }

    public void moveNucAndConfigFiles() {
        String zipSrc = iWorkingDir + "/" + iSeriesName + ".zip";
        String zipEditSrc = makeZipEditName(zipSrc);
        String datSrc = iWorkingDir + "/" + iSeriesName + ".xml";
        String parSrc = iWorkingDir + "/" + iSeriesName + "-parameters";

        // mod for Zafer stuff as of 20090514
        String zafer1Name = "svm_output_correct_division_calls.txt";
        String zafer2Name = "svm_output_movements_called_as_divisions.txt";

        String zafer1Src = iWorkingDir + "/" + zafer1Name;
        String zafer2Src = iWorkingDir + "/" + zafer2Name;
        boolean bz1 = new File(zafer1Src).exists();
        boolean bz2 = new File(zafer2Src).exists();


        // if a database entry exists for this series then put the new
        // annotations into that directory in place of what is already there
        // otherwise create the dats directory in the series directory
        // where the images were
        String cDBLocation = ManifestX.getManifestValue("DBLocation");
        DBEntryMaker.cDBLocation = cDBLocation;
        String dbentry = cDBLocation + iSeriesName + ".xml";
        File f = new File(dbentry);
        File dir1 = null;
        boolean bdir1 = false;
        EmbryoXML exml = null;
        if (!f.exists()) {
        	dir1 = new File(iTopDir + "/" + iSeriesName + "/dats");
        	bdir1 = dir1.mkdir();
        } else {
            try {
            	exml = new EmbryoXML(iSeriesName);
                dir1 = new File(exml.iRecord[EmbryoXML.ANNOTS] + "/dats/");

            } catch(IOException ioe) {
            	ioe.printStackTrace();
            }

        }


        String path = dir1.getPath() + "/";
        println("moveNucAndConfigFiles, " + path);
        //String path = iTopDir + "/" + iSeriesName + "/dats/";
        String zipDst = path + iSeriesName + ".zip";
        String zipEditDst = makeZipEditName(zipDst);
        String datDst = path + iSeriesName + ".xml";
        String parDst = path + iSeriesName + "-parameters";

        //String zafer1Dst = path + zafer1Name;
        //String zafer2Dst = path + zafer2Name;

        try {
            ChannelCopy.transferCopy(datSrc, datDst);
            ChannelCopy.transferCopy(parSrc, parDst);
            new File(parSrc).delete();
            if (iEndCode == 20 || iEndCode == 0) {
                ChannelCopy.transferCopy(zipSrc, zipDst);
                ChannelCopy.transferCopy(zipEditSrc, zipEditDst);

                //if (bz1) ChannelCopy.transferCopy(zafer1Src, zafer1Dst);
                //if (bz2) ChannelCopy.transferCopy(zafer2Src, zafer2Dst);
            }
        } catch(IOException ioe) {
            ioe.printStackTrace();
        }
        new File(zipSrc).delete();
        new File(zipEditSrc).delete();
        new File(datSrc).delete();

        if (bz1) new File(zafer1Src).delete();
        if (bz2) new File(zafer2Src).delete();

        new File(iWorkingDir).delete();
        new File(iWorkingDir).deleteOnExit();

    }

    private String makeZipEditName(String zipName) {
        int m = zipName.indexOf(".zip");
        String zipEditName = zipName.substring(0, m) + "-edit.zip";
        return zipEditName;

    }

    public void processNucleiFiles(String zipName, String params, String imageName) {
        System.out.println("processNucleiFiles: " + zipName);
        NucZipper2 nz = new NucZipper2(zipName);
        Pattern pattern = Pattern.compile("t\\d\\d\\d-nuclei");
        Matcher matcher;
        File [] fa = new File(iWorkingDir).listFiles();
        int lowestTime = 999;
        for (int i=0; i < fa.length; i++) {
            //System.out.println(fa[i]);
            String s1 = fa[i].toString();
            matcher = pattern.matcher(s1);
            if (!matcher.find()) continue;
            String s2 = s1.substring(matcher.start(), matcher.end());
            //System.out.println(s2);
            String s3 = s2.substring(1,4);
            int k = Integer.parseInt(s3);
            //System.out.println("process..: " + s3 + CS + k);
            lowestTime = Math.min(k, lowestTime);
            File fin = new File(iWorkingDir + "/" + s2);
            //System.out.println("process..: " + "nuclei" + "/" + s2);
            nz.addFile(fin, "nuclei" + "/" + s2);
            //fin.deleteOnExit();
            boolean deleted = fin.delete();
            //println("processNucleiFiles, " + deleted + CS + s2);
            iNucFileCount++;
        }
        //System.out.println("process..: " + lowestTime);
        iStartingIndex = lowestTime;
        File log = new File(iWorkingDir + "/log");
        if (log.exists()) {
            nz.addFile(log, "parameters/log");
            log.deleteOnExit();
            log.delete();
        }

        File parameters = new File(params);
        if (parameters.exists()) {
            nz.addFile(parameters, "parameters/" + iSeriesName + "-parameters");
            //boolean b = parameters.renameTo(new File(iSeriesName + "-parameters"));
            File f = new File(iTempParams);
            f.deleteOnExit();
            f.delete();
        }
        nz.close();
        try {
            ChannelCopy.transferCopy(zipName, makeZipEditName(zipName));
        } catch(IOException ioe) {
            ioe.printStackTrace();
        }
    }

    private String extractName(String imageName) {
        int k = imageName.lastIndexOf("/");
        String name = imageName.substring(k+1);
        int m = name.length() - "-t000-p00.tif".length();
        name = name.substring(0, m);
        name += "-parameters";
        return name;
    }

    public void createConfigFile(String zipName, String tiffs) {
    	Config config = new Config();
    	String leader = iTopDir + "/" + iSeriesName + "/dats/" + zipName;
    	config.iZipFileName = makeZipEditName(leader + ".zip");;
    	config.iTypicalImage = iSampleImageName;
    	config.iEndingIndex = iStartingIndex + iNucFileCount - 1;
    	config.iPlaneEnd = iPlaneEnd;
    	config.iXy_res = (float)iXy_res;
    	config.iZ_res = (float)iZ_res;
    	config.writeXMLConfig(new File(iWorkingDir + "/" + iSeriesName + ".xml"));

    }


    public void createConfigFile(String zipName, String tiffs, boolean oldVersion) {
        try {
            //String s = new File(".").getCanonicalPath();
            //System.out.println(s);
            File configFile = new File(iWorkingDir + "/" + zipName + ".dat");
            PrintWriter pw = new PrintWriter(new FileOutputStream(configFile));
            String zipEditName = makeZipEditName(zipName + ".zip");
            //println("createConfigFile: tiffs1: " + tiffs);
            tiffs = unixStyle(tiffs);
            int k = tiffs.lastIndexOf("/");
            String tif1 = tiffs.substring(0, k);
            k = tif1.lastIndexOf("/");
            tif1 = tiffs.substring(0, k);
            String tif2 = tiffs.substring(k+1);
            //System.out.println(tiffs + CS + tif1 + CS + tif2);
            pw.println("tif directory, " + tif1);
            pw.println("tifPrefix, " + tif2 + "-");
            if (iEndCode == 20 || iEndCode == 0) {
                pw.println("zipFileName, " + zipEditName);
                pw.println("nuclei directory, nuclei");
                pw.println("root name, " + "P0");
                pw.println("starting index, " + iStartingIndex);
                pw.println("ending index, " + 590); //(iStartingIndex + iNucFileCount - 1));
                pw.println("use zip, " + 0);
                pw.println("namingMethod, NEWCANONICAL");
            } else {
                pw.println("zipFileName, NULL");
                pw.println("ending index, 599");
            }
            pw.close();
        } catch(Exception ioe) {
            ioe.printStackTrace();
        }

    }
    private String unixStyle(String s) {
        int len = s.length();
        StringBuffer sb = new StringBuffer(s);
        for (int i=len-1; i > -1; i--) {
            int c = sb.charAt(i);
            //println("char: " + sb.charAt(i) + CS + c);
            if (c == 92) {
                sb.replace(i, i+1, "/");
                if (i > 0) {
                    c = sb.charAt(i - 1);
                    if (c == 92) sb.deleteCharAt(--i);
                }
            }
        }
        return sb.toString();
    }


    /* (non-Javadoc)
     * @see org.rhwlab.pipeline.CmdInterface#processNewInput(java.lang.String, int)
     */
    public void processNewInput(String input, int signature) {
        if (input.length() > 0) println("SNmsgI, " + input);

    }

    /* (non-Javadoc)
     * @see org.rhwlab.pipeline.CmdInterface#processNewError(java.lang.String, int)
     */
    public void processNewError(String error, int signature) {
        if (error.length() > 0) println("SNmsgE, " + error);

    }

    /* (non-Javadoc)
     * @see org.rhwlab.pipeline.CmdInterface#processEnded(int, int)
     */
    public void processEnded(int exitValue, int signature) {
        println("processEnded, " + exitValue + CS + signature);
        iEndCode = exitValue;
        iStarryNiteRunning = false;

    }

    public static final int
         STARRYNITE = 7
        ;

    public static final String
         SLASH = "/"
        ;


    private void prepareParametersFile() {
    	String path = iParametersFileName;
        if (path == null || path.length() == 0) path = "/nfs/waterston/tools3/standard-parametersDEFAULT";
        boolean startFound = false;
        boolean endFound = false;
        boolean firstFound = false;
        boolean lastFound = false;
        try {
            FileInputStream fis = new FileInputStream(path);
            BufferedReader br = new BufferedReader(new InputStreamReader(fis));
            FileOutputStream fos = new FileOutputStream(iTempParams);
            PrintWriter pw = new PrintWriter(fos);
            while (br.ready()) {
                String s = br.readLine();
                if (!startFound) startFound = s.indexOf("time_start") == 0;
                if (!endFound) endFound = s.indexOf("time_end") == 0;
                if (!firstFound) firstFound = s.indexOf("plane_start") == 0;
                if (!lastFound) lastFound = s.indexOf("plane_end") == 0;
                pw.println(s);
            }
            br.close();
            if (!startFound) pw.println("time_start " + String.valueOf(1)); //iConfigXML.iRecord[ConfigXML.START]);
            if (!endFound) {

            	pw.println("time_end " + String.valueOf(iMaxTimePoint)); //iConfigXML.iRecord[ConfigXML.END]);
            }
            //if (!firstFound) pw.println("plane_start " + iConfigXML.iRecord[ConfigXML.FIRST]);
            //if (!lastFound) pw.println("plane_end " + iConfigXML.iRecord[ConfigXML.LAST]);
            pw.close();
            br.close();


        } catch(IOException ioe) {
        	ioe.printStackTrace();
        }
    }





    public static void mainX(String [] args) {
        String iTopDir = "/nfs/waterston/murray/elt2red";
        String iSeriesName = "20070521_elt2_sk";
        SNRunner snrunner = new SNRunner(iTopDir, iSeriesName, "tempParams", false);
        snrunner.finishUp();

    }


    public static void main(String[] args) {
        //String iTopDir = "/nfs/waterston/biowolp";
        //String iSeriesName = "20070502-skr10RNAi";
        //SNRunner snrunner = new SNRunner(iTopDir, iSeriesName, "tempParams", false);
    	if (args.length < 1) {
    		println("SNRunner.main, must have one argument, the path to a series");
    		System.exit(0);
    	}
    	String pathToSeries = args[0];
        int m = pathToSeries.lastIndexOf(SLASH);
        String topDir = pathToSeries.substring(0, m);
        String seriesName = pathToSeries.substring(m+1);


    	boolean swap = false;
    	String swaps = pathToSeries + "/swap.txt";
    	File fs = new File(swaps);
    	if (fs.exists()) swap = true;
    	String paramsPath = null;
   		String sp = pathToSeries + "/snParams";
   		File fp = new File(sp);
   		if (fp.exists()) paramsPath = sp;
        SNRunner snrunner = new SNRunner(topDir, seriesName, paramsPath, swap);
        snrunner.prepareParametersFile();
        snrunner.runStarryNite();
        snrunner.finishUp();
        if (swap) {
            String imageDir =topDir + SLASH + seriesName + SLASH;
            (new DirSwapper(imageDir)).swap();

        }
        fs.delete();
        //fp.delete();

        //SNRunner snrunner = new SNRunner(topDir, seriesName);
        //snrunner.getSampleImageName();
        //snrunner.runStarryNite();
        //snrunner.finishUp();
    }


    private static void println(String s) {System.out.println(s);}
    private static void print(String s) {System.out.print(s);}
    private static final String CS = ", ", SP = " ";
    private static final DecimalFormat DF0 = new DecimalFormat("####");
    private static final DecimalFormat DF1 = new DecimalFormat("####.#");
    private static final DecimalFormat DF4 = new DecimalFormat("####.####");
    private static String fmt1(double d) {return DF1.format(d);}
    private static String fmt0(double d) {return DF1.format(d);}

}
