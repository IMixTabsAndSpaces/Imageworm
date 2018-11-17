/*
 * Created on Nov 4, 2006
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package org.rhwlab.image;


import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GraphicsConfiguration;
import java.awt.GridLayout;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.image.BufferedImage;
//import java.awt.image.Raster;
import javax.media.j3d.Raster;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.util.Collections;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import javax.imageio.ImageIO;
import javax.media.j3d.Appearance;
import javax.media.j3d.Background;
import javax.media.j3d.BoundingSphere;
import javax.media.j3d.BranchGroup;
import javax.media.j3d.Canvas3D;
import javax.media.j3d.DirectionalLight;
import javax.media.j3d.GraphicsContext3D;
import javax.media.j3d.ImageComponent;
import javax.media.j3d.ImageComponent2D;
import javax.media.j3d.Light;
import javax.media.j3d.Material;
import javax.media.j3d.Transform3D;
import javax.media.j3d.TransformGroup;
import javax.media.j3d.TransparencyAttributes;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTabbedPane;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.border.Border;
import javax.vecmath.Color3f;
import javax.vecmath.Matrix4d;
import javax.vecmath.Point3d;
import javax.vecmath.Point3f;
import javax.vecmath.Vector3d;
import javax.vecmath.Vector3f;

import org.rhwlab.acetree.AceTree;
import org.rhwlab.snight.Config;
import org.rhwlab.snight.NucleiMgr;
import org.rhwlab.snight.Nucleus;
import org.rhwlab.tree.Cell;
import org.rhwlab.tree.CellData;

import qdxml.DocHandler;
import qdxml.QDParser;

import com.sun.j3d.utils.behaviors.mouse.MouseRotate;
import com.sun.j3d.utils.geometry.Primitive;
import com.sun.j3d.utils.geometry.Sphere;
import com.sun.j3d.utils.picking.PickCanvas;
import com.sun.j3d.utils.picking.PickResult;
import com.sun.j3d.utils.universe.SimpleUniverse;
import com.sun.j3d.utils.universe.ViewingPlatform;
import javax.media.j3d.GraphicsConfigTemplate3D;
import java.awt.GraphicsEnvironment;
import javax.media.j3d.View;
/**
 * @author biowolp
 *
 */

public class Image3D2AceAtlas extends Image3D2 {

    public Image3D2AceAtlas(AceTree aceTree, String title) {
        iAceTree = aceTree;
        iNucleiMgr = iAceTree.getNucleiMgr();
        iFrame = new JFrame(title);
        iTitle = title;
        iNewConstruction = true;
        iMinRed = MINRED;
        iMaxRed = MAXRED;
        iUseExpression = false;
        iFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        //iFrame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
	//  WinEventMgr wem = new WinEventMgr();
	//  iFrame.addWindowListener(wem);
       // GraphicsConfiguration config = SimpleUniverse.getPreferredConfiguration();

        
        
        GraphicsConfigTemplate3D template = new GraphicsConfigTemplate3D();
        template.setSceneAntialiasing(template.PREFERRED);
        GraphicsConfiguration config =
                    GraphicsEnvironment.getLocalGraphicsEnvironment().
                     getDefaultScreenDevice().getBestConfiguration(template);
       
        
        iCanvas = new Canvas3D(config);
        iCanvas.setSize(ImageWindow.cImageWidth, ImageWindow.cImageHeight);
        iUniverse = new SimpleUniverse(iCanvas);
        
        iUniverse.getViewer().getView().setSceneAntialiasingEnable(true);
        
        
        iUniverse.getViewingPlatform().setNominalViewingTransform();

        iOfflineCanvas = new Canvas3D(config, true);
        iOfflineCanvas.setSize(ImageWindow.cImageWidth, ImageWindow.cImageHeight);
        iOfflineUniverse = new SimpleUniverse(iOfflineCanvas);
        iOfflineUniverse.getViewingPlatform().setNominalViewingTransform();


        ViewingPlatform viewingPlatform = iUniverse.getViewingPlatform( );

        iTranslateGroup = viewingPlatform.getViewPlatformTransform( );
        iMatrix  = new Matrix4d( );
        Transform3D t3d = new Transform3D();
        iTranslateGroup.getTransform(t3d);
        Matrix4d m4d = new Matrix4d();
        /*
        t3d.get(m4d);
        println("m4d, " + m4d);
        println("t3d, " + t3d);
        m4d.m23 = .554; // near center of SimpleUniverse
        t3d.set(m4d);
        iTranslateGroup.setTransform(t3d);
        */
        buildOutUI();

        iTabbedPane = new JTabbedPane();
        iTabbedPane.addTab("Image", null, iImagePanel, "View 3D image");

        Object dispProps = iAceTree.getDispProps3D2();
        if (dispProps == null) iAceTree.setDispProps3D2((Object)getDisplayProps());
        iDispProps2 = (SublineageDisplayProperty [])iAceTree.getDispProps3D2();

        iPT2 = new PropertiesTab2(this);
        iControlPanel = iPT2.getPanel();
        iTabbedPane.addTab("Properties", null, iControlPanel, "Set color scheme");

        iFrame.getContentPane().add(iTabbedPane);

        iCanvas.addMouseListener(this);
        iFrame.pack();
        iFrame.show();
        iUndo = new Vector();
        insertContent(iTitle);
        iSaveImageAsDir = "";
        iLastSaveAsName = "";
        setKeyboardActions();//attempt to add keyboard controls to this
        
    }

    protected void buildOutUI() {
        iPick = new JLabel("pick");
        iImagePanel = new JPanel();
        iImagePanel.setLayout(new BorderLayout());
        iImagePanel.add(iCanvas, "Center");
        JPanel secondPanel = new JPanel(new BorderLayout());

        JPanel newPanel = new JPanel();
        newPanel.setLayout(new BoxLayout(newPanel, BoxLayout.PAGE_AXIS));
	newPanel.add(iAceTree.getPlayerControl());

        JPanel rotatePanels = new JPanel();
        rotatePanels.setLayout(new GridLayout(0, 1));
        JPanel rotatePanel = new JPanel();

        rotatePanel = new JPanel();
        rotatePanel.setLayout(new GridLayout(1, 0));
        rotatePanel.add(iPick);
        rotatePanels.add(rotatePanel);

        rotatePanel = new JPanel();
        rotatePanel.setLayout(new GridLayout(1, 0));
        rotatePanel.add(new JLabel("angX"));
        iAngXInc = new JTextField("30", 5);
        iAngX = new JTextField("0", 10);
        iXUp = new JButton("up");
        iXDn = new JButton("dn");
        rotatePanel.add(iAngXInc);
        rotatePanel.add(iAngX);
        rotatePanel.add(iXUp);
        rotatePanel.add(iXDn);
        iXUp.addActionListener(this);
        iXDn.addActionListener(this);
        rotatePanels.add(rotatePanel);

        rotatePanel = new JPanel();
        rotatePanel.setLayout(new GridLayout(1, 0));
        rotatePanel.add(new JLabel("angY"));
        iAngYInc = new JTextField("30", 5);
        iAngY = new JTextField("0", 10);
        iYUp = new JButton("up");
        iYDn = new JButton("dn");
        rotatePanel.add(iAngYInc);
        rotatePanel.add(iAngY);
        rotatePanel.add(iYUp);
        rotatePanel.add(iYDn);
        iYUp.addActionListener(this);
        iYDn.addActionListener(this);
        rotatePanels.add(rotatePanel);

        rotatePanel = new JPanel();
        rotatePanel.setLayout(new GridLayout(1, 0));
        rotatePanel.add(new JLabel("angZ"));
        iAngZInc = new JTextField("30", 5);
        iAngZ = new JTextField("0", 10);
        iZUp = new JButton("up");
        iZDn = new JButton("dn");
        rotatePanel.add(iAngZInc);
        rotatePanel.add(iAngZ);
        rotatePanel.add(iZUp);
        rotatePanel.add(iZDn);
        iZUp.addActionListener(this);
        iZDn.addActionListener(this);
        rotatePanels.add(rotatePanel);

        rotatePanel = new JPanel();
        rotatePanel.setLayout(new GridLayout(1, 0));
        rotatePanel.add(new JLabel("Pos"));

        Transform3D t3d = new Transform3D();
        iTranslateGroup.getTransform(t3d);
        t3d.get(iMatrix);

        iPosIncr = new JTextField("0.2", 5);
        iPos = new JTextField(fmt1(iMatrix.m23), 10);
        iPIn = new JButton("in");
        iPOut = new JButton("out");
        rotatePanel.add(iPosIncr);
        rotatePanel.add(iPos);
        rotatePanel.add(iPIn);
        rotatePanel.add(iPOut);
        iPIn.addActionListener(this);
        iPOut.addActionListener(this);
        rotatePanels.add(rotatePanel);

        rotatePanel = new JPanel();
        rotatePanel.setLayout(new GridLayout(1, 0));
        iRestore = new JButton("restore");
        rotatePanel.add(iRestore);
        iRestore.addActionListener(this);
        iUndoButton = new JButton("undo");
        iUndoButton.addActionListener(this);
	// rotatePanel.add(iUndoButton);
        rotatePanels.add(rotatePanel);

        rotatePanel = new JPanel();
        rotatePanel.setLayout(new GridLayout(1, 0));
        iLoadButton = new JButton("load from file");
        rotatePanel.add(iLoadButton);
        iLoadButton.addActionListener(this);
        iSaveButton = new JButton("save to file");
        iSaveButton.addActionListener(this);
        rotatePanel.add(iSaveButton);
	// rotatePanels.add(rotatePanel);
        iCurrentRotDir = ".";


        rotatePanel = new JPanel();
        rotatePanel.setLayout(new GridLayout(1, 0));
        iSaveImageButton = new JButton("saveImageAs");
        rotatePanel.add(iSaveImageButton);
        iSaveImageButton.addActionListener(this);
        rotatePanel.add(iSaveImageButton);
        //rotatePanels.add(rotatePanel);


        newPanel.add(rotatePanels);

        secondPanel.add(newPanel, "West");
        iIndicator = new Indicator3D();
	//iIndicator.setPreferredSize(new Dimension(80,80));
        secondPanel.add(iIndicator, "East");

        iImagePanel.add(secondPanel, "South");
         
    }



   
/*
    public void actionPerformed(ActionEvent e) {
        Object o = e.getSource();
        if (   o == iXUp || o == iXDn
            || o == iYUp || o == iYDn
            || o == iZUp || o == iZDn
            || o == iPIn || o == iPOut
            || o == iRestore || o == iUndoButton) {
            handleRotatePanel(o);
            return;
        }
        if ( o == iLoadButton) {
            loadRotations();
        } else if (o == iSaveButton) {
            saveRotations();
        } else if (o == iSaveImageButton) {
            saveImageAs();
        }

    }

*/

   
}

