import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.text.DecimalFormat;

import org.rhwlab.manifest.ManifestX;


public class MatlabRunner implements CmdInterface {

	boolean			iMatlabRunning;
	String			iParametersFileName;
	String			iSeriesName;
	String			iTopDir;
	int				iMaxTimePoint;
	String			iMatlabImageArg;
	String			iWorkingDir;
	String			iTempParams;

	public MatlabRunner(String topDir, String seriesName, String inputParamFilePath) {
    	ManifestX.reportAndUpdateManifest();
    	iMatlabRunning = false;
        iParametersFileName = inputParamFilePath; //"standard-parameters";
        iSeriesName = seriesName;
        iTopDir = topDir;
        iMaxTimePoint = Integer.parseInt(DBEntryMaker.timePointsEstimate(iTopDir + SLASH + iSeriesName));
        getSampleImageName();

        iWorkingDir = iTopDir + "/" + iSeriesName + "/dats";
        new File(iWorkingDir).mkdir();
        iTempParams = iWorkingDir + "/mparm.txt";
	}

    void runMatlab() {
    	if (iMatlabRunning) return;
    	String command = "";
        StringBuffer sb = new StringBuffer();
        sb.append("/nfs/waterston/matlab/matlab.sh");
        sb.append(SP + iTopDir + "/" + iSeriesName);
        //sb.append(SP + iTempParams);
        //sb.append(SP + iWorkingDir);
        sb.append(SP + iMatlabImageArg);


        command = sb.toString();
        try {
            Runtime.getRuntime().exec(command);
            //iMatlabRunning = true;
            println("runMatlab, " + command);
            //CmdThread cmdT = CmdThread.exec(this, command, MATLAB, new File(iWorkingDir));
            System.out.println(" running..\n" + sb.toString());
        } catch (IOException ex) {
            println("runStarryNite, got IOException");
        	processNewError(ex.getMessage(), MATLAB);
        }

        boolean waiter = true;
        while (waiter) {
            try {Thread.sleep(1);} catch(Exception e){e.printStackTrace();}
            waiter = iMatlabRunning;
        }

        /*
        sb = new StringBuffer();
        sb.append("nice java -cp snLauncherM.jar SNLMNoUI ");
        sb.append(iTopDir + "/" + iSeriesName);
        println("runMatlab, phase 2, " + sb.toString());
        try {
            CmdThread cmdT = CmdThread.exec(this, sb.toString(), MATLAB);
            iMatlabRunning = true;
        } catch (IOException ex) {
            println("runLSMInfo, IOException, " + ex);
            processNewError(ex.getMessage(), 0);
        }

        waiter = true;
        while (waiter) {
            try {Thread.sleep(1);} catch(Exception e){e.printStackTrace();}
            waiter = iMatlabRunning;
        }
        */

    }

	private void getSampleImageName() {
        String tif = "/tif";
        String tifDir = iTopDir + SLASH + iSeriesName + tif;
        //println("getSampleImageName, " + tifDir);
        File tifDirFile = new File(tifDir);
        String [] names = tifDirFile.list();
        for (int i=0; i < names.length; i++) {
            String name = names[i];
            //println("getSampleImageName, " + name);
            if (name.endsWith(".tif")) {
                //iSNSampleImageName = iTopDir + SLASH + iSeriesName + tif + "/" + name;
            	String [] sa = name.split("-t\\d+");
                //iMatlabImageArg = iTopDir + SLASH + iSeriesName + "/tif/" + sa[0];
                iMatlabImageArg = sa[0];;
                break;

            }
        }
    }

    private void prepareParametersFile() {
    	String path = iParametersFileName;
        if (path == null || path.length() == 0) path = "/nfs/waterston/matlab/mparmDEFAULT.txt";
        boolean startFound = false;
        boolean endFound = false;
        try {
            FileInputStream fis = new FileInputStream(path);
            BufferedReader br = new BufferedReader(new InputStreamReader(fis));
            FileOutputStream fos = new FileOutputStream(iTempParams);
            PrintWriter pw = new PrintWriter(fos);
            while (br.ready()) {
                String s = br.readLine();
                if (!startFound) startFound = s.startsWith("start_time");
                if (!endFound) endFound = s.startsWith("end_time");
                pw.println(s);
            }
            br.close();
            if (!startFound) pw.println("start_time = " + String.valueOf(1)); //iConfigXML.iRecord[ConfigXML.START]);
            if (!endFound) {

            	pw.println("end_time = " + String.valueOf(iMaxTimePoint)); //iConfigXML.iRecord[ConfigXML.END]);
            }
            pw.close();


        } catch(IOException ioe) {

        }
    }



	/**
	 * @param args
	 */
	public static void main(String[] args) {
		println("MatlabRunner.main, ");
    	if (args.length < 1) {
    		println("SNRunner.main, must have one argument, the path to a series");
    		System.exit(0);
    	}
    	String pathToSeries = args[0];
        int m = pathToSeries.lastIndexOf(SLASH);
        String topDir = pathToSeries.substring(0, m);
        String seriesName = pathToSeries.substring(m+1);
    	String paramsPath = null;
   		String sp = pathToSeries + "/matlabParams";
   		File fp = new File(sp);
   		if (fp.exists()) paramsPath = sp;
   		MatlabRunner mrunner = new MatlabRunner(topDir, seriesName, paramsPath);
   		mrunner.prepareParametersFile();
   		mrunner.runMatlab();


	}

    public static final String
    SLASH = "/"
   ;
    public static final int
    MATLAB = 8
   ;



    private static void println(String s) {System.out.println(s);}
    private static void print(String s) {System.out.print(s);}
    private static final String CS = ", ", SP = " ";
    private static final DecimalFormat DF0 = new DecimalFormat("####");
    private static final DecimalFormat DF1 = new DecimalFormat("####.#");
    private static final DecimalFormat DF4 = new DecimalFormat("####.####");
    private static String fmt1(double d) {return DF1.format(d);}
    private static String fmt0(double d) {return DF1.format(d);}

	public void processEnded(int exitValue, int signature) {
		println("processEnded, " + exitValue + CS + signature);
		iMatlabRunning = false;

	}

	public void processNewError(String error, int signature) {
		println("processNewError, " + error + CS + signature);

	}

	public void processNewInput(String input, int signature) {
		println("processNewInput, " + input + CS + signature);

	}

}
