# Introducción a la solución
El problema pide calcular la probabilidad máxima de aprobar un examen tipo test, dada la probabilidad de acertar cada una de las preguntas.

## Primeras observaciones
Si el examen tiene un total de $N$ preguntas y contestamos $c$ preguntas, podemos definir $P(c)$ como la probabilidad de aprobar el examen respodiendo exactamente $c$ preguntas. Por tanto, nuestra solución es: $$\max_{c=0}^N\left(P(c)\right)$$

La condición de aprobar el examen con $c$ preguntas respondidas depende de cuántas preguntas acertaríamos: si llamamos $b$ al número de preguntas acertadas, el número de preguntas respondidas erróneamente es $c-b$. La puntuación del examen sería: $$1 \cdot b + 0.5 \cdot (c-b)$$ Como la puntuación mínima para aprobar es $\frac{N}{2}$ puntos, nuestra condición para pasar el examen es $$1 \cdot b - 0.5 \cdot (c-b) \geq \frac{N}{2}$$ Aplicando aritmética podemos obtener esta simplificación: $$3b-c \geq N$$ Incluso podemos despejar $b$: $$b \geq \left\lceil\frac{N+c}{3}\right\rceil$$

De esta forma podemos definir $P(c,b)$ como la probabilidad de acertar exactamente $b$ preguntas si contestamos $c$. Ahora podemos definir matemáticamente la probabilidad de aprobar el examen: $$P(c) = \sum_{b=0}^c \left[P(c,b) \text{ si } b \geq \left\lceil\frac{N+c}{3}\right\rceil\right]$$ Teniendo en cuenta la cota inferior de $b$: $$P(c) = \sum_{b=\left\lceil\frac{N+c}{3}\right\rceil}^c P(c,b)$$

Otra observación importante es que si contestamos a $c$ preguntas, siempre obtenemos la probabilidad máxima de aprobar si contestamos a las $c$ preguntas con mayor probabilidad de acierto, por lo que no hace falta tener en cuenta todas las combinaciones de $c$ preguntas. Si las probabilidades ($p_1,p_2,\dots,p_N$) están ordenadas, solo hace falta tener en cuenta de $p_1$ a $p_c$.

# Programación dinámica
Solo falta ver cómo obtener $P(c,b)$. Podemos usar una definición recursiva para ello: se puede observar que la probabilidad de acertar exactamente $b$ preguntas de $c$ contestadas es igual a la suma de:
- la probabilidad de acertar $b-1$ preguntas de $c-1$ contestadas y acertar la pregunta $p_c$.
- la probabilidad de acertar $b$ preguntas de $c-1$ contestadas y fallar la pregunta $p_c$

Esto se traduce a: $$P(c,b) = P(c-1,b-1) \cdot p_c + P(c-1,b) \cdot (1-p_c)$$

Es decir, que conociendo $P(c-1,b-1)$ y $P(c-1,b)$ podemos calcular $P(c,b)$: recorriendo primero por los distintos valores de $c$, y después por los de $b$ podemos obtener todos los valores de $P(c,b)$.

Para los estados imposibles definimos la convención $P(c,b)=0$ cuando $b<0$, $b>c$ o $c<0$. Así, el único caso base real es $P(0,0)=1$.

# Algoritmo
1. Ordenamos las preguntas de mayor a menor probabilidad de acierto.
2. Mantenemos una matriz `dp[c][b]` para ir calculando y almacenando los valores de $P(c,b)$
3. Recorremos la matriz por filas para obtener cada $P(c)$ haciendo el sumatorio de los valores $P(c,b)$ a partir de b $\geq \left\lceil\frac{N+c}{3}\right\rceil$. El $P(c)$ actual se compara con la máxima probabilidad obtenida hasta el momento y se guarda el mayor resultado de los dos.

> [!NOTE]
> En Python, en vez de mantener una matriz se puede hacer que una función memorice valores de retorno mediante el decorador `@functools.cache`.

# Soluciones

| Solución | Descripción | Verificado con el juez |
| :------: | :---------- | :--------------------: |
| [N.cpp](src/N.cpp) | Programación dinámica, $O(n^2)$ | :white_check_mark: |
| [N.py](src/N.py) | Programación dinámica, $O(n^2)$| :white_check_mark: |