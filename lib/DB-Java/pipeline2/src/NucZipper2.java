

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

/*
 * Created on Jun 8, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

/**
 * @author biowolp
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class NucZipper2 {
    
    ZipOutputStream iZos;
    
    public NucZipper2(String zipName) {
        //System.out.println("NucZipper2: " + zipName);
        try {
            iZos = new ZipOutputStream(new FileOutputStream(zipName));
        } catch(IOException ioe) {
            ioe.printStackTrace();
        }
    }
    
    public void close() {
        try {
            iZos.close();
        } catch(IOException ioe) {
            ioe.printStackTrace();
        }
    }
    
    public void addFile(File file, String addName) {
        BufferedReader br;
        try {
            br = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            iZos.putNextEntry(new ZipEntry(addName));
            while (br.ready()) {
                String s = br.readLine() + NL;
                byte [] b = s.getBytes();
                iZos.write(b, 0, b.length);
            }
            iZos.closeEntry();
        } catch(Exception fnfe) {
            fnfe.printStackTrace();
        }
        
    }

    private final static String
         NL = "\n"
        ;
    
    public static void main(String[] args) {
    }
}
