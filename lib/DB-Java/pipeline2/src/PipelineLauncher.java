import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.Border;

import org.rhwlab.manifest.ManifestX;


public class PipelineLauncher implements ActionListener, ItemListener, CmdInterface, Runnable {

    JPanel 				iMainPanel;
    JTextField  		iEmailAddress;
    JTextField  		iSeriesDirectory;
    JButton     		iSeriesButton;
    String      		iFileOpenPath;
    String				iSeriesName;
    String				iCustomParametersFile;
    String				iCustomMatlabParametersFile;
    JButton				iRunButton;
    boolean				iRunning;

    Border				iBlackline;

    JCheckBox			iLSMSeparation;
    JCheckBox			iLSMProcessing;
    JCheckBox			iDirSwapping;
    JCheckBox			iStarryNite;
    JCheckBox			iMatlab;
    JCheckBox			iSwitchChannels;
    JCheckBox			iCustomParameters;
    JCheckBox			iCustomMatlabParameters;
    JTextField  		iParametersFile;
    JTextField  		iMatlabParametersFile;
    JTextField			iCustomEndTime;
    JTextField			iCustomMatlabEndTime;
    JButton     		iLocateParametersFile;
    JButton     		iLocateMatlabParametersFile;
    JLabel				iCustomParametersLabel;
    JLabel				iCustomMatlabParametersLabel;
    JCheckBox			iMakeDBEntry;
    JCheckBox			iExtractRed;
    JCheckBox			iMeasure;
    JCheckBox			iGenerateCSVFiles;

    boolean				iDisableControls;

    boolean				iRunOnLocalMachine;

    String				iUser;

    int					iLife;

    String				iSeriesDefaultDir;
    String				iToolsLocation;





    static JFrame      iFrame;
    static String []   cArgs;
    static int		   PIPELINE = 1;
    static int         LABELWIDTH = 130;

	public PipelineLauncher() {
		if (cArgs.length > 1) {
			iRunOnLocalMachine = true;
			LIFETIME = 600000;
		}
        iMainPanel = new JPanel();
        iMainPanel.setLayout(new BoxLayout(iMainPanel, BoxLayout.PAGE_AXIS));
        iMainPanel.setMinimumSize(new Dimension(250, 450));
        //iMainPanel.setPreferredSize(new Dimension(200, 100));
        iBlackline = BorderFactory.createLineBorder(Color.black);

        iDisableControls = true;

        JPanel rp = new JPanel();
        rp.setBorder(iBlackline);
        JLabel lab2 = new JLabel("Email address");
        lab2.setPreferredSize(new Dimension(LABELWIDTH, 50));
        iEmailAddress = new JTextField("", 20);
        rp.add(lab2, BorderLayout.WEST);
        rp.add(iEmailAddress, BorderLayout.CENTER);
        iMainPanel.add(rp);

        if (cArgs.length > 0) iEmailAddress.setText(cArgs[0]);

        rp = new JPanel();
        rp.setBorder(iBlackline);
        JLabel lab = new JLabel("Series directory");
        lab.setPreferredSize(new Dimension(LABELWIDTH, 30));
        iSeriesDirectory = new JTextField("", 20);
        JButton jb = new JButton();
        iSeriesButton = new JButton("Browse");
        iSeriesButton.addActionListener(this);
        rp.add(lab, BorderLayout.WEST);
        rp.add(iSeriesDirectory, BorderLayout.CENTER);
        rp.add(iSeriesButton);
        iMainPanel.add(rp);

        addLSMSeparation();
        addLSMProcessing();
        addDirSwapping();
        addStarryNiteProcessing();
        addMatlabProcessing();
        addMeasure();
        addMakeDBEntry();
        addExtractRed();
        addCSVGeneration();

        rp = new JPanel(new FlowLayout());
        rp.setBorder(iBlackline);
        iRunButton = new JButton("Run");
        iRunButton.addActionListener(this);
        rp.add(iRunButton);
        iMainPanel.add(rp);
        enableControls(false);
        iRunButton.setEnabled(false);
        iLSMSeparation.setEnabled(false);
        iLife = LIFETIME;
        new Thread(this).start();


	}

	void enableControls(boolean enable) {
		iLSMProcessing.setEnabled(enable);
		iDirSwapping.setEnabled(enable);

		iStarryNite.setEnabled(enable);
		iCustomParametersLabel.setEnabled(enable);
		iLocateParametersFile.setEnabled(enable);

		iMatlab.setEnabled(enable);
		iCustomMatlabParametersLabel.setEnabled(enable);
		iLocateMatlabParametersFile.setEnabled(enable);

		iMakeDBEntry.setEnabled(enable);
		iExtractRed.setEnabled(enable);
		iMeasure.setEnabled(enable);
		iGenerateCSVFiles.setEnabled(enable);
	}

	void addLSMSeparation() {
		JPanel rp = new JPanel();
        rp.setLayout(new GridLayout(0,1));
        rp.setBorder(iBlackline);
        iLSMSeparation = new JCheckBox("     separate lsms to individual series ", false);
        iLSMSeparation.addItemListener(this);
        rp.add(iLSMSeparation);
        iMainPanel.add(rp);

	}

	void addDirSwapping() {
		JPanel rp = new JPanel();
        rp.setLayout(new GridLayout(0,1));
        rp.setBorder(iBlackline);
        iDirSwapping = new JCheckBox("     swap tif and tifR directories ", false);
        iDirSwapping.addItemListener(this);
        rp.add(iDirSwapping);
        iMainPanel.add(rp);

	}


	void addLSMProcessing() {
		JPanel rp = new JPanel();
        rp.setLayout(new GridLayout(0,1));
        rp.setBorder(iBlackline);
        iLSMProcessing = new JCheckBox("     process lsm files ", false);
        rp.add(iLSMProcessing);
        iMainPanel.add(rp);
	}

	void addStarryNiteProcessing() {
		JPanel rp = new JPanel();
        rp.setLayout(new GridLayout(0,1));
        rp.setBorder(iBlackline);
        iStarryNite = new JCheckBox("     run starrynite ", false);
        iStarryNite.addItemListener(this);
        rp.add(iStarryNite);
        //iSwitchChannels = new JCheckBox("     switch channels ", false);
        //rp.add(iSwitchChannels);
        JPanel p = new JPanel();
        iCustomParametersLabel = new JLabel("Custom parameters");
        iCustomParametersLabel.setPreferredSize(new Dimension(LABELWIDTH, 30));
        iParametersFile = new JTextField("", 20);
        iLocateParametersFile = new JButton("Browse");
        iLocateParametersFile.addActionListener(this);
        p.add(iCustomParametersLabel, BorderLayout.WEST);
        p.add(iParametersFile, BorderLayout.CENTER);
        p.add(iLocateParametersFile);
        rp.add(p);
        iMainPanel.add(rp);

	}

	void addMatlabProcessing() {
		JPanel rp = new JPanel();
        rp.setLayout(new GridLayout(0,1));
        rp.setBorder(iBlackline);
        iMatlab = new JCheckBox("     run matlab ", false);
        iMatlab.addItemListener(this);
        rp.add(iMatlab);
        //iSwitchChannels = new JCheckBox("     switch channels ", false);
        //rp.add(iSwitchChannels);

        JPanel p = new JPanel();
        //p.setLayout(new GridLayout(1,3));
        /*
        JLabel customMatlabEndTimeLabel = new JLabel("Custom endtime");
        customMatlabEndTimeLabel.setPreferredSize(new Dimension(LABELWIDTH, 30));
        iCustomMatlabEndTime = new JTextField("", 5);
        iCustomMatlabEndTime.setPreferredSize(new Dimension(LABELWIDTH, 20));
        p.add(customMatlabEndTimeLabel, BorderLayout.WEST);
        p.add(iCustomMatlabEndTime, BorderLayout.CENTER);
        JLabel dummy = new JLabel("        ");
        dummy.setPreferredSize(new Dimension(LABELWIDTH, 30));
        dummy.setPreferredSize(new Dimension(240, 30));
        p.add(dummy, BorderLayout.EAST);
        rp.add(p);

        p = new JPanel();
        */
        iCustomMatlabParametersLabel = new JLabel("Custom matlab parameters");
        iCustomMatlabParametersLabel.setPreferredSize(new Dimension(LABELWIDTH, 30));
        iMatlabParametersFile = new JTextField("", 20);
        iLocateMatlabParametersFile = new JButton("Browse");
        iLocateMatlabParametersFile.addActionListener(this);
        p.add(iCustomMatlabParametersLabel, BorderLayout.WEST);
        p.add(iMatlabParametersFile, BorderLayout.CENTER);
        p.add(iLocateMatlabParametersFile);
        rp.add(p);
        iMainPanel.add(rp);


	}

