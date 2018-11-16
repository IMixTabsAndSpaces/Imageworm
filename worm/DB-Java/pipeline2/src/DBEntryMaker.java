

import java.io.File;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.Date;
import java.util.Vector;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.rhwlab.manifest.ManifestX;


/**
 * @author biowolp
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class DBEntryMaker {

    public static String       cDBLocation;


    public static void makeXMLFromImgLoc(String topDir, String seriesName) {
        //println("makeXMLFromImgLoc, " + topDir + CS + seriesName);
        String maindir = topDir + "/" + seriesName;
        String seriesEstimate = seriesName; //seriesEstimate(maindir);
        String timePointsEstimate = timePointsEstimate(maindir);
        String dateEstimate = dateEstimate(maindir);
        String personEstimate = personEstimate(maindir);
        String annotsEstimate = maindir; //annotsEstimate(maindir, seriesEstimate);
        String acetreeEstimate = seriesName + ".xml"; //acetreeEstimate(maindir, seriesEstimate);

        String [] data = new String[EmbryoXML.elem.length];
        for (int i=0; i < data.length; i++) {
            data[i] = "n/a";
        }
        data[EmbryoXML.STATUS] = "new";
        data[EmbryoXML.IMAGELOC] = maindir;
        data[EmbryoXML.SERIES] = seriesEstimate;
        data[EmbryoXML.TIMEPOINTS] = timePointsEstimate;
        data[EmbryoXML.DATE] = dateEstimate;
        data[EmbryoXML.PERSON] = personEstimate;
        data[EmbryoXML.ANNOTS] = annotsEstimate;
        data[EmbryoXML.ATCONFIG] = acetreeEstimate;
        String dbloc = cDBLocation;
        println("makeXMLFromImgLoc, " + dbloc);
        EmbryoXML.writeXML(dbloc, data);

    }

    private static void preventDupXMLNames(String seriesName) {
        String [] sa = new File(cDBLocation).list();
        String mine = seriesName + ".xml";
        for (int i=0; i < sa.length; i++) {
            if (sa[i].indexOf(mine) == 0) {
                println("");
                println("###########################################");
                println("## Your series name, " + seriesName);
                println("## duplicates that of an existing series.");
                println("## Please change the directory name of your series, ");
                println("## or see you systems administrator.");
                println("###########################################");
                System.exit(0);
            }

        }
    }



    public static String timePointsEstimate(String imgDir, boolean old) {
        String tp = null;
        try {
            String infodir = imgDir + "/info";
            File f = new File(infodir);
            String [] sa = f.list();
            int k = sa.length;
            if (k > 0) {
                tp = String.valueOf(k);
            } else {
                String tifdir = imgDir + "/tif";
                f = new File(tifdir);
                sa = f.list();
                tp = String.valueOf(sa.length/ 31);
            }
        } catch(Exception e) {
            println("timePointsEstimate exception: " + e);
        }
        return tp;
    }

    public static String timePointsEstimate(String imgDir) {
        String tp = null;
        Vector v = new Vector();
        File d = new File(imgDir + "/tif");
        String [] tifs = d.list();
        String rex = ".*-t(\\d\\d\\d)-p\\d\\d.*";
        Pattern p = Pattern.compile(rex);
        for (int i=0; i < tifs.length; i++) {
            Matcher mm = p.matcher(tifs[i]);
            boolean bool = mm.find();
            if (bool) v.add(mm.group(1));

        }
        Collections.sort(v);
        //for (int i=0; i < v.size(); i++) {
        //	String s = (String)v.get(i);
        //    println("timePointsEstimate, " + i + CS + s);
        //}

        return (String)v.get(v.size() - 1);
    }

    public static String dateEstimate(String imgDir) {
        String estimate = null;
        try {
            String lsmdir = imgDir + "/lsm";
            File lsmf = new File(lsmdir);
            if (lsmf.exists()) {
                String [] sa = lsmf.list();
                if (sa.length > 0) {
                    estimate = formatEstimate(lsmdir + "/" + sa[0]);
                }
            } else if (new File(imgDir + "/tif").exists()){
                String tifdir = imgDir + "/tif";
                File tiff = new File(tifdir);
                String [] sa = tiff.list();
                if (sa.length > 0) {
                    estimate = formatEstimate(tifdir + "/" + sa[0]);
                }
            } else {
                String tifdir = imgDir + "/zip";
                File tiff = new File(tifdir);
                String [] sa = tiff.list();
                if (sa.length > 0) {
                    estimate = formatEstimate(tifdir + "/" + sa[0]);
                }
            }

        } catch(Exception e) {
            println("DBEntryMaker.dateEstimate: exception: " + e);
        }

        return estimate;
    }

    public static String formatEstimate(String filename) throws Exception{
        String formattedEstimate = "";
        File f = new File(filename);
        if (f.exists()) {
            long t = f.lastModified();
            Date d = new Date(t);
            SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd");
            formattedEstimate = sdf.format(d);
        }
        return formattedEstimate;
    }

    public static String personEstimate(String imgDir) {
        // this based on a forward scan of imgDir which
        // we assume will be like
        // /nfs/waterston/murray/20070430_pha4I2L_3_lit1i
        // but could be like
        // /nfs/waterston/murray/pha4red/20070430_pha4I2L_3_lit1i
        String personEstimate = null;
        String sPersonLoc = ManifestX.getManifestValue("UserNameLocation");
        int personLoc = 2;
        if (sPersonLoc != null) personLoc = Integer.parseInt(sPersonLoc);

        int k = imgDir.indexOf("/");
        for (int i=0; i < personLoc; i++) {
        	k = imgDir.indexOf("/", k + 1);
        }
        int k2 = imgDir.indexOf("/", k + 1);
        personEstimate = imgDir.substring(k + 1, k2);
        return personEstimate;
    }

/*
    public static String personEstimate(String imgDir) {
        String personEstimate = null;
        int k = imgDir.lastIndexOf("/");
        String x = imgDir.substring(0, k);
        int n = x.lastIndexOf("/");
        x = x.substring(0, n);
        //println("experimental: " + x);
        n = x.lastIndexOf("/");
        x = x.substring(n + 1);
        //println("experimental: " + x);
        personEstimate = x;
        return personEstimate;
    }

    public static String seriesEstimate(String maindir) {
        String series = null;
        try {
            int k = maindir.lastIndexOf("/");
            series = maindir.substring(k + 1);
        } catch(Exception e) {
            println("seriesEstimate: exception: " + e);
        }
        return series;

    }
*/

    public static String annotsEstimate(String maindir, String series) {
        String annotsEstimate = null;
        String zip1 = maindir + "/dats/" + series + "-edit.zip";
        if (new File(zip1).exists()) {
            annotsEstimate = zip1;
        } else {
            String zip2 = maindir + "/dats/" + series + ".zip";
            if (new File(zip2).exists()) {
                annotsEstimate = zip2;
            }
        }
        return annotsEstimate;
    }

    public static String acetreeEstimate(String maindir, String series) {
        String acetreeEstimate = null;
        String xml = maindir + "/dats/" + series + ".xml";
        if (new File(xml).exists()) {
            acetreeEstimate = xml;
        } else {
            String dat = maindir + "/dats/" + series + ".dat";
            if (new File(dat).exists()) {
                acetreeEstimate = dat;
            }
        }
        return acetreeEstimate;
    }




    public static void main(String[] args) {
        //String cDBLocation = ManifestX.reportAndUpdateManifest();
        ManifestX.reportAndUpdateManifest();
        String cDBLocation = ManifestX.getManifestValue("DBLocation");
        DBEntryMaker.cDBLocation = cDBLocation;

    	if (args.length < 1) {
    		println("SNRunner.main, must have one argument, the path to a series");
    		System.exit(0);
    	}
    	String pathToSeries = args[0];
        int m = pathToSeries.lastIndexOf(SNRunner.SLASH);
        String topDir = pathToSeries.substring(0, m);
        String seriesName = pathToSeries.substring(m+1);

        //String maindir = topDir + "/" + seriesName;

        //String tp = timePointsEstimate(maindir);
        //println("DBEntryMaker.main, " + maindir + CS + tp);



        preventDupXMLNames(seriesName);

        DBEntryMaker.makeXMLFromImgLoc(topDir, seriesName);

        //String points = DBEntryMaker.timePointsEstimate(imgdir);
        //println("DBEntryMaker.main, " + points);

    }

    private static void println(String s) {System.out.println(s);}
    private static void print(String s) {System.out.print(s);}
    private static final String CS = ", ";
    private static final DecimalFormat DF0 = new DecimalFormat("####");
    private static final DecimalFormat DF1 = new DecimalFormat("####.#");
    private static final DecimalFormat DF4 = new DecimalFormat("####.####");
    private static String fmt1(double d) {return DF1.format(d);}
    private static String fmt0(double d) {return DF1.format(d);}


}
