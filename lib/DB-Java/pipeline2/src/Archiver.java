

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Date;
import java.util.GregorianCalendar;

import org.rhwlab.manifest.ManifestX;
import org.rhwlab.snight.Config;

/**
 * @author biowolp
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class Archiver {

    public static String cDBLocation = "/nfs/waterston/embryoDBtest2";
    String      iImages;
    String      iAnnots;
    String      iEntry;
    EmbryoXML   iEmbryo;
    String      iDicDir;
    String      iInfoDir;
    String      iTifDir;
    String      iTifRDir;
    String      iDatsDir;
    String      iPath;
    Config      iConfig;
    String      iImgSubdir;
    boolean		iSeriesDirectoryPreExists; //needed during transition period only

    private static final String ARCHIVELOCATION = "ArchiveLocation";
    private static final String ANNOTSLOCATION = "AnnotsLocation";

    public Archiver(String series) {
        iEmbryo = new EmbryoXML(series, cDBLocation);
        //String archLoc = ManifestX.getManifestParameter(ARCHIVELOCATION);
        //String annotsLoc = ManifestX.getManifestParameter(ANNOTSLOCATION);
        String archLoc = ManifestX.getManifestValue(ARCHIVELOCATION);
        String annotsLoc = ManifestX.getManifestValue(ANNOTSLOCATION);
        iImages = archLoc + "images";
        iAnnots = annotsLoc + "annots";
        //String images = "/nfs/waterston2/images";
        //String annots = "/nfs/waterston2/annots";
        //boolean b1 = images.equals(iImages);
        //boolean b2 = annots.equals(iAnnots);
        iEntry = series;
        String configFileName = iEmbryo.iRecord[EmbryoXML.ATCONFIG];
        String annotsDir = iEmbryo.iRecord[EmbryoXML.ANNOTS];
        iConfig = new Config(annotsDir + "/dats/" + configFileName);
        iImgSubdir = "tif";
        if (iConfig.iUseZip == 2) iImgSubdir = "zip";
    }

    public EmbryoXML updateDatabase() {
        String person = iEmbryo.iRecord[EmbryoXML.PERSON];
        iEmbryo.iRecord[EmbryoXML.ANNOTS] = iAnnots + "/" + person + "/" + iEntry;
        iEmbryo.iRecord[EmbryoXML.IMAGELOC] = iImages + "/" + person + "/" + iEntry;
        iEmbryo.iRecord[EmbryoXML.STATUS] = EmbryoXML.statuses[EmbryoXML.ARC2STATUS];
        String series = iEmbryo.iRecord[EmbryoXML.SERIES];
        String seriesxml = series + ".xml";
        iEmbryo.iRecord[EmbryoXML.ATCONFIG] = seriesxml;
        EmbryoXML.writeXML(cDBLocation + "/", iEmbryo.iRecord);
        //EmbryoXML.writeXML(Pipeline2.cDBLocation + "/" + seriesxml, iEmbryo.iRecord);
        return iEmbryo;

    }

    public void deleteParentDirectories() {
        String annots = iEmbryo.iRecord[EmbryoXML.ANNOTS];
        File annotsD = new File(annots);
        File annotsDP = annotsD.getParentFile();
        boolean b = annotsD.delete();
        println("deleteParentDirectories, " + annotsD + CS + b);
        b = annotsDP.delete();
        println("deleteParentDirectories, " + annotsDP + CS + b);
    }

    public void deleteFiles() {
        int type = EmbryoXML.ANNOTS;
        deleteFileDetails(type, "dats");
        type = EmbryoXML.IMAGELOC;
        deleteFileDetails(type, "info");
        println("deleteFiles, " + System.currentTimeMillis());
        deleteFileDetails(type, "dic");
        println("deleteFiles, " + System.currentTimeMillis());
        deleteFileDetails(type, iImgSubdir);
        println("deleteFiles, " + System.currentTimeMillis());
        deleteFileDetails(type, iImgSubdir + "R");
        println("deleteFiles, " + System.currentTimeMillis());

    }

    public void deleteFileDetails(int type, String subDir) {
        subDir = "/" + subDir + "/";
        String srcDir = iEmbryo.iRecord[type] + subDir;
        File srcDirF = new File(srcDir);

        if (!srcDirF.exists()) return;
        String [] list = srcDirF.list();
        for (int i=0; i < list.length; i++) {
            String s = srcDir + list[i];
            File f = new File(s);
            boolean bb = f.delete();
            println("deleteFileDetails, " + bb + CS + s);
        }

        boolean b = srcDirF.delete();
        println("deleteFileDetails, " + b + CS + srcDir);

    }

    public void copyFiles() {
        int type = EmbryoXML.ANNOTS;
        //makeXMLAceTreeConfig();
        if (!iSeriesDirectoryPreExists) copyFileDetails(type, "dats");
        //println("copyFiles, " + System.currentTimeMillis());
        type = EmbryoXML.IMAGELOC;
        copyFileDetails(type, "info");
        //println("copyFiles, " + System.currentTimeMillis());
        copyFileDetails(type, "dic");
        //println("copyFiles, " + System.currentTimeMillis());
        copyFileDetails(type, iImgSubdir);
        //println("copyFiles, " + System.currentTimeMillis());
        copyFileDetails(type, iImgSubdir + "R");
        //println("copyFiles, " + System.currentTimeMillis());
    }

    public void makeXMLAceTreeConfig() {
        //String configFileName = iEmbryo.iRecord[EmbryoXML.ATCONFIG];
        //String annotsDir = iEmbryo.iRecord[EmbryoXML.ANNOTS];
        String series = iEmbryo.iRecord[EmbryoXML.SERIES];
        String person = iEmbryo.iRecord[EmbryoXML.PERSON];
        //Config config = new Config(annotsDir + "/dats/" + configFileName);
        String zip = iConfig.iZipFileName;
        int k = zip.lastIndexOf("/");
        String zipName = zip.substring(k + 1);
        String typical = iConfig.iTypicalImage;
        k = typical.lastIndexOf("/");
        int k2 = typical.substring(0, k).lastIndexOf("/");
        String typicalName = typical.substring(k2);
        File configF = new File(iAnnots + "/"  + person + "/" + series + "/dats/" +  series + ".xml");
        iConfig.iZipFileName = iAnnots + "/" + person + "/" + series + "/dats/" + zipName;
        iConfig.iTypicalImage = iImages + "/" + person + "/" + series + typicalName;
        iConfig.iEndingIndex = 590;
        //iConfig.saveConfigXMLFile(configF);
        iConfig.writeXMLConfig(configF);

    }

    private void copyFileDetails(int type, String subDir) {
        subDir = "/" + subDir + "/";
        String srcDir = iEmbryo.iRecord[type] + subDir;
        String dstDir = iImages;
        if (type == EmbryoXML.ANNOTS) dstDir = iAnnots;
        dstDir += iPath + subDir;
        //String dstDir = iAnnots + iPath + subDir;
        File srcDirF = new File(srcDir);
        if (!srcDirF.exists()) return;
        String [] list = srcDirF.list();
        if (list.length == 0) return;
        int count = 0;
        for (int i=0; i < list.length; i++) {
            //println("copyFileDetails, " + i + CS + list[i]);
            String s = list[i];
            if (s.equals("matlab")) {
            	copyMatlabDirectory(srcDir, dstDir);
            	continue;
            }
            try {
                ChannelCopy.transferCopy(srcDir + s, dstDir + s);

            } catch(Exception e) {
                e.printStackTrace();
            }
        }
    }

    void copyMatlabDirectory(String srcDir, String dstDir) {
    	File d = new File(dstDir + "/matlab");
    	d.mkdir();
    	File sd = new File(srcDir + "/matlab");
    	String [] list = sd.list();
        for (int i=0; i < list.length; i++) {
        	String s = list[i];
            try {
                ChannelCopy.transferCopy(srcDir + "/matlab/"+ s, dstDir + "/matlab/"+ s);

            } catch(Exception e) {
                e.printStackTrace();
            }

        }
    }

    public void prepareTargetDirectories() {
        // we expect the imageloc and annots to have the form
        // /nfs/waterston/biowolp/hsp3red/20070314_hsp3_a1aX
        // /nfs/waterston/biowolp/hsp3red/20070314_hsp3_a1aX
        // how to characterize the form
        // a storage space
        // an experimenter name
        // a series group
        // an actual series
        // for sure we will be creating a directory at the series level
        // perhaps we will be creating a directory at the series group level
        // rarely we will be creating a directory at the experimenter level
        //
        // one approach is to parse the existing imageloc back to the
        // person level and then see what needs to be done
        // a different approach would be to assume that the series
        // is as given in the database -- use that
        // if it is not then there is something wrong

        // similarly with the person, we can just use the entry value
        // which means all we need to parse out is the series group


        String imgloc = iEmbryo.iRecord[EmbryoXML.IMAGELOC];
        String annots = iEmbryo.iRecord[EmbryoXML.ANNOTS];
        //println("prepareTargetDirectories, " + imgloc + CS + annots);

        int m = imgloc.lastIndexOf("/");
        String s = imgloc.substring(0, m);
        //println("prepareTargetDirectories, " + s);
        //m = s.lastIndexOf("/");
        //String seriesGroup = s.substring(m + 1);
        //println("prepareTargetDirectories, " + seriesGroup);

        String path = "/" + iEmbryo.iRecord[EmbryoXML.PERSON];
        path +=  "/" + iEntry;
        iPath = path;

        //println("prepareTargetDirectories, " + path);


        makeKeySubdirectories(iImages);


        makeKeySubdirectories(iAnnots);
        makeImageSubdirectories(iImages + path);

        iDatsDir = iAnnots + path + "/dats";
        File datsDir = new File(iDatsDir);
        datsDir.mkdir();
        if (!datsDir.exists()) {
            println("prepareTargetDirectories, could not create annots subdirectories " + iDatsDir);
            System.exit(0);

        }
        //println("prepareTargetDirectories, succeeded");

    }

    private void makeKeySubdirectories(String top) {
        String s = top + "/" + iEmbryo.iRecord[EmbryoXML.PERSON];
        File personDir = new File(s);
        boolean bpd = personDir.mkdir();
        //s += "/" + seriesGroup;
        //File groupDir = new File(s);
        //groupDir.mkdir();
        s += "/" + iEntry;
        File seriesDir = new File(s);
        iSeriesDirectoryPreExists = seriesDir.exists() && seriesDir.isDirectory();
        if (!iSeriesDirectoryPreExists) {
        	try {
        		boolean bsd = seriesDir.mkdir();
        	} catch(Exception eee) {
        		eee.printStackTrace();
        	}
        }
        if (!seriesDir.exists()) {
            println("makeKeySubdirectories, could not create directory, " + s);
            System.exit(0);
        }

    }

    private void makeImageSubdirectories(String seriesDir) {
        String s = seriesDir;
        //println("makeImageSubdirectories, " + s);
        iInfoDir = s + "/info";
        File infoDir = new File(iInfoDir);
        infoDir.mkdir();
        iDicDir = s + "/dic";
        File dicDir = new File(iDicDir);
        dicDir.mkdir();
        iTifDir = s + "/" + iImgSubdir;
        File tifDir = new File(iTifDir);
        tifDir.mkdir();
        iTifRDir = s + "/" + iImgSubdir + "R";
        File tifRDir = new File(iTifRDir);
        tifRDir.mkdir();
        boolean b = infoDir.exists();
        b = b && dicDir.exists();
        b = b && tifDir.exists();
        b = b && tifRDir.exists();
        if (!b) {
            println("makeImageSubdirectories, could not create image subdirectories ");
            System.exit(0);
        }

    }



    public static void archiveDriver() {
        now = System.currentTimeMillis();
        Date d = new GregorianCalendar().getTime();
        PrintWriter pw = null;
        println("Main.main, " + d);
        String targetFile = cDBLocation + "/log/a" + String.valueOf(start) + ".txt";
        try {
            FileOutputStream fos = new FileOutputStream(targetFile, true);
            pw = new PrintWriter(fos);
            pw.println("time: " + d);
        } catch(IOException ioe) {
            ioe.printStackTrace();
        }

        String dirS = cDBLocation + "/archiveMe";
        File dir = new File(dirS);
        String [] dbxmls = dir.list();
        for (int i=0; i < dbxmls.length; i++) {
            if (dbxmls[i].endsWith(".arc")) {
                String series = dbxmls[i];
                series = series.substring(0, series.length() - 4);
                println("archiveDriver, " + series);
                EmbryoXML exmlOld = new EmbryoXML(series, cDBLocation);
                boolean b = false;
                try {
                    b = archiveMe(series);
                } catch(Exception e) {
                    // try to continue processing other arc files
                    // even if one or more fail
                    e.printStackTrace();
                }
                if (b) {
                    EmbryoXML.writeXML(cDBLocation + "/cleanupPending", exmlOld.iRecord);
                    new File(dirS + "/" + series + ".arc").delete();

                }
                now = System.currentTimeMillis();
                pw.println(series + " archived in " + (now - inter) + " milliseconds");


                inter = now;
            }
        }
        pw.println("everything archived in " + (now - start) + " milliseconds");
        d = new GregorianCalendar().getTime();
        pw.println("time: " + d);
        pw.close();
    }

    public static boolean archiveMe(String series) throws Exception {
    	println("archiveMe, " + series);
        Archiver arc = new Archiver(series);
        arc.prepareTargetDirectories();
        arc.copyFiles();
        arc.updateDatabase();
        arc.makeXMLAceTreeConfig();
        return true;

    }

    private static long start;
    private static long inter;
    private static long now;
    private static long end;

    public static void main(String[] args) {
        println("Archiver.main, ");
        start = System.currentTimeMillis();
        inter = start;
        //cDBLocation = ManifestX.reportAndUpdateManifest(false);
        ManifestX.reportAndUpdateManifest();
        cDBLocation = ManifestX.getManifestValue("DBLocation");
        println("Archiver.main, cDBLocation, " + cDBLocation);
        //if (1 == 1) System.exit(0);
        archiveDriver();
        now = System.currentTimeMillis();
        println("everything archived in " + (now - start) + " milliseconds");
    }



    private static void println(String s) {System.out.println(s);}
    private static final String CS = ", ";


}
