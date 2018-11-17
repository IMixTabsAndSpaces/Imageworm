

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Hashtable;

import qdxml.DocHandler;
import qdxml.QDParser;

/**
 * @author biowolp
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class EmbryoXML implements DocHandler {

    public String [] iRecord;
    public Hashtable iTags;

    public EmbryoXML() {
        iRecord = new String[elem.length];
        iTags = new Hashtable();
        for (int i=0; i < elem.length; i++) {
            iTags.put(elem[i], new Integer(i));
            iRecord[i] = "n/a";
        }
        iRecord[elem.length - 1] = statuses[0];

    }

    /*
    public EmbryoXML(String embryoName) {
        this();
        String path = Pipeline2.cDBLocation + "/" + embryoName + ".xml";
        //String path = EmbryoDB.cDBLocation + "/" + embryoName + ".xml";
        try {
            FileReader fr = new FileReader(path);
            QDParser.parse(this, fr);
        } catch(FileNotFoundException fnfe) {
            println("EmbryoXML: filenotfoundexception: " + fnfe);
            fnfe.printStackTrace();
        } catch(Exception e) {
            println("EmbryoXML: exception: " + e);
            e.printStackTrace();
        }
    }
    */

    public EmbryoXML(String embryoName) {
        this(embryoName, DBEntryMaker.cDBLocation);
    }

    public EmbryoXML(String embryoName, String dbPath) {
        this();
        String path = dbPath + "/" + embryoName + ".xml";
        //String path = EmbryoDB.cDBLocation + "/" + embryoName + ".xml";
        try {
            FileReader fr = new FileReader(path);
            QDParser.parse(this, fr);
        } catch(FileNotFoundException fnfe) {
            println("EmbryoXML: filenotfoundexception: " + fnfe);
            fnfe.printStackTrace();
        } catch(Exception e) {
            println("EmbryoXML: exception: " + e);
            e.printStackTrace();
        }
    }

    public EmbryoXML(String path, int x) {
        this();
        try {
            FileReader fr = new FileReader(path);
            QDParser.parse(this, fr);
        } catch(FileNotFoundException fnfe) {
            println("EmbryoXML: filenotfoundexception: " + fnfe);
            fnfe.printStackTrace();
        } catch(Exception e) {
            println("EmbryoXML: exception: " + e);
            e.printStackTrace();
        }

    }

    public static void writeXML(String filePath, String [] data) {
        int k = filePath.lastIndexOf("/");
        String s = "";
        if (k == filePath.length() - 1) s = filePath + data[SERIES];
        else s = filePath + "/" + data[SERIES];
        //println("writeXML: " + s + CS + data[SERIES]);
        // need provision for not allowing a new series with same
        // name as an existing one
        try {
            FileOutputStream fos = new FileOutputStream(s + XML);
            PrintWriter pw = new PrintWriter(fos);
            pw.println(HEADER);
            pw.println();
            pw.println(EXPERIMENT);
            for (int i=0; i < elem.length; i++) {
                //println("writeXML: " + i + data[i]);
                if (data[i] == null || data[i].length() == 0) continue;
                s = S + elem[i] + SP + att[i] + EQ + data[i] + E;
                pw.println(s);
            }
            pw.println(EEXPERIMENT);
            pw.close();

        } catch(IOException ioe) {
            ioe.printStackTrace();
            System.exit(1);
        }

    }

    public static final String
    S = "<"
   ,SP = " "
   ,E = "\"/>"
   ,EQ = "=\""
   ,XML = ".xml"
   ,HEADER = "<?xml version='1.0' encoding='utf-8'?>"
   ,EXPERIMENT = "<experiment>"
   ,EEXPERIMENT = "</experiment>"
   ;

    public static final int
    SERIES = 0
   ,DATE = 1
   ,PERSON = 2
   ,STRAIN = 3
   ,TREATMENTS = 4
   ,REDSIGNAL = 5
   ,IMAGELOC = 6
   ,TIMEPOINTS = 7
   ,ANNOTS = 8
   ,ATCONFIG = 9
   ,EDITEDBY = 10
   ,EDITEDTP = 11
   ,EDITEDCELLS = 12
   ,CHECKED = 13
   ,COMMENTS = 14
   ,STATUS = 15
   ;

    public static String [] elem = {
            "series"
           ,"date"
           ,"person"
           ,"strain"
           ,"treatments"
           ,"redsig"
           ,"imageloc"
           ,"timepts"
           ,"annots"
           ,"acetree"
           ,"editedby"
           ,"editedtimepts"
           ,"editedcells"
           ,"checkedby"
           ,"comments"
           ,"status"
   };

    public static String [] att = {
            "name"     //series
           ,"date"     //date
           ,"name"     //person
           ,"name"     //strain
           ,"desc"     //treatments
           ,"value"    //red signal
           ,"loc"      //image location
           ,"num"      //time points
           ,"loc"      //annotation location
           ,"config"   //acetree
           ,"name"     //edited by
           ,"num"      //edited TP
           ,"num"      //edited cells
           ,"name"     //checked
           ,"text"     //comments
           ,"case"     //status

   };

    public static String [] statuses = {
             "new"
            ,"archived"
            ,"deleted"
            ,"arc1"
            ,"arc2"
            ,"arc3"
            ,"del1"

    };

    public static final int
         NEWSTATUS = 0
        ,ARCHIVEDSTATUS = 1
        ,DELETEDSTATUS = 2
        ,ARC1STATUS = 3
        ,ARC2STATUS = 4
        ,ARC3STATUS = 5
        ,DEL1STATUS = 6
        ;

    /* (non-Javadoc)
     * @see qdxml.DocHandler#startElement(java.lang.String, java.util.Hashtable)
     */
    public void startElement(String tag, Hashtable h) throws Exception {
        if (tag.equals("experiment")) {
            ;
        } else {
            //println("startElement: " + tag);
            try {
            int m = ((Integer)iTags.get(tag)).intValue();
            iRecord[m] = (String)h.get(att[m]);
            } catch(Exception e) {
                e.printStackTrace();
                println("startElement: " + tag + CS + h);
                System.exit(1);
            }

        }
    }

    /* (non-Javadoc)
     * @see qdxml.DocHandler#endElement(java.lang.String)
     */
    public void endElement(String tag) throws Exception {
        // TODO Auto-generated method stub

    }

    /* (non-Javadoc)
     * @see qdxml.DocHandler#startDocument()
     */
    public void startDocument() throws Exception {
        // TODO Auto-generated method stub

    }

    /* (non-Javadoc)
     * @see qdxml.DocHandler#endDocument()
     */
    public void endDocument() throws Exception {
        // TODO Auto-generated method stub

    }

    /* (non-Javadoc)
     * @see qdxml.DocHandler#text(java.lang.String)
     */
    public void text(String str) throws Exception {
        // TODO Auto-generated method stub

    }

    public static void updateRecord(String [] data) {
        data[EmbryoXML.ANNOTS] = data[EmbryoXML.IMAGELOC];
        data[EmbryoXML.ATCONFIG] = data[EmbryoXML.SERIES] + ".dat";

    }

    public static void updateDatabase(String newLoc) {
        File dir = new File(DBEntryMaker.cDBLocation);
        //File dir = new File(EmbryoDB.cDBLocation);
        String [] list = dir.list();
        for (int i=0; i < list.length; i++) {
            if (i < 2) continue;
            if (i > 4) break;
            String s = list[i];
            if (!s.endsWith(".xml")) continue;
            int k = s.indexOf(".xml");
            s = s.substring(0, k);
            println("updateDatabase, " + i + CS + s);
            EmbryoXML exml = new EmbryoXML(s);
            updateRecord(exml.iRecord);
            EmbryoXML.writeXML(newLoc, exml.iRecord);
        }

    }

    public static void main(String[] args) {
        DBEntryMaker.cDBLocation = "/nfs/waterston/embryoDB";
        //EmbryoDB.cDBLocation = "/nfs/waterston/embryoDB";
        String newLoc = "/nfs/waterston/embryoDB2";
        updateDatabase(newLoc);

    }

    private static void println(String s) {System.out.println(s);}
    private static final String CS = ", ";

}
