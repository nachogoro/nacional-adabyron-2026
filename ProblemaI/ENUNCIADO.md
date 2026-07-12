# Problema I - La ampolleta

Durante siglos, antes del cuarzo y de los relojes atómicos, el tiempo en el mar se
midió grano a grano. La *ampolleta* era el reloj de arena de a bordo: un paje le
daba la vuelta al terminar cada media hora y cantaba en voz alta el paso del
tiempo, marcando con ello las guardias de la tripulación. Una de las primeras
imágenes claras de un reloj de arena aparece en un fresco que Ambrogio Lorenzetti
pintó en Siena hacia 1338; a diferencia del reloj de sol o de la clepsidra (reloj
de agua), la ampolleta seguía funcionando con marejada y balanceo, y por eso fue
durante mucho tiempo insustituible para la navegación.

Aquí no nos hará falta su mecánica, solo su imagen hipnótica: la arena que se
desliza de la ampolla de arriba a la de abajo, levantando un montículo mientras
arriba se va abriendo un hueco. Vamos a dibujar exactamente eso, congelado en un
instante dado.

Nuestra ampolleta tiene dos cámaras triangulares iguales: una arriba, que apunta
hacia abajo, y otra abajo, que apunta hacia arriba, unidas por un cuello de una
sola celda. Cada cámara tiene $L$ niveles de celdas de arena; contados desde el
cuello, los niveles tienen anchuras $1, 3, 5, \ldots$ (siempre impares), de modo
que cada nivel tiene una única celda central, por la que los granos pasan de uno en
uno. En total, cada cámara alberga $N = L^2$ celdas. En el instante 0 la cámara de
arriba está completamente llena de arena y la de abajo, vacía.

Para dibujarla usamos estos caracteres: las paredes con `\` y `/`, una celda con
arena con un asterisco `*` y una celda vacía con un espacio. Así se ve una
ampolleta con $L = 5$ recién dada la vuelta (instante 0):

```
\*********/
 \*******/
  \*****/
   \***/
    \*/
    / \
   /   \
  /     \
 /       \
/         \
```

Cada segundo cae exactamente un grano de la cámara de arriba a la de abajo. La
ampolleta está ligerísimamente inclinada hacia la izquierda, y eso decide todos los
empates.

### Cómo se apila la arena abajo

Un grano entra por el cuello y desciende por la columna central. Mientras puede,
baja a la celda que tiene justo debajo; si esa está ocupada, rueda a la celda de la
diagonal inferior izquierda; si esa también está ocupada, a la de la diagonal
inferior derecha; y cuando las tres celdas que tiene debajo están ocupadas (o ya ha
llegado al suelo), se detiene ahí. Todo esto ocurre dentro del mismo segundo: en
los dibujos solo mostramos la posición final tras cada caída.

Con $L = 5$, los ocho primeros granos se asientan así. Las letras `a`, `b`, `c`,
etcétera, marcan el orden en que caen; en el dibujo de verdad todas serían `*`:

```
    t=1               t=2               t=3               t=4
    / \               / \               / \               / \
   /   \             /   \             /   \             /   \
  /     \           /     \           /     \           /     \
 /       \         /       \         /       \         /   d   \
/    a    \       /   ba    \       /   bac   \       /   bac   \

    t=5               t=6               t=7               t=8
    / \               / \               / \               / \
   /   \             /   \             /   \             /   \
  /     \           /     \           /     \           /     \
 /   d   \         /  fd   \         /  fd   \         /  fdh  \
