import java.io.IOException;
import java.text.DecimalFormat;



public class Script3 implements CmdInterface {

	boolean 	iSCRIPT3running;

	public void run() {
        println("run, SCRIPT3 starting, ");
        runTestScript();
        boolean waiter = true;
        while (waiter) {
            try {Thread.sleep(1);} catch(Exception e){e.printStackTrace();}
            waiter = iSCRIPT3running;
        }
        println("run, ending");

	}

	void runTestScript() {
        println("runTestScript, entered, ");
        StringBuffer sb = new StringBuffer();
        //sb.append("java -jar LSMInfo.jar ");
        String [] sa = new String[0];
        sb.append("java -jar pScripts.jar");
        try {
            CmdThread cmdT = CmdThread.exec(this, sb.toString(), SCRIPT3);
            iSCRIPT3running = true;
        } catch (IOException ex) {
            println("runLSMInfo, IOException, " + ex);
            processNewError(ex.getMessage(), 0);
        }
        println("runTestScript, exiting, ");

	}


	public void processEnded(int exitValue, int signature) {
        println("processEnded, " + exitValue + CS + signature);
        switch(signature) {
            case SCRIPT3:
                iSCRIPT3running = false;
                break;
            default:
                System.exit(0);
        }

	}
	public void processNewError(String error, int signature) {
		println("processNewError, " + error);

	}
	public void processNewInput(String input, int signature) {
		println("processNewInput, " + input);

	}

	public static final int
		 SCRIPT3 = 1
		;

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		println("Script3.main, ");
		Script3 s3 = new Script3();
		s3.run();

	}
    public static void println(String s) {System.out.println(s);}
    private static void print(String s) {System.out.print(s);}
    private static final String CS = ", ";
    private static final DecimalFormat DF0 = new DecimalFormat("####");
    private static final DecimalFormat DF1 = new DecimalFormat("####.#");
    private static final DecimalFormat DF4 = new DecimalFormat("####.####");
    private static String fmt1(double d) {return DF1.format(d);}
    private static String fmt0(double d) {return DF1.format(d);}
}