	void addMeasure() {
		JPanel rp = new JPanel();
        rp.setLayout(new GridLayout(0,1));
        rp.setBorder(iBlackline);
        iMeasure = new JCheckBox("     measure embryo ", false);
        rp.add(iMeasure);
        iMainPanel.add(rp);

	}

	void addMakeDBEntry() {
		JPanel rp = new JPanel();
        rp.setLayout(new GridLayout(0,1));
        rp.setBorder(iBlackline);
        iMakeDBEntry = new JCheckBox("     make db entry ", false);
        rp.add(iMakeDBEntry);
        iMainPanel.add(rp);

	}

	void addExtractRed() {
		JPanel rp = new JPanel();
        rp.setLayout(new GridLayout(0,1));
        rp.setBorder(iBlackline);
        iExtractRed = new JCheckBox("     extract red ", false);
        rp.add(iExtractRed);
        iMainPanel.add(rp);

	}

	void addCSVGeneration() {
		JPanel rp = new JPanel();
        rp.setLayout(new GridLayout(0,1));
        rp.setBorder(iBlackline);
        iGenerateCSVFiles = new JCheckBox("     generate csv files ", false);
        rp.add(iGenerateCSVFiles);
        iMainPanel.add(rp);

	}

    private void seriesButton() {
        println("seriesButton");
        iLife = LIFETIME;
        enableControls(true);
        iLSMSeparation.setEnabled(true);
        iRunButton.setEnabled(true);
        if (iSeriesDefaultDir == null) {
        	String defaultDir = ManifestX.getManifestValue("DBLocation");
        	File f = new File(defaultDir);
        	iSeriesDefaultDir = f.getParent();
        	iToolsLocation = ManifestX.getManifestValue("ToolsLocation");

        }

        JFileChooser fc = new JFileChooser(iSeriesDefaultDir);
        fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

        int returnVal = fc.showOpenDialog(null);
        if(returnVal == JFileChooser.APPROVE_OPTION) {
            String path = fc.getSelectedFile().getPath();
            iSeriesDirectory.setText(path);
            File f = new File(path);
            iFileOpenPath = path;
            println("You chose to open this file: " + path);
            int k = path.lastIndexOf("/");
            iSeriesName = path.substring(k + 1);
            println("on this series: " + iSeriesName);

        }
    }


