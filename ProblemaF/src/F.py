# Un castillo es un árbol binario que cuelga del estallido 1. La "altura" de un
# subárbol (hojas = 0) determina la duración de su subcastillo (duración = altura + 1),
# así que trabajamos directamente con alturas.
#
# El castillo es robusto si sigue siendo armonioso tras el fallo de cualquier hoja.
# Quitar una hoja reduce la altura de un subárbol en 0 o en 1, nunca más. Definimos:
#   height(v)      -> altura del subárbol de raíz v.
#   can_shorten(v) -> ¿existe una hoja cuyo fallo reduzca height(v)?
#   is_robust(v)   -> ¿el subárbol de raíz v aguanta armonioso cualquier fallo de hoja?
#
# Como el árbol es armonioso (hermanos con alturas que difieren <= 1) y los nodos de un
# solo hijo cuelgan siempre de una hoja, el árbol está equilibrado al estilo AVL: su
# altura es O(log N), de modo que la recursión no se acerca al límite de Python.

import sys
from functools import lru_cache


def solve(children):
	@lru_cache(maxsize=None)
	def height(v):
		ch = children[v]
		if not ch:
			return 0
		return 1 + max(height(c) for c in ch)

	@lru_cache(maxsize=None)
	def can_shorten(v):
		ch = children[v]
		if not ch:
			# Una hoja desaparece: su altura pasa de 0 a "nada", el padre la nota.
			return True
		if len(ch) == 1:
			# El único hijo es una hoja; el nodo se acorta si su hijo se acorta.
			return can_shorten(ch[0])
		left, right = ch
		hl, hr = height(left), height(right)
		if hl == hr:
			# La altura viene de ambos hijos: ningún fallo aislado la reduce.
			return False
		# La altura viene solo del hijo más alto: se acorta si ese se acorta.
		return can_shorten(left) if hl > hr else can_shorten(right)

	@lru_cache(maxsize=None)
	def is_robust(v):
		ch = children[v]
		# Todo subcastillo interno debe aguantar sus propios fallos.
		for c in ch:
			if not is_robust(c):
				return False
		if not ch:
			return True

		hl, sl = height(ch[0]), can_shorten(ch[0])
		if len(ch) > 1:
			hr, sr = height(ch[1]), can_shorten(ch[1])
		else:
			# Un solo hijo (hoja): al fallar, el nodo queda como estrella final. Sano.
			hr, sr = 0, False

		# Nos interesa el hijo más bajo: es el que rompe la armonía si se acorta.
		if hl <= hr:
			short_h, short_can, tall_h = hl, sl, hr
		else:
			short_h, short_can, tall_h = hr, sr, hl

		if short_h == tall_h:
			# Alturas iguales: aunque una baje en 1, la diferencia queda en 1. Armonioso.
			return True
		# Difieren en 1: si el más bajo puede encoger, la diferencia sube a 2. Roto.
		return not short_can

	return is_robust(0)


def main():
	data = sys.stdin.buffer.read().split()
	pos = 0
	T = int(data[pos]); pos += 1
	out = []
	for _ in range(T):
		N = int(data[pos]); pos += 1
		children = [None] * N
		for i in range(N):
			k = int(data[pos]); pos += 1
			children[i] = [int(data[pos + j]) - 1 for j in range(k)]
			pos += k
		out.append("SI" if solve(children) else "NO")
	sys.stdout.write("".join(line + "\n" for line in out))


main()
