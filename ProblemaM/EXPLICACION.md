# Introducción a la solución
El problema consiste en determinar el calor de cada parcela, dado el conjunto de hogueras, en el que cada una de ellas tiene una posición $p$ y una intensidad $c$. Se puede ver el problema como una lista de $N$ celdas (cada una de ellas representa una parcela) y las hogueras como pirámides o triángulos de altura $c$.

# Definiciones y observaciones 
A primera vista, cada hoguera $(p,c)$:
- tiene vértice en $p$ con altura $c$
- comienza en $p-c+1$ con altura $1$
- acaba en $p+c-1$ con altura $1$
- la altura en la parcela $q$ que está dentro del rango de la pirámide es $h(q)=c-|q-p|$

Podemos también estudiar la variación de altura entre las parcelas adyacentes en una misma hoguera. Llamando $D(q)=h(q)-h(q-1)$:
- $D(q)=0 \text{ si } q \in [1,\dots,p-c]$
- $D(q)=1 \text{ si } q \in [p-c+1,\dots,p]$
- $D(q)=-1 \text{ si } q \in [p+1,\dots,p+c]$
- $D(q)=0 \text{ si } q \in [p+c+1,\dots,N]$

De la misma manera, podemos estudiar la segunda variación, es decir, la variación de $D(q)$. Llamando $DD(q) = D(q) - D(q-1)$ se observa que:
- $DD(q)=0 \text{ si } q \in [1,\dots,p-c]$
- $\boxed{DD(q)=1 \text{ si } q = p-c+1}$
- $DD(q)=0 \text{ si } q \in [p-c+2,\dots,p]$
- $\boxed{DD(q)=-2 \text{ si } q = p+1}$
- $DD(q)=0 \text{ si } q \in [p+2,\dots,p+c]$
- $\boxed{DD(q)=1 \text{ si } q = p+c+1}$


# Arreglo de diferencias
Juntando la idea de la segunda diferencia, $DD(q)$, y el concepto de arreglo de diferencias (*difference array* en inglés), podemos obtener una solución con una complejidad temporal de $O(n)$.

Partiendo de las definiciones de $D(q)$ y $DD(q)$ llegamos a que:

$$ h(q)=h(q-1)+\overbrace{DD(q)+D(q-1)}^{D(q)} \\\\
h(q+1) = h(q)+\overbrace{DD(q+1)+D(q)}^{D(q+1)} \\\\
h(q+2) = h(q+1)+\overbrace{DD(q+2)+D(q+1)}^{D(q+2)} \\\\
\dots
$$

De esta forma se puede calcular la altura de una celda a partir de las variaciones y la altura de la celda anterior.

Si quisiéramos almacenar en un vector de diferencias la primera variación, $D(q)$, sería necesario actualizar $2c+1$ casillas, lo cual haría que el algoritmo general tuviera una complejidad temporal de $O(n^2)$. Sin embargo, podemos almacenar las segundas variaciones $DD(q)$, ya que solo es necesario modificar el vector de diferencias:
- en $p-c+1$, incrementándolo una unidad: $DD(p-c+1)=1$
- en $p+1$, disminuyendo en dos unidades $DD(p+1)=-2$
- en $p+c+1$, incrementándolo una unidad: $DD(p+c+1)=1$

Solamente 3 celdas, lo cual hace que el algoritmo general sea de complejidad $O(3n) = O(n)$.

Como las sumas de las variaciones son lineales, podemos almacenar en un vector todas las variaciones de las hogueras, simplemente sumándolas:


Sea $diff$ nuestro vector de diferencias definido como $${diff}_i = \sum_{h_k} DD_{h_k}(i) \, \forall i \in [1,\dots,N]$$

Y el vector acumulador $A$ como $$ A_i= \sum_{h_k} D_{h_k}(i) \, \forall i \in [1,\dots,N]$$

que es igual a
$$
\begin{matrix}
A_i & = & A_{i-1} & +  & diff_i \\\\
\uparrow & & \uparrow & & \uparrow \\\\
D(i) & = & D(i-1) & +  & DD(i)
\end{matrix}
$$

>[!NOTE]
> $h_k$ representa cada hoguera.

La suma de las alturas de todas las hogueras en la celda $q$, $H(q)$, es igual a:
$$H(q) = H(q-1) + A_q$$

La solución al problema es el vector $H$.

Como caso base, podemos indicar que $H(0)=0$, para que esta definición sea válida $\forall q \in [1,\dots,N]$.

## Pirámides cortadas
Si tenemos pirámides cortadas por la izquierda, significa que el comienzo de estas está fuera de rango, es decir, $$p-c+1 < 1$$

Esto no nos permitiría calcular el valor correcto de las alturas, ya que no tendríamos en cuenta $DD(p-c+1)=1$ porque $diff_{p-c+1}$ no existe (está fuera de rango).

Para solucionarlo, podemos calcular el valor de $H_0$ y $A_0$ como si fuese una celda válida y calcular $H_1$ y $A_1$ usando estas variables:

El calor que le llegaría a la celda $0$ de cada hoguera cortada sería $c-|0-p|=c-p$, por tanto $H_0=\sum_{h_k}\left( c_{h_k}-p_{h_k} \right)$. A simple vista se ve que, si la pirámide está cortada, está en un tramo ascendente; es decir, $D(q)=1$, por tanto $A_0 = \sum_{h_k} 1 = \text{nº de hogueras cortadas}$.

# Algoritmo

>[!NOTE]
> Nuestro algoritmo, en vez de usar un arreglo $A$, simplemente guarda una variable `A` con el valor de $A_{i-1}$ y la va actualizando a medida que iteramos.

1. Mantenemos el arreglo de diferencias `diff` y por cada hoguera actualizamos (si existe la posición):
    - `diff[p-c+1] += 1`
    - `diff[p+1] -= 2`
    - `diff[p+c+1] += 1`
    - Si la hoguera estuviera cortada actualizamos:
        - `diff[0] += c-p`
        - `A += 1`
2. Iteramos con `i` de `1` a `N`:
    - Actualizamos `A += diff[i]`
    - Actualizamos `H[i] = H[i-1] + A`

>[!NOTE]
> En la implementación, para ahorrar memoria, usamos un único vector llamado `res`, que guarda al principio los valores de $diff$ y al iterar los va cambiando por los valores de $H$.

# Soluciones

| Solución | Descripción | Verificado con el juez |
| :------: | :---------- | :--------------------: |
| [M.cpp](src/M.cpp) | Arreglo de diferencias, $O(n)$ | :white_check_mark: |
| [M.py](src/M.py) | Arreglo de diferencias, $O(n)$| :white_check_mark: |