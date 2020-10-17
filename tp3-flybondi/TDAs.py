class _Nodo:
	def __init__(self,dato,prox):
		self.dato = dato
		self.prox = prox
		self.ant = None

#-------------------------------------------------------------------
#----------------------------PILA-----------------------------------

class Pila:
	"""Representa una pila con operaciones de apilar, desapilar y
	verificar si está vacía."""
	def __init__(self):
		"""Crea una pila vacía."""
		self.items = []
	#El método esta_vacia simplemente se fija si la lista de Python está vacía:
	def esta_vacia(self):
		"""Devuelve True si la lista está vacía, False si no."""
		return len(self.items) == 0
	#El método apilar agrega el nuevo elemento al final de la lista:
	def apilar(self, x):
		"""Apila el elemento x."""
		self.items.append(x)
	#Para implementar desapilar se usamos el método pop de lista que hace exactamente lo re-
	#querido: elimina el último elemento de la lista y devuelve el valor del elemento eliminado. Si la
	#lista está vacía desapilar lanza una excepción.
	def desapilar(self):
		"""Devuelve el elemento tope y lo elimina de la pila.
		Si la pila está vacía levanta una excepción."""
		if self.esta_vacia():
			raise IndexError("La pila está vacía")
		return self.items.pop()

	def ver_tope(self):
		return self.items[-1]

	def pila_a_lista(self):
		return self.items

#-------------------------------------------------------------------
#----------------------------COLA-----------------------------------

class Cola:
	def __init__(self):
		"""Crea una cola vacía."""
		self.primero = None
		self.ultimo = None
	def encolar(self, x):
		"""Encola el elemento x."""
		nuevo = _Nodo(x,None)
		if self.ultimo is not None:
			self.ultimo.prox = nuevo
			self.ultimo = nuevo
		else:
			self.primero = nuevo
			self.ultimo = nuevo

	def desencolar(self):
		"""Desencola el primer elemento y devuelve su valor.
		Si la cola está vacía, levanta ValueError."""
		if self.primero is None:
			raise ValueError("La cola está vacía")
		valor = self.primero.dato
		self.primero = self.primero.prox
		if not self.primero:
			self.ultimo = None
		return valor

	def esta_vacia(self):
		"""Devuelve True si la cola esta vacía, False si no."""
		return self.primero is None

	def ver_primero(self):
		return self.primero


