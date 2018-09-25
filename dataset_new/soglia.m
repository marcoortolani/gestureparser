function immagine = soglia(subject, gesture, image)
    path=['S', int2str(subject), '\G', int2str(gesture),'\',int2str(image)];
    filename_d=[path, '-conf.bin'];
    filename_c=[path, '-color.png'];
    %subplot(2,2,1), imshow(imread(filename_c)), title('immagine a colori');
    fileID = fopen(filename_d);
    A = fread(fileID,[320,240],'uint16=>uint16');
    fclose(fileID);
    A=double(A);
    A=reshape(A, 320, 240).';
    %subplot(2,2,2), imagesc(A), title('Immagine Confidence');
    H=fspecial('average', [5,5]);   %filtro di media
    A=imfilter(A,H);
    A=A>400;
    %A=A>(max(A(:))/3);
    SE = strel('arbitrary',eye(5));
    A=imerode(A,SE);
    SE = strel('arbitrary',eye(5));
    A=imdilate(A,SE);
    %subplot(2,2,3), imshow(A), title('Immagine sogliata, erosa e dilatata');
    A=edge(A,'canny');
    %subplot(2,2,4), imshow(A), title('Contorno estratto con Canny');
    immagine= bwareaopen(A,80,8);
end