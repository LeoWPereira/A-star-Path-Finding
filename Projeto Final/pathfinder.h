/*
* algoritmos.h
* Projeto Final
*
* Neste arquivo ficaram separadas as funções encarregadas de realizar a busca pelo melhor caminho!
* Foram realizadas bastantes pesquisas sobre o assunto, afim de tentar otimizar ao máximo a busca pelo caminho "correto"
* Basicamente, quatro foram os algoritmos considerados ótimos:
* > Algoritmo de Djikstra
* > Algoritmo de Floyd-Warshall
* > Algoritmo A*
* > Algoritmo de Bolland-Ford
*
* O algoritmo escolhido foi o A*
*
* A seguir estão alguns links interessantes sobre as teorias citadas acima:
* http://www.policyalmanac.org/games/aStarTutorial_port.htm
* http://www.ime.usp.br/~pf/algoritmos_para_grafos/
* http://forum.clubedohardware.com.br/algoritmo-dijkstra/365896?s=a9e9e44659013f04a01e53bddf2c13f0&amp;
*
* Outros links úteis, para que seja possível o entendimento desse algoritmo:
* http://pt.wikipedia.org/wiki/Heap
* http://www.policyalmanac.org/games/binaryHeaps.htm
*
* Created by Leonardo Winter Pereira & André Eleutério
* All Rights reserved
*/

#define CAMPO_VAZIO 0
#define PAREDE -1
#define OBSTACULO -2
#define POS_INICIAL 3
#define BONUS 50
#define POS_FINAL 5
#define CAMINHO 6
#define DIAGONAL 14 // Relacionado ao custo para se andar em diagonal
#define HORIZONTAL_OU_VERTICAL 10 // Relacionado ao custo para se andar na vertical ou horizontal

// -------------------------
// Funções por prototipação
// -------------------------
void desenha_caminho(int **qual_lista, int **custo_G, int **matriz, int tamanho_matriz, int linha_inicial, int coluna_inicial, int linha_final, int coluna_final);

//-----------------------------------------
// Name: procura_caminho
// Desc: Procura o menor caminho usando A*
//-----------------------------------------
int procura_caminho (int tamanho_matriz, int **matriz, int linha_inicial, int coluna_inicial, int linha_final, int coluna_final, bool desenha)
{
	//----------------------------------
	// 1. Cria as variáveis necessárias
	//----------------------------------
	int **replica_matriz_original = aloca_matriz(tamanho_matriz); // Matriz que armazena se a célula está na lista aberta ou na lista fechada

	int **matriz_de_bonus = aloca_matriz(tamanho_matriz); // Matriz que armazena se a célula está na lista aberta ou na lista fechada

	int *lista_aberta = aloca_vetor(tamanho_matriz * tamanho_matriz); // Vetor que armazena as células que serão testadas

	int *linha_aberta = aloca_vetor(tamanho_matriz * tamanho_matriz); // Vetor que armazena a linha da célula que esta na lista aberta

	int *coluna_aberta = aloca_vetor(tamanho_matriz * tamanho_matriz); // Vetor que armazena a coluna da célula que esta na lista aberta

	int **vizinho_linha = aloca_matriz(tamanho_matriz); // Matriz que armazena os vizinhos_linha de cada célula
	
	int **vizinho_coluna = aloca_matriz(tamanho_matriz); // Matriz que armazena os vizinhos_coluna de cada célula

	int *custo_F = aloca_vetor(tamanho_matriz * tamanho_matriz); // Vetor que armazena o custo F da célula que está na lista aberta
	
	int **custo_G = aloca_matriz(tamanho_matriz); // Matriz que armazena o custo G de cada célula

	int *custo_H = aloca_vetor(tamanho_matriz * tamanho_matriz); // Vetor que armazena o custo H da célula que está na lista aberta
	
	const int caminho_nao_comecou = 0; // Variável relacionada ao caminho

	const int caminho_encontrado = 1; // Variável relacionada ao caminho

	const int caminho_nao_existe = 2; // Variável relacionada ao caminho

	const int celula_passavel = 0; // Variável relacionada à célula. Ela determina que o caminho pode andar por ela
	
	const int celula_nao_passavel = 1; // Variável relacionada à célula. Ela determina que o caminho não pode andar por ela

	int na_lista_fechada = 10; // Os elementos que estão na lista fechada. Inicialmente com o valor 10

	int na_lista_aberta = 0; // O elemento que está na lista aberta. Inicialmente nulo
	
	int vizinho_linha_valido = 0;
	
	int vizinho_coluna_valido = 0;
	
	int m = 0;
	
	int u = 0;
	
	int v = 0;
	
	int temp = 0;
	
	int canto = 0;
	
	int num_de_celulas_na_lista_aberta = 0;

	int custo_G_adicionado = 0;
	
	int custo_G_temp = 0;

	int caminho = 0;

	int ID_novo_item_lista_aberta = 0;

	int tamanho_caminho[2]; // Armazena o tamanho do caminho encontrado

	int localizacao_caminho[2]; // Armazena a posição atual ao longo do caminho escolhido


	//---------------------------------------------------------------------------------------------
	// 2. Checagens rápidas do caminho: Em algumas circunstâncias o caminho não precisa ser gerado
	//---------------------------------------------------------------------------------------------

	// Se a posição inicial é a mesma que a posição final
	if ( (linha_inicial == linha_final) && (coluna_inicial == coluna_final) && (localizacao_caminho[1] > 0) )
		return caminho_encontrado;

	if ( (linha_inicial == linha_final) && (coluna_inicial == coluna_final) && (localizacao_caminho[1] == 0) )
		return caminho_nao_existe;


	//----------------------------
	// 3. Clona a matriz original
	//----------------------------
	for (int linha = 0; linha < tamanho_matriz; linha++)
	{
		for (int coluna = 0; coluna < tamanho_matriz; coluna++)
			replica_matriz_original[linha][coluna] = matriz[linha][coluna];
	}


	//---------------------------------------------------------------------------------------------------------------------------------------------
	// 4. Zera todas as posições da matriz replica, excluindo apenas as Paredes e Obstaculos, para saber que tais posições não podem ser acessadas
	//---------------------------------------------------------------------------------------------------------------------------------------------
	for (int linha = 0; linha < tamanho_matriz; linha++)
	{
		for (int coluna = 0; coluna < tamanho_matriz; coluna++)
		{
			if ( (replica_matriz_original[linha][coluna] != PAREDE) && (replica_matriz_original[linha][coluna] != OBSTACULO) )
				replica_matriz_original[linha][coluna] = 0;
		}
	}

	for (int linha = 0; linha < tamanho_matriz; linha++)
	{
		for (int coluna = 0; coluna < tamanho_matriz; coluna++)
		{
			if (matriz[linha][coluna] != BONUS)
				matriz_de_bonus[linha][coluna] = 2;
			else
				matriz_de_bonus[linha][coluna] = 1;
		}
	}

	na_lista_fechada = 2; // Troca os valores da variavel
	
	na_lista_aberta = 1; // Troca os valores da variavel
	
	tamanho_caminho[1] = caminho_nao_comecou; // Determina que o tamanho do caminho é, inicialmente, nulo
	
	localizacao_caminho[1] = caminho_nao_comecou;

	custo_G[linha_inicial][coluna_inicial] = 0; // Zera o valor da posição inicial na tabela custo_G


	//-----------------------------------------------
	// 5. Adiciona a posição inicial na lista aberta
	//-----------------------------------------------
	num_de_celulas_na_lista_aberta = 1;
	
	lista_aberta[1] = 1; // Coloca o primeiro item na lista aberta, o qual será mantido como um heap binário
	
	linha_aberta[1] = linha_inicial; // Adiciona a linha inicial na lista de linha aberta
	
	coluna_aberta[1] = coluna_inicial; // Adiciona a coluna inicial na lista de coluna aberta


	//-------------------------------------------------------------------------------------------------------------------
	// 6. O seguinte bloco de comandos será realizado até que um caminho seja encontrado ou seja considerado não existente
	//-------------------------------------------------------------------------------------------------------------------
	do
	{
		//--------------------------------------------------------------------------------------------------------------------------
		// 7. Se a lista aberta é não nula, pegue a primeira célula fora da lista. Esta é a célula com o menor custo_F na lista aberta
		//--------------------------------------------------------------------------------------------------------------------------
		if (num_de_celulas_na_lista_aberta != 0)
		{
			//-----------------------------------------
			// 8. Tira o primeiro item da lista aberta
			//-----------------------------------------
			vizinho_linha_valido = linha_aberta[ lista_aberta[1] ]; // Armazena a linha do vizinho
			
			vizinho_coluna_valido = coluna_aberta[ lista_aberta[1] ]; // Armazena a coluna do vizinho
			
			replica_matriz_original[vizinho_linha_valido][vizinho_coluna_valido] = na_lista_fechada; // Adiciona a célula na lista fechada

			//-------------------------------------------------------------------------------------------------------------------------------------
			// Lista aberta = Heap Binário:
			// > Delete o item na lista aberta, o qual é mantido como um heap bináario. Para saber mais sobre, acesse as páginas no início do arquivo
			//-------------------------------------------------------------------------------------------------------------------------------------
			
			num_de_celulas_na_lista_aberta = num_de_celulas_na_lista_aberta - 1; // Diminui o número de itens la lista aberta
		
			// Deleta o primeiro item no heap binário e reordena o heap, sendo que o item com o menor custo_F sobre para primeiro
			lista_aberta[1] = lista_aberta[num_de_celulas_na_lista_aberta + 1]; // Move o último item no heap para primeiro
				
			v = 1;

			// O seguindo bloco de comandos é executado até que o novo item na primeira posição do heap esteja na posição adequada do heap
			do
			{
				u = v;		

				if ( (2 * u + 1) <= num_de_celulas_na_lista_aberta) // Se ambos os filhos existirem
				{
					// Verifica se o custo_F do item Pai é maior que o custo_F de cada filho. Seleciona o menor custo_F dos dois filhos.
					if (custo_F[ lista_aberta[u] ] >= custo_F[ lista_aberta[2 * u] ]) 
						v = 2 * u;

					if (custo_F[ lista_aberta[v] ] >= custo_F[ lista_aberta[2 * u + 1] ]) 
						v = 2 * u + 1;		
				}
	
				else
				{
					if ( (2 * u) <= num_de_celulas_na_lista_aberta) // Se só existe um filho
					{
	 					// Verifica se o custo_F do Pai é maior que o custo_F do filho	
						if (custo_F[ lista_aberta[u] ] >= custo_F[ lista_aberta[2 * u] ]) 
							v = 2 * u;
					}
				}

				if (u != v) // Se o custo_F do Pai é maior que o custo_F de um dos filhos, troque um pelo outro
				{
					temp = lista_aberta[u];
						
					lista_aberta[u] = lista_aberta[v];
		
					lista_aberta[v] = temp;			
				}

				else
					break; // Saia do loop
		
			} while (true); // Reordena o heap binário


			//---------------------------------------------------------------------------------------------------------------
			// 9. Verifica as células adjacentes. 
			// Adiciona as células adjacentes à lista aberta para depois verificar se um possível caminho por ela é apropriado.
			//---------------------------------------------------------------------------------------------------------------
			for (int coluna = (vizinho_coluna_valido - 1); coluna <= (vizinho_coluna_valido + 1); coluna++)
			{
				for (int linha = (vizinho_linha_valido - 1); linha <= (vizinho_linha_valido + 1); linha++)
				{
					// Verifica primeiramente se os testes não irão dar acesso indevido à memória (decorrente de uma linha / coluna não existente)
					if ( (linha != -1) && (coluna != -1) && (linha != tamanho_matriz) && (coluna != tamanho_matriz) )
					{
						// Se ainda não estiver na lista fechada (itens na lista fechada já foram considerados e agora podem ser ignorados)
						if (replica_matriz_original[linha][coluna] != na_lista_fechada)
						{
							// Se for diferente de uma parede ou obstáculo
							if ( (replica_matriz_original[linha][coluna] != PAREDE) && (replica_matriz_original[linha][coluna] != OBSTACULO) )
							{
								// Não "corte" cantos
								canto = celula_passavel;	
							
								if (linha == (vizinho_linha_valido - 1) ) 
								{
									if (coluna == (vizinho_coluna_valido - 1) )
									{
										if ( (replica_matriz_original[vizinho_linha_valido - 1][vizinho_coluna_valido] == PAREDE) || (replica_matriz_original[vizinho_linha_valido - 1][vizinho_coluna_valido] == OBSTACULO) || (replica_matriz_original[vizinho_linha_valido][vizinho_coluna_valido - 1] == PAREDE) || (replica_matriz_original[vizinho_linha_valido][vizinho_coluna_valido - 1] == OBSTACULO) )
											canto = celula_nao_passavel;
									}
		
									else if (coluna == (vizinho_coluna_valido + 1) )
									{
										if ( (replica_matriz_original[vizinho_linha_valido][vizinho_coluna_valido + 1] == PAREDE) || (replica_matriz_original[vizinho_linha_valido][vizinho_coluna_valido + 1] == OBSTACULO) || (replica_matriz_original[vizinho_linha_valido - 1][vizinho_coluna_valido] == PAREDE) || (replica_matriz_original[vizinho_linha_valido - 1][vizinho_coluna_valido] == OBSTACULO) )
											canto = celula_nao_passavel; 
									}
								}
	
								else if (linha == (vizinho_linha_valido + 1) )
								{
									if (coluna == (vizinho_coluna_valido - 1) )
									{
										if ( (replica_matriz_original[vizinho_linha_valido][vizinho_coluna_valido - 1] == PAREDE) || (replica_matriz_original[vizinho_linha_valido][vizinho_coluna_valido - 1] == OBSTACULO) || (replica_matriz_original[vizinho_linha_valido + 1][vizinho_coluna_valido] == PAREDE) || (replica_matriz_original[vizinho_linha_valido + 1][vizinho_coluna_valido] == OBSTACULO) )
											canto = celula_nao_passavel;
									}
	
									else if (coluna == (vizinho_coluna_valido + 1) )
									{
										if ( (replica_matriz_original[vizinho_linha_valido + 1][vizinho_coluna_valido] == PAREDE) || (replica_matriz_original[vizinho_linha_valido + 1][vizinho_coluna_valido] == OBSTACULO) || (replica_matriz_original[vizinho_linha_valido][vizinho_coluna_valido + 1] == PAREDE) || (replica_matriz_original[vizinho_linha_valido][vizinho_coluna_valido + 1] == OBSTACULO) )
											canto = celula_nao_passavel; 
									}
								}	

								if (canto == celula_passavel)
								{
									// Se ainda não estiver na lista aberta, adicione o item para a lista aberta
									if (replica_matriz_original[linha][coluna] != na_lista_aberta) 
									{	
										// Cria um novo item da lista aberta no heap binário
										ID_novo_item_lista_aberta = ID_novo_item_lista_aberta + 1; // Cada novo item tem um ID único
											
										m = num_de_celulas_na_lista_aberta + 1;
											
										lista_aberta[m] = ID_novo_item_lista_aberta; // Coloca o novo item da lista aberta (seu ID) no fundo do heap
											
										linha_aberta[ID_novo_item_lista_aberta] = linha; // Armazena a coordenada linha do novo item
											
										coluna_aberta[ID_novo_item_lista_aberta] = coluna; // Armazena a coordenada coluna do novo item

										// Calcula o custo_G
										if ( (abs(linha - vizinho_linha_valido) == 1) && (abs(coluna - vizinho_coluna_valido) == 1) )
											custo_G_adicionado = DIAGONAL; // Custo de andar na diagonal
		
										else	
											custo_G_adicionado = HORIZONTAL_OU_VERTICAL; // Custo de andar na vertical / horizontal
		
										custo_G[linha][coluna] = (custo_G[vizinho_linha_valido][vizinho_coluna_valido] + custo_G_adicionado); // Adiciona o custo na célula verificada

										// Calcula o custo_H e custo_F e encontra o "pai"
										custo_H[ lista_aberta[m] ] = 10 * (abs(linha - linha_final) + abs(coluna - coluna_final));

										custo_F[ lista_aberta[m] ] = custo_G[linha][coluna] + custo_H[ lista_aberta[m] ];
											
										vizinho_linha[linha][coluna] = vizinho_linha_valido;
											
										vizinho_coluna[linha][coluna] = vizinho_coluna_valido;	


										//----------------------------------------------------------------------------------------------------------------------
										// 10. Move o novo item da lista aberta para o lugar apropriado do heap binário.
										// Iniciando no fundo do heap, comparar com os itens "pai", trocando de posição sempre que necessário,
										// até que o item encontre seu lugar no heap, lembrando que este pode não mudar de posição (caso tenha o menor custo_F)
										//----------------------------------------------------------------------------------------------------------------------
										
										 // O seguinte bloco de comandos será executado até que o item teste todos os itens "pai" ( m == 1)
										while (m != 1)
										{
											// Verifica se o csuto_F do filho é menor que o custo_F do pai. Se sim, troque um pelo outro
											if (custo_F[ lista_aberta[m] ] <= custo_F[ lista_aberta[m / 2] ])
											{
												temp = lista_aberta[m / 2];
													
												lista_aberta[m / 2] = lista_aberta[m];
													
												lista_aberta[m] = temp;
													
												m = m / 2;
											}
			
											else
												break;
										}
		
										num_de_celulas_na_lista_aberta = num_de_celulas_na_lista_aberta + 1; // Adiciona um para o número de itens no heap

										// Troca a posição do replica_matriz_original para mostrar que o novo item está na lista aberta
										replica_matriz_original[linha][coluna] = na_lista_aberta;
									}


									//--------------------------------------------------------------------------------------------------------------------------------
									// 11. Se a célula adjacentejá está na lista aberta, verifica se o caminho na posição inicial até a atual célula é a melhor opção
									// Se sim, troque o custo_G e o custo_F da mesma.
									//--------------------------------------------------------------------------------------------------------------------------------
									else // Se replica_matriz_original[linha][coluna] = na_lista_aberta
									{
										// Calcula o custo_G do possível novo caminho
										if ( (abs(linha - vizinho_linha_valido) == 1) && (abs(coluna - vizinho_coluna_valido) == 1) )
											custo_G_adicionado = DIAGONAL; // Custo para andar na diagonal	
		
										else	
											custo_G_adicionado = HORIZONTAL_OU_VERTICAL; // Custo para andar na horizontal / vertical			
		
										custo_G_temp = custo_G[vizinho_linha_valido][vizinho_coluna_valido] + custo_G_adicionado;  // Adiciona o custo em uma variável temporária
		
										// Se este caminho é menor. Então troque o custo_G e o custo_F da célula pai		
										if (custo_G_temp < custo_G[linha][coluna]) // Se o custo_G é menor
										{
											vizinho_linha[linha][coluna] = vizinho_linha_valido; // Troca a coordenada linha do pai

											vizinho_coluna[linha][coluna] = vizinho_coluna_valido; // Troca a coordenada coluna do pai
											
											custo_G[linha][coluna] = custo_G_temp; // Troca o custo_G			


											// Como, ao trocar o custo_G também trocamos o custo_F, caso o item esteja na lista aberta,
											// precisamos trocar o custo_F e a posição do item na lista aberta, para nos certificarmos de que
											// a lista aberta esteja correta
											for (int x = 1; x <= num_de_celulas_na_lista_aberta; x++) // Olha os itens do heap
											{
												if ( (linha_aberta[ lista_aberta[x] ] == linha) && (coluna_aberta[ lista_aberta[x] ] == coluna) ) // Célula encontrada
												{
													custo_F[ lista_aberta[x] ] = custo_G[linha][coluna] + custo_H[ lista_aberta[x] ]; // Troca o custo_F
				
													// Verifica se, ao trocar o custo_F, teve alguma mudança no heap
													m = x;
												
													// O seguinte bloco de comandos será executado enquanto o item não chegar no topo do heap (m == 1)
													while (m != 1)
													{
														// Verifica se filho é menor que pai. Se sim, troque-os
														if (custo_F[ lista_aberta[m] ] < custo_F[ lista_aberta[m / 2] ])
														{
															temp = lista_aberta[m / 2];
						
															lista_aberta[m / 2] = lista_aberta[m];
						
															lista_aberta[m] = temp;
						
															m = m / 2;
														}
						
														else
															break;
													} 

													break;

												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		//-------------------------------------------------------
		// 12. Se a lista aberta é nula então não existe caminho	
		//-------------------------------------------------------
		else
		{
			caminho = caminho_nao_existe;
				
			break;
		}  

		// Se posição_final foi adicionada para a lista aberta então um caminho foi encontrado
		if (replica_matriz_original[linha_final][coluna_final] == na_lista_aberta)
		{
			caminho = caminho_encontrado;
				
			break;
		}

	} while (true);

	caminho = custo_G[linha_final][coluna_final];

	if (desenha)
		desenha_caminho(replica_matriz_original, custo_G, matriz, tamanho_matriz, linha_inicial, coluna_inicial, linha_final, coluna_final); // Atualiza a matriz com o caminho e a imprime na tela

	// Libera as matrizes e os vetores alocados dinamicamente
	libera_matriz(replica_matriz_original, tamanho_matriz);

	libera_matriz(matriz_de_bonus, tamanho_matriz);

	libera_matriz(vizinho_linha, tamanho_matriz);

	libera_matriz(vizinho_coluna, tamanho_matriz);

	libera_matriz(custo_G, tamanho_matriz);

	libera_vetor(lista_aberta, tamanho_matriz * tamanho_matriz);

	libera_vetor(linha_aberta, tamanho_matriz * tamanho_matriz);

	libera_vetor(coluna_aberta, tamanho_matriz * tamanho_matriz);

	libera_vetor(custo_F, tamanho_matriz * tamanho_matriz);

	libera_vetor(custo_H, tamanho_matriz * tamanho_matriz);

	return caminho;
}

//-------------------------------------------------------------------------------------------------------
// Name: desenha_caminho
// Desc: Atualiza a matriz principal, substituindo por CAMINHO o próximo passo, sendo este o menor possível
//-------------------------------------------------------------------------------------------------------
void desenha_caminho(int **qual_lista, int **custo_G, int **matriz, int tamanho_matriz, int linha_inicial, int coluna_inicial, int linha_final, int coluna_final)
{
	int linha_atual = linha_final; // Variável auxiliar

	int coluna_atual = coluna_final; // Variável auxiliar

	int dificuldade = tamanho_matriz / 10; // Variável utilizada para melhor entendimento do código

	// O seguinte bloco de comando será realizado até que custo_g[linha_atual][coluna_atual] == 0
	do
	{
		if ( (linha_atual - 1 >= 0) && (coluna_atual + 1 < tamanho_matriz) && (custo_G[linha_atual - 1][coluna_atual + 1] == custo_G[linha_atual][coluna_atual] - DIAGONAL) ) // Testa diagonal cima-direita
		{
			matriz[linha_atual - 1][coluna_atual + 1] = CAMINHO;

			linha_atual--;

			coluna_atual++;
		}

		else if ( (linha_atual - 1 >= 0) && (coluna_atual - 1 >= 0) && (custo_G[linha_atual - 1][coluna_atual - 1] == custo_G[linha_atual][coluna_atual] - DIAGONAL) ) // Testa diagonal cima-esquerda
		{
			matriz[linha_atual - 1][coluna_atual - 1] = CAMINHO;

			linha_atual--;

			coluna_atual--;
		}

		else if ( (linha_atual + 1 < tamanho_matriz) && (coluna_atual + 1 < tamanho_matriz) && (custo_G[linha_atual + 1][coluna_atual + 1] == custo_G[linha_atual][coluna_atual] - DIAGONAL) ) // Testa diagonal baixo-direita
		{
			matriz[linha_atual + 1][coluna_atual + 1] = CAMINHO;

			linha_atual++;

			coluna_atual++;
		}

		else if ( (linha_atual + 1 < tamanho_matriz) && (coluna_atual - 1 >= 0) && (custo_G[linha_atual + 1][coluna_atual - 1] == custo_G[linha_atual][coluna_atual] - DIAGONAL) ) // Testa diagonal baixo-esquerda
		{
			matriz[linha_atual + 1][coluna_atual - 1] = CAMINHO;

			linha_atual++;

			coluna_atual--;
		}

		else if ( (coluna_atual + 1 < tamanho_matriz) && (custo_G[linha_atual][coluna_atual + 1] == custo_G[linha_atual][coluna_atual] - HORIZONTAL_OU_VERTICAL) ) // Testa caminho da direita
		{
			if ( (custo_G[linha_atual][coluna_atual] - HORIZONTAL_OU_VERTICAL == 0) && ( (qual_lista[linha_atual][coluna_atual + 1] == PAREDE) || (qual_lista[linha_atual][coluna_atual + 1] == OBSTACULO) ) )
				coluna_atual++;

			else
			{
				matriz[linha_atual][coluna_atual + 1] = CAMINHO;

				coluna_atual++;
			}
				
		}

		else if ( (coluna_atual - 1 >= 0) && (custo_G[linha_atual][coluna_atual - 1] == custo_G[linha_atual][coluna_atual] - HORIZONTAL_OU_VERTICAL) ) // Testa caminho da esquerda
		{
			if ( (custo_G[linha_atual][coluna_atual] - HORIZONTAL_OU_VERTICAL == 0) && ( (qual_lista[linha_atual][coluna_atual - 1] == PAREDE) || (qual_lista[linha_atual][coluna_atual - 1] == OBSTACULO) ) )
				coluna_atual--;
				
			else
			{
				matriz[linha_atual][coluna_atual - 1] = CAMINHO;

				coluna_atual--;
			}
				
		}

		else if ( (linha_atual + 1 < tamanho_matriz) && (custo_G[linha_atual + 1][coluna_atual] == custo_G[linha_atual][coluna_atual] - HORIZONTAL_OU_VERTICAL) ) // Testa caminho de baixo
		{
			if ( (custo_G[linha_atual][coluna_atual] - HORIZONTAL_OU_VERTICAL == 0) && ( (qual_lista[linha_atual + 1][coluna_atual] == PAREDE) || (qual_lista[linha_atual + 1][coluna_atual] == OBSTACULO) ) )
				linha_atual++;
				
			else
			{
				matriz[linha_atual + 1][coluna_atual] = CAMINHO;

				linha_atual++;
			}
		}

		else if ( (linha_atual - 1 >= 0) && (custo_G[linha_atual - 1][coluna_atual] == custo_G[linha_atual][coluna_atual] - HORIZONTAL_OU_VERTICAL) ) // Testa caminho de cima
		{
			if ( (custo_G[linha_atual][coluna_atual] - HORIZONTAL_OU_VERTICAL == 0) && ( (qual_lista[linha_atual - 1][coluna_atual] == PAREDE) || (qual_lista[linha_atual - 1][coluna_atual] == OBSTACULO) ) )
				linha_atual--;
				
			else
			{
				matriz[linha_atual - 1][coluna_atual] = CAMINHO;

				linha_atual--;
			}
		}

		imprime_matriz(matriz, dificuldade); // Desenha a matriz na tela a cada passo realizado

		al_flip_display(); // Atualiza a tela

	} while ( (custo_G[linha_atual][coluna_atual] != HORIZONTAL_OU_VERTICAL) && (custo_G[linha_atual][coluna_atual] != DIAGONAL) );

	imprime_matriz(matriz, dificuldade); // Desenha a matriz na tela

	al_flip_display(); // Atualiza a tela
}