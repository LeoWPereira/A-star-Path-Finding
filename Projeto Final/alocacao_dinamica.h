/*
* alocacao_dinamica.h
* Projeto Final
*
* Neste arquivo ficaram as funções de alocação e liberação dinâmica de memória
* como não são funções que são necessárias "abertas" o tempo todo, separá-las em um arquivo diferente fez com que o código ficasse mais simples
*
* Created by Leonardo Winter Pereira & André Eleutério
* All Rights reserved
*/

//-------------------------------------------------------------------------------------------------------
// Nome: aloca_matriz
// Desc: Função utilizada para alocar dinamicamente uma matriz dependendo do nível de dificuldade escolhido
//-------------------------------------------------------------------------------------------------------
int **aloca_matriz(int tamanho_matriz)
{
	int **matriz; // Devemos criar um ponteiro para ponteiro (por se tratar de uma matriz bi-dimensional)

	// Aloca as linhas da matriz 
	matriz = (int **)malloc(tamanho_matriz * sizeof(int));

	// Aloca as colunas da matriz
	for (int linha = 0; linha < tamanho_matriz; linha++)
		matriz[linha] = (int *)malloc(tamanho_matriz * sizeof(int));
	
	// Determina os valores das células
	for (int linha = 0; linha < tamanho_matriz; linha++)
		for (int coluna = 0; coluna < tamanho_matriz; coluna++)
			matriz[linha][coluna] = 0;

	return matriz; // Aqui retornamos a própria matriz para que a matriz da função principal possa receber a matriz como criada aqui
}

//----------------------------------------------------------------------------------------
// Nome: libera_matriz
// Desc: Função utilizada para liberar o espaço utilizado pela matriz alocada previamente
//----------------------------------------------------------------------------------------
void **libera_matriz(int **matriz, int tamanho_matriz)
{
	if (matriz == NULL)
		return (NULL);

	// Libera as linhas da matriz
	for (int linha = 0; linha < tamanho_matriz; linha++)
		free (matriz[linha]);
		
	free (matriz); // Libera a matriz
}

//-----------------------------------------------------------------------------------------------------
// Nome: libera_matriz
// Desc: Função utilizada para alocar dinamicamente um vetor dependendo do nível de dificuldade escolhido
//-----------------------------------------------------------------------------------------------------
int *aloca_vetor(int tamanho_vetor)
{
	int *vetor; // Devemos criar um ponteiro para ponteiro (por se tratar de uma matriz bi-dimensional)

	// Aloca o vetor
	for (int linha = 0; linha < tamanho_vetor; linha++)
		vetor = (int *)malloc(tamanho_vetor * sizeof(int));

	// Determina os valores das células
	for (int linha = 0; linha < tamanho_vetor; linha++)
		vetor[linha] = 0;


	return vetor; // Aqui retornamos a própria matriz para que a matriz da função principal possa receber a matriz como criada aqui
}

//---------------------------------------------------------------------------------------
// Nome: libera_matriz
// Desc: Função utilizada para liberar o espaço utilizado pelo vetor alocada previamente
//---------------------------------------------------------------------------------------
void *libera_vetor(int *vetor, int tamanho_vetor)
{
	if (vetor == NULL)
		return (NULL);
		
	free (vetor); // Libera o vetor
}
