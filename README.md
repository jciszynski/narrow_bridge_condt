# Wąski most
Implemetacja problemu współdzielenia zasobów: "Wąski most" 

Z miasta A do miasta B prowadzi droga, na której znajduje się wąski most umożliwiający tylko ruch jednokierunkowy. Most jest również dość słaby, także może po nim przejeżdżać tylko jeden samochód na raz. Napisać program w którym N samochodów (wątków) będzie nieustannie przejeżdżało z miasta do miasta, pokonując po drodze most (N przekazywane jako argument linii poleceń). "Miasto" jest to funkcja, którą wątki mogą wykonywać niezależnie od siebie przez krótki, losowy czas (maks. kilka sekund).

W praktycznej realizacji projektu wykorzystano mechanizm "ticket Lock", dostęp do aktualnie wydawanego numeru jest chroniony przez mutex.
W funkcji ```town()``` oraz ```bridge()``` program generuje losową tablice liczb po czym ją sortuje z wykorzystaniem sortowania bąbelkowego. W zależności od procesora na którym uruchomiono program, funkcja ta wykonuje się przez ok. 1-3s


### Użycie
```bridge <N> [-debug]```

```N```  liczba samochodów (wątków)  
```-debug``` program wypisuje zawartości kolejek oraz aut przebywających w danych miastach



## Działanie programu
Bez parametru ```-debug``` program wraz z każdą aktualizacją stanu jednego z samochodów wypisuje następującą linie:  
```A-X1 X2>>> [>> X3 >>] <<<X4 X5-B```  
  
Gdzie:  
```X1``` Liczba aut w mieście A  
```X2``` Liczba aut w mieście A znajdujących się w kolejce do mostu  
```X3``` ID samochodu znajdującego się na moście, nawiasy określają kierunek w którym podąża samochód  
```X4``` Liczba aut w mieście B znajdujących się w kolejce do mostu  
```X5``` Liczba aut w mieście B  

Przy uruchomieniu programu z parametrem ```-debug``` program na dole ekranu wypisuje dodatkowo stany poszczególnych kolejek:

```
Cars in TOWN A:         5 8  
Cars in TOWN A QUEUE:  
Cars on bridge:         10  
Cars in TOWN B QUEUE:   11 12 13 14 0 1 3 7 2 4 6 9  
Cars in TOWN B:    
```
> !!!Aby program wyswietlał sie poprawnie, terminal powinien mieć min. 28 lini!!!

## Budowanie projektu
Program można zbudować z wykorzystaniem makefile'a ```make```

