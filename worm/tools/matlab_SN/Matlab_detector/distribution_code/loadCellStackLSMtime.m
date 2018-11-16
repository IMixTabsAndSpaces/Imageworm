function stack=loadCellStackLSMtime(name,time,channel,nPlanes)
%name is filename of combined tif
%channel is which r/g to get out 2 is nuclei green 1 is red
%not botherting to write a general loader assuming all the mouse images are
%paired brightfield? and 2 channel florescence images


%imageinfo=imfinfo(name);
%L=imageinfo(1).Height;

%W=imageinfo(1).Width;
L=512;
W=512;
stack=zeros(L,W,nPlanes);
for j=1:nPlanes

    subimagenum=((time-1)*2)*nPlanes+(j-1)*2+1;
    im=imread(name,subimagenum);
    stack(:,:,j)=im(:,:,channel);

end

%stack=stack./(2^16);