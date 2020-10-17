#!/usr/bin/python3
from grafo import Grafo
import funciones_grafo as repo
import csv
import random
import sys

operaciones = ["camino_mas","camino_escalas",
	"centralidad","centralidad_aprox",
	"pagerank","nueva_aerolinea","recorrer_mundo",
	"recorrer_mundo_aprox","vacaciones",
	"itinerario","exportar_kml"]

#-------------------------------------------------------------------
#-----------------------------MAIN----------------------------------
#-------------------------------------------------------------------

def main():
	ruta_aeropuertos = sys.argv[1]
	ruta_vuelos = sys.argv[2]
	grafo_general,datos_aeropuertos,aeropuertos_de_ciudades = database(ruta_aeropuertos,ruta_vuelos)
	procesador_de_comandos(grafo_general,datos_aeropuertos,aeropuertos_de_ciudades)
	
#-------------------------------------------------------------------
#---------------------------DATABASE--------------------------------
#-------------------------------------------------------------------

def database(ruta_aeropuertos,ruta_vuelos):
	"""
	Recibe rutas de aeropuertos y vuelos para procesar la información y guardarla en memoria.
	Devuelve el grafo ya armado.
	"""
	with open(ruta_aeropuertos) as aeropuertos, open(ruta_vuelos) as vuelos:
		lector_aeropuertos = csv.reader(aeropuertos)
		lector_vuelos = csv.reader(vuelos)
		grafo_general = Grafo()
		datos_aeropuertos = {} #Ciudad y posición geográfica de cada aeropuerto.
		aeropuertos_de_ciudades = {} #Cuando me pasan por comando la ciudad busco los aeropuertos que tiene acá.
		for ciudad,codigo,latitud,longitud in lector_aeropuertos:
			grafo_general.agregar_vertice(codigo)
			datos_aeropuertos[codigo] = datos_aeropuertos.get(codigo,[ciudad,latitud,longitud])
			aeropuertos_de_ciudades[ciudad] = aeropuertos_de_ciudades.get(ciudad,[])
			aeropuertos_de_ciudades[ciudad].append(codigo)
		for origen,destino,tiempo,precio,cant_vuelos in lector_vuelos:
			#Arista con formato (precio,tiempo,cant_vuelos)
			grafo_general.agregar_arista(origen,destino,(precio,tiempo,cant_vuelos))
			grafo_general.agregar_arista(destino,origen,(precio,tiempo,cant_vuelos))

		return grafo_general,datos_aeropuertos,aeropuertos_de_ciudades

#-------------------------------------------------------------------
#---------------------------COMANDOS--------------------------------
#-------------------------------------------------------------------

def procesador_de_comandos(grafo,datos_aeropuertos,aeropuertos_de_ciudades):
	camino_kml = None
	for linea in sys.stdin:
		if linea.rstrip() == "listar_operaciones":
			listar_operaciones()
			continue

		comando = linea.split(" ")[0]
		for i in range(len(linea)):
			if linea[i] == " ":
				cadena_parametros = linea[i+1:].rstrip()
				break
		parametros = cadena_parametros.split(",")
		if comando == "exportar_kml": ejecutor_de_comandos(comando,parametros,grafo,datos_aeropuertos,aeropuertos_de_ciudades,camino_kml)
		else: camino_kml = ejecutor_de_comandos(comando,parametros,grafo,datos_aeropuertos,aeropuertos_de_ciudades,camino_kml)

def ejecutor_de_comandos(comando,parametros,grafo,datos_aeropuertos,aeropuertos_de_ciudades,camino_kml):
	if comando == "camino_mas": return camino_mas(grafo,aeropuertos_de_ciudades,parametros[0],parametros[1],parametros[2])
	if comando == "camino_escalas": return camino_escalas(grafo,aeropuertos_de_ciudades,parametros[0],parametros[1])
	if comando == "centralidad": centralidad(grafo,int(parametros[0]))
	if comando == "centralidad_aprox": centralidad_aprox(grafo,int(parametros[0]))
	if comando == "pagerank": pagerank(grafo,int(parametros[0]))
	if comando == "nueva_aerolinea": nueva_aerolinea(grafo,parametros[0])
	if comando == "recorrer_mundo_aprox": return recorrer_mundo_aprox(grafo,aeropuertos_de_ciudades,parametros[0])
	if comando == "recorrer_mundo": return recorrer_mundo(grafo,aeropuertos_de_ciudades,parametros[0])
	if comando == "vacaciones": return vacaciones(grafo,aeropuertos_de_ciudades,parametros[0],int(parametros[1]))
	if comando == "itinerario": itinerario(grafo,aeropuertos_de_ciudades,parametros[0])
	if comando == "exportar_kml": exportar_kml(parametros[0],datos_aeropuertos,camino_kml)

#-------------------------------------------------------------------
#-------------------------OPERACIONES-------------------------------
#-------------------------------------------------------------------

def listar_operaciones():    
	for i in range(0,len(operaciones)):
		print(operaciones[i])

#-------------------------------------------------------------------

def camino_mas(grafo,aeropuertos_de_ciudades,modo,origen,destino):
	"""
	Imprime el camino más rápido/barato desde una ciudad origen a
	una ciudad destino.
	"""
	if modo == "barato": parametro = 0
	else: parametro = 1
	caminos = []
	for aerop_origen in aeropuertos_de_ciudades[origen]:
		for aerop_destino in aeropuertos_de_ciudades[destino]:
			padre,peso = repo.camino_minimo(grafo,aerop_origen,parametro,aerop_destino)
			camino = repo.reconstruir_camino(aerop_origen,aerop_destino,padre,peso)
			#Camino es la tupla (camino,peso)
			if camino: caminos.append(camino)
	camino_minimo = menor_condicion(caminos)
	imprimir_camino(camino_minimo)
	return camino_minimo

#-------------------------------------------------------------------

def camino_escalas(grafo,aeropuertos_de_ciudades,origen,destino):
	"""
	Imprime el camino con menos escalas entre una ciudad origen y una ciudad
	destino.
	"""
	caminos = []
	for aerop_origen in aeropuertos_de_ciudades[origen]:
		padres,orden = repo.bfs(grafo,aerop_origen)
		for aerop_destino in aeropuertos_de_ciudades[destino]:
			camino = repo.reconstruir_camino(aerop_origen,aerop_destino,padres,None)
			if aerop_destino not in orden: continue
			caminos.append((camino,orden[aerop_destino]))
	imprimir_camino(menor_condicion(caminos))
	return caminos

#-------------------------------------------------------------------

def centralidad(grafo,cantidad):
	cent = repo.centralidad(grafo)
	imprimir_centralidad(cent,cantidad)

#-------------------------------------------------------------------

def centralidad_aprox(grafo,cantidad):
	cent = repo.centralidad_aprox(grafo)
	imprimir_centralidad(cent,cantidad)

#-------------------------------------------------------------------

#Funcion obtenida de: http://dpk.io/pagerank
#Fue modificada para que funcionara con diccionarios de diccionarios
def pagerank(grafo,cantidad):
	pagerank = repo.pagerank(grafo,cantidad)
	imprimir_centralidad(pagerank,cantidad)

#-------------------------------------------------------------------

def nueva_aerolinea(grafo,ruta_salida):
	"""
	Genera una nueva aerolínea con los vuelos más baratos
	pero conectando todo el mundo.
	"""
	grafo_prim = repo.prim(grafo,0)
	with open(ruta_salida, "a") as salida:
		escritor = csv.writer(salida)
		lista_aristas = grafo_prim.obtener_aristas()
		for origen,destino,tupla in lista_aristas:
			escritor.writerow([origen,destino,tupla[1],tupla[0],tupla[2]])
		print("OK")

#-------------------------------------------------------------------

def _recorrer_mundo_aprox(grafo,origen):
	recorrido_minimo = []	
	tiempo_minimo = float("inf") 

	for i in range(100): #Se calculan 100 recorridos distintos y se queda con el mejor.
		recorrido,tiempo = repo._recorrer_mundo_aprox(grafo,origen)
		if tiempo[0] < tiempo_minimo:
			tiempo_minimo = tiempo[0]
			recorrido_minimo = recorrido	
	return recorrido_minimo,tiempo_minimo

def recorrer_mundo_aprox(grafo,aeropuertos_de_ciudades,origen):
	recorrido_minimo = []	
	tiempo_minimo = float("inf")    
	for aerop_origen in aeropuertos_de_ciudades[origen]:
		recorrido,tiempo = _recorrer_mundo_aprox(grafo,aerop_origen)
		if tiempo < tiempo_minimo:
			tiempo_minimo = tiempo
			recorrido_minimo = recorrido

	imprimir_camino(list(recorrido_minimo))
	print("Costo: {}".format(tiempo_minimo))
	return recorrido_minimo

#-------------------------------------------------------------------

def _recorrer_mundo(grafo_prim,origen):
	recorrido_rapido,tiempo_rapido = repo.dfs_recorrer_mundo(grafo_prim,origen)
	for i in range(5000): #Se calculan 5000 recorridos distintos y se queda con el mejor.
		recorrido,tiempo = repo.dfs_recorrer_mundo(grafo_prim,origen)
		if tiempo < tiempo_rapido:
			tiempo_rapido = tiempo
			recorrido_rapido = recorrido
	return recorrido_rapido,tiempo_rapido


def recorrer_mundo(grafo,aeropuertos_de_ciudades,origen):
	grafo_prim = repo.prim_no_dirigido(grafo,1)	
	mejor_recorrido = []	
	mejor_tiempo = float("inf")

	for aerop_origen in aeropuertos_de_ciudades[origen]:
		recorrido,tiempo = 	_recorrer_mundo(grafo_prim,aerop_origen)
		if tiempo < mejor_tiempo:
			mejor_tiempo = tiempo
			mejor_recorrido = recorrido
	imprimir_camino(mejor_recorrido)
	print("Costo: {}".format(mejor_tiempo))
	return mejor_recorrido

#-------------------------------------------------------------------
#NAR -> WAC -> RIV -> ATL -> JFK -> BH6 -> NAR

def _vacaciones(grafo,origen,cantidad,iteracion,actual,viaje,visitados):
	if(iteracion > cantidad): return
	adyacentes = grafo.adyacentes(actual)
	for adyacente in adyacentes:
		if(cantidad == iteracion and adyacente != origen): continue
		if(adyacente not in visitados):
			viaje.append(adyacente)
			visitados.add(adyacente)
			_vacaciones(grafo,origen,cantidad,iteracion+1,adyacente,viaje,visitados)
		if(len(viaje) > 1 and len(viaje) == cantidad+1):
			if(viaje[-1] == origen): 
				return
	viaje.pop()
	visitados.remove(actual)

def vacaciones(grafo,aeropuertos_de_ciudades,origen,cantidad):
	"""
	Recibe grafo, aeropuertos, origen y cantidad de lugares que se desea recorrer
	antes de volver a la ciudad de origen.
	Imprime un camino válido a realizar cumpliendo la condición.
	"""
	encontro_recorrido = True
	for aeropuerto_origen in aeropuertos_de_ciudades[origen]:
		viaje = []
		visitados = set()
		viaje.append(aeropuerto_origen)
		_vacaciones(grafo,aeropuerto_origen,cantidad,1,aeropuerto_origen,viaje,visitados)
		if(len(viaje) != cantidad+1): encontro_recorrido = False
		else:
			imprimir_camino(viaje)
			return viaje
	if not encontro_recorrido: print("No se encontro recorrido")
		
#-------------------------------------------------------------------

def itinerario(grafo,aeropuertos_de_ciudades,ruta):
	"""
	Recibe grafo, aeropuertos y una ruta de la cual se procesa y genera
	un itinerario cultural con las condiciones dadas.
	Imprime el orden el cual se debe realizar el viaje con los caminos mínimos
	entre cada ciudad.
	"""
	with open(ruta) as archivo:
		lector_itinerario = csv.reader(archivo)
		itinerario_ordenado = creador_grafo_itinerario(lector_itinerario)
		for ciudad in range(0,len(itinerario_ordenado)):
			if ciudad == len(itinerario_ordenado)-1: print("{}".format(itinerario_ordenado[ciudad]))
			else: print("{}, ".format(itinerario_ordenado[ciudad]),end = "")
		for i in range(0,len(itinerario_ordenado)-1):
			camino_mas(grafo,aeropuertos_de_ciudades,"rapido",itinerario_ordenado[i],itinerario_ordenado[i+1])

def creador_grafo_itinerario(lector_itinerario):
	"""
	Crea un grafo a partir de las condiciones dadas por el archivo csv recibido.
	Realiza un orden topológico y luego devuelve una lista con el orden el cual
	se debe realizar el viaje.
	"""
	linea_cero = 0
	grafo_itinerario = Grafo()
	for linea in lector_itinerario:
		if linea_cero == 0:
			a_agregar = linea[::]
			random.shuffle(a_agregar)
			grafo_itinerario.crear(a_agregar)
			linea_cero += 1
		grafo_itinerario.agregar_arista(linea[0],linea[1],0)
	itinerario_ordenado = repo.orden_topologico_bfs(grafo_itinerario)
	return itinerario_ordenado

#-------------------------------------------------------------------

def exportar_kml(ruta,datos_aeropuertos,aeropuertos):
	"""
	Recibe ruta, datos de aeropuertos y el camino a exportar a un archivo KML.
	Escribe el archivo y luego imprime OK.
	"""
	with open(ruta,"w") as archivo:
		archivo.write('<?xml version="1.0" encoding="UTF-8"?>\n')
		archivo.write('<kml xmlns="http://www.opengis.net/kml/2.2">\n')
		archivo.write("\t<Document>\n")
		archivo.write("\t\t<name>KML TP3</name>\n")
		for aeropuerto in aeropuertos:
			agregar_vuelo_kml(archivo,aeropuerto,datos_aeropuertos[aeropuerto])
		for i in range(0,len(aeropuertos)-1):
			agregar_linea_kml(archivo,aeropuertos[i],aeropuertos[i+1],datos_aeropuertos)
		archivo.write("\t</Document>\n")
		archivo.write("</kml>\n")

	print("OK")

#-------------------------------------------------------------------

#-------------------------------------------------------------------
#--------------------OPERACIONES AUXILIARES-------------------------
#-------------------------------------------------------------------


def agregar_linea_kml(archivo,aeropuerto1,aeropuerto2,datos_aeropuertos):
	datos_aerop1 = datos_aeropuertos[aeropuerto1]
	datos_aerop2 = datos_aeropuertos[aeropuerto2]
	archivo.write('\t\t<Placemark>\n')
	archivo.write('\t\t\t<LineString>\n')
	archivo.write('\t\t\t\t<coordinates>{}, {} {}, {}</coordinates>\n'.format(datos_aerop1[2],datos_aerop1[1],datos_aerop2[2],datos_aerop2[1]))
	archivo.write('\t\t\t</LineString>\n')
	archivo.write('\t\t</Placemark>\n')
	archivo.write('\n')  


def agregar_vuelo_kml(archivo,aeropuerto,datos_aeropuertos):
	archivo.write('\t\t<Placemark>\n')
	archivo.write('\t\t\t<name>{}</name>\n'.format(aeropuerto))
	archivo.write('\t\t\t<Point>\n')
	archivo.write('\t\t\t\t<coordinates>{}, {}</coordinates>\n'.format(datos_aeropuertos[2],datos_aeropuertos[1]))
	archivo.write('\t\t\t</Point>\n')
	archivo.write('\t\t</Placemark>\n')
	archivo.write('\n')

def imprimir_camino(lista):
	for i in range(0,len(lista)):
		if i == len(lista)-1: print("{}".format(lista[i]))
		else: print("{} -> ".format(lista[i]),end = "")

def menor_condicion(caminos):
	if len(caminos) == 0: return []
	camino_min = caminos[0][0]
	condicion_min = caminos[0][1]
	for camino,condicion in caminos:
		if condicion < condicion_min:
			camino_min = camino
			condicion_min = condicion
	return camino_min

def imprimir_centralidad(centralidad,cantidad):
	resul = []
	for aeropuerto,frecuencia in centralidad.items():
		resul.append((frecuencia,aeropuerto))
	resul.sort()
	resul_invertido = resul[::-1]
	for i in range(0,cantidad):
		if i == cantidad-1: print("{}".format(resul_invertido[i][1]))
		else: print("{}".format(resul_invertido[i][1]),end = ",")

#-------------------------------------------------------------------
main()