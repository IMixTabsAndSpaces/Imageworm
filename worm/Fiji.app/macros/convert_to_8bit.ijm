//Stack.setChannel(1);
//Stack.setSlice(1);
//run("Brightness/Contrast...");
//resetMinAndMax();

//Stack.setChannel(2);
//run("Brightness/Contrast...");
//resetMinAndMax();
//run("Close");
//run("8-bit", "grayscale)

//print(t);
/*
list = getList("image.titles");
    if (list.length==0)
        print("No image windows are open");
  	else {
     	print("Image windows:");
     	for (i=0; i<list.length; i++)
            print("   "+list[i]);
 	    }
    print("");
//selectWindow("B&C");
//t=getTitle();
//    run("Stack Slicer", "split_channels stack_order=XYCZT");
//    
   list = getList("image.titles");
    if (list.length==0)
        print("No image windows are open");
  	else {
     	print("Image windows:");
     	for (i=0; i<list.length; i++)
            print("   "+list[i]);
  	    }
    print("");


getDateAndTime(year, month, dayOfWeek, dayOfMonth, hour, minute, second, msec);
month += 1;
date = d2s(year,0)+"-"+d2s(month,0)+"-"+d2s(dayOfMonth,0)+"-"+d2s(hour,0)+":"+d2s(minute,0);
print(date); 

path = "/mnt/Disk_2/work/images/raw/channelgfp,txred_seq0000_xy1.nd2";
Str = split(path, ",");
Str = split(Str[1], ".");
Str = Str[0];
*/

//print(Str);

path = "/media/zachlab/Windows/LinuxStorage/images/ND2_Files/20181030_seq0_xy1.nd2";
run("Bio-Formats Importer", "open="+path+" autoscale=False color_mode=Default view=Hyperstack stack_order=XYCZT open_all_series concatenate_series");

//getDateAndTime(year, month, dayOfWeek, dayOfMonth, hour, minute, second, msec);
//month += 1;
//date = split(path, "/");
//date = date[lengthOf(date)-1];
//date = split(date, ".");
//date = date[0];
//print(date);

//date = split(date[1], ".");
//date = date[0];












//run("Bio-Formats Importer", "open="+path+" autoscale color_mode=Default view=Hyperstack stack_order=XYCZT open_all_series concatenate_series");	