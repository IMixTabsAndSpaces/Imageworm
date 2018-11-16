import java.io.IOException;
import java.text.DecimalFormat;


public class Script2 implements CmdInterface {

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
        sb.append("/net/waterston/vol2/home/biowolp/eclipse/pipelineScripts/" + "biowolp___1269967989031.sh");
        try {
            CmdThread cmdT = CmdThread.exec(this, sb.toString(), SCRIPT2);
            iSCRIPT3running = true;
        } catch (IOException ex) {
            println("runTestScript, IOException, " + ex);
            processNewError(ex.getMessage(), 0);
        }
        println("runTestScript, exiting, ");

	}

	public static final int
	 SCRIPT2 = 1
	;



	/**
	 * @param args
	 */
	public static void main(String[] args) {
		println("Split2.main, entered");
		Script2 s2 = new Script2();
		s2.runTestScript();


	}
    public static void println(String s) {System.out.println(s);}
    private static void print(String s) {System.out.print(s);}
    private static final String CS = ", ";
    private static final DecimalFormat DF0 = new DecimalFormat("####");
    private static final DecimalFormat DF1 = new DecimalFormat("####.#");
    private static final DecimalFormat DF4 = new DecimalFormat("####.####");
    private static String fmt1(double d) {return DF1.format(d);}
    private static String fmt0(double d) {return DF1.format(d);}

    public void processEnded(int exitValue, int signature) {
		println("processEnded, " + exitValue + CS + signature);

	}
	public void processNewError(String error, int signature) {
		println("processNewError, " + error + CS + signature);

	}
	public void processNewInput(String input, int signature) {
		println("processNewInput, " + input + CS + signature);

	}

}
