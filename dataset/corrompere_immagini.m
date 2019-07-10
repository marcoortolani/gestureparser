for l=1:14
    for m=1:100
        path_orig=['dataset\G', int2str(l),'\',int2str(m), '-cont.bmp'];
        path_ruotata=['rotated\G', int2str(l),'\',int2str(m), '-cont.bmp'];
        path_rimpicciolita=['scaled\G', int2str(l),'\',int2str(m), '-cont.bmp'];
        path_ruotata_rimpicciolita=['rotated_and_scaled\G', int2str(l),'\',int2str(m), '-cont.bmp'];
        immagine_originale=imread(path_orig);
        R = [-45 45];
        rand_rot = rand(1,1)*range(R)+min(R);
        immagine_ruotata=imrotate(immagine_originale, rand_rot);
        R = [0.5 0.8];
        rand_rimp = rand(1,1)*range(R)+min(R);
        immagine_rimpicciolita=imresize(immagine_originale, rand_rimp);
        immagine_ruotata_rimpicciolita=imresize(imrotate(immagine_originale, rand_rot), rand_rimp);
        imwrite(immagine_ruotata, path_ruotata);
        imwrite(immagine_rimpicciolita, path_rimpicciolita);
        imwrite(immagine_ruotata_rimpicciolita, path_ruotata_rimpicciolita);
    end
end
