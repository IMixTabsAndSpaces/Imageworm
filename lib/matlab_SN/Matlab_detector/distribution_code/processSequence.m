if(~exist('NUC_FILE_OUTPUT'))
    NUC_FILE_OUTPUT=false;
end
if(~exist('LSM'))
    LSM=false;
end


if(~exist('LSM_time'))
    LSM_time=false;
end
   if(~exist('MATLAB_STACK'))
        MATLAB_STACK=false;
    end

for example=1:length(tlist)
    %put last volume data into place for use in local prediction
    if (tlist(example)==firsttimestep)
        previous=0;
    else
        previous=esequence{example-1};
    end

    time=tlist(example);
    e=[];
 
    
    if (MATLAB_STACK)
        load([embryodir,embryonumber,'_',num2str(time,'%04d'),'.mat']);
        X=stack(:,:,1:slices);
        clear('stack');
    else
    if(newscope)
        if(LSM_time)
            X=im2single(loadCellStackLSMtime([embryodir,embryonumber],time,1,slices));
        else
            
            
            if rednuclei
                X=im2double(((loadCellStackMetamorph([embryodir,embryonumber],time,1,slices,[0,0,0,0],zeropadding))));
                %end
                Xr=im2double(((loadCellStackMetamorph([embryodir,embryonumber],time,2,slices,[0,0,0,0],zeropadding))));
                
            else
                X=im2double(((loadCellStackMetamorph([embryodir,embryonumber],time,2,slices,[0,0,0,0],zeropadding))));
                %end
                Xr=im2double(((loadCellStackMetamorph([embryodir,embryonumber],time,1,slices,[0,0,0,0],zeropadding))));
            end
            
        end
    else
        if (LSM)
            X=im2single((loadCellStackLSM([embryodir,embryonumber],time,1,slices)));
        else
%             X=im2single((loadCellStack([embryodir,embryonumber,'_L1'],slices,time)));
            X=im2single((loadCellStack([embryodir,embryonumber],slices,time)));  
        end
    end
    end
    %output slices before subsampling via ROI
    if(outputSlice)
        if(slices>35)%if big stack dont try to take percentile will run out of memory
            minval=1;
            maxval=max(max(max(X)));
        else
            minval=prctile(reshape(X,[1,numel(X)]),25);
            maxval=prctile(reshape(X,[1,numel(X)]),99.99);
        end
        
        outputAceTreeSlice(X,embryodir, embryonumber,time,minval,maxval,1,false);
        if (exist('Xr'))
        minval=prctile(reshape(Xr,[1,numel(X)]),25);
        maxval=prctile(reshape(Xr,[1,numel(X)]),99.99);
        outputAceTreeSlice(Xr,embryodir, embryonumber,time,minval,maxval,1,true);
        %outputAceTreeSlice(X,embryodir, embryonumber,time,1400,1900,downsample);
        end
    end
    
    if(ROI)
        X=X(ROIymin:ROIymax,ROIxmin:ROIxmax,:);
    end
    
        X=imresize(X,downsample);
    
    svol=size(X);
    
    processVolume;
    %{
       % display code
        example=350;
        time=tlist(example);
        e=esequence{example};
        Xorig=im2double((imresize(loadCellStackMetamorph([embryodir,embryonumber],time,2,512,1024,30,[0,0,0,0]),downsample))); 
       s=size(e.finalpoints);
        fakematch=linspace(1,s(1),s(1));
        displayData(fakematch,fakematch,round(e.finalpoints),round(e.finalpoints),Xorig)
    %}    
    esequence{example}=e;
    
end

%bottom detection
if(~exist('bottomdetection'))
    bottomdetection=true;
end
if (bottomdetection&&~singlevolume&&nodatause)
    numcells=[];
    for i=1:length(esequence)
        numcells=[numcells;length(esequence{i}.finalpoints)];
    end
    good=find(numcells>250);
    bottoms1=[];
    bottoms2=[];

       numonplane=zeros(1,svol(3));
    for i=1:length(good)
       % ind=find(esequence{good(i)}.finalmaximas>1.5*parameters.intensitythreshold);
       % planes=esequence{good(i)}.finalpoints(ind,3);
       % bottoms1=[bottoms1;max(planes)];
        planes=esequence{good(i)}.finalpoints(:,3);
     
         for j=1:svol(3) %ignore maxima on bottom 2 planes, they're artifacts
            numonplane(j)=numonplane(j)+length(find(planes==j));
         end
       
    end
    %bottom 2 definition is lowest plane with at least 20% of max number of
    %planes
    
    bottom=max(find(numonplane>=max(numonplane)*.2))+1;%with bottom maximas first small number plane seems to be real...
     %   bottom=max(find(numonplane>=max(numonplane)*.18));%can use this threshold if take them out
    %bottom=median(bottoms1);
    
%    bottomdata{emind}.bottom=bottom;
%    bottomdata{emind}.maxbottom=median(bottoms1);
%    bottomdata{emind}.bottoms=bottoms1;
%    bottomdata{emind}.numonplane=numonplane;
    backup_esequence=esequence;
    
    for i=1:length(esequence)
        if(~isempty(esequence{i}.finalpoints))
            goodpoints=esequence{i}.finalpoints(:,3)<=bottom;
            esequence{i}.finalpoints=esequence{i}.finalpoints(goodpoints,:);
            esequence{i}.finaldiams=esequence{i}.finaldiams(goodpoints,:);
            esequence{i}.finalmaximas=esequence{i}.finalmaximas(goodpoints,:);
        end
        if (~nodata)
            %overwrite final with final after bottom detection
            [celldata,cellnames]=readnuclei([nucleidir,embryonumber_ed,'\t',num2str(tlist(i),'%03d'),'-nuclei']);
            threshold=.75*celldiameter; %for identity of detected and labeled nuclei
            celllocations=celldata(:,4:6);
            %Ucelllocations=Ucelldata(:,4:6);
            %stats from post bottom processing matching
            celllocations(:,1:2)=celllocations(:,1:2).*downsample;
            [matches,matchessr]=compareDetectionWRadius_3(esequence{i}.finalpoints,celllocations,celldata(:,7)*downsample*.5,1.5,anisotropy);
            
            FP=esequence{i}.finalpoints((matches==-1),:);
            FN=celllocations((matchessr==-1),:);
            esequence{i}.finalFN=FN;
            esequence{i}.finalFP=FP;
        end
    end
end

%output results to SN 

if (SNoutput)
%{
    alli=[];
    for i=1:length(esequence)
        alli=[alli;esequence{i}.finalmaximas];
    end

maxval=prctile(alli,99.99);
   maxval=prctile(alli,25); 
    
   
 
    
    
   
 
  esequence=backup_esequence;
    %}
    %hack for small diam bug in sn cant get smaller than 9 
    %{
smallest=[]
    
for i=1:length(esequence)
    esequence{i}.finaldiams=max(esequence{i}.finaldiams,9);
   smallest=[smallest,min(esequence{i}.finaldiams)];
end
%}
  %{
    
   %parameters.staging=[25,102,181,251,351];
   %parameters.intensitythreshold=[10,20,50,50,50,100];
    
    newthresh=100;
    xmax=421;
    ymax=315;
    zmax=26; %first illegal rather than last legal as above
    for i=1:length(esequence)
    numcells=length(esequence{i}.finalmaximas);
    if(numcells<25)
        newthresh=10;
    elseif(numcells<102) 
        newthresh=20;
    elseif(numcells<351)
        newthresh=75;
    else
        newthresh=125;
    end
    good=esequence{i}.finalmaximas>newthresh;
    good=good&esequence{i}.finalpoints(:,2)<xmax;
    good=good&esequence{i}.finalpoints(:,1)<ymax;
    good=good&esequence{i}.finalpoints(:,3)<zmax;
    esequence{i}.finaldiams=esequence{i}.finaldiams(good);
    esequence{i}.finalmaximas=esequence{i}.finalmaximas(good);
    esequence{i}.finalpoints=esequence{i}.finalpoints(good,:);
    end
     
    
  %}
    
    
    %if (newscope)
    if(isempty(suffix))
        name=embryonumber;
    else
       name=[embryonumber,'_',suffix];
    end
    
    if(ROI)
        outputSNFiles(embryodir,name,esequence,min(tlist),max(tlist),downsample,ROIxmin,ROIymin);
    else
         outputSNFiles(embryodir,name,esequence,min(tlist),max(tlist),downsample,1,1);

    end
        %else
    %    outputSNFiles([embryodir,'\',embryonumber],embryonumber,esequence,min(tlist),max(tlist),downsample);
    %end
end


clear X;
clear Xr;
save([embryodir,name,'_fullmatlabresult.mat']);
