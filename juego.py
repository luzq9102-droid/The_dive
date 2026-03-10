#Entorno bidimensional representado como una matriz
laberinto = [
[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
[1,0,0,0,1,0,0,0,1,0,0,0,0,0,1],
[1,0,1,0,1,0,1,0,1,0,1,1,1,0,1],
[1,0,1,0,0,0,1,0,0,0,1,0,0,0,1],
[1,0,1,1,1,0,1,1,1,0,1,0,1,1,1], #listas y tuplas
[1,0,0,0,1,0,0,0,1,0,1,0,0,0,1],
[1,1,1,0,1,0,1,0,1,0,1,1,1,0,1],
[1,0,0,0,0,0,1,0,0,0,0,0,1,0,1],
[1,0,1,1,1,0,1,1,1,1,1,0,1,0,1],
[1,0,1,0,0,0,0,0,0,0,1,0,1,0,1],
[1,0,1,0,1,1,1,1,1,0,1,0,1,0,1],
[1,0,0,0,1,0,0,0,1,0,0,0,1,0,1],
[1,1,1,0,1,0,1,0,1,0,1,1,1,0,1],
[1,0,0,0,0,0,1,0,0,0,0,0,0,0,1],
[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
]

#indico posicion en la que se enuentra cada cosa 
raton =(13,13)
queso = (7,7) #tuplas
gato = (1,1)

#visualización
def mostrar_laberinto():
    print("_" * 30)
    for i, fila in enumerate(laberinto):
        linea = ""
        for j, celda in enumerate(fila):
            if(i,j) == queso:
                linea += "🧀"
            elif(i,j) == gato:
                linea += "🐱"
            elif(i,j) == raton:
                linea += "🐭"
            elif celda == 1:
                linea += "🟥"
            else :
                linea += "⬜"

        print (linea)
    print("_" * 30)

#vauacion estimada
#Distancia Manhattan = permite calcular la ruta mas corta posible
#Porque solo hay movimientos verticales y horizontales.
def distancia_manhattan(pos1, pos2):
        return abs(pos1[0] - pos2[0]) + abs(pos1[1] - pos2[1])

# Otro tipo de distancia: Chebyshev si permitiera movimientos diagonales, o Euclidiana para distancia “en línea recta
def mover_gato():
    global gato
    mejor_pos = None
    menor_dis = 999

    movimientos = {
                "1": (-1, 0),  #arriba
                "2": (1,  0),  # abajo      
                "3": (0, -1),  # izquierda
                "4": (0, 1),   # derecha
            }

    for df, dc in movimientos.values(): # Aca "nacen" df y dc
        nueva_pos = (gato[0] + df, gato[1] + dc) 
        if laberinto[nueva_pos[0]][nueva_pos[1]] != 1:
            dist = minimax(nueva_pos, raton, 3 , False)
            if dist < menor_dis:
                menor_dis = dist
                mejor_pos = nueva_pos

    gato = mejor_pos            

#O(b^d), b = movimientos posibles, d = profundidad.
#pq es exponencial :pq en cada nivel se multiplican las posibles jugadas
#La complejidad es exponencial por lo que el árbol máximo evalúa alrededor de 64 estados.
def minimax(pos_gato, pos_raton, profundidad, es_turno_gato): # recursividad: se llama a sí misma para cada movimiento posible.

    if profundidad == 0: #Profundidad: Limita niveles del árbol para evitar crecimiento exponencial y mantener velocidad.
        return distancia_manhattan(pos_gato, pos_raton)
    
    movimientos_dirs = [(-1,0),(1,0),(0,-1),(0,1)]
    
    if es_turno_gato:    #aca es turno del michu
            mejor = 999
            for df, dc in movimientos_dirs: # Aca "nacen" df y dc
                nueva_pos = (pos_gato[0] + df, pos_gato[1] + dc)
                if laberinto[nueva_pos[0]][nueva_pos[1]] != 1:  #no es pared
                    valor = minimax(nueva_pos, pos_raton, profundidad - 1 , False)
                    mejor = min(mejor, valor)
    else:               #aca turno del raton
            mejor = -999
            for df, dc in movimientos_dirs:
                nueva_pos = (pos_raton[0] + df, pos_raton[1] + dc)
                if laberinto[nueva_pos[0]][nueva_pos[1]] != 1:
                    valor = minimax(pos_gato, nueva_pos, profundidad - 1 , True)
                    mejor = max(mejor, valor)

    return mejor

while True:
    mostrar_laberinto()
    #Movimientos
    movimientos = {
        "1": (-1, 0),  #arriba
        "2": (1,  0),  # abajo
        "3": (0, -1),  # izquierda
        "4": (0, 1),   # derecha
    }

    entrada = input("¿Donde quieres ir? (1 = arriba, 2 = abajo, 3 =izquierda, 4 = derecha):")
    df, dc = movimientos[entrada]
    nueva_pos = (raton[0] + df, raton[1] + dc)
    print("Ahora estas en:", nueva_pos)

    if laberinto[nueva_pos[0]][nueva_pos[1]] == 1 :
        print("Aqui hay una pared, intenta otra dirección")

    elif nueva_pos == queso :
        print("¡¡Lo lograste!!")
        break

    else:
        print("podes seguir")
        raton = nueva_pos
        mover_gato()
        if gato == raton :
            print("Perdite, el gato te atrapo")
            break

#¿Como podria optimizar minimax?implementando poda alfa-beta
# reudciendo asi las rams sin alterar el resultado optimo 