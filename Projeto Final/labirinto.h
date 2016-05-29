/*
*labirinto.h
*Projeto Final
*
* Created by Leonardo Winter Pereira & André Eleutério
* All Rights reserved
*/

#define PAREDE -1
#define CAMINHO 0

// -------------------------
// Funções por prototipação
// -------------------------
void init_labirinto(int **labirinto, int tamanho);

int fecha_labirinto(int **labirinto, int x, int y);

void gera_labirinto(int indeks, int **labirinto, int *backtrack_x, int *backtrack_y, int x, int y, int tamanho, int pos_visitadas);
// ---------------------------------
// Fim das funções por prototipação
// ---------------------------------

//-----------------------------------------------------------------------------------------------------------
// Nome: carrega_labirinto
// Desc: Essa é a principal função do arquivo, é ela que chama todas as outras funções e gera o labirinto em si
//-----------------------------------------------------------------------------------------------------------
int **carrega_labirinto(int dificuldade)
{
    //srand( (unsigned)time(NULL) );

	int tamanho = dificuldade * 5; // Variável auxiliar criada apenas para melhoria na visualização do código

    int indeks = 0;

	int **labirinto = aloca_matriz(tamanho * 2 + 1); // matriz armazenada dinamicamente de acordo com a dificuldade

	int *backtrack_linha = aloca_vetor(tamanho * tamanho); // vetor armazenado dinamicamente de acordo com a dificuldade

    int *backtrack_coluna = aloca_vetor(tamanho * tamanho); // vetor armazenado dinamicamente de acordo com a dificuldade

	init_labirinto(labirinto, (tamanho * 2 + 1) ); // Chama a função para inicializar o labirinto

    backtrack_linha[indeks] = 1;

    backtrack_coluna[indeks] = 1;

    gera_labirinto(indeks, labirinto, backtrack_linha, backtrack_coluna, 1, 1, tamanho, 1); // Chama a função para gerar o labirinto

	al_flip_display(); // Atualiza tela

    return labirinto; // Retornamos o labirinto para que a função que a chamar copie seu conteúdo para a matriz "geral"
}

//-------------------------------------------
// Nome: init_labirinto
// Desc: Inicializa as posições do labirinto
//-------------------------------------------
void init_labirinto(int **labirinto, int tamanho)
{
	for (int linha = 0; linha < tamanho; linha++)
     {
         for (int coluna = 0; coluna < tamanho; coluna++)
         {
             if ( (linha % 2 == 0) || (coluna % 2 == 0) )
				 labirinto[linha][coluna] = PAREDE;

             else
				 labirinto[linha][coluna] = CAMINHO;
         }
     }
}

//----------------------------------------------------------------------------------------------------------------
// Nome: fecha_labirinto
// Desc: Realiza os testes para saber se a posição atual do labirinto deve ser fechada (se tornar uma parede) ou não
//----------------------------------------------------------------------------------------------------------------
int fecha_labirinto(int **labirinto, int linha, int coluna)
{
    if( (labirinto[linha - 1][coluna]  == PAREDE) && (labirinto[linha][coluna - 1] == PAREDE) && (labirinto[linha][coluna + 1] == PAREDE) && (labirinto[linha + 1][coluna] == PAREDE) )
        return PAREDE;

    return CAMINHO;
}

//-------------------------------------------------------------------------------
// Nome: gera_labirinto
// Desc: Gera o labirinto em si, testando as posições com teoria de backtracking
//-------------------------------------------------------------------------------
void gera_labirinto(int indeks, int **labirinto, int *backtrack_linha, int *backtrack_coluna, int linha, int coluna, int tamanho, int pos_visitadas)
{
    if (pos_visitadas < (tamanho * tamanho) )
    {
        int vizinho_valido = -1;

        int vizinho_linha[4]; // É necessário testar vizinhos de uma e duas células anteriores e posteriores

        int vizinho_coluna[4]; // É necessário testar vizinhos de uma e duas células anteriores e posteriores

        int prox_passo[4]; // Por se tratar de um labirinto, só se pode andar para cima, baixo, esquerda ou direita, não se pode andar em diagonal

        int prox_linha;

        int prox_coluna;

		int randomization; // Variável responsável por randomizar o labirinto

        int random; // Variável responsável por randomizar o labirinto

		int rand_step;

		if ( (linha - 2 > 0) && (fecha_labirinto(labirinto, linha - 2, coluna) ) )  // Cima
        {
            vizinho_valido++;

            vizinho_linha[vizinho_valido] = linha - 2;

            vizinho_coluna[vizinho_valido] = coluna;

			prox_passo[vizinho_valido] = 1;
        }

        if ( (coluna - 2 > 0) && (fecha_labirinto(labirinto, linha, coluna - 2) ) )  // Esquerda
        {
            vizinho_valido++;

            vizinho_linha[vizinho_valido] = linha;

            vizinho_coluna[vizinho_valido] = coluna - 2;

            prox_passo[vizinho_valido] = 2;
        }

        if ( (coluna + 2 < (tamanho * 2 + 1) ) && (fecha_labirinto(labirinto, linha, coluna + 2) ) )  // Direita
        {
            vizinho_valido++;

            vizinho_linha[vizinho_valido] = linha;

            vizinho_coluna[vizinho_valido] = coluna + 2;

            prox_passo[vizinho_valido] = 3;
        }

        if ( (linha + 2 < (tamanho * 2 + 1) ) && (fecha_labirinto(labirinto, linha + 2, coluna) ) )  // Baixo
        {
            vizinho_valido++;

            vizinho_linha[vizinho_valido] = linha + 2;

            vizinho_coluna[vizinho_valido] = coluna;

            prox_passo[vizinho_valido] = 4;
        }

        if (vizinho_valido == -1)
        {
            // backtrack
			prox_linha = backtrack_linha[indeks];

            prox_coluna = backtrack_coluna[indeks];

            indeks--;
        }

        if(vizinho_valido != -1)
        {
            randomization = vizinho_valido + 1;

            random = rand() % randomization;

			prox_linha = vizinho_linha[random];

            prox_coluna = vizinho_coluna[random];

            indeks++;

            backtrack_linha[indeks] = prox_linha;

            backtrack_coluna[indeks] = prox_coluna;

			rand_step = prox_passo[random];

            if(rand_step == 1)
                labirinto[prox_linha + 1][prox_coluna] = CAMINHO;

            else if(rand_step == 2)
                labirinto[prox_linha][prox_coluna + 1] = CAMINHO;

            else if(rand_step == 3)
                labirinto[prox_linha][prox_coluna - 1] = CAMINHO;

            else if(rand_step == 4)
                labirinto[prox_linha - 1][prox_coluna] = CAMINHO;

            pos_visitadas++;
        }

        gera_labirinto(indeks, labirinto, backtrack_linha, backtrack_coluna, prox_linha, prox_coluna, tamanho, pos_visitadas);
    }
}


