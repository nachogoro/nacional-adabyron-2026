# Introducción a la solución

Un castillo es un *árbol binario*: cuelga de un estallido inicial (el nodo 1) y
cada estallido enciende a 0, 1 o 2 más, que revientan un compás después. Las
hojas del árbol son las *estrellas finales*. La *duración* de un subcastillo es
el número de compases desde su estallido inicial hasta que se apaga su última
luz; una estrella final sola dura 1 compás.

El árbol de entrada ya es *armonioso*:

- cuando un estallido enciende a dos, los dos subcastillos que arrancan en ellos
  tienen duraciones que difieren en **1 compás como mucho**;
- cuando un estallido enciende a uno solo, ese uno es siempre una estrella final.

La pregunta es si el castillo es *robusto*: si sigue siendo armonioso aunque
falle **una** estrella final cualquiera (a lo sumo falla una). Hay que responder
`SI` o `NO`.

# Traducir requisitos a propiedades del árbol

Antes de nada, conviene traducir "duración" a algo cómodo de calcular sobre el
árbol. Si a cada nodo le asignamos su *altura*, definida como el número de
nodos en el camino más largo desde dicho nodo a una hoja (una hoja tiene altura
$1$, y un nodo interno tiene altura $1 + \max$ de las alturas de sus hijos),
entonces la duración de su subcastillo es exactamente $\text{altura}$.

Podemos entonces olvidarnos de duraciones, compases y fuegos artificiales y
hablar siempre de alturas: "*armonioso*" en un nodo significa que las alturas
de sus subárboles difieren en $1$ como mucho (teniendo en cuenta que si un
subárbol no existe, su altura es $0$).

Cuando una estrella final no prende, desaparece del árbol. Así, un árbol es
*robusto* si y solo si todos sus subárboles permanecen armoniosos tras la
desaparición de cualquier hoja.

Quitar una sola hoja reduce la altura de cualquier subárbol en $0$ o en $1$,
nunca más. Como nuestro árbol original es armonioso (y por tanto lo son todos
sus subárboles), el problema consiste en saber, para cada subárbol, si existe
una hoja tal que, al eliminarla, sus dos subárboles se queden con una
diferencia de altura igual o mayor a $2$.

Esto puede hacerse fácilmente si conseguimos saber, para cada subárbol, si
existe alguna hoja cuyo fallo le reste ese $1$ de altura.

Definimos tres cantidades sobre cada nodo $v$:

- `altura(v)`: la altura del subárbol de raíz $v$.
- `acortable(v)`: ¿existe una hoja en el subárbol de $v$ cuyo fallo reduzca
  `altura(v)` en $1$?
- `robusto(v)`: ¿el subcastillo de raíz $v$ sigue siendo armonioso ante el
  fallo de cualquiera de sus hojas?

La respuesta al problema es `robusto(1)`, que podemos evaluar de forma
recursiva.

# `acortable`: ¿puede el fallo de una hoja acortar la altura del árbol?

Existen tres casos a distinguir para determinar si el subárbol con raíz $v$
puede llegar a perder un nivel completo (y por tanto disminuir su altura) si
falla una hoja, en función del número de hijos de $v$:

- **Ningún hijo.** $v$ es a la vez raíz y hoja, luego árbol de altura $1$. Al
  fallar ella misma, el subárbol desaparece (pasa de altura $1$ a $0$), luego
  es `acortable`.
- **Un solo hijo.** Por ser armonioso, ese hijo es una hoja. El árbol se acorta
  si y solo si su hijo se acorta (y una hoja siempre se acorta), así que también
  es `acortable`.
- **Dos hijos.** La altura del nodo viene del hijo más alto. Aquí hay que
  distinguir:
  - Si los dos hijos tienen la misma altura, ningún fallo aislado reduce la
    altura del nodo: el fallo cae en uno de los dos subárboles, pero el otro
    conserva su altura y sigue marcando el máximo. **No es acortable.**
  - Si un hijo es estrictamente más alto que el otro, la altura del nodo la
    fija ese hijo. El nodo se acorta si y solo si ese hijo más alto se acorta.

Puede definirse la propiedad de forma recursiva de manera muy sencilla.

# `robusto`: ¿aguanta cualquier fallo?

Un fallo de hoja afecta a un único camino a la raíz, así que basta comprobar,
nodo a nodo, que ese nodo sigue armonioso pase lo que pase por debajo. Un
subcastillo de raíz $v$ es robusto cuando se cumplen las dos condiciones
siguientes:

1. **Todos sus hijos son robustos.** Si un fallo ocurre dentro del subárbol de
   un hijo, ese hijo debe seguir armonioso por su cuenta; esto lo garantiza la
   recursión.
2. **El propio $v$ sigue armonioso ante cualquier fallo.** Un fallo cae en uno
   de los dos subárboles y le puede restar (a lo sumo) $1$ de altura. Sean
   $h_{\text{baja}}$ y $h_{\text{alta}}$ las alturas de sus dos hijos
   ($h_{\text{baja}} \le h_{\text{alta}}$), que difieren en $0$ o $1$, por ser
   armonioso:
   - Si $h_{\text{baja}} = h_{\text{alta}}$: aunque un fallo reste $1$ a uno de
     los dos, la diferencia pasa de $0$ a $1$, que sigue siendo armonioso.
     En este caso, el árbol es robusto.
   - Si difieren en $1$: el peligro es que se acorte el subárbol **más bajo**,
     porque entonces la diferencia sube a $2$ y se rompe la armonía. Esto puede
     pasar exactamente cuando el subárbol más bajo es `acortable`. Por tanto, $v$
     es robusto si y solo si el más bajo **no** es acortable. (Que se acorte el
     más alto nunca hace daño: solo iguala las dos alturas.)

El caso del nodo con un solo hijo encaja sin esfuerzo: su hijo es una hoja
(altura $0$) y podemos tratar al "hermano ausente" como altura $0$ no acortable;
las dos alturas coinciden en $0$, así que sale robusto.

Como cada una de las tres cantidades se calcula en $O(1)$ a partir de las de
los hijos y se memoiza, el coste total es $O(N)$ por castillo, de sobra con
$\sum N \le 10^6$.

# Sobre la profundidad de la recursión

La condición de armonioso hace que el árbol esté equilibrado al estilo AVL. Los
nodos de un solo hijo cuelgan siempre de una hoja, así que tienen altura fija
$1$; una altura mayor solo se consigue con nodos de dos hijos, y en ellos las
alturas de los subárboles difieren en $1$ como mucho. Un árbol con esa
propiedad y $N$ nodos tiene altura $O(\log N)$. Para $N = 2 \cdot 10^5$ la
profundidad ronda los $30$ niveles, muy lejos de cualquier límite. Por eso la
solución recursiva es segura tal cual, sin ampliar la pila.

# Soluciones

| Solución | Descripción | Verificado con el juez |
| :------: | :---------- | :--------------------: |
| [F.cpp](src/F.cpp) | Recursión sobre el árbol con `altura`, `acortable` y `robusto` memoizadas, $O(N)$. | :white_check_mark: |
| [F.py](src/F.py) | Misma recursión en Python, apoyándose en `functools.lru_cache` para la memoización. | :white_check_mark: |
