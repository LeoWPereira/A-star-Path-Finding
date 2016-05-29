/*
* alocacao_dinamica.h
* Projeto Final
*
* Neste arquivo ficaram as fun��es de aloca��o e libera��o din�mica de mem�ria
* como n�o s�o fun��es que s�o necess�rias "abertas" o tempo todo, separ�-las em um arquivo diferente fez com que o c�digo ficasse mais simples
*
* Created by Leonardo Winter Pereira & Andr� Eleut�rio
* All Rights reserved
*/

//-------------------------------------------------------------------------------------------------------
// Nome: aloca_matriz
// Desc: Fun��o utilizada para alocar dinamicamente uma matriz dependendo do n�vel de dificuldade escolhido
//-------------------------------------------------------------------------------------------------------
int **aloca_matriz(int tamanho_matriz)
{
	int **matriz; // Devemos criar um ponteiro para ponteiro (por se tratar de uma matriz bi-dimensional)

	// Aloca as linhas da matriz 
	matriz = (int **)malloc(tamanho_matriz * sizeof(int));

	// Aloca as colunas da matriz
	for (int linha = 0; linha < tamanho_matriz; linha++)
		matriz[linha] = (int *)malloc(tamanho_matriz * sizeof(int));
	
	// Determina os valores das c�lulas
	for (int linha = 0; linha < tamanho_matriz; linha++)
		for (int coluna = 0; coluna < tamanho_matriz; coluna++)
			matriz[linha][coluna] = 0;

	return matriz; // Aqui retornamos a pr�pria matriz para que a matriz da fun��o principal possa receber a matriz como criada aqui
}

//----------------------------------------------------------------------------------------
// Nome: libera_matriz
// Desc: Fun��o utilizada para liberar o espa�o utilizado pela matriz alocada previamente
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
// Desc: Fun��o utilizada para alocar dinamicamente um vetor dependendo do n�vel de dificuldade escolhido
//-----------------------------------------------------------------------------------------------------
int *aloca_vetor(int tamanho_vetor)
{
	int *vetor; // Devemos criar um ponteiro para ponteiro (por se tratar de uma matriz bi-dimensional)

	// Aloca o vetor
	for (int linha = 0; linha < tamanho_vetor; linha++)
		vetor = (int *)malloc(tamanho_vetor * sizeof(int));

	// Determina os valores das c�lulas
	for (int linha = 0; linha < tamanho_vetor; linha++)
		vetor[linha] = 0;


	return vetor; // Aqui retornamos a pr�pria matriz para que a matriz da fun��o principal possa receber a matriz como criada aqui
}

//---------------------------------------------------------------------------------------
// Nome: libera_matriz
// Desc: Fun��o utilizada para liberar o espa�o utilizado pelo vetor alocada previamente
//---------------------------------------------------------------------------------------
void *libera_vetor(int *vetor, int tamanho_vetor)
{
	if (vetor == NULL)
		return (NULL);
		
	free (vetor); // Libera o vetor
}
