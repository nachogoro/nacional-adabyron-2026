# Introducción a la solución
El problema pide calcular la probabilidad máxima de aprobar un examen tipo
test, dada la probabilidad de acertar cada una de las preguntas, suponiendo que
se aprueba obteniendo la mitad o más de la puntuación del examen, con todas las
preguntas valiendo lo mismo, y con una respuesta errónea restando la mitad del
peso de una pregunta.

## Primeras observaciones
Si el examen tiene un total de $N$ preguntas, podemos definir $P(c)$ como la
probabilidad de aprobar el examen respodiendo exactamente $c$ preguntas. Como
estudiante, sólo tenemos control sobre el número de preguntas a responder, nada
más, luego la estrategia óptima consiste en elegir el número de preguntas a
responder $c$, de tal manera que maximicemos $P(c)$. Es decir:

$$\max_{c=0}^N\left(P(c)\right)$$

La condición de aprobar el examen con $c$ preguntas respondidas depende de
cuántas preguntas acertemos: si llamamos $b$ al número de preguntas
acertadas, el número de preguntas respondidas erróneamente es $c-b$. La
puntuación del examen sería $1 \cdot b - 0.5 \cdot (c-b)$, siendo $N$ la
puntuación máxima.

Como la puntuación mínima para aprobar es $\frac{N}{2}$ puntos, nuestra
condición para pasar el examen es

$$1 \cdot b - 0.5 \cdot (c-b) \geq \frac{N}{2}.$$

Aplicando aritmética podemos obtener esta simplificación:

$$3b-c \geq N$$

Incluso podemos despejar $b$ en función de $N$ y $c$:

$$b \geq \left\lceil\frac{N+c}{3}\right\rceil$$

De esta forma podemos definir $P(b|c)$ como la probabilidad de acertar
exactamente $b$ preguntas si contestamos $c$.

Ahora podemos definir matemáticamente la probabilidad de aprobar el examen:

$$P(c) = \sum_{b=0}^c \left[P(b|c) \text{ si } b \geq \left\lceil\frac{N+c}{3}\right\rceil\right]$$

Teniendo en cuenta la cota inferior de $b$:

$$P(c) = \sum_{b=\left\lceil\frac{N+c}{3}\right\rceil}^c P(b|c)$$

La otra observación importante es que, si contestamos a $c$ preguntas, siempre
obtenemos la probabilidad máxima de aprobar si contestamos a las $c$ preguntas
con mayor probabilidad de acierto, por lo que no hace falta tener en cuenta
todas las combinaciones de $c$ preguntas, sólo considerar las $c$ preguntas
cuya respuesta es más segura.

# Programación dinámica

Solo falta ver cómo obtener $P(b|c)$. Podemos usar una definición recursiva
para ello: se puede observar que la probabilidad de acertar exactamente $b$
preguntas de $c$ contestadas es igual a la suma de:
- la probabilidad de acertar $b-1$ preguntas de $c-1$ contestadas y acertar la
  pregunta $p_c$.
- la probabilidad de acertar $b$ preguntas de $c-1$ contestadas y fallar la
  pregunta $p_c$

Esto se traduce en:

$$P(b|c) = P(c-1,b-1) \cdot p_c + P(c-1,b) \cdot (1-p_c)$$

Es decir, que conociendo $P(b-1 | c-1)$ y $P(b | c-1)$ podemos calcular $P(b|c)$:
recorriendo primero por los distintos valores de $c$, y después por los de $b$
podemos obtener todos los valores de $P(b|c)$.

Para los estados imposibles definimos la convención $P(b|c)=0$ cuando $b<0$,
$b>c$ o $c<0$. Así, el único caso base real es $P(0,0)=1$.

# Algoritmo
1. Ordenamos las preguntas de mayor a menor probabilidad de acierto.
2. Mantenemos una matriz `dp[c][b]` para ir calculando y almacenando los
   valores de $P(b|c)$
3. Recorremos la matriz por filas para obtener cada $P(c)$ haciendo el
   sumatorio de los valores $P(b|c)$ a partir de
   $b \geq \left\lceil\frac{N+c}{3}\right\rceil$.

   El $P(c)$ actual se compara con la máxima probabilidad obtenida hasta el
   momento y se guarda el mayor resultado de los dos.

> [!NOTE]
> En Python, en vez de mantener una matriz se puede hacer que una función
> memorice valores de retorno mediante el decorador `@functools.cache`.

# Optimización de memoria

Al calcular la matriz `dp` se observa que cada fila $c$ se obtiene únicamente a
partir de la fila anterior $c-1$ (así lo dice la recurrencia de $P(b|c)$). En
cuanto tenemos la fila $c$, las filas $c-2$ y anteriores ya no se vuelven a
consultar.

Esto permite ahorrar memoria: en lugar de guardar la matriz entera, basta con
mantener **dos filas**, la anterior (`prev`) y la que estamos calculando
(`next`). Al terminar cada fila, `next` pasa a ser la `prev` de la siguiente
iteración. Con ello la memoria baja de $O(N^2)$ a $O(N)$, sin cambiar el número
de operaciones (sigue siendo $O(N^2)$ en tiempo).

Con $N \le 100$ la matriz completa ocupa muy poco y no hay ningún problema de
memoria, así que esta optimización no es realmente necesaria aquí; la incluimos
por su valor didáctico, ya que es una técnica muy habitual en programación
dinámica.

Como ya no conservamos la matriz para recorrerla al final, aprovechamos el
mismo bucle para ir acumulando $P(c)$ sobre la marcha: mientras rellenamos la
fila, sumamos las celdas que cumplen la condición de aprobado.

# Soluciones

| Solución | Descripción | Verificado con el juez |
| :------: | :---------- | :--------------------: |
| [N.cpp](src/N.cpp) | Programación dinámica con matriz completa, $O(n^2)$ | :white_check_mark: |
| [N_single_row.cpp](src/N_single_row.cpp) | Programación dinámica con una sola fila, memoria $O(n)$ | :white_check_mark: |
| [N.py](src/N.py) | Programación dinámica (memoización), $O(n^2)$| :white_check_mark: |
