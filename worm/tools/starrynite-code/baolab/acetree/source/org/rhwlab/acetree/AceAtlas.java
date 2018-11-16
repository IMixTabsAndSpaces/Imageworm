package org.rhwlab.acetree;
import javax.swing.UIManager.*;
import javax.swing.UIManager;
import java.io.IOException;
import javax.swing.UnsupportedLookAndFeelException;
import java.awt.Color;
import javax.swing.JFrame;
import java.awt.BorderLayout;
import java.awt.Toolkit;
import java.awt.Dimension;
import org.rhwlab.image.Image3D2AceAtlas;
import org.rhwlab.manifest.ManifestX;
import java.io.File;
import java.net.URLDecoder;

public class AceAtlas extends AceTree{
    

    public AceAtlas(String path, String file){
	super(path+file,false);
	//	runningFrom=path;
	
    }
    // override to not make main frame visible
    protected void createAndShowGUI() {
        //skip ui except for setting up variables
        iMainFrame = new JFrame(TITLE);   
        iAceMenuBar = new AceMenuBar(this);
        iWinEvtHandler = new WindowEventHandler();
	//	iMainFrame.setVisible(true);
	iPlayerControl=new PlayerControlAceAtlas(this);
	try {
	    iImage3D2 = new Image3D2AceAtlas(this, "AceAtlas");
	    //iImage3D2.insertContent(s);
	} catch(NoClassDefFoundError ee) {
	    System.out.println("you need to install Java3D");
	  
	}
    }

    

    public static void main(String[] args) throws IOException {
	boolean setui=false;
       
	try {
	    for (LookAndFeelInfo info : UIManager.getInstalledLookAndFeels()) {
		if ("Nimbus".equals(info.getName())) {
		    UIManager.setLookAndFeel(info.getClassName());
		    UIManager.put("control", new Color(255,255,255));
		    setui=true;
		    break;
		}
	    }
	    if(!setui){
		UIManager.setLookAndFeel(
					 UIManager.getSystemLookAndFeelClassName());
	    }
		
	} catch (UnsupportedLookAndFeelException e) {
	    // handle exception
	} catch (ClassNotFoundException e) {
	    // handle exception
	} catch (InstantiationException e) {
	    // handle exception
	} catch (IllegalAccessException e) {
	    // handle exception
	}

	//	ManifestX.reportAndUpdateManifest();
        String config = null;
        AceTree ot;
        //System.exit(1);
	ot=null;
        if (args.length > 0) {
            System.out.println("AceAtlas args[0]: " + args[0]);
            ot = new AceAtlas("",args[0]);
	    ot.run("");
        ot.debugTest(false);
	    
	    //ot.threeDview2();
	    //	ot.getMainFrame().setVisible(false);
        }
        else {
	    try{
		System.out.println("AceAtlas no arg");
		//	String pathname=new File(AceView.class.getProtectionDomain().getCodeSource().getLocation().toURI()).getName();
		//String pathname=ClassLoader.getSystemClassLoader().getResource(".").getPath();

		String pathname=  AceAtlas.class.getProtectionDomain().getCodeSource().getLocation().getPath();
		pathname=URLDecoder.decode(pathname, "UTF-8");
		
		//	String pathname=URLDecoder.decode(ClassLoader.getSystemClassLoader().getResource(".").getPath(),"UTF-8");
		System.out.println(pathname            );//
		//assumption about jar name embedded here
		if (pathname.endsWith(".jar"))
		    pathname=pathname.substring(0,pathname.length()-12);
		
		//athname=(new File(pathname)).getParentFile().getPath();
		String defaultfile="20120401_JIM122_L3.xml";
		System.out.println(pathname+defaultfile);
		//pathname=pathname.substring(1,pathname.length-11);
		try{
		    Thread.sleep(2000);}
		catch(Exception e){}
		//	System.out.println(pathname+defaultfile);
		ot = new AceAtlas(pathname,defaultfile);
		ot.run("");
		ot.debugTest(false);
	    }catch (Exception e){
		System.out.println(e);
	    }
	    
	}
       
	// ot.iCmdLineRun = true;
        System.out.println("main exiting");
	try{
	    Thread.sleep(20000);}
	catch(Exception e){}
    }

}