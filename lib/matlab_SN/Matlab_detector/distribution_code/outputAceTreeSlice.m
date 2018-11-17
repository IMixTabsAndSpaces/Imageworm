function  outputAceTreeSlice(X,directoryname, basename,time,minvalue, maxvalue, downsample,red)
%newDir=[directoryname,basename,'\tif'];
if (red)
    newDir=[directoryname,'image\tifR'];
else
newDir=[directoryname,'image\tif'];
end
if(~(exist(newDir,'dir')))
    mkdir(newDir);
end

s=size(X);
%minx=min(min(min(X)));
%maxx=max(max(max(X)));
X=single(X);
%X=(X-minx)/(maxx-minx);
X=(X-single(minvalue))./single(maxvalue-minvalue);
maxthere=max(max(max(X)));
for slice=1:s(3)
   % if(s(3)<100)
        imwrite(((X(:,:,slice))),sprintf('%s/%s-t%03d-p%02d.tif',newDir,basename,time,slice),'tiff','Compression','none');
    %else
    %    imwrite(((X(:,:,slice))),sprintf('%s/%s-t%03d-p%03d.tif',newDir,basename,time,slice),'tiff','Compression','none');
    %end
end
