import random

class Grafo:
	'''
	Implementado con diccionario de diccionarios.
	'''
	def __init__(self):
		self.grafo = {}
		self.cantidad_vertices = 0
		self.cantidad_aristas = 0
	
	def agregar_vertice(self,vertice):
		self.grafo[vertice] = self.grafo.get(vertice,{})
		self.cantidad_vertices += 1
	
	def agregar_arista(self,vertice,arista,peso_arista):
		self.grafo[vertice][arista] = self.grafo[vertice].get(arista,peso_arista)
		self.cantidad_aristas += 1
	
	def esta_conectado(self,vertice1,vertice2):
		return vertice2 in self.grafo[vertice1]

	def adyacentes(self,vertice):
		return list(self.grafo[vertice])

	def __len__(self):
		return self.cantidad_vertices
	
	def obtener_cantidad_aristas(self):
		return self.cantidad_aristas
	
	def aristas_de_vertice(self,vertice):
		'''
		Devuelve lista de claves del diccionario de aristas de un vertice
		cuyo valor es el peso.
		'''
		return list(self.grafo[vertice])

	def sacar_vertice(self,vertice):
		for v in self.grafo:
			if vertice in self.grafo[v]:
				self.grafo[v].pop(vertice)
		self.grafo.pop(vertice)
		self.cantidad_vertices -= 1

	def sacar_arista(self,vertice,arista):
		self.grafo[vertice].pop(arista)
		self.cantidad_aristas -= 1
	
	def vertice_aleatorio(self):
		return random.choice(list(self.grafo.keys()))

	def peso_union(self,vertice1,vertice2):
		return self.grafo[vertice1][vertice2]
	
	def obtener_vertices(self):
		return list(self.grafo)
	
	def obtener_aristas(self):
		'''
		Devuelve arista completa en tupla (vertice1,vertice2,peso_union).
		'''
		r = []
		for v in self.grafo:
			for a in self.grafo[v]:
				r.append((v,a,self.grafo[v][a]))
		return r

	def __iter__(self):
		return iter(self.grafo)
	
	def crear(self,lista_vertices):
		'''
		Agrega vertices a un grafo a partir de una lista de vertices.
		'''
		for vertice in lista_vertices:
			self.agregar_vertice(vertice)
