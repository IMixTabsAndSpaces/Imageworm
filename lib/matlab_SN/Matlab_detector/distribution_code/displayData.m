function displayData(points,stack)

anchorval=mean(mean(median(stack)))*4;


pmatches=round(points);



%detected centers
%visualize for heck of it
data=zeros(size(stack));


data(sub2ind(size(data),pmatches(:,2),pmatches(:,1),pmatches(:,3)))=.5*anchorval;
data=imdilate(data,strel('disk',2));

datatest=max(data,stack);

nii=make_nii(datatest);
view_nii(nii);