    private void runButton() {
    	println("runButton, entered, ");
    	iLife = LIFETIME;
    	int k = checkSelections();
    	if (k != 0) {
    		StringBuffer sb = new StringBuffer("ERROR:\n");
    		if (k == 1 || k == 11) sb.append("bad email address\n");
    		if (k > 1) sb.append("bad series directory");
    		JOptionPane.showMessageDialog(null, sb.toString());
    		return;

    	}

    	if (iStarryNite.isSelected()) {

        	processSpecialStarryNiteFeatures();
    	}

    	if (iMatlab.isSelected()) {

        	processSpecialMatlabFeatures();
    	}


    	String runScriptName = generateRunScriptName();


    	prepareRunScript(runScriptName, createRunScript());
    	try {
    		Runtime.getRuntime().exec("chmod 777 " + runScriptName);
    	} catch(Exception e) {
    		e.printStackTrace();
    	}
        try {Thread.sleep(1000);} catch(Exception e){e.printStackTrace();}

    	StringBuffer sb = new StringBuffer("");
    	String clusterStart = "qsub -M " + iEmailAddress.getText() + SP;
    	String localStart = "./";
    	if (iRunOnLocalMachine) sb.append(localStart);
    	else sb.append(clusterStart);
    	sb.append(runScriptName + SP + iFileOpenPath + SP + iSeriesName);
    	println("runButton, " + sb.toString());
        try {
            CmdThread cmdT = CmdThread.exec(this, sb.toString(), PIPELINE);
            System.out.println(" running..\n" + sb.toString());
            iRunning = true;
            iRunButton.setEnabled(false);
        } catch (IOException ex) {
            processNewError(ex.getMessage(), PIPELINE);
        }
        trashTrash(iUser);

    }

    private static final String TIMESEP = "___";

    String generateRunScriptName() {
    	String emailAddr = iEmailAddress.getText();
    	int k = emailAddr.indexOf("@");
    	String user = emailAddr.substring(0, k);
    	long time = System.currentTimeMillis();
    	String stime = String.valueOf(time);
    	println("generateRunScriptName, " + stime + CS + stime.length() + CS + user);
    	iUser = user;
    	return user + TIMESEP + stime + ".sh";
    }

    void processSpecialStarryNiteFeatures() {
    	if (iFileOpenPath == null) {
    		iFileOpenPath = iSeriesDirectory.getText();
            int k = iFileOpenPath.lastIndexOf("/");
            iSeriesName = iFileOpenPath.substring(k + 1);
    	}
    	println("processSpecialStarryNiteFeatures, " + iFileOpenPath);
    	String parametersFile = iParametersFile.getText();
		String dst = iFileOpenPath + "/snParams";
    	if (parametersFile.length() > 0) {
    		String src = parametersFile;
    		try {
				ChannelCopy.transferCopy(src, dst);
			} catch(IOException ioe) {
				ioe.printStackTrace();
				//System.exit(0);

    		}
    	}
    }


    void processSpecialMatlabFeatures() {
    	if (iFileOpenPath == null) {
    		iFileOpenPath = iSeriesDirectory.getText();
            int k = iFileOpenPath.lastIndexOf("/");
            iSeriesName = iFileOpenPath.substring(k + 1);
    	}
    	println("processSpecialStarryNiteFeatures, " + iFileOpenPath);
		String dst = iFileOpenPath + "/matlabParams";
    	String parametersFile = iMatlabParametersFile.getText();
    	if (parametersFile.length() > 0) {
    		String src = parametersFile;
    		try {
				ChannelCopy.transferCopy(src, dst);
			} catch(IOException ioe) {
				ioe.printStackTrace();
    		}
    	}

    }

    void checkEndTime() {

    }


    int checkSelections() {
    	int k = 0;
    	String email = iEmailAddress.getText();
    	if (email.length() == 0) k++;
    	String seriesDir = iSeriesDirectory.getText();
    	if (seriesDir.length() == 0) k += 10;
    	else {
    		File sdf = new File(seriesDir);
    		if (!sdf.exists()) k += 10;
    	}
    	return k;

    }



    Vector createRunScript() {
    	Vector v = new Vector();
    	v.add("echo \"launch basicPipeline\"");
    	v.add("echo $1");
    	v.add("echo $2");
    	v.add("cd " + iToolsLocation);
    	//v.add("cd /nfs/waterston/tools3");
    	if (iLSMSeparation.isSelected()) {
    		v.add("./splitter.pl $1");
    		v.add("./sageProcessingComplete.pl");
    		return v;
    	}
    	if (iLSMProcessing.isSelected()) {
    		v.add("./extractTif.pl $1");
    		v.add("./getLSMInfo.pl $1");
    		v.add("./deleteLSMFiles.pl $1");
    	}
    	if (iDirSwapping.isSelected()) {
    		v.add("./swapDirectories.pl $1");
    	}
    	if (iStarryNite.isSelected()) {
    		v.add("./snrunner.pl $1");
    	}
    	if (iMatlab.isSelected()) {
    		v.add("./matlabRunner.pl $1");
    	}
    	if (iMakeDBEntry.isSelected()) {
    		v.add("./makeDBEntry.pl $1");
    	}
    	if (iExtractRed.isSelected()) {
    		v.add("./acebatch3.pl RedExtractor1 $2");
    	}
    	if (iMeasure.isSelected()) {
    		v.add("./acebatch3.pl Measure1 $2");
    	}
    	if (iGenerateCSVFiles.isSelected()) {
    		v.add("./acebatch3.pl RedExcel1 $2");
    		v.add("./acebatch3.pl RedExcel2 $2");
    		v.add("./acebatch3.pl Align1 $2");
    	}
		v.add("./sageProcessingFinished.pl");

    	return v;
    }

    void prepareRunScript(String scriptName, Vector v) {
    	boolean activePart = true;;
    	try {
    		FileOutputStream fos = new FileOutputStream(scriptName);
    		PrintWriter pw = new PrintWriter(fos, true);
    		FileInputStream fis = new FileInputStream("pipelineTest.txt");


    		BufferedReader br = new BufferedReader(new InputStreamReader(fis));
    		while(br.ready()) {
    			String s = br.readLine();
    			if (s.indexOf("MARKER1") > 0) {
    				println("prepareRunScript, MARKER1 found, ");
    				pw.println(s);
    				for (int i=0; i < v.size(); i++) {
    					s = (String)v.get(i);
    					pw.println(s);
    				}
    				activePart = false;
    			}
    			if (activePart) pw.println(s);
    			if (s.indexOf("MARKER2") > 0) {
    				println("prepareRunScript, MARKER2 found, ");
    				activePart = true;
    				pw.println(s);
    			}
    		}

    		fis.close();
    		pw.close();

    	} catch(IOException ioe) {
    		ioe.printStackTrace();
    	}
    }

    void getCustomParametersFile() {
        println("starryniteButton");
        JFileChooser fc = new JFileChooser(iSeriesDefaultDir);
        fc.setFileSelectionMode(JFileChooser.FILES_ONLY);

        int returnVal = fc.showOpenDialog(null);
        if(returnVal == JFileChooser.APPROVE_OPTION) {
            String path = fc.getSelectedFile().getPath();
            iCustomParametersFile = path;
            iParametersFile.setText(path);
            println("Using custom parameters file, " + path);

        }

    }

    void getCustomMatlabParametersFile() {
        println("matlabButton");
        JFileChooser fc = new JFileChooser(iSeriesDefaultDir);
        fc.setFileSelectionMode(JFileChooser.FILES_ONLY);

        int returnVal = fc.showOpenDialog(null);
        if(returnVal == JFileChooser.APPROVE_OPTION) {
            String path = fc.getSelectedFile().getPath();
            iCustomMatlabParametersFile = path;
            iMatlabParametersFile.setText(path);
            println("Using custom matlab parameters file, " + path);

        }


    }

    private static final long OLDSTUFF = 300000000;
    public static void trashTrash(String user) {
    	println("checking for obsolete shell scripts and logs for " + user);
    	long now = System.currentTimeMillis();
    	File dir = new File(".");
    	String [] sa = dir.list();
    	for (int i=0; i < sa.length; i++) {
    		String s = sa[i];
    		if (s.startsWith(user)) {
    			String ss = s.substring(user.length() + TIMESEP.length());
    			ss = ss.substring(0, ss.indexOf("."));
        		long interval = now - Long.parseLong(ss);
        		//println("trashTrash, " + s + CS + ss + CS + interval);
        		if (interval > OLDSTUFF) {
        			new File(s).delete();
            		println("deleting, " + s);

        		}
    		}
    	}
    }

    private static void createAndShowGUI() {
        println("createAnsShowGUI: ");
        //Make sure we have nice window decorations.
        JFrame.setDefaultLookAndFeelDecorated(true);

        //Create and set up the window.
        iFrame = new JFrame("PipelineLauncher");
        iFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        //Create and set up the content pane.
        PipelineLauncher pl = new PipelineLauncher();
        //if (cArgs.length > 3) {
        //    pl.setTextFields(cArgs);
            //snl.runStarryNite();
        //}


        JPanel newContentPane = pl.iMainPanel;
        newContentPane.setOpaque(true); //content panes must be opaque
        iFrame.setContentPane(newContentPane);

        //Display the window.
        iFrame.pack();
        //frame.setSize(snl.iMainPanel.getSize());
        iFrame.setVisible(true);
    }



	/**
	 * @param args
	 */
	public void actionPerformed(ActionEvent e) {
        Object o = e.getSource();
        if (o == iSeriesButton) seriesButton();
        if (o == iRunButton) runButton();
        if (o == iLocateParametersFile) getCustomParametersFile();
        if (o == iLocateMatlabParametersFile) getCustomMatlabParametersFile();

	}

	public void processEnded(int exitValue, int signature) {
		println("processEnded, " + signature + CS + exitValue);
		iLife = LIFETIME;

	}

	public void processNewError(String error, int signature) {
		println("processNewError, " + signature + CS + error);

	}

	public void processNewInput(String input, int signature) {
		if (input.length() > 0) println("processNewInput, " + signature + CS + input);
		iLife = LIFETIME;

	}

	public void itemStateChanged(ItemEvent e) {
		Object source = e.getItemSelectable();

	    if (source == iLSMSeparation) {
	    	iDisableControls = !iDisableControls;
	    	enableControls(iDisableControls);

	    }
	    if (source == iStarryNite) {
	    	if (iStarryNite.isSelected()) iMatlab.setEnabled(false);
	    	else iMatlab.setEnabled(true);
	    	iMeasure.setSelected(true);
    		File p = new File(iFileOpenPath + "/snParams");
    		if (!p.exists()) {
    	        String defaultParms = ManifestX.getManifestValue("SNDefaultParms");
    			String dst = iFileOpenPath + "/snParams";
        		try {
    				ChannelCopy.transferCopy(defaultParms, dst);
    			} catch(IOException ioe) {
    				ioe.printStackTrace();
        		}

    		}
	    }

	    if (source == iMatlab) {
	    	if (iMatlab.isSelected()) iStarryNite.setEnabled(false);
	    	else iStarryNite.setEnabled(true);
	    	iMeasure.setSelected(true);
    		File p = new File(iFileOpenPath + "/matlabParams");
    		if (!p.exists()) {
    	        String defaultParms = ManifestX.getManifestValue("MatlabDefaultParams");
    			String dst = iFileOpenPath + "/matlabParams";
        		try {
    				ChannelCopy.transferCopy(defaultParms, dst);
    			} catch(IOException ioe) {
    				ioe.printStackTrace();
        		}

    		}
	    }

	}

	private static int LIFETIME = 600;
    public void run() {
	    while (iLife-- > 0) {
	        try{
	           Thread.sleep(1000);
	        } catch( InterruptedException e ) {
	            System.out.println("Interrupted Exception caught");
	        }
	    }
	    println("run, timeout, ");
	    System.exit(0);

	}


	public static void main(String[] args) {
		println("PipelineLauncher.main, ");
        cArgs = args;
        //createAndShowGUI();
        ManifestX.reportAndUpdateManifest();

        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });


	}
    public static void println(String s) {System.out.println(s);}
    private static void print(String s) {System.out.print(s);}
    private static final String CS = ", ", SP = " ";
    private static final DecimalFormat DF0 = new DecimalFormat("####");
    private static final DecimalFormat DF1 = new DecimalFormat("####.#");
    private static final DecimalFormat DF4 = new DecimalFormat("####.####");
    private static String fmt1(double d) {return DF1.format(d);}
    private static String fmt0(double d) {return DF1.format(d);}



}
