
/*
 * - converts ND2 or ome.tiff files into separate TIFF. 
 * - fornow manualy change the script output and input path.
 * - each series is saved to a new directory of tiff. The name of the series is appended to the file name and directory.
 * - the TIFFs will be scaled becuse they are in 12 bit format but computers stores them in 16 bit and we want 8 bit.
 * - the TIFFs are saved in a folder generated by the macro
 * 
 * stephen Nehrbass, Oct 2018
 * 
 */

args = split(getArgument()," "); 
path = args[0];
output = args[1];
//path = "/mnt/Disk_2/work/images/ND2_Files/ChannelGFP,TxRed_Seq0000.nd2-series-01.tif";
//output =  "/mnt/Disk_2/work/images/output/";

//run("Bio-Formats Macro Extensions");


seriesCount = 1;
getDateAndTime(year, month, dayOfWeek, dayOfMonth, hour, minute, second, msec);
month += 1;
date = split(path, "/");
date = date[lengthOf(date)-1];
date = split(date, ".");
date = date[0];
for (s=1; s<=seriesCount; s++){
    run("Bio-Formats Importer", "open="+path+" autoscale=False color_mode=Default view=Hyperstack stack_order=XYCZT open_all_series concatenate_series");


    t=getTitle();
    run("Stack Slicer", "split_channels stack_order=XYCZT");
    list = getList("image.titles");
    sub_output = output + date;
    File.makeDirectory(sub_output);
    chdirs = newArray(sub_output+"/tif", sub_output+"/tifR");
    File.makeDirectory(chdirs[0]);
    File.makeDirectory(chdirs[1]);

    for (i=0; i<=1; i++)
    	{
	selectWindow(list[i]);
    	//resetMinAndMax();
		run("Grays");
		run("Apply LUT", "stack");
    	run("8-bit");
    	run("Image Sequence... ", "format=TIFF save=" + chdirs[i] + "/ name=" + date);
    }
    run ("Close All");
}
eval("script", "System.exit(0);");







