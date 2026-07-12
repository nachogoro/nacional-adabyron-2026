# Introducción a la solución

Tenemos un anillo de $N$ bombillas, cada una encendida o apagada. La única
restricción es que no puede haber más de $K$ encendidas seguidas en ningún tramo
del anillo (y "tramo" incluye el punto donde la guirnalda se cierra sobre sí
misma). Hay que contar cuántas iluminaciones distintas cumplen eso, módulo
$10^9 + 7$. Como cada bombilla cuelga de un punto fijo, dos iluminaciones son la
misma solo si coinciden bombilla a bombilla: girar el anillo produce, en general,
una iluminación diferente y cuenta aparte.

Dicho de otro modo: buscamos el número de **cadenas binarias circulares** de
longitud $N$ (un $1$ por bombilla encendida, un $0$ por apagada) en las que
ninguna racha de unos consecutivos (contando la que pueda envolver el cierre)
supere $K$.

# Primera aproximación: fuerza bruta

Recorrer las $2^N$ iluminaciones es imposible: con $N$ hasta $10^6$, el número
de iluminaciones es astronómico y ni siquiera podríamos enumerarlas.
Necesitamos *contarlas sin listarlas*. Por suerte, la condición "no más de $K$
encendidas seguidas" es local, y las condiciones locales se prestan muy bien a
recorrer la secuencia llevando un pequeño resumen del pasado reciente.

# Recorrer el anillo con una matriz de transferencia

Podemos avanzar sobre las bombillas una a una. Lo único que necesitamos
recordar del pasado, para saber si podemos encender la siguiente, es cuántas
encendidas llevamos seguidas justo ahora. Ese número (llamémoslo *estado*) va
de $0$ (la última estaba apagada) hasta $K$ (llevamos el máximo permitido).
Las transiciones son:

- **apagar** la bombilla actual: siempre se puede, y el estado vuelve a $0$;
- **encender** desde un estado $i < K$: se puede, y el estado pasa a $i + 1$;
- **encender** desde el estado $K$: **no se puede**, porque dejaría $K + 1$
  encendidas seguidas.

Si la guirnalda fuese una *fila* abierta, contar las secuencias válidas con
esto sería inmediato usando programación dinámica con un estado doble:
*(bombilla actual, racha de bombillas encendidas)*. Pero es un *anillo*: la
racha con la que terminamos de recorrerlo se pega a la racha con la que
empezamos, y hay que cerrar el círculo de forma coherente.

La herramienta natural para este tipo de recuentos es la *matriz de
transferencia*: una matriz $M$ de tamaño $(K+1) \times (K+1)$ donde
$M[a][b] = 1$ si existe la transición del estado $b$ al estado $a$, y $0$ si
no. Para $K = 2$, por ejemplo:

$$M = \begin{pmatrix} 1 & 1 & 1 \\ 1 & 0 & 0 \\ 0 & 1 & 0 \end{pmatrix}$$

La primera fila dice que desde cualquier estado podemos apagar la bombilla
actual (y volver al estado $0$); los otros unos, que desde el estado $i < K$
podemos encenderla (y pasar al estado $i + 1$).

¿Qué ganamos escribiendo las transiciones así? Que multiplicar matrices hace
exactamente la cuenta que nos interesa. Por la definición del producto,

$$(M^2)[a][b] = \sum_c M[a][c] M[c][b],$$

y cada sumando vale $1$ exactamente cuando existen las dos transiciones
$b \to c$ y $c \to a$. Es decir, $(M^2)[a][b]$ es el número de formas de ir del
estado $b$ al estado $a$ en dos pasos. Repitiendo el argumento (o por
inducción):

$$(M^n)[a][b] = \text{número de caminos de longitud } n \text{ del estado } b \text{ al estado } a,$$

donde un *camino* es una sucesión $b = s_0 \to s_1 \to \cdots \to s_n = a$ en
la que cada paso es una transición permitida.

Y los caminos son justo lo que queremos contar: cada paso es una decisión
sobre una bombilla (apagarla si el paso baja al estado $0$, encenderla si sube
de $i$ a $i + 1$). Un camino de longitud $N$ equivale, por tanto, a una
iluminación de las $N$ bombillas junto con una suposición sobre el estado
inicial $s_0$: la racha de encendidas con la que llegamos a la bombilla $1$.

Si la guirnalda fuese una fila abierta, antes de la primera bombilla no habría
nada: arrancaríamos en $s_0 = 0$ y valdría cualquier estado final. En el
anillo no existe ese "antes de la primera bombilla": lo que precede a la
bombilla $1$ es la bombilla $N$. La racha con la que de verdad entramos a la
posición $1$ no la podemos fijar nosotros; es la que queda tras recorrer el
anillo entero, es decir, $s_N$.

Por eso la suposición inicial tiene que coincidir con la realidad: solo los
caminos con $s_0 = s_N$ describen un anillo coherente. Un camino con
$s_0 = s_N$ es un *recorrido cerrado*: empieza y termina en el mismo estado. Y
contar recorridos cerrados es exactamente lo que hace la **traza** (la suma de
la diagonal):

$$\operatorname{tr}(M^N) = \sum_s (M^N)[s][s] = \text{número de recorridos cerrados de longitud } N.$$

## Un detalle: la traza no cuenta el anillo todo encendido

El anillo **todo encendido** es el único que la traza no puede representar:
encender sube el estado en cada paso, así que un camino que solo enciende
nunca vuelve a su estado de partida (y en cuanto alcanza $K$ ni siquiera puede
continuar). Por tanto,

$$P_n := \operatorname{tr}(M^n) = \text{anillos válidos de longitud } n \text{ con al menos un apagado.}$$

Cuando $K < N$ esto no nos quita nada: el anillo todo encendido tendría una
racha de longitud $N > K$ y no es válido de todos modos, así que la respuesta
es exactamente $P_N$. El único caso en que el todo encendido sí vale es
$K = N$, y lo trataremos por separado.

# Optimizar el cálculo de la traza

Queda el problema de calcular $\operatorname{tr}(M^N)$ sin elevar de verdad la
matriz: $M$ puede ser de tamaño $\sim 10^6$, así que ni exponenciación rápida de
matrices ($O(K^3 \log N)$) nos sirve. La salida es que la sucesión
$P_n = \operatorname{tr}(M^n)$ cumple una **recurrencia lineal**.

El polinomio característico de $M$ resulta ser

$$x^{K+1} = x^K + x^{K-1} + \cdots + x + 1$$

(se comprueba sin dificultad; para $K = 1$ es $x^2 = x + 1$, el de Fibonacci, y
para $K = 2$ es $x^3 = x^2 + x + 1$). Por el teorema de *Cayley–Hamilton*, $M$
satisface su propio polinomio característico:

$$M^{K+1} = M^K + M^{K-1} + \cdots + M + I.$$

Multiplicando por $M^{n-K-1}$ y tomando trazas (la traza es lineal), la misma
relación pasa a la sucesión de trazas:

$$P_n = P_{n-1} + P_{n-2} + \cdots + P_{n-K-1}, \qquad n \ge K + 1.$$

Es decir, cada término es la suma de los $K + 1$ anteriores.

## Condiciones iniciales

Para arrancar la recurrencia necesitamos las $K + 1$ primeras trazas
$P_0, P_1, \ldots, P_K$. Aquí la interpretación combinatoria de $P_n$ nos las
regala:

- $P_0 = \operatorname{tr}(I) = K + 1$ (la matriz es $(K+1) \times (K+1)$).
- Para $1 \le n \le K$: $P_n$ cuenta los anillos de longitud $n$ con algún
  apagado. Pero si $n \le K$, **cualquier** anillo con al menos un apagado es
  válido: teniendo un $0$, la racha de unos más larga mide a lo sumo $n - 1 < K + 1$,
  así que nunca se pasa. Como el total de cadenas de longitud $n$ es $2^n$ y solo
  excluimos el todo encendido,

  $$P_n = 2^n - 1 \qquad (1 \le n \le K).$$

# Juntándolo todo

El algoritmo por caso queda así:

1. Si $K = N$, la respuesta es $2^N \bmod (10^9 + 7)$ (por exponenciación rápida):
   no hay restricción efectiva, todas las iluminaciones valen.
2. Si $K < N$, calculamos $P_0 = K + 1$ y $P_n = 2^n - 1$ para $1 \le n \le K$, y
   luego aplicamos $P_n = P_{n-1} + \cdots + P_{n-K-1}$ hasta $P_N$. La suma de los
   $K + 1$ términos anteriores no hace falta recalcularla entera cada vez: se
   mantiene una **ventana deslizante** (al avanzar de $n$ a $n + 1$ se suma el
   término nuevo y se resta el que se cae por el otro extremo), de modo que cada
   $P_n$ cuesta $O(1)$.

El coste es $O(N)$ por caso y $O\left(\sum N\right)$ en total, de sobra con
$\sum N \le 10^6$.

# Soluciones

| Solución | Descripción | Verificado con el juez |
| :------: | :---------- | :--------------------: |
| [H.cpp](src/H.cpp) | Recurrencia lineal $P_n = P_{n-1} + \cdots + P_{n-K-1}$ (traza de la matriz de transferencia) con ventana deslizante, $O(N)$ por caso. | :white_check_mark: |
