# Introducción a la solución

Tenemos una ampolleta (un reloj de arena) formada por dos cámaras triangulares
unidas por un cuello de una sola celda: la de arriba apunta hacia abajo y la de
abajo apunta hacia arriba, con $L$ niveles cada una. En el instante $0$ la de
arriba está llena y la de abajo vacía; cada segundo cae un grano por el cuello.
Dados $L$ y $T$, hay que **dibujar** en arte ASCII cómo se ve la ampolleta
justo tras $T$ segundos.

No hay nada que *contar* ni optimizar en el sentido habitual: es un problema de
*simulación*. Toda la dificultad está en modelar bien la física de la arena
y volcar el resultado en el dibujo exacto que pide el enunciado, con sus
espacios y sus paredes. Los límites son minúsculos ($L \le 20$, y $\sum L^2 \le
2 \cdot 10^5$), así que no hay que preocuparse por la eficiencia: cualquier
simulación directa entra de sobra.

# Simular la cámara de abajo

Empecemos por la cámara inferior, donde la arena se apila. La representamos como
una rejilla y dejamos caer los granos uno a uno, exactamente como describe el
enunciado.

Numeramos los niveles de la cámara de abajo desde el cuello: el nivel $0$ es la
celda del cuello (arriba del todo de esta cámara, de anchura $1$) y el nivel
$L-1$ es el suelo (anchura $2L-1$). Cada grano nuevo entra por la celda del
cuello y desciende (hacia niveles mayores) siguiendo la regla:

- baja a la celda justo debajo si está libre;
- si no, rueda a la diagonal inferior izquierda;
- si tampoco, a la diagonal inferior derecha;
- y se detiene cuando ya ha llegado al suelo o cuando las tres celdas de debajo
  están ocupadas.

El orden de las diagonales (primero la izquierda) es lo que el enunciado llama
la inclinación "hacia la izquierda" de la ampolleta: es la regla que rompe los
empates y hace que el montículo se cargue hacia ese lado. Repetimos el proceso
$T$ veces, una por grano, y la cámara inferior queda determinada.

Un detalle práctico de implementación: aunque la cámara es triangular, conviene
reservar la rejilla como un rectángulo de $L$ filas por $2L-1$ columnas (con la
celda del cuello en la columna central) y trabajar siempre dentro de él. Así los
desplazamientos diagonales $j \pm 1$ nunca se salen del array, y no hace falta
vigilar los bordes en cada paso. A la hora de dibujar recortamos cada nivel a su
anchura real ($2i+1$ en el nivel $i$) para formar la silueta del triángulo.

Con esto, el coste por caso es $O(T \cdot L)$: cada uno de los $T$ granos recorre
a lo sumo $L$ niveles, y, dados los límites, es despreciable.

# Reutilización para la cámara de arriba

Podríamos ahora escribir una segunda simulación para la cámara superior. Allí lo
que se mueve no es arena sino *huecos*: cada segundo aparece un hueco en el cuello
y **asciende**, subiendo a la celda de encima mientras haya arena; si la de
encima ya es un hueco, se desvía a la diagonal superior derecha, y si tampoco, a
la superior izquierda; se detiene cuando no puede subir más.

Pero antes de duplicar código conviene fijarse en una simetría que el propio
enunciado insinúa: "*el montículo de abajo se carga hacia [la izquierda]; el
hueco de arriba, en cambio, se desvía hacia la derecha*". Esa inversión
(izquierda abajo, derecha arriba) implica que si giramos la ampolleta entera
180º alrededor del cuello, la cámara de arriba coincide con la de abajo,
intercambiando los granos por huecos.

Es decir: la configuración de huecos de la cámara superior tras $T$ segundos es
la imagen de la configuración de granos de la inferior al girarla media vuelta
(y cambiar "hay grano" por "hay hueco").

# Dibujar el resultado

Con la cámara inferior ya calculada, el dibujo son $2L$ líneas:

- **Mitad inferior** (los $L$ niveles de la cámara de abajo, del cuello al suelo):
  cada línea lleva sus espacios de sangría a la izquierda, la pared `/`, las
  celdas del nivel (un `*` por grano, un espacio por celda vacía) y la pared `\`.
- **Mitad superior**: se recorre la misma cámara inferior, pero girada media
  vuelta (niveles de fuera hacia dentro y columnas invertidas) y con la lectura
  cambiada —donde hay grano abajo, arriba hay un hueco (espacio), y donde no,
  arriba hay arena (`*`)—, encajada entre las paredes `\` y `/`.

Hay que ser cuidadoso con dos exigencias del enunciado: cada línea va precedida
de exactamente los espacios que alinean la silueta, y ninguna línea termina en
espacios (todas acaban en una pared `/` o `\`). Como las celdas siempre quedan
encerradas entre dos paredes, lo segundo sale solo; lo primero es cuestión de
contar bien la sangría de cada nivel.

# Soluciones

| Solución | Descripción | Verificado con el juez |
| :------: | :---------- | :--------------------: |
| [I.cpp](src/I.cpp) | Simula la caída de los $T$ granos en la cámara inferior y dibuja la superior como su reflejo (giro de $180°$ con arena y huecos intercambiados). | :white_check_mark: |
