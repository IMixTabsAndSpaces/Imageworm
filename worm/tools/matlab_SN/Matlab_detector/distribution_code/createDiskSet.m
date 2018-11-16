function [diskSet,centerindicies]=createDiskSet(X,maximathreshold,zlevel,celldiameter,anisotropy,numcells)

s=size(X);
diskSet=struct;

%2d max
Xmax2d=zeros(size(X),'int8');
for i=1:s(3)
    Xmax2d(:,:,i)=imregionalmax(X(:,:,i));
end

%wipe out bottom XYmaxes before they are even considered anywhere
%Xmax2d(:,:,zlevel:s(3))=0;
twodmax=find(Xmax2d);

twodmax=twodmax(X(twodmax)>maximathreshold);%apply threshold to maximas

[x,y,z]=ind2sub(s,twodmax);
%xymax=[y,x,z];
diskSet.xymax=[y,x,z];
[diskSet.xydetdiameters,diskSet.centeredxymax,diskSet.xycoverage]=calculateSphereDiameters_geometric(diskSet.xymax,X,celldiameter,true);
diskSet.xymaximavals=X(twodmax);

centerindicies=pickCenterIndicies(X,twodmax,celldiameter,anisotropy,numcells);