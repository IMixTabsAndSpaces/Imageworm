sub_output = "/mnt/Disk_2/work/images/raw/ChannelGFP1";
s = 1;
data = "ChannelGFP";
run("Image Sequence... ", "format=TIFF save=["+sub_output+"/"+data+s+"_Seq0000_t001_z001_c001.tif]");
print("format=TIFF save=["+sub_output+"/"+data+s+"_Seq0000_t001_z001_c001.ti]")