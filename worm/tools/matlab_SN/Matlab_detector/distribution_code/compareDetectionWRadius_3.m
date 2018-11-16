
function [matches,correct_matched]=compareDetectionWRadius_3(detectedpoints,correctpoints,correctpointsizes,distancefactor,zscalingfactor)
%assigns matching labels to data based on radius
%returns detected long array of indicies into correctpoints
%a->b b->a less than *individual labeld radius

%z values are in planes not pixels
s_detectedpoints=[detectedpoints(:,1),detectedpoints(:,2),detectedpoints(:,3)*zscalingfactor];
s_correctpoints=[correctpoints(:,1),correctpoints(:,2),correctpoints(:,3)*zscalingfactor];


distances=distance(s_detectedpoints',s_correctpoints');
[mincorrect,Icorrect]=min(distances);
[mindetected,Idetected]=min(distances');

%I think this sorting is an artifact of previous version where matching was
%not bidirectional 

%sort mindetected,idetected so closest match gets first crack
[svals,sortIndex]=sortrows([mindetected',Idetected'],1);
smindetected=svals(:,1)';
sIdetected=svals(:,2)';


correct_matched=zeros(1,length(correctpointsizes));
%for each correct point 
FN=[];
s=size(detectedpoints);
matches=zeros(1,s(1));
 %match a closest to b and distance ab is less than threhhold
 % because data is cleaned to 1 radius distance between cells
 %if threshold .5 radius this is stronger than match ab ba
 for i=1:length(smindetected)
     
     %detected point
     dpoint=s_detectedpoints(sortIndex(i),:);
     %correct point thats closes
     lpoint=s_correctpoints(sIdetected(i),:);
     
     %elliptical threshold distancefactor in z distancefactor/2 in x,y
   %  thresh_factor=correctpointsizes(sIdetected(i))*distancefactor;
    % thresh_val=(dpoint-lpoint).^2;
    % ovalratio=.75;
    % thresh_val=thresh_val(1)/(thresh_factor*ovalratio).^2+thresh_val(2)/(thresh_factor*ovalratio).^2+thresh_val(3)/(thresh_factor).^2;
     
  
    if( (smindetected(i)<correctpointsizes(sIdetected(i))*distancefactor) && Icorrect(sIdetected(i))==sortIndex(i))  
     %now instead a-b b-a and less than elipse threshold
   %if(thresh_val<=1 && Icorrect(sIdetected(i))==sortIndex(i))  
        matches(sortIndex(i))=sIdetected(i);
        correct_matched(sIdetected(i))=sortIndex(i);
   else
       matches(sortIndex(i))=-1;
    end
end

for i=1:length(correct_matched)
    if(correct_matched(i)==0)
        correct_matched(i)=-1;
    end
    
end
 
 
 
%for i=1:length(mindetected)
   
%   if( (mindetected(i)<correctpointsizes(Idetected(i))*distancefactor) && correct_matched(Idetected(i))==0)  
%        matches(i)=Idetected(i);
%        correct_matched(Idetected(i))=i;
 %  else
 %      matches(i)=-1;
 %   end
%end

%for i=1:length(correct_matched)
%    if(correct_matched(i)==0)
%        correct_matched(i)=-1;
%    end
    
%end




