
list = getList("image.titles");
windows = run("Stack Slicer", "split_channels stack_order=XYCZT");
for (i=0; i<=1; i++) {
	print(list[i]);
	
}
