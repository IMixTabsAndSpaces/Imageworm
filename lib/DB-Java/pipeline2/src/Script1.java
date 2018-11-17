import java.io.File;
import java.io.IOException;
import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class Script1 {

	void makeTestCase() {
		// try taking all files up to GR0010
		long start = System.currentTimeMillis();
		String inDirName = "/nfs/waterston/zhao/20091228nhr-49b";
		String typeInfileName = "20091228nhr-49b_R1_GR001_B1_L1.lsm";
		String newDirName = "/nfs/waterston/biowolp/20091230_nhr-49b_TEST";
		File newDir = new File(newDirName);
		newDir.mkdir();
		Pattern p = Pattern.compile("(.*)(_R.*)(_B.*)(_L.*)");
		File inDir = new File(inDirName);
		String [] list = inDir.list();
		println("makeTestCase, " + list.length);
		//System.exit(0);
		int count = 0;
		for (int i=0; i < list.length; i++) {
			Matcher m = p.matcher(list[i]);
			boolean good = m.matches();
			if (!good) continue;
			String s3 = m.group(3);
			String ns = s3.substring(2,3);
			int n = Integer.parseInt(ns);
			if (n <= 1) {
				String newName = "20091230_nhr-49b_TEST" + m.group(2) + s3 + m.group(4);
				//println("makeTestCase, " + count++ + CS + m.group(0) + CS + newName);
				String inFileName = inDirName + "/" + list[i];
				String outFileName = newDirName + "/" + newName;
				println("makeTestCase,  infile, " + count + CS + inFileName);
				println("makeTestCase, outfile, " + count++ + CS + outFileName);
				//*
				try {
					ChannelCopy.transferCopy(inFileName, outFileName);
				} catch(IOException ioe) {
					ioe.printStackTrace();
					System.exit(0);
				}
				//*/

			}
		}
		println("makeTestCase, elapsed, " + (System.currentTimeMillis() - start)/1000);
	}

	void test1() {
		String dir = "/nfs/waterston/zhao/20091228nhr-49b";
		File d = new File(dir);
		String [] list = d.list();
		for (int i=0; i < 10; i++) {
			String fname = dir + "/" + list[i];
			File ff = new File(fname);
			long lm = ff.lastModified();
			Date date = new Date(lm);
			String DATE_FORMAT_NOW = "yyyy-MM-dd HH:mm:ss";

		    SimpleDateFormat sdf = new SimpleDateFormat(DATE_FORMAT_NOW);

			String sd = sdf.format(date);
			println(i + CS + list[i] + CS + lm + CS + sd);
		}
	}

	void test2() {
		// create an additional test directory with 10 lsm files
		String dir = "/nfs/waterston/zhao/biowolp/20091228nhr-49b";
		File d = new File(dir);
		File pd = d.getParentFile();
		String pds = pd.getAbsolutePath();
		new File(pds + "/test").mkdir();
		String [] list = d.list();
		for (int i=0; i < 10; i++) {
			String fname = dir + "/" + list[i];
			String fname2 = pds + "/test/20091228nhr-49b/" + list[i];
			try {
				ChannelCopy.transferCopy(fname, fname2);
			} catch(IOException ioe) {
				ioe.printStackTrace();
				System.exit(0);
			}

		}


	}

	void splitTest(String dirName) {
		Pattern p = Pattern.compile("(.*)(_R.*)(_L\\d)(.lsm)");
		//String dir = "/nfs/waterston/zhao/biowolp/test/20091228nhr-49b";
		String dir = dirName;
		File d = new File(dir);
		File pd = d.getParentFile();
		String pds = pd.getAbsolutePath();
		//println("splitTest, " + pd);
		String [] list = d.list();
		for (int i=0; i < list.length; i++) {
			println("splitTest, " + i + CS + list[i]);
			Matcher m = p.matcher(list[i]);
			boolean b = m.matches();
			String s1 = pds + "/" + m.group(1) + m.group(3);
			File nd = new File(s1);
			if (!nd.exists()) {
				nd.mkdir();
				File lsm = new File(s1 + "/lsm");
				lsm.mkdir();

			}
			String s2 = dir + "/" + list[i];
			File f = new File(s2);
			String s3 = s1 + "/lsm/" + m.group(1) + m.group(3) +  m.group(2) + m.group(4);
			//println("splitTest, " + s3);
			f.renameTo(new File(s3));;
			//println("splitTest, 1, " + i + CS + s1);
			//println("splitTest, 2, " + i + CS + s2);
			//println("splitTest, 3, " + i + CS + s3);
		}
		d.deleteOnExit();

	}

	void regexTest() {
		String e = "20091228nhr-49b_R1_GR001_B1_L1.lsm";
		//Pattern p = Pattern.compile("(.*_L)(\\d)(.lsm)");
		Pattern p = Pattern.compile("(.*)(_R.*)(_L\\d)(.lsm)");
		Matcher m = p.matcher(e);
		boolean b = m.matches();
		println("regexTest, " + b);
		println(m.group(0));
		println(m.group(1));
		println(m.group(2));
		println(m.group(3));
		println(m.group(4));
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		println("Script1.main, ");
		Script1 s1 = new Script1();
		//s1.splitTest();
		//s1.regexTest();
		//s1.test2();
		//s1.makeTestCase();
		s1.splitTest(args[0]);

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
