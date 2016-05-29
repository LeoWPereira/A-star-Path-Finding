/*
* funcoes_para_escrever.h
* Projeto Final
*
* Neste arquivo ficaram somente as funções que escrevem na tela as opções (somente funções que não chamam outras funções)
*
* Created by Leonardo Winter Pereira & André Eleutério
* All Rights reserved
*/

#define LARGURA_MAX 600 // define a largura máxima
#define ALTURA_MAX 400 // define a altura máxima
#define CAMPO_VAZIO 0 // define o valor do campo vazio
#define PAREDE -1 // define o valor da parede
#define OBSTACULO -2 // define o valor do obstaculo
#define POS_INICIAL 3 // define o valor da posição inicial
#define BONUS 4 // define o valor do bônus
#define POS_FINAL 5 // define o valor da posição final
#define ALVO 8 // define o valor do alvo
#define CAMINHO 6 // define o valor do caminho

//----------------------------------------------------------------------------------------------------------------------------------------
// Nome: escreve_opcoes
// Desc: Foi decidido utilizar uma função para escrever as opções do menu pois dessa forma o código diminui e facilita na hora de manutenção
//----------------------------------------------------------------------------------------------------------------------------------------
void escreve_opcoes(char *opcoes[4], int opcao, ALLEGRO_FONT *fonte_menu)
{
	if(opcao == 0)
		al_draw_text(fonte_menu, al_map_rgb(255, 255, 255), LARGURA_MAX / 1.42, ALTURA_MAX / 4, ALLEGRO_ALIGN_CENTRE, opcoes[0]);

	else
		al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 1.42, ALTURA_MAX / 4, ALLEGRO_ALIGN_CENTRE, opcoes[0]);

	if(opcao == 1)
		al_draw_text(fonte_menu, al_map_rgb(255, 255, 255), LARGURA_MAX / 1.42, ALTURA_MAX / 2.6, ALLEGRO_ALIGN_CENTRE, opcoes[1]);

	else
		al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 1.42, ALTURA_MAX / 2.6, ALLEGRO_ALIGN_CENTRE, opcoes[1]);

	if(opcao == 2)
		al_draw_text(fonte_menu, al_map_rgb(255, 255, 255), LARGURA_MAX / 1.42, ALTURA_MAX / 1.9, ALLEGRO_ALIGN_CENTRE, opcoes[2]);

	else
		al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 1.42, ALTURA_MAX / 1.9, ALLEGRO_ALIGN_CENTRE, opcoes[2]);

	if(opcao == 3)
		al_draw_text(fonte_menu, al_map_rgb(255, 255, 255), LARGURA_MAX / 1.42, ALTURA_MAX / 1.5, ALLEGRO_ALIGN_CENTRE, opcoes[3]);

	else
		al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 1.42, ALTURA_MAX / 1.5, ALLEGRO_ALIGN_CENTRE, opcoes[3]);
}

//-------------------------------------------------------
// Nome: escreve_config
// Desc: Função para escrever as opções de configurações
//-------------------------------------------------------
void escreve_config(ALLEGRO_FONT *fonte_menu, int dificuldade, bool obstaculo, bool labirinto)
{
	char *opcoes[3] = {"Dificuldade: ",
					 "Obstaculo: ",
					 "Labirinto: "};

	ALLEGRO_BITMAP *botao_config = al_load_bitmap("images/config button.jpg"); // Cria e carrega arquivo config button.jpg

	ALLEGRO_BITMAP *botao_config_selec = al_load_bitmap("images/config button selec.jpg"); // Cria e carrega arquivo config button selec.jpg

	//
	// Os comandos a seguir escreverão a opção dificudade, assim como os cinco possíveis níveis (padrão: 1)
	//
	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 7, ALTURA_MAX / 8, ALLEGRO_ALIGN_CENTRE, opcoes[0]);

	if(dificuldade == 1) 
		al_draw_bitmap(botao_config_selec, LARGURA_MAX / 4, ALTURA_MAX / 8, NULL); // Cria o "botão" para dificuldade (se for a opção selecionada)

	else
		al_draw_bitmap(botao_config, LARGURA_MAX / 4, ALTURA_MAX / 8, NULL); // Cria o "botão" para dificuldade (se não for a opção selecionada)

	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 3.4, ALTURA_MAX / 8, ALLEGRO_ALIGN_CENTRE, "1");

	if(dificuldade == 2)
		al_draw_bitmap(botao_config_selec, LARGURA_MAX / 2.8, ALTURA_MAX / 8, NULL); // Cria o "botão" para dificuldade (se for a opção selecionada)

	else
		al_draw_bitmap(botao_config, LARGURA_MAX / 2.8, ALTURA_MAX / 8, NULL); // Cria o "botão" para dificuldade (se não for a opção selecionada)

	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 2.45, ALTURA_MAX / 8, ALLEGRO_ALIGN_CENTRE, "2");
	
	if(dificuldade == 3)
		al_draw_bitmap(botao_config_selec, LARGURA_MAX / 2.1, ALTURA_MAX / 8, NULL); // Cria o "botão" para dificuldade (se for a opção selecionada)

	else
		al_draw_bitmap(botao_config, LARGURA_MAX / 2.1, ALTURA_MAX / 8, NULL); // Cria o "botão" para dificuldade (se não for a opção selecionada)

	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 1.9, ALTURA_MAX / 8, ALLEGRO_ALIGN_CENTRE, "3");

	if(dificuldade == 4)
		al_draw_bitmap(botao_config_selec, LARGURA_MAX / 1.65, ALTURA_MAX / 8, NULL); // Cria o "botão" para dificuldade (se for a opção selecionada)

	else
		al_draw_bitmap(botao_config, LARGURA_MAX / 1.65, ALTURA_MAX / 8, NULL); // Cria o "botão" para dificuldade (se não for a opção selecionada)

	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 1.52, ALTURA_MAX / 8, ALLEGRO_ALIGN_CENTRE, "4");

	if(dificuldade == 5)
		al_draw_bitmap(botao_config_selec, LARGURA_MAX / 1.35, ALTURA_MAX / 8, NULL); // Cria o "botão" para dificuldade (se for a opção selecionada)

	else
		al_draw_bitmap(botao_config, LARGURA_MAX / 1.35, ALTURA_MAX / 8, NULL); // Cria o "botão" para dificuldade (se não for a opção selecionada)

	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 1.27, ALTURA_MAX / 8, ALLEGRO_ALIGN_CENTRE, "5");
	//
	//Fim das opções de dificuldade
	//

	//
	// Os comandos a seguir escreverão  a opção da presença ou nao de obstáculos (padrão: Sim)
	//
	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 7, ALTURA_MAX / 3, ALLEGRO_ALIGN_CENTRE, opcoes[1]);

	if(obstaculo == true)
		al_draw_bitmap(botao_config_selec, LARGURA_MAX / 4, ALTURA_MAX / 3, NULL); // Cria o "botão" para diogonal (se for a opção selecionada)

	else
		al_draw_bitmap(botao_config, LARGURA_MAX / 4, ALTURA_MAX / 3, NULL); // Cria o "botão" para diogonal (se não for a opção selecionada)

	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 3.15, ALTURA_MAX / 3, ALLEGRO_ALIGN_CENTRE, "Sim");

	if(obstaculo == false)
		al_draw_bitmap(botao_config_selec, LARGURA_MAX / 2.8, ALTURA_MAX / 3, NULL); // Cria o "botão" para obstaculo (se for a opção selecionada)

	else
		al_draw_bitmap(botao_config, LARGURA_MAX / 2.8, ALTURA_MAX / 3, NULL); // Cria o "botão" para obstaculo (se não for a opção selecionada)

	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 2.35, ALTURA_MAX / 3, ALLEGRO_ALIGN_CENTRE, "Nao");
	//
	// Fim das opções de obstáculos
	//

	//
	// Os comandos a seguir escreverão  a opção de existir ou não labirinto (padrão: Não)
	//
	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 7, ALTURA_MAX / 1.85, ALLEGRO_ALIGN_CENTRE, opcoes[2]);

	if(labirinto == true)
		al_draw_bitmap(botao_config_selec, LARGURA_MAX / 4, ALTURA_MAX / 1.85, NULL); // Cria o "botão" para labirinto (se for a opção selecionada)

	else
		al_draw_bitmap(botao_config, LARGURA_MAX / 4, ALTURA_MAX / 1.85, NULL); // Cria o "botão" para labirinto (se não for a opção selecionada)

	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 3.15, ALTURA_MAX / 1.85, ALLEGRO_ALIGN_CENTRE, "Sim");

	if(labirinto == false)
		al_draw_bitmap(botao_config_selec, LARGURA_MAX / 2.8, ALTURA_MAX / 1.85, NULL); // Cria o "botão" para labirinto (se for a opção selecionada)

	else
		al_draw_bitmap(botao_config, LARGURA_MAX / 2.8, ALTURA_MAX / 1.85, NULL); // Cria o "botão" para labirinto (se não for a opção selecionada)

	al_draw_text(fonte_menu, al_map_rgb(0, 0, 0), LARGURA_MAX / 2.35, ALTURA_MAX / 1.85, ALLEGRO_ALIGN_CENTRE, "Nao");
	//
	// Fim das opções de labirinto
	//

	al_destroy_bitmap(botao_config); // Finaliza a imagem botao config

	al_destroy_bitmap(botao_config_selec); // Finaliza a imagem botao config
}

//------------------------------------------------------------------------------------------------------------------------------
// Nome: imprime_matriz
// Desc: A função desenha a matriz em um espaço de 600x300
//		 Essa função também é a responsável pela atualização da matriz a cada vez que o usuário entrar com a célula correspondente
//------------------------------------------------------------------------------------------------------------------------------
void imprime_matriz(int **matriz, int dificuldade)
{
	// Esse vetor de (char *) foi criado para diminuir a quantidade de código (basicamente: if) da função. O vetor depende da dificuldade 
	char *tamanho_campo_matriz[5] = {"images/campo vazio da matriz 60x30.bmp",
								     "images/campo vazio da matriz 30x15.bmp",
									 "images/campo vazio da matriz 20x10.bmp",
									 "images/campo vazio da matriz 15x8.bmp",
									 "images/campo vazio da matriz 12x6.bmp"};

	// Esse vetor de (char *) foi criado para diminuir a quantidade de código (basicamente: if) da função. O vetor depende da dificuldade 
	char *tamanho_campo_parede[5] = {"images/campo parede da matriz 60x30.bmp",
								     "images/campo parede da matriz 30x15.bmp",
									 "images/campo parede da matriz 20x10.bmp",
									 "images/campo parede da matriz 15x8.bmp",
									 "images/campo parede da matriz 12x6.bmp"};

	// Esse vetor de (char *) foi criado para diminuir a quantidade de código (basicamente: if) da função. O vetor depende da dificuldade 
	char *arquivo_campo_posicao_inicial[5] = {"images/campo posicao inicial da matriz 60x30.bmp",
											 "images/campo posicao inicial da matriz 30x15.bmp",
											 "images/campo posicao inicial da matriz 20x10.bmp",
											 "images/campo posicao inicial da matriz 15x8.bmp",
											 "images/campo posicao inicial da matriz 12x6.bmp"};

	// Esse vetor de (char *) foi criado para diminuir a quantidade de código (basicamente: if) da função. O vetor depende da dificuldade 
	char *arquivo_campo_posicao_final[5] = {"images/campo posicao final da matriz 60x30.bmp",
											 "images/campo posicao final da matriz 30x15.bmp",
											 "images/campo posicao final da matriz 20x10.bmp",
											 "images/campo posicao final da matriz 15x8.bmp",
											 "images/campo posicao final da matriz 12x6.bmp"};

	// Esse vetor de (char *) foi criado para diminuir a quantidade de código (basicamente: if) da função. O vetor depende da dificuldade
	char *arquivo_campo_obstaculo[5] = {"images/campo obstaculo da matriz 60x30.bmp",
										"images/campo obstaculo da matriz 30x15.bmp",
										"images/campo obstaculo da matriz 20x10.bmp",
										"images/campo obstaculo da matriz 15x8.bmp",
										"images/campo obstaculo da matriz 12x6.bmp"};

	// Esse vetor de (char *) foi criado para diminuir a quantidade de código (basicamente: if) da função. O vetor depende da dificuldade
	char *arquivo_campo_alvo[5] = {"images/campo alvo da matriz 60x30.bmp",
										"images/campo alvo da matriz 30x15.bmp",
										"images/campo alvo da matriz 20x10.bmp",
										"images/campo alvo da matriz 15x8.bmp",
										"images/campo alvo da matriz 12x6.bmp"};

	// Esse vetor de (char *) foi criado para diminuir a quantidade de código (basicamente: if) da função. O vetor depende da dificuldade
	char *arquivo_campo_bonus[5] = {"images/campo bonus da matriz 60x30.bmp",
									"images/campo bonus da matriz 30x15.bmp",
									"images/campo bonus da matriz 20x10.bmp",
									"images/campo bonus da matriz 15x8.bmp",
									"images/campo bonus da matriz 12x6.bmp"};

	char *arquivo_caminho_percorrido[5] = {"images/campo caminho percorrido da matriz 60x30.bmp",
										   "images/campo caminho percorrido da matriz 30x15.bmp",
										   "images/campo caminho percorrido da matriz 20x10.bmp",
										   "images/campo caminho percorrido da matriz 15x8.bmp",
										   "images/campo caminho percorrido da matriz 12x6.bmp"};

	ALLEGRO_BITMAP *campo_matriz_vazio = al_load_bitmap(tamanho_campo_matriz[dificuldade - 1]); // Cria o BITMAP campo_matriz_vazio

	ALLEGRO_BITMAP *campo_matriz_parede = al_load_bitmap(tamanho_campo_parede[dificuldade - 1]); // Cria o BITMAP campo_matriz_parede

	ALLEGRO_BITMAP *campo_matriz_posicao_inicial = al_load_bitmap(arquivo_campo_posicao_inicial[dificuldade - 1]); // Cria o BITMAP campo_matriz_posicao_inicial

	ALLEGRO_BITMAP *campo_matriz_posicao_final = al_load_bitmap(arquivo_campo_posicao_final[dificuldade - 1]); // Cria o BITMAP campo_matriz_posicao_final

	ALLEGRO_BITMAP *campo_matriz_obstaculo = al_load_bitmap(arquivo_campo_obstaculo[dificuldade - 1]); // Cria o BITMAP campo_matriz_obstaculo

	ALLEGRO_BITMAP *campo_matriz_alvo = al_load_bitmap(arquivo_campo_alvo[dificuldade - 1]); // Cria o BITMAP campo_matriz_alvo

	ALLEGRO_BITMAP *campo_matriz_bonus = al_load_bitmap(arquivo_campo_bonus[dificuldade - 1]); // Cria o BITMAP campo_matriz_bonus

	ALLEGRO_BITMAP *campo_caminho_percorrido = al_load_bitmap(arquivo_caminho_percorrido[dificuldade - 1]); // Cria o BITMAP campo_matriz_bonus

	int tamanho_matriz = (dificuldade * 10); // Variável criada apenas para melhorar a visualização do código

	// Os seguintes for realizam o preenchimento da matriz conforme o valor numérico de cada célula
	for (int linha = 0; linha < tamanho_matriz; linha++)
	{
		for (int coluna = 0; coluna < tamanho_matriz; coluna++)
		{
			if (matriz[linha][coluna] == CAMPO_VAZIO)
				al_draw_bitmap(campo_matriz_vazio, coluna * (LARGURA_MAX / tamanho_matriz), linha * ( (ALTURA_MAX - 100) / tamanho_matriz), NULL);

			else if (matriz[linha][coluna] == PAREDE)
				al_draw_bitmap(campo_matriz_parede, coluna * (LARGURA_MAX / tamanho_matriz), linha * ( (ALTURA_MAX - 100) / tamanho_matriz), NULL);

			else if (matriz[linha][coluna] == OBSTACULO)
				al_draw_bitmap(campo_matriz_obstaculo, coluna * (LARGURA_MAX / tamanho_matriz), linha * ( (ALTURA_MAX - 100) / tamanho_matriz), NULL);

			else if (matriz[linha][coluna] == ALVO)
				al_draw_bitmap(campo_matriz_alvo, coluna * (LARGURA_MAX / tamanho_matriz), linha * ( (ALTURA_MAX - 100) / tamanho_matriz), NULL);

			else if (matriz[linha][coluna] == POS_INICIAL)
				al_draw_bitmap(campo_matriz_posicao_inicial, coluna * (LARGURA_MAX / tamanho_matriz), linha * ( (ALTURA_MAX - 100) / tamanho_matriz), NULL);

			else if (matriz[linha][coluna] == BONUS)
				al_draw_bitmap(campo_matriz_bonus, coluna * (LARGURA_MAX / tamanho_matriz), linha * ((ALTURA_MAX - 100) / tamanho_matriz), NULL);

			else if (matriz[linha][coluna] == POS_FINAL)
				al_draw_bitmap(campo_matriz_posicao_final, coluna * (LARGURA_MAX / tamanho_matriz), linha * ( (ALTURA_MAX - 100) / tamanho_matriz), NULL);

			else if (matriz[linha][coluna] == CAMINHO)
				al_draw_bitmap(campo_caminho_percorrido, coluna * (LARGURA_MAX / tamanho_matriz), linha * ( (ALTURA_MAX - 100) / tamanho_matriz), NULL);
		}
	}

	al_destroy_bitmap(campo_matriz_vazio); // finaliza o BITMAP campo_matriz_vazio para não ocupar espaço na memória

	al_destroy_bitmap(campo_matriz_parede); // finaliza o BITMAP campo_matriz_parede para não ocupar espaço na memória

	al_destroy_bitmap(campo_matriz_posicao_inicial); // finaliza o BITMAP campo_matriz_posicao_inicial para não ocupar espaço na memória

	al_destroy_bitmap(campo_matriz_posicao_final); // finaliza o BITMAP campo_matriz_posicao_inicial para não ocupar espaço na memória

	al_destroy_bitmap(campo_matriz_obstaculo); // finaliza o BITMAP campo_matriz_obstaculo para não ocupar espaço na memória

	al_destroy_bitmap(campo_matriz_alvo); // finaliza o BITMAP campo_matriz_alvo para não ocupar espaço na memória

	al_destroy_bitmap(campo_matriz_bonus); // finaliza o BITMAP campo_matriz_bonus para não ocupar espaço na memória

	al_destroy_bitmap(campo_caminho_percorrido); // finaliza o BITMAP campo_caminho_percorrido para não ocupar espaço na memória
}