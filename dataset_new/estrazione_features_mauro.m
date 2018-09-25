function [features_stampa] = estrazione_features_mauro (label, immagine)
stats=regionprops(immagine);
centroid=floor(stats(1).Centroid);
bwboud = bwboundaries(immagine);
contorno_list = bwboud{1};
max_value=0;
%punto_iniziale= contorno_list(1,:);
%im_centroide_e_punto_iniziale=zeros(size(immagine));
%im_centroide_e_punto_iniziale(centroid(2)-2:centroid(2)+2,centroid(1)-2:centroid(1)+2) =1;
%im_centroide_e_punto_iniziale(punto_iniziale(1)-2:punto_iniziale(1)+2,punto_iniziale(2)-2:punto_iniziale(2)+2) =1;
%figure(1), imshow(immagine | im_centroide_e_punto_iniziale), title('immagine originale con evidenziati il centroide e il punto iniziale');
vettore_distanze=zeros(1,size(contorno_list,1));
max_value_index=1;
for i = 1 : size(contorno_list)
    punto=[contorno_list(i,2),contorno_list(i,1)];
    vettore_distanze(i) = distanza_tra_2_punti( punto,centroid);
    if vettore_distanze(i)> max_value
        max_value = vettore_distanze(i);
        max_value_index=i;
    end
end
vettore_distanze=[vettore_distanze(max_value_index:size(contorno_list)), vettore_distanze(1:max_value_index-1)];
%vettore_distanze=200*vettore_distanze/max_value;
%figure(2)
%points=[1: size(contorno_list)];
%subplot(1,2,1),plot(points,vettore_distanze), title('Features prima del campionamento');
%ylim([0, 1]);
%provo un campionamento
indici_vettore_iniziale=[1:size(contorno_list,1)];
costante=200/size(contorno_list,1); %il vettore finale sarà di 200 campioni
features=zeros(1,200);
indici_vettore_iniziale=ceil(indici_vettore_iniziale(:)*costante); %ho un vettore di indici per il campionamento
if costante < 1 %sottocampiono
    for i=1:200
        temp_val=0;
        temp_ind=find(indici_vettore_iniziale==i);
        for j=1:size(temp_ind,1)
            temp_val=temp_val+vettore_distanze(temp_ind(j));
        end
        features(i)=temp_val/(size(temp_ind,1));
    end
else %sovracampiono
    features(1)=vettore_distanze(1);
    for i=2:200
        temp_ind=[];
        temp_ind=find(indici_vettore_iniziale==i);
        j=i;
        while size(temp_ind,1)==0
            j=j+1;
            temp_ind=find(indici_vettore_iniziale==j);
        end
        if i==j
            features(i)=vettore_distanze(temp_ind);
        else
            features(i)=(features(i-1)+vettore_distanze(temp_ind))/2;
        end
    end
end  
%points=[1:200];
%subplot(1,2,2), plot(points,features),title('Features dopo il campionamento');
%ylim([0, 1]);
features_stampa=int2str(label);
for i=1:200
    features_stampa=[ features_stampa, ' ', int2str(i), ':', num2str(features(1,i))];
end
end

function [ distanza ] = distanza_tra_2_punti(P, Q )
Xdiff = Q(1) - P(1);
Ydiff = Q(2) - P(2);
QX = Xdiff^2;
QY = Ydiff^2;
Rad = QX + QY;
distanza = sqrt(Rad);
end