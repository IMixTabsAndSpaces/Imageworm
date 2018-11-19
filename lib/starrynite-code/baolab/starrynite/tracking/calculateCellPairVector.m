function [ datavector ] =calculateCellPairVector(nucs,i,nucs2,j,anisotropy)
%computes gfp data change for a pair
%{
 datavector=[log(distance_anisotropic(nucs.finalpoints(i,:)',nucs2.finalpoints(j,:)',anisotropy)/mean(nucs.selfdistance)+1),...
    log(nucs2.totalGFP(j)/nucs.totalGFP(i)+1),...
      nucs2.avgGFP(j)/nucs.avgGFP(i),...
    nucs2.finaldiams(j)/nucs.finaldiams(i)];
%}
datavector=[    log(nucs2.totalGFP(j)/nucs.totalGFP(i)+1),...
    nucs2.avgGFP(j)/nucs.avgGFP(i),...
    ];
datavector(isinf(datavector))=0;
datavector(isnan(datavector))=0;
end
