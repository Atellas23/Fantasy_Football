# AP3_Fantasy_Football
## Coses que funcionen
### `driver.cc` :
`driver.cc` és un programa que compila i executa l'arxiu `greedy.cc` sobre la base de dades `data_base.txt` i totes les consultes dins de la carpeta `public_benchs` .
### Comentaris
ALEX: he escrit aquestes merdes perque ho llegeixis a veure que en penses
1. igual que 3, son les millors pels hard
return double(points*points*points)+double(1)/(price+1) > double(J.points*J.points*J.points)+double(1.0)/(J.price+1);
2. millora en mes del 60% pero em molesta
return double(points*points*points)/(pow(log(price + 2),1.25)) > double(J.points*J.points*J.points)/pow(log(J.price + 2),1.25);
3. funciona igual de be que 1
if (points == J.points) return price < J.price;
return points > J.points;
COM A CONCLUSIO: diria que si ho provem amb les bases de dades per la
                    metaheuristica sortirem de dubtes, ja que tindrem mes
                    merda per comparar.
4. ho provem amb el preu per jugador
cout << (double(points*points*points)) << " " << (double(1)/(j - price)) << endl;
ALEIX: he posat uns coeficients que equilibren, milloren bastant els easy pero empitjoren poquet els medium
Aquestes comparacions son inutils pero jo optaria per posar-les en el que entreguem
if (j == price) return true;
if (j == J.price) return false;
