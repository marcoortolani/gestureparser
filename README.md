# gestureparser
Gesture parser through a probabilistic grammar

Nota: il codice include lo unit testing tramite Google test.
Le librerie di testing sono aggiunte come un submodule tramite il comando:

`git submodule add https://github.com/google/googletest.git gtest`

Per aggiornare il submodule, una volta clonato il progetto da git, si possono dare i comandi:

`git submodule init`

`git submodule update`

oppure direttamente:

git clone --recursive `https://github.com/marcoortolani/gestureparser.git`

Per compilare:
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make

oppure (per il testing):
cmake -G "Unix Makefiles" -Dtest=on ..
make && make test
