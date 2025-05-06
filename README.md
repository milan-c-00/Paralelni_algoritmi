U repozitorijumu se nalazi dokumentacija i programski kodovi projekta u kojem je obradjena paralelizacija BFS i DFS algoritma. 
Programski kod za ova dva algoritma se nalazi u fajlovima bfs_par.cpp i dfs_par.cpp. Oba programa su napisana u jeziku C++ uz koriscenje OpenMP.

Za pokretanje programa potreban je odgovarajuci kompajler. 
Kompajliranje se vrsi sledecim naredbama kroz terminal:
g++ -fopenmp bfs_par.cpp -o bfs_par
g++ -fopenmp dfs_par.cpp -o dfs_par

Pokretanje programa se takodje vrsi kroz terminal:
./bfs_par
./dfs_par

Napomena: Programi u datoj verziji vrse testiranje na generisanim grafovima sa milion cvorova.
Za grafove drugacije velicine promijeniti velicinu u funkciji main i ponovo kompajlirati program.
Trenutno je aktivno generisanje binarnog stabla. 
Za generisanje grid grafa zakomentarisati binarno stablo i skloniti komentare sa grid grafa.