/  ebac   \       /  ebac   \       /  ebacg  \       /  ebacg  \
```

Fíjate en el grano `d`: cae cuando `a`, `b` y `c` ya ocupan las tres celdas de
debajo de la columna central, así que se queda un nivel más arriba. Y `e` tampoco
encuentra sitio en el centro, de modo que rueda hacia la izquierda y baja dos
diagonales hasta el suelo, mientras que `g` rueda hacia la derecha.

### Cómo se vacía la arena arriba

A medida que los granos se marchan, la cámara de arriba se va vaciando. El grano que
está en el cuello sale hacia abajo y la arena de encima se reacomoda para rellenar
el sitio. Dicho de otro modo: cada segundo aparece un *hueco* en el cuello y
asciende. Sube a la celda que tiene justo encima mientras allí haya arena; si la
celda de encima ya es un hueco, se desplaza a la diagonal superior derecha (y, si
tampoco puede, a la superior izquierda), y se detiene cuando ya no puede subir más.

Con $L = 5$, los ocho primeros huecos se forman así. Ahora las mayúsculas `A`, `B`,
`C`, etcétera, marcan el orden en que se vacía cada celda; recuerda que un hueco es
en realidad un espacio:

```
    t=1               t=2               t=3               t=4
\****A****/       \****AB***/       \***CAB***/       \***CAB***/
 \*******/         \*******/         \*******/         \***D***/
  \*****/           \*****/           \*****/           \*****/
   \***/             \***/             \***/             \***/
    \*/               \*/               \*/               \*/

    t=5               t=6               t=7               t=8
\***CABE**/       \***CABE**/        \**GCABE**/       \**GCABE**/
 \***D***/         \***DF**/          \***DF**/         \**HDF**/
  \*****/           \*****/            \*****/           \*****/
   \***/             \***/              \***/             \***/
    \*/               \*/                \*/               \*/
```

Como la ampolleta se inclina hacia la izquierda, el montículo de abajo se carga
hacia ese lado; el hueco de arriba, en cambio, se desvía hacia la derecha.

### El reloj completo

Juntando las dos cámaras, en cada instante $t$ (con $0 \le t \le N$) hay exactamente
$t$ granos caídos: $t$ huecos arriba y $t$ granos apilados abajo. Para $L = 5$:

```
    t=0               t=6               t=12              t=25
\*********/       \***    **/       \**      */       \         /
 \*******/         \***  **/         \**    */         \       /
  \*****/           \*****/           \**  */           \     /
   \***/             \***/             \***/             \   /
    \*/               \*/               \*/               \ /
    / \               / \               / \               /*\
   /   \             /   \             /   \             /***\
  /     \           /     \           / **  \           /*****\
 /       \         /  **   \         / ****  \         /*******\
/         \       /  ****   \       / ******  \       /*********\
```

Dados $L$ y $T$, queremos dibujar la ampolleta tal y como se ve justo después de
$T$ segundos.

## Entrada

La primera línea contiene un entero $C$ ($0 \le C \le 2000$), el número de casos de
prueba. Cada una de las $C$ líneas siguientes contiene dos enteros $L$ y $T$
separados por un espacio: $L$ es el número de niveles de cada cámara y $T$ el número
de segundos transcurridos desde que se dio la vuelta a la ampolleta, con
$1 \le L \le 20$ y $0 \le T \le L^2$. La suma de los valores $L^2$ de todos los
casos de prueba no supera $2 \cdot 10^5$.

## Salida

Para cada caso de prueba se dibujará su ampolleta tras $T$ segundos, que ocupará
exactamente $2L$ líneas, y a continuación una línea en blanco (también después del
último dibujo). Cada celda con arena se representa con un asterisco `*` y cada celda
vacía con un espacio en blanco; las paredes son los caracteres `/` y `\`. Ninguna
línea del dibujo termina en espacios: todas acaban en una pared (`/` o `\`). Sí
hay, en cambio, espacios a la izquierda: la primera y la última línea (las más
anchas) empiezan en la primera columna, y las demás van precedidas de los espacios
justos para alinear las paredes y formar la silueta de la ampolleta.

## Ejemplos

### Entrada de ejemplo
```
3
4 0
4 6
3 9
```

### Salida de ejemplo
```
\*******/
 \*****/
  \***/
   \*/
   / \
  /   \
 /     \
/       \

\**    */
 \**  */
  \***/
   \*/
   / \
  /   \
 / **  \
/ ****  \

\     /
 \   /
  \ /
  /*\
 /***\
/*****\
```
