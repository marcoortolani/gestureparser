tic
file = fopen('dataset-mauro','w');

for l=1:11
    for subject=1:4
    for m=1:30
        path=['BMP\S', int2str(subject), '\G', int2str(l),'\',int2str(m), '-cont.bmp'];
        imwrite(soglia(subject, l, m), path);
        fprintf(file,estrazione_features_mauro(l,soglia(subject, l, m)));
        fprintf(file,'\n');
    end
    end
end
fclose(file);
toc