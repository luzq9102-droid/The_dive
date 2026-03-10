//vector s como mi lista de pytho, pero tengo que decirle que tipo e dato va adentro pq c++ todavia no adivina solo
// crear la grilla 
#include <iostream>  //cout para imprimir
#include <vector>  //vector, arreglos dinamicos
#include <cstdlib> //atoi para pasar de texto a nro
#include <ctime>  //time para nros aleatorios 
#include <queue>   //colas para bfs
#include <chrono>  //medir tiempo de ejecucion 

using namespace std;   //no escribir std::cout, solo cout

//: Crear la grilla vacía de muros
//una grila nxn, cada celda es "#"

vector<vector<char>> crear_laberinto(int N){  //devuelve matriz de caracteres, recibe nro de tamaño de laberinto
    vector<vector<char>> laberinto; //crea matriz vacia
    for(int i = 0; i < N; i++){   //crea filas
        vector<char> fila;  //crea fila vacia
        for(int j = 0; j < N; j++){ //crea columnas
            fila.push_back('#'); //agrega # a la fila y cada # es un muro
        }
        laberinto.push_back(fila); //agregas la fila completa a la matriz
    }
    return laberinto; // devuleve matriz llena de #
}


// 2: Imprimir el laberinto, recorrre cada ceda fila x fila t las mprime

void imprimir_laberinto(vector<vector<char>>& lab, int N){ //void no deuelve nada, lab recibe la matriz, & usa matriz original no copia
    for(int i = 0; i < N; i++){  //recorre fila 
        for(int j = 0; j < N; j++){ //recorre columnas
            cout << lab[i][j] << " "; //imprime cada celda
        }
        cout << endl; //salto de linea
    }
}
//3: Generar el laberinto (backtracking)
//abre caminos explorando desde /(x,y)
//si no tiene "vecinos" disponible se vuelve hacia atras y retoma 

void generar(vector<vector<char>>& lab, int x, int y, int N){ //x y y posicion actual, N tamaño
    //marca la celda "actual" como camino abierto 
    lab[x][y] = ' '; //convierte # en espacio vacio

    // se mueve de 2 en 2 para respetar las escructuras de los muros
    int dx[] = {0, 0, 2, -2}; //derecha, izquierda
    int dy[] = {2, -2, 0, 0};  //abajo, arriba

    int orden[] = {0, 1, 2, 3};  //mezcla las direcciones del laberinto 
    for(int i = 3; i > 0; i--){  //para que sea siempre unico, mezcla el orden aleatoriamente
        int j = rand() % (i + 1); 
        swap(orden[i], orden[j]); // intercambia posiciones para hacer el laberinto diferente
    }

    for(int i = 0; i < 4; i++){ //revisa las 4 diferentes 
        int nx = x + dx[orden[i]];
        int ny = y + dy[orden[i]]; //calcula nueva posicion
        //verific que la celda "vecina" este dentrode la grilla y sea un "muro", y que no etse visitado
        if(nx >= 0 && nx < N && ny >= 0 && ny < N && lab[nx][ny] == '#'){
            //abro el camino del medio entre "celda_actual" y las "vecina"
            lab[x + dx[orden[i]]/2][y + dy[orden[i]]/2] = ' '; 
            //llamada recursiva, continua generando desde la celda "vecina"
            generar(lab, nx, ny, N);
        }
    }
}


// FUNCIÓN 4: Resolver el laberinto (BFS) busqueda por nivel, no por profundidad
//la primera vez que llega a la salida este es el camino mas "corto"


void resolver(vector<vector<char>>& lab, int N){ //resuelve encontrando camino mas corto
    //martrices para "rastrear" las "celdas_visitadas" y el "camino" recorrido
    vector<vector<bool>> visitado(N, vector<bool>(N, false)); //gurada lo que ya visito
    vector<vector<pair<int,int>>> padre(N, vector<pair<int,int>>(N, {-1,-1})); //guarda desde que celda llegaste a otra
    
    //cola para BFS, desde la entrada
    queue<pair<int,int>> cola; //BFS usa cola (FIFO)
    //empieza desde laentrada
    cola.push({1, 0});
    visitado[1][0] = true; 

    //las 4 direcciones, se 1 en 1 pq BFS "resuleve", no genera
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    //mietras cola no etse vacia
    while(!cola.empty()){  //toma el primer elemento de la cola
        auto [x, y] = cola.front();
        cola.pop();

        //termina si llegamos a la salida 
        if(x == N-2 && y == N-1) break;

        for(int i = 0; i < 4; i++){  //explora vecinos, si esta dentro, no se visito, no es muro
            int nx = x + dx[i];
            int ny = y + dy[i];
            //Agrega "vecino" a la cola si está "dentro", no fue "visitado" y no es "muro"
            if(nx >= 0 && nx < N && ny >= 0 && ny < N 
               && !visitado[nx][ny] 
               && lab[nx][ny] != '#'){
                visitado[nx][ny] = true;
                padre[nx][ny] = {x, y};
                cola.push({nx, ny});
            }
        }
    }

    //traza el camino desde la "salida" hasta la "entrada" siguiendo los "padres"
    int x = N-2, y = N-1;  // empieza desde la salida
    while(padre[x][y].first != -1){ //retrocede usando a los padres
        int px = padre[x][y].first;
        int py = padre[x][y].second;
        if(lab[x][y] != 'E' && lab[x][y] != 'S')
            lab[x][y] = '*'; //marca el camino con *
        x = px;
        y = py;
    } //como seguir migajas de pan hacia atras
}


// main
//"entrada" del programa, "N" se pasa d¿como argumento
//si no se "pasa", usa 21 por defecto

//argc:cntidad de argumentos/ argv:arreglo de argumentos
int main(int argc, char* argv[]){ //fincion principal
    srand(time(0)); //aleatoria basada en el tiempo 

    int N = 21; //tamaño por "defecto"
    if(argc > 1){ //pasa un nro para ejecutar
        N = atoi(argv[1]);
        if(N % 2 == 0) N++; //"N" dede ser impar para que la estructur funcione
    }  //si es par lo convierte en impar
    vector<vector<char>> lab = crear_laberinto(N); //crea el laberinto

    //medir tiempo de gneracion y resolucion 
    auto inicio = chrono::high_resolution_clock::now(); //guarda tiempo inicial

    generar(lab, 1, 1, N);  //generar y resolver
    lab[1][0] = 'E';   //entrada
    lab[N-2][N-1] = 'S';  //salida
    resolver(lab, N);

    auto fin = chrono::high_resolution_clock::now(); //calcula
    auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio);
    cout << "\nTiempo de ejecucion: " << duracion.count() << " microsegundos" << endl;
    //imprimir tiempo

    imprimir_laberinto(lab, N);
    //imprimir laberinto

    return 0;
}

//