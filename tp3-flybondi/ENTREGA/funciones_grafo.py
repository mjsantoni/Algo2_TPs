from grafo import Grafo
from TDAs import Pila,Cola
from heapq import heappush,heapify,heappop
import random

#-------------------------------------------------------------------
#--------------------------RECORRIDOS-------------------------------
#-------------------------------------------------------------------

#O(V + E)
def bfs(grafo,origen):
	visitados = set()
	padres = {}
	orden = {}
	cola = Cola()
	visitados.add(origen)
	padres[origen] = None
	orden[origen] = 0
	cola.encolar(origen)

	while not cola.esta_vacia():
		v = cola.desencolar()
		for w in grafo.adyacentes(v):
			if w not in visitados:
				visitados.add(w)
				padres[w] = v
				orden[w] = orden[v] + 1
				cola.encolar(w)
	return padres,orden

#-------------------------------------------------------------------

def recorrido_dfs(grafo,v,visitados,padres,recorrido,contador):
	visitados.add(v)
	if len(visitados) == grafo.cantidad_vertices: return contador
	adyacentes_de_v = grafo.adyacentes(v)
	random.shuffle(adyacentes_de_v)

	for w in adyacentes_de_v:
		if len(visitados) == grafo.cantidad_vertices: return contador
		if w not in visitados:
			padres[w] = v
			recorrido.append(w)
			contador[0] += int(grafo.peso_union(v,w)[1])
			recorrido_dfs(grafo,w,visitados,padres,recorrido,contador)
	if padres[v] is not None: 
		if len(visitados) == grafo.cantidad_vertices: return contador
		recorrido.append(padres[v])
		contador[0] += int(grafo.peso_union(padres[v],v)[1])
	return contador[0]

def dfs_recorrer_mundo_aprox(grafo,origen):
	"""
	Recorre de manera dfs el mundo a partir de un arbol/grafo de tendido
	mínimo.
	"""
	visitados = set()
	padres = {}
	recorrido = []
	padres[origen] = None
	contador = [0]
	recorrido.append(origen)
	contador[0] += recorrido_dfs(grafo,origen,visitados,padres,recorrido,contador)
	return recorrido,contador[0]

#-------------------------------------------------------------------

def _buscar_mejor_camino(grafo,actual,tiempo_min,tiempo,viaje,visitados):	
	""" Funcion que ejecuta el backtracking"""
	if tiempo[0] > tiempo_min: return False
	if len(visitados) == grafo.cantidad_vertices: return True
	adyacentes = grafo.adyacentes(actual)	
	lista_aux = []
	if actual not in visitados: visitados.add(actual)
	for adyacente in adyacentes:
		tiempo_aux = int(grafo.peso_union(actual,adyacente)[1])
		lista_aux.append((tiempo_aux,adyacente))
	lista_ordenados = sorted(lista_aux)
	for vertice in lista_ordenados:
		if(tiempo[0]+vertice[0] > tiempo_min): continue
		if vertice not in visitados: visitados.add(vertice[1])
		viaje.append(vertice[1])
		tiempo[0] += vertice[0]
		if(_buscar_mejor_camino(grafo,vertice[1],tiempo_min,tiempo,viaje,visitados)): return True
		else: tiempo[0] -= vertice[0]
	if (len(viaje) > 0): viaje.pop()
	if actual not in viaje: visitados.remove(actual)
    	

def _recorrer_mundo_preciso(grafo,origen,tiempo_min):
	"""Funcion que llama al backtracking y devuelve los resultados parciales"""
	visitados = set()
	viaje = []
	tiempo = [0]
	viaje.append(origen)
	visitados.add(origen)
	_buscar_mejor_camino(grafo,origen,tiempo_min,tiempo,viaje,visitados)
	return tiempo[0],viaje


def recorrer_mundo_preciso(grafo,origen,tiempo_min):
	"""Funcion que se encarga de llamar hasta que se consiga el resultado optimo"""	
	camino = []
	tiempo_rapido = [tiempo_min]
	tiempo = 1
	while tiempo != 0:
		tiempo,recorrido = _recorrer_mundo_preciso(grafo,origen,tiempo_rapido[0]-1)
		if tiempo != 0 and tiempo < tiempo_rapido[0]:
			tiempo_rapido[0] = tiempo
			camino = recorrido
	return camino,tiempo_rapido[0]

#----------------------VIAJE DE N LUGARES--------------------------

def vacaciones(grafo,origen,cantidad,iteracion,actual,viaje,visitados):
	if(iteracion > cantidad): return
	adyacentes = grafo.adyacentes(actual)
	if(len(viaje) == cantidad): #Poda, que evita poner en la ante ultima posicion a algun aeropuerto que no tenga como final a el origen.
		if origen not in adyacentes: return
	for adyacente in adyacentes:
		if(cantidad == iteracion and adyacente != origen): continue
		if(adyacente not in visitados):
			viaje.append(adyacente)
			visitados.add(adyacente)
			vacaciones(grafo,origen,cantidad,iteracion+1,adyacente,viaje,visitados)
		if(len(viaje) > 1 and len(viaje) == cantidad+1):
			if(viaje[-1] == origen): 
				return
	viaje.pop()
	visitados.remove(actual)

#-------------------------------------------------------------------
#--------------------------ORDEN TOPO-------------------------------
#-------------------------------------------------------------------

#O(V + E)
def orden_topologico_bfs(grafo):
	grados = {}
	for v in grafo:
		grados[v] = 0
	for v in grafo:
		for w in grafo.adyacentes(v):
			grados[w] += 1
	cola = Cola()
	for v in grafo:
		if grados[v] == 0:
			cola.encolar(v)
	resul = []

	while not cola.esta_vacia():
		v = cola.desencolar()
		resul.append(v)
		for w in grafo.adyacentes(v):
			grados[w] -= 1
			if grados[w] == 0:
				cola.encolar(w)
	if len(resul) == len(grafo):
		return resul

#-------------------------------------------------------------------
#---------------------------DIJKSTRA--------------------------------
#-------------------------------------------------------------------

#O(V + E log E) ---> O(E log V)
def camino_minimo(grafo,origen,modo,destino):
	dist = {}
	padre = {}
	for v in grafo: dist[v] = float("inf")
	dist[origen] = 0
	padre[origen] = None
	heap = []
	heappush(heap,(dist[origen],origen))
	while len(heap) > 0:
		distancia,v = heappop(heap)
		if v == destino:
			return padre,dist
		for w in grafo.adyacentes(v):
			if dist[v] + int(grafo.peso_union(v,w)[modo]) < dist[w]:
				dist[w] = dist[v] + int(grafo.peso_union(v,w)[modo])
				padre[w] = v
				heappush(heap,(dist[w],w))
	return padre,dist

#O(V + E log E) ---> O(E log V)
def camino_maximo(grafo,origen,modo,destino):
	'''
	Utilizado para centralidad exclusivamente ya que necesita
	obtener el camino con mayor peso. Se cambió la manera de calcular
	las distancias invirtiéndolas ya que Dijkstra no admite pesos negativos.
	'''
	dist = {}
	padre = {}
	for v in grafo: dist[v] = float("inf")
	dist[origen] = 0
	padre[origen] = None
	heap = []
	heappush(heap,(dist[origen],origen))
	while len(heap) > 0:
		distancia,v = heappop(heap)
		if v == destino:
			return padre,dist
		for w in grafo.adyacentes(v):
			if dist[v] + (1/int(grafo.peso_union(v,w)[modo])) < dist[w]:
				dist[w] = dist[v] + (1/int(grafo.peso_union(v,w)[modo]))
				padre[w] = v
				heappush(heap,(dist[w],w))
	return padre,dist

def reconstruir_camino(origen,destino,padre,peso):
	"""
	Reconstruye el camino mínimo/máximo provisto por el algoritmo de
	Dijkstra volviendo desde destino a través de sus padres.
	"""
	if destino not in padre: return []
	actual = padre[destino]
	if peso: peso_total = peso[destino]
	camino_reconstruido = []
	camino_reconstruido.append(destino)
	while actual != origen:
		camino_reconstruido.append(actual)
		actual = padre[actual]
		if peso: peso_total += peso[actual]

	camino_reconstruido.append(actual)
	if peso: return camino_reconstruido[::-1],peso_total
	else: return camino_reconstruido[::-1]

#-------------------------------------------------------------------
#---------------------BETWEENESS CENTRALITY-------------------------
#-------------------------------------------------------------------

#O(V.(V+E)log V) PESADO
def centralidad(grafo):
	cent = {}
	for v in grafo: cent[v] = 0
	for v in grafo:
		padre, dist = camino_maximo(grafo,v,2,None)
		cent_aux = {}
		filtrar_infinitos(dist)
		for w in grafo:
			cent_aux[w] = 0
		vertices_ordena = sorted(grafo.obtener_vertices())
		vertices_ordenados = vertices_ordena[::-1]
		for w in vertices_ordenados:
			if w not in padre: continue
			if padre[w] not in cent_aux: continue
			cent_aux[padre[w]] += 1
			cent_aux[padre[w]] += cent_aux[w]
		for w in grafo:
			if w == v:
				continue
			cent[w] += cent_aux[w]
	return cent

#O(V)
def centralidad_aprox(grafo):
	"""
	Contabiliza la cantidad de aristas que tiene cada vertice, obteniendo
	los más centrales.
	"""
	cent = {}
	for v in grafo:
		cent[v] = len(grafo.aristas_de_vertice(v))
	return cent

#-------------------------------------------------------------------
#---------------------------PAGERANK--------------------------------
#-------------------------------------------------------------------

#Funcion obtenida de: http://dpk.io/pagerank
#Fue modificada para que funcionara con diccionarios de diccionarios
def pagerank(grafo,cantidad,damping=0.85, epsilon=1.0e-8):
	inlink_map = {}
	outlink_counts = {}
	
	def new_node(node):
		if node not in inlink_map: inlink_map[node] = set()
		if node not in outlink_counts: outlink_counts[node] = 0

	for vertices in grafo:
		for adyacente in grafo.adyacentes(vertices):
			new_node(vertices)
			new_node(adyacente)
			if vertices == adyacente: continue
        
			if vertices not in inlink_map[adyacente]:
				inlink_map[adyacente].add(vertices)
				outlink_counts[vertices] += 1
		
	all_nodes = set(inlink_map.keys())
	for node, outlink_count in outlink_counts.items():
		if outlink_count == 0:
			outlink_counts[node] = len(all_nodes)
			for l_node in all_nodes: inlink_map[l_node].add(node)
    
	initial_value = 1 / len(all_nodes)
	ranks = {}
	for node in inlink_map.keys(): ranks[node] = initial_value

	new_ranks = {}
	delta = 1.0
	while delta > epsilon:
		new_ranks = {}
		for node, inlinks in inlink_map.items():
			new_ranks[node] = ((1 - damping) / len(all_nodes)) + (damping * sum(ranks[inlink] / outlink_counts[inlink] for inlink in inlinks))
		delta = sum(abs(new_ranks[node] - ranks[node]) for node in new_ranks.keys())
		if delta == float("inf"): break
		ranks, new_ranks = new_ranks, ranks

	return ranks

#-------------------------------------------------------------------
#-----------------------TENDIDOS MINIMOS----------------------------
#-------------------------------------------------------------------

#O(E log V)
def prim(grafo,modo):
	vertices_ordenados = sorted(grafo.obtener_vertices())
	vertice = vertices_ordenados[0]
	visitados = set()
	visitados.add(vertice)
	heap = []
	arbol = Grafo()
	for w in grafo.adyacentes(vertice):
		heappush(heap,(grafo.peso_union(vertice,w)[modo],vertice,w))
	arbol.crear(grafo.obtener_vertices())
	while len(heap) > 0:
		peso_union,v,w = heappop(heap)
		if w in visitados:
			continue
		arbol.agregar_arista(v,w,grafo.peso_union(v,w))
		visitados.add(w)
		for x in grafo.adyacentes(w):
			if x not in visitados:
				heappush(heap,(grafo.peso_union(w,x)[modo],w,x))
	return arbol

def prim_no_dirigido(grafo,modo):
	vertices_ordenados = sorted(grafo.obtener_vertices())
	vertice = vertices_ordenados[0]
	visitados = set()
	visitados.add(vertice)
	heap = []
	arbol = Grafo()
	for w in grafo.adyacentes(vertice):
		heappush(heap,(grafo.peso_union(vertice,w)[modo],vertice,w))
	arbol.crear(grafo.obtener_vertices())
	while len(heap) > 0:
		peso_union,v,w = heappop(heap)
		if w in visitados:
			continue
		arbol.agregar_arista(v,w,grafo.peso_union(v,w))
		arbol.agregar_arista(w,v,grafo.peso_union(v,w))
		visitados.add(w)
		for x in grafo.adyacentes(w):
			if x not in visitados:
				heappush(heap,(grafo.peso_union(w,x)[modo],w,x))
	return arbol

#-------------------------------------------------------------------
#---------------------------AUXILIARES------------------------------
#-------------------------------------------------------------------

def filtrar_infinitos(dist):
	a_eliminar = []
	for v in dist:
		if dist[v] == float("inf"):
			a_eliminar.append(v)
	for v in a_eliminar:
		dist.pop(v)

#-------------------------------------------------------------------