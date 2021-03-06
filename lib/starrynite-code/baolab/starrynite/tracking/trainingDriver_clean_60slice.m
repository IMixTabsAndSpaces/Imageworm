
%to train novel data set place unzipped edited and unedited lineages in a
%directory 'basedir' along with .mat files from the initial segmentation
%run that generated the unedited lineages (from naive mode or other parameter set) 
%set lineages to base names of data sets, edittimes is the list of last
%edited timepoint. basedir is the directory where files are located

%red 10 training set, when these are used filtering needs to be on in
%answer key generation

%setup of cases for tracking model 10 training embryos

edittimes=270;
lineages={'ZD_JIM113_0.5_20130310_2_s1_emb1'}
%{'ZD_RW10348_60_20130409_3_s2_emb_offset_corrected2'}

basedir='L:\santella\unzipped_lineages\training_60slice\'


%setup for 5 test embs 
%grafted here from training driver to compute confusion matrix on test data
%for bmc bioinformatics revision
%when these are used for training filtering should be off
%{
lineages={'ZD_BV82_WT_20100809_2_s1_emb_linbiftest_lowthresh1','ZD_RW10425_WT_20100412_2_s1_emb1',...
    'ZD_RW10425_WT_20100412_2_s1_emb2','ZD_RW10425_WT_20100412_2_s1_emb3','ZD_RW10434_WT_20110429_2_s1_emb_linbiftest1'};

lineagedir={'ZD_BV82_WT_20100809_2','ZD_RW10425_WT_20100412_2',...
    'ZD_RW10425_WT_20100412_2','ZD_RW10425_WT_20100412_2','ZD_RW10434_WT_20110429_2'};
lineageimage={'ZD_BV82_WT_20100809_2_s1','ZD_RW10425_WT_20100412_2_s1',...
    'ZD_RW10425_WT_20100412_2_s1','ZD_RW10425_WT_20100412_2_s1','ZD_RW10434_WT_20110429_2_s1'};
edittimes=[200,192,201,185,185]
eightcelltimes=[20,20,30,20,20];
basedir='L:\santella\unzipped_lineages\test_data\'
%}

 
nondivthress=[];

%'green mode'
%parameterConfigurationGreen
'red mode'

%note this call is necessary if initial segmentation lacked
%trackingparameters in parameter file, if that run did have
%trackingparameters they will be overwritten by those in
%parameterconfiguration, and if intentional changes were made to
%trackingparameters this should be remmed out, or parameters in
%parameterconfiguration should be updated.
%paramet erConfiguration

%trainingmode refers to bif. classifier training not confidence
%uses oracle

answerkey=true;
outputtrimmed=false;
errors=cell(1,length(lineages));
replacemodel=false;
allexpectedchange={};
evalforced=false;
evalfinal=false;
allbifurcationinfo=[];

for lin=1:length(lineages)
    endtime=edittimes(lin);
    trackingparameters.endtime=endtime;
    %'loader for files in zhuos directory'
     %load([basedir,lineagedir{lin},'/',lineages{lin},'_fullmatlabresult.mat']);
    'loader for training files'
     load([basedir,lineages{lin},'_fullmatlabresult.mat']);
    
    trackingparameters.trainingmode=true;
    trackingparameters.useclassifieroracle=false;
        
    trackingparameters.anisotropyvector=[1,1,anisotropy];
    parameters.anisotropyvector=[1,1,anisotropy];
    
    embryonumbers = {};
    nucleidir=basedir;
    embryonumbers_c={[lineages{lin},'_edited\nuclei\']};
    %endtime=edittimes(lin)+11;
    outputdirectory=[lineages{lin},'/nuclei/'];
    if (answerkey)
        train_tracking_statistics_function;
        train_confidence_function;
    end
    endtime=endtime+10;
    trackingparameters.endtime=trackingparameters.endtime+10;
    

  
  
    trackingparameters.recordanswers=false;
    
    tic
    tracking_driver_new_classifier_based_version;
    toc
    
    %time adjustment back for final evaluation
    %endtime=endtime-9;
    %trackingparameters.endtime=trackingparameters.endtime-9;
    %reset back for final evaluation
    if(trackingparameters.trainingmode==false)
        endtime=endtime-11;
        trackingparameters.endtime=trackingparameters.endtime-11;
    end
    
    
    %section for storing training info
    ncells=[];
    for i=1:size(removed,1)
        % ncells=[ncells,length(esequence{removed(i,1)}.FP)];
        ncells=[ncells,mean(esequence{removed(i,1)}.selfdistance)./mean(esequence{removed(i,1)}.finaldiams)];
    end
    if (trackingparameters.trainingmode) %training mode reall means training for bifurcation model
        allbifurcationinfo(lin).Divdata=Divdata;
        allbifurcationinfo(lin).Tripledata=Tripledata;
        allbifurcationinfo(lin).NoDivdata=NoDivdata;
        allbifurcationinfo(lin).ncells=ncells;
        allbifurcationinfo(lin).removed=removed;
        allbifurcationinfo(lin).confidenceData=confidenceData;
        allbifurcationinfo(lin).splitFNMatchScore=splitFNMatchScore;
        allbifurcationinfo(lin).BifurcationMeasures=BifurcationMeasures;
        allbifurcationinfo(lin).classround=classround;
        allbifurcationinfo(lin).computedclassificationvector=computedclassificationvector;
        allbifurcationinfo(lin).refclassificationvector= refclassificationvector;
    end
    if(~outputtrimmed)
        [ linkconfidencedata] ...
            = extractTrainingConfidenceDataVectors( esequence,trackingparameters,embryonumbers_c,nucleidir,ROI,ROIxmin,ROIymin  );
        allbifurcationinfo(lin).linkconfidencedata=linkconfidencedata;
    end
    if(evalfinal)
        
        mkdir(outputdirectory);
        saveGreedyNucleiFiles(esequence,endtime,outputdirectory,anisotropy,ROIxmin,ROIymin);
        zipname=[lineages{lin},'/',embryonumber,'_',suffix,'.zip'];
        zip(zipname,[outputdirectory,'']);
        
        uneddir=outputdirectory;
        eddir=[basedir,lineages{lin},'_edited\nuclei\'];
        %    uneddir=[basedir,lineages{i},'\nuclei\'];
        test=evaluate_lineage_error(eddir,uneddir,endtime,anisotropy);
        test{37}=test{10}+test{21}+test{32};
        test{38}=test{12}+test{23}++test{34};
        test{39}=sum(test{37}+test{38});
        test{40}=sum(test{37});
        test{41}=sum(test{38});
        
        errors{lin}=test;
    end
    if(trackingparameters.trainingmode)
        allexpectedchange{lin}=expected_corrections;
    end
    
    
   
     finaloutputdirectory='L:\santella\lineage_automerge\80version_conservativemodel_ab8_wholescore\';
        save([finaloutputdirectory,lineages{lin},'.mat']);
   
    
end
    multiple_embryo_train;

           