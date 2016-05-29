/*
* main.cpp
* Projeto Final
*
* Created by Leonardo Winter Pereira & André Eleutério
* All Rights reserved
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <conio.h>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "alocacao_dinamica.h"
#include "funcoes_para_escrever.h"
#include "labirinto.h"
#include "pathfinder.h"

#define LARGURA_MAX 600 // define a largura máxima
#define ALTURA_MAX 400 // define a altura máxima
#define CAMPO_VAZIO 0 // define o valor do campo vazio
#define PAREDE -1 // define o valor da parede
#define OBSTACULO -2 // define o valor do obstáculo
#define POS_INICIAL 3 // define o valor da posição inicial
#define BONUS 4 // define o valor do bônus
#define POS_FINAL 5 // define o valor da posição final
#define ALVO 8 // define o valor do alvo
#define CAMINHO 6 // define o valor do caminho
 
// -------------------------
// Funções por prototipação
// -------------------------
int init();

int deinit();

int menu();

void inicializa_evento();

int jogar();

void config();

void sobre();

void escreve_jogar(int **matriz);
// ----------------------------------
//  Fim das funções por prototipação
// ----------------------------------

bool fechar = false; // variável global do programa - é ela que fecha todo o programa caso de algum erro ou seja solicitado

// As seguintes variáveis globais foram definidas (mas se for melhor podem ser reescritas como locais), elas determinam as configurações do jogo 
bool labirinto = false;

bool obstaculo = true;

int dificuldade = 1; 
// Fim das variáveis globais de configuração

int pos_x_mouse; // variável global do programa - coordenada x do mouse

int pos_y_mouse; // variável global do programa - coordenada y do mouse

ALLEGRO_DISPLAY *janela_principal = NULL; // Variável representando a janela principal

ALLEGRO_FONT *fonte_menu = NULL; // Váriavel grobal referente à fonte principal do programa

ALLEGRO_EVENT_QUEUE *eventos = NULL; // Evento para os eventos

ALLEGRO_EVENT ev; // Cria um evento do Allegro

//-----------------------------------------------------------------------------------------------
// Nome: main
// Desc: Essa é a principal função do programa, é ela que define a ordem de execução das funções
//-----------------------------------------------------------------------------------------------
int main()
{
	init(); // Função para inicializar as bibliotecas

	menu(); // Função geradora do menu

	deinit(); // Função que finaliza o programa

    return 0;
}

//-----------------------------------------------------------------------------------------------
// Nome: init
// Desc: Foi criada uma função somente para inicialização dos principais componentes
//		 Dessa forma, fica mais fácil a identificação de problemas na declaração de:
//		 janelas
//		 imagens
//		 fontes
//----------------------------------------------------------------------------------------------
int init()
{
	// Inicialização da biblioteca Allegro
	if (al_init() == NULL)
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
		return -1;
    }

	janela_principal = al_create_display(LARGURA_MAX, ALTURA_MAX); // Criamos a nossa janela - dimensões de 600*400 px

	// Criação da janela
	if (janela_principal == NULL)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
		return -1;
    }

	// Inicialização dos addons de imagens
	if (al_init_image_addon() == NULL)
    {
        fprintf(stderr, "Falha ao inicializar addon de Imagens.\n");
        return -1;
    }

	// Inicialização do teclado
	if (al_install_keyboard() == NULL)
    {
        fprintf(stderr, "Falha ao inicializar teclado.\n");
        return -1;
    }

	// Inicialização dos addons primitivos
	if (al_init_primitives_addon() == NULL)
    {
        fprintf(stderr, "Falha ao inicializar Addon primitivos.\n");
        return -1;
    }

	// Inicialização do mouse
	if (al_install_mouse() == NULL)
	{
        fprintf(stderr, "Falha ao inicializar Mouse.\n");
        return -1;
    }

	// Inicialização do add-on para uso de fontes
	al_init_font_addon(); 

	// Inicialização do add-on para uso de fontes True Type
    if (al_init_ttf_addon() == NULL)
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
		return -1;
    }

	fonte_menu = al_load_font("fonts/ALGER.TTF", 18, 0); // Carregando o arquivo de fonte

	// Retorna erro caso o arquivo de fonte não seja encontrado
	if (fonte_menu == NULL)
    {
        al_destroy_display(janela_principal);
		fprintf(stderr, "Falha ao carregar fonte ALGER.\n");
		return -1;
    }

	eventos = al_create_event_queue(); // Criamos o nosso evento para o teclado

	// Retorna erro caso ocorra algum problema ao criar a fila de eventos
    if (eventos == NULL)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela_principal);
        return -1;
    }

	al_set_window_title(janela_principal, "Projeto Final"); // Definimos o título do programa

	return 0;
}

//-----------------------------------------------------------------------------------------------
// Nome: deinit
// Desc: Foi criado uma função para fechar (destruir) as:
//		 imagens
//		 fontes
//		 janelas
//		 No final do programa, afim de o mesmo não ficar utilizando memória
//---------------------------------------------------------------------------------------------
int deinit()
{
	al_destroy_font(fonte_menu); // Finaliza a fonte

	al_destroy_event_queue(eventos); // Finaliza os eventos

	al_destroy_display(janela_principal); // Finaliza a janela

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// Nome: inicializa_evento
// Desc: Função utilizada somente para diminuir código e para o mesmo ficar mais clarom, pois é utilizado várias vezes na função escreve_jogar
//-----------------------------------------------------------------------------------------------------------------------------------------
void inicializa_evento()
{
	al_wait_for_event(eventos, &ev); // O programa espera pelo evento

	// Fecha o programa caso seja selecionado tal opção na janela
	if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
		fechar = true;

	// Vamos colocar as coordenadas do mouse em variáveis
	if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		pos_x_mouse = ev.mouse.x;

		pos_y_mouse = ev.mouse.y;
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------
// Nome: menu
// Desc: A principal sub-função do programa. É ela que cria a primeira janela do programa, assim como chama todas as outras sub-funções
//-----------------------------------------------------------------------------------------------------------------------------------
int menu()
{
	ALLEGRO_BITMAP *imagem_principal = al_load_bitmap("images/imagem_principal.jpg"); // Carrega arquivo imagem_principal.jpg

	// Retorna erro caso a imagem não seja encontrada
	if (imagem_principal == NULL)
    {
        al_destroy_display(janela_principal);
		fprintf(stderr, "Falha ao carregar imagem imagem_principal.jpg\n");
		return -1;
    }

	// Esse vetor de (char *) foi criado para diminuir a quantidade de código (basicamente: if) da função. O vetor depende da dificuldade 
	char *opcoes[4] = {"Jogar",
					   "Config.",
					   "Sobre",
					   "Sair"};

	int opcao = 0; // variável utilizada para controle das opções

	al_register_event_source(eventos, al_get_keyboard_event_source()); // Cria o evento para o teclado

	al_register_event_source(eventos, al_get_mouse_event_source()); // Cria o evento para o Mouse

	al_register_event_source(eventos, al_get_display_event_source(janela_principal)); // Cria eventos para a janela principal

	// O próximo bloco de comandos será realizado até que a variável fechar seja igual a true
	do
	{
		al_draw_bitmap(imagem_principal, 0, 0, NULL); // Desenha a imagem de fundo

		escreve_opcoes(opcoes, opcao, fonte_menu); // Escreve e reescreve as opções cada vez que o usuário aperta uma seta direcional do teclado

		al_flip_display(); // Atualiza a tela

		inicializa_evento(); // Função utilizada para melhorar o programa em termos de visualização de código

		if(ev.type == ALLEGRO_EVENT_KEY_DOWN) // Seta direcional selecionada pelo usuário
		{
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_UP: // caso seja selecionada a seta direcional para cima, a variável opção muda de valor, para que o texto da opção mude de cor
					if(opcao == 0)
						opcao = 3;

					else if(opcao == 1)
						opcao = 0;

					else if(opcao == 2)
						opcao = 1;
					
					else
						opcao = 2;
					
					break;

				case ALLEGRO_KEY_DOWN: // caso seja selecionada a seta direcional para baixo, a variável opção muda de valor, para que o texto da opção mude de cor
					if(opcao == 0)
						opcao = 1;

					else if(opcao == 1)
						opcao = 2;

					else if(opcao == 2)
						opcao = 3;
					
					else
						opcao = 0;
					
					break;

				case ALLEGRO_KEY_ENTER: // Caso seja selecionada a tecla ENTER, o programa entra na opção correspondente
					switch (opcao)
					{
					case 0:
						jogar(); // Executa o jogo
						break;

					case 1:
						config(); // Entra no menu de Opções
						break;

					case 2:
						sobre(); // Entra no menu Sobre
						break;

					case 3:
						fechar = true; // Fecha o programa
						break;
					}
					al_set_window_title(janela_principal, "Projeto Final"); // Definimos o título do programa
					
					break;
			}
		}
	} while(!fechar);

	al_destroy_bitmap(imagem_principal); // Finaliza a imagem principal

	return 0;
}

//----------------------------------------------
// Nome: jogar
// Desc: Realiza os principais comandos do jogo
//----------------------------------------------
int jogar()
{
	ALLEGRO_BITMAP *botao_ok = al_load_bitmap("images/ok button.jpg"); // Cria e carrega arquivo ok button.jpg

	ALLEGRO_BITMAP *botao_start = al_load_bitmap("images/start button.jpg"); // Cria e carrega arquivo start button.jpg

	int **matriz; // Para alocação dinâmica de matrizes deve-se criar um ponteiro para ponteiro

	int tamanho_matriz = (dificuldade * 10); // Variável criada apenas para melhoria do código

	al_clear_to_color(al_map_rgb(255, 255, 255)); // limpa a tela e a pinta toda de branco

	al_draw_bitmap(botao_ok, LARGURA_MAX / 1.12, ALTURA_MAX / 1.07, NULL); // Desenha o botão voltar, necessário para poder continuar o programa sem precisar sair

	al_draw_bitmap(botao_start, LARGURA_MAX / 1.12, ALTURA_MAX / 1.31, NULL); // Desenha o botão voltar, necessário para poder continuar o programa sem precisar sair

	matriz = aloca_matriz(tamanho_matriz); // Chama a função para Alocar a matriz
	
	imprime_matriz(matriz, dificuldade); // Chama a função para desenhar a matriz na tela

	escreve_jogar(matriz); // É esta a função que pede para o usuário entrar com os dados do jogo, assim como atualizar a matriz desenhada

	libera_matriz(matriz, tamanho_matriz); // Chama a função para liberar a memória ocupada pela matriz

	al_destroy_bitmap(botao_ok); // finaliza o BITMAP botao_ok para não ocupar espaço na memória

	al_destroy_bitmap(botao_start); // finaliza o BITMAP botao_ok para não ocupar espaço na memória

	return 0;
}

//----------------------------------------------
// Nome: config
// Desc: Esta função determina as regras do jogo
//----------------------------------------------
void config()
{
	ALLEGRO_BITMAP *botao_ok = al_load_bitmap("images/ok button.jpg"); // Cria e carrega arquivo ok button.jpg

	al_set_window_title(janela_principal, "Configurações"); // Definimos o título do programa

	al_clear_to_color(al_map_rgb(255, 255, 255)); // Limpa a tela e pinta toda de branco

	al_draw_bitmap(botao_ok, LARGURA_MAX / 1.12, ALTURA_MAX / 1.07, NULL); // Desenha o botão voltar, necessário para poder continuar o programa sem precisar sair

	// O próximo bloco de comandos será realizado até que a variável fechar seja igual a true
	do
	{
		escreve_config(fonte_menu, dificuldade, obstaculo, labirinto); // Utilizado essa função fica mais simples o código, se fosse escrito tudo direto na função config ficaria confuso e grande

		al_flip_display(); // Atualiza a tela

		inicializa_evento(); // Função utilizada para melhorar o programa em termos de visualização de código

		if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) // De acordo com o botão do mouse
		{
			if(ev.mouse.button & 1)
			{
				//
				// Os seguintes comandos estão relacionados às dificuldades
				//
				if( (pos_x_mouse >= 153) && (pos_x_mouse <= 166) && (pos_y_mouse >= 55) && (pos_y_mouse <= 65) )
					dificuldade = 1; // Caso seja selecionada a dificuldade 1

				else if( (pos_x_mouse >= 217) && (pos_x_mouse <= 230) && (pos_y_mouse >= 55) && (pos_y_mouse <= 65) )
					dificuldade = 2; // Caso seja selecionada a dificuldade 2

				else if( (pos_x_mouse >= 289) && (pos_x_mouse <= 302) && (pos_y_mouse >= 55) && (pos_y_mouse <= 65) )
					dificuldade = 3; // Caso seja selecionada a dificuldade 3

				else if( (pos_x_mouse >= 367) && (pos_x_mouse <= 380) && (pos_y_mouse >= 55) && (pos_y_mouse <= 65) )
					dificuldade = 4; // Caso seja selecionada a dificuldade 4

				else if( (pos_x_mouse >= 447) && (pos_x_mouse <= 460) && (pos_y_mouse >= 55) && (pos_y_mouse <= 65) )
					dificuldade = 5; // Caso seja selecionada a dificuldade 5
				//
				// Fim dos comandos relacionados às dificuldades
				//

				//
				// Os seguintes comandos estão relacionados à existência ou não de obstáculos
				//
				else if( (pos_x_mouse >= 153) && (pos_x_mouse <= 166) && (pos_y_mouse >= 137) && (pos_y_mouse <= 149) )
					obstaculo = true; // Caso seja selecionada a opção de existir obstáculos

				else if( (pos_x_mouse >= 218) && (pos_x_mouse <= 230) && (pos_y_mouse >= 137) && (pos_y_mouse <= 149) )
					obstaculo = false; // Caso seja selecionada a opção de não existir obstáculos
				//
				// Fim dos comandos relacionados à existência ou não de obstáculos
				//

				//
				// Os seguintes comandos estão relacionados à existência ou não de labirinto
				//
				else if( (pos_x_mouse >= 153) && (pos_x_mouse <= 166) && (pos_y_mouse >= 220) && (pos_y_mouse <= 232) )
					labirinto = true; // Caso seja selecionada a opção de possuir labirinto
			

				else if( (pos_x_mouse >= 218) && (pos_x_mouse <= 230) && (pos_y_mouse >= 220) && (pos_y_mouse <= 232) )
					labirinto = false; // Caso seja selecionada a opção de não possuir labirinto
				//
				// Fim dos comandos relacionados à existência ou não de labirinto
				//

				else if( (pos_x_mouse >= 536) && (pos_x_mouse <= 595) && (pos_y_mouse >= 374) && (pos_y_mouse <= 393) )
					break; // Volta para o menu principal caso seja selecionado o botão "voltar"
			}
		}
	} while(!fechar);

	al_destroy_bitmap(botao_ok); // Finaliza a imagem do botão ok
}

//-----------------------------------------------------------------------------------------------------
// Nome: sobre
// Desc: Nada mais é que a função para mostrar os responsáveis pela criação do jogo e outras informações.
//       Foram utilizadas imagens para mostrar o conteúdo
//-----------------------------------------------------------------------------------------------------
void sobre()
{
	ALLEGRO_BITMAP *imagem_sobre = al_load_bitmap("images/about.jpg"); // Cria e carrega arquivo imagem_principal.jpg

	ALLEGRO_BITMAP *botao_ok = al_load_bitmap("images/ok button.jpg"); // Cria e carrega arquivo ok button.jpg

	al_set_window_title(janela_principal, "Sobre Projeto Final"); // Definimos o título do programa

	al_draw_bitmap(imagem_sobre, 0, 0, NULL); // A imagem sobre o programa (a ser definida)

	al_draw_bitmap(botao_ok, LARGURA_MAX / 1.12, ALTURA_MAX / 1.07, NULL); // O botão "voltar", para retornar ao menu principal

	al_flip_display(); // Atualiza a tela

	// O próximo bloco de comandos será realizado até que a variável fechar seja igual a true
	do
	{
		inicializa_evento(); // Função utilizada para melhorar o programa em termos de visualização de código

		if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button & 1)
			{
				if( (pos_x_mouse >= 536) && (pos_x_mouse <= 595) && (pos_y_mouse >= 374) && (pos_y_mouse <= 393) )
					break; // Voltar para o menu principal
			}
			
		}
	} while(!fechar);

	al_destroy_bitmap(imagem_sobre); // Finaliza a imagem sobre

	al_destroy_bitmap(botao_ok); // Finaliza o botão ok
}

//----------------------------------------------
// Nome: escreve_jogar
// Desc: Esta função pede ao usuário as posições de tudo que é necessário para a execução do jogo
//----------------------------------------------
void escreve_jogar(int **matriz)
{
	// Esse vetor de (char *) foi criado para diminuir a quantidade de código (basicamente: if) da função. O vetor depende da dificuldade 
	char *texto_inicial[5] = {"A dificuldade 1 consiste em escolher:",
							  "A dificuldade 2 consiste em escolher:",
							  "A dificuldade 3 consiste em escolher:",
							  "A dificuldade 4 consiste em escolher:",
							  "A dificuldade 5 consiste em escolher:"};

	// Esse vetor de (char *) foi criado para diminuir a quantidade de código (basicamente: if) da função. O vetor depende da dificuldade 
	char *bonus = {"> O Bonus"};

	ALLEGRO_FONT *fonte_jogar = al_load_font("fonts/AGENCYB.TTF", 14, 0); // Carrega o arquivo de fonte

	bool parar_obstaculos = false;

	// A seguinte variável foi criada para facilitar a leitura do código do programa
	int tamanho_matriz = dificuldade * 10;

	//
	// As seguintes variáveis foram criadas para converter as coordenadas do mouse e armazenar as  nas células da matriz
	//
	int pos_x;

	int pos_y;

	int linha_inicial;

	int coluna_inicial;

	int linha_final;

	int coluna_final;

	int linha_alvo;

	int coluna_alvo;

	int linha_obstaculo;

	int coluna_obstaculo;

	int linha_bonus;

	int coluna_bonus;

	int custo_total;

	int custo_aux;

	int custo_aux2;
	//
	// Fim da declaração das variáveis
	//

	al_draw_text(fonte_jogar, al_map_rgb(0, 0, 0), LARGURA_MAX / 2, 310, ALLEGRO_ALIGN_CENTRE, texto_inicial[dificuldade - 1]); // Escreve o texto central

	//
	// As seguintes funções escrevem no espaço determinado o que tem que ser escolhido pelo usuário
	//
	al_draw_text(fonte_jogar, al_map_rgb(0, 0, 0), 20, 340, NULL, "> A Posicao Inicial");

	al_draw_text(fonte_jogar, al_map_rgb(0, 0, 0), 20, 375, NULL, "> A Posicao Final");

	al_draw_text(fonte_jogar, al_map_rgb(0, 0, 0), 250, 340, NULL, "> Obstaculo (ENTER para cancelar)");

	al_draw_text(fonte_jogar, al_map_rgb(0, 0, 0), 250, 375, NULL, bonus);
	//
	// Fim das funções  que escrevem as escolhas necessárias
	//

	al_flip_display(); // Atualiza a tela
	
	// Caso a opção de existir labirinto seja verdadeira
	if (labirinto == true)
	{
		matriz = carrega_labirinto(dificuldade); // Chama a função responsável por gerar, aleatoriamente, um labirinto perfeito 

		imprime_matriz(matriz, dificuldade); // Imprime o labirinto no espaço reservado

		al_flip_display(); // Atualiza tela
	}

	//
	// ----------------------------------------------------------------------------------------
	//  O seguinte do while é utilizado para que o usuário entre com a posição inicial do jogo
	// ----------------------------------------------------------------------------------------
	//

	// O seguinte bloco de comando é executado enquanto o usuário não digita uma posição inicial válida
	do
	{
		pos_x = 0; // Zera a variável

		pos_y = 0; // Zera a variável

		al_draw_text(fonte_jogar, al_map_rgb(255, 0, 0), 20, 340, NULL, "> A Posicao Inicial"); // Pinta o texto para melhor visualização

		al_flip_display(); // Atualiza a tela

		inicializa_evento(); // Função utilizada para melhorar o programa em termos de visualização de código

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) // Ativa quando o usuário clicar com algum botão do mouse
		{
			if (ev.mouse.button & 1)
			{
				// quando o usuário clicar com o botão esquerdo do mouse as coordenadas serão armazenadas em variáveis
				pos_x = pos_x_mouse;

				pos_y = pos_y_mouse;

				if (pos_y <= 300)
				{
					coluna_inicial = (pos_x * tamanho_matriz) / 600; // Converte a coordenada do mouse em coluna

					// Precisa ser verificado o nível de dificuldade 4, pois o tamanho correto seria 15x7.5 px, mas não é possível
					if (dificuldade == 4)
						linha_inicial = (pos_y * tamanho_matriz ) / 280; // Converte a coordenada do mouse em linha

					else
						linha_inicial = (pos_y * tamanho_matriz ) / 300; // Converte a coordenada do mouse em linha

					// Determina a posição inicial e atualiza a matriz
					if (matriz[linha_inicial][coluna_inicial] != PAREDE)
					{
						matriz[linha_inicial][coluna_inicial] = POS_INICIAL;

						imprime_matriz(matriz, dificuldade);
					}
				}
			}
		}
	} while ( (pos_y >= 300) || (pos_y == 0) || (matriz[linha_inicial][coluna_inicial] == PAREDE) );

	//
	// --------------------------------------------------------------------------------------
	//  O seguinte do while é utilizado para que o usuário entre com a posição final do jogo
	// --------------------------------------------------------------------------------------
	//

	// O seguinte bloco de comando é executado enquanto o usuário não digita uma posição final válida
	do
	{
		pos_x = 0; // Zera variável

		pos_y = 0; // Zera variável

		al_draw_text(fonte_jogar, al_map_rgb(0, 0, 255), 20, 375, NULL, "> A Posicao Final"); // Pinta o texto para melhor visualização

		al_flip_display(); // Atualiza tela

		inicializa_evento(); // Função utilizada para melhorar o programa em termos de visualização de código

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) // Ativa quando o usuário clicar com algum botão do mouse
		{
			if (ev.mouse.button & 1)
			{
				// quando o usuário clicar com o borão esquerdo do mouse as coordenadas serão armazenadas em variáveis
				pos_x = pos_x_mouse;

				pos_y = pos_y_mouse;

				if (pos_y <= 300)
				{
					coluna_final = (pos_x * tamanho_matriz ) / 600; // Converte a coordenada do mouse em coluna

					// Precisa ser verificado o nível de dificuldade 4, pois o tamanho correto seria 15x7.5 px, mas não é possível
					if (dificuldade == 4)
						linha_final = (pos_y * tamanho_matriz ) / 280; // Converte a coordenada do mouse em linha

					else
						linha_final = (pos_y * tamanho_matriz) / 300; // Converte a coordenada do mouse em linha

					// Determina a posição final e atualiza a matriz
					if ( ( (linha_final != linha_inicial) || (coluna_final != coluna_inicial) ) && (matriz[linha_final][coluna_final] != PAREDE) )
					{
						matriz[linha_final][coluna_final] = POS_FINAL;

						imprime_matriz(matriz, dificuldade);
					}
				}
			}
		}
	} while ( (pos_y >= 300) || (pos_y == 0) || ( (linha_final == linha_inicial) && (coluna_final == coluna_inicial) ) || (matriz[linha_final][coluna_final] == PAREDE) );

		//
	// ----------------------------------------------------------------------------------------
	//  O seguinte do while é utilizado para que o usuário entre com a posição do alvo do jogo
	// ----------------------------------------------------------------------------------------
	//

	// O seguinte bloco de comando é executado enquanto o usuário não digita uma posição válida
	do
	{
		pos_x = 0; // Zera variável

		pos_y = 0; // Zera variável

		//*******al_draw_text(fonte_jogar, al_map_rgb(0, 0, 255), 20, 375, NULL, "> A Posicao Final"); // Pinta o texto para melhor visualização

		al_flip_display(); // Atualiza tela

		inicializa_evento(); // Função utilizada para melhorar o programa em termos de visualização de código

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) // Ativa quando o usuário clicar com algum botão do mouse
		{
			if (ev.mouse.button & 1)
			{
				// quando o usuário clicar com o borão esquerdo do mouse as coordenadas serão armazenadas em variáveis
				pos_x = pos_x_mouse;

				pos_y = pos_y_mouse;

				if (pos_y <= 300)
				{
					coluna_alvo = (pos_x * tamanho_matriz ) / 600; // Converte a coordenada do mouse em coluna

					// Precisa ser verificado o nível de dificuldade 4, pois o tamanho correto seria 15x7.5 px, mas não é possível
					if (dificuldade == 4)
						linha_alvo = (pos_y * tamanho_matriz ) / 280; // Converte a coordenada do mouse em linha

					else
						linha_alvo = (pos_y * tamanho_matriz) / 300; // Converte a coordenada do mouse em linha

					// Determina a posição do alvo e atualiza a matriz
					if ( ( (linha_alvo != linha_inicial) || (coluna_alvo != coluna_inicial) ) || ( (linha_alvo != linha_final) || (coluna_alvo != coluna_final) ) && (matriz[linha_alvo][coluna_alvo] != PAREDE) )
					{
						matriz[linha_alvo][coluna_alvo] = ALVO;

						imprime_matriz(matriz, dificuldade);
					}
				}
			}
		}
	} while ( (pos_y >= 300) || (pos_y == 0) || ( (linha_alvo == linha_inicial) && (coluna_alvo == coluna_inicial) ) || ( (linha_alvo == linha_final) && (coluna_alvo == coluna_final) ) || (matriz[linha_alvo][coluna_alvo] == PAREDE) );

	//
	// ------------------------------------------------------------------------------------------
	//  O seguinte for é utilizado para que o usuário entre com a posição dos obstáculos do jogo
	// ------------------------------------------------------------------------------------------
	//

	// O seguinte bloco de comando será executado até que o usuário digite ENTER (para cancelar a entrada de obstáculos)
	do
	{
		pos_x = 0; // Zera variável

		pos_y = 0; // Zera variável

		al_draw_text(fonte_jogar, al_map_rgb(255, 165, 0), 250, 340, NULL, "> Obstaculo (ENTER para cancelar)"); // Pinta o texto para melhor visualização

		al_flip_display(); // Atualiza tela

		// O seguinte bloco de comando só será executado caso a opção obstaculo esteja habilitada
		if (obstaculo == true)
		{
			// O seguinte bloco de comando é executado enquanto o usuário não digita uma posição válida para o obstáculo
			do
			{
				inicializa_evento(); // Função utilizada para melhorar o programa em termos de visualização de código

				// O seguinte bloco de comando será executado caso o usuário entre com a tecla ENTER
				if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					switch(ev.keyboard.keycode)
					{
					case ALLEGRO_KEY_ENTER:
						parar_obstaculos = true;

						break;
					}

					// O seguinte break é necessário para que, ao apertar a tecla ENTER, o usuário não precise entrar com mais um obstáculo
					break;
				}

				if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) // Ativa quando o usuário clicar com algum botão do mouse
				{
					if (ev.mouse.button & 1)
					{
						// quando o usuário clicar com o botão esquerdo do mouse as coordenadas serão armazenadas em variáveis
						pos_x = pos_x_mouse;

						pos_y = pos_y_mouse;
					}

					if (pos_y <= 300)
					{
						coluna_obstaculo = (pos_x * tamanho_matriz) / 600; // Converte a coordenada do mouse em coluna

						// Precisa ser verificado o nível de dificuldade 4, pois o tamanho correto seria 15x7.5 px, mas não é possível
						if (dificuldade == 4)
							linha_obstaculo = (pos_y * tamanho_matriz) / 280; // Converte a coordenada do mouse em linha

						else
							linha_obstaculo = (pos_y * tamanho_matriz) / 300; // Converte a coordenada do mouse em linha

						// Determina a posição dos obstáculos e atualiza a matriz
						if ( ( ( (linha_obstaculo != linha_inicial) || (coluna_obstaculo != coluna_inicial) ) && ( (linha_obstaculo != linha_final) || (coluna_obstaculo != coluna_final) ) && ( (linha_obstaculo != linha_alvo) || (coluna_obstaculo != coluna_alvo) ) ) && (matriz[linha_obstaculo][coluna_obstaculo] != PAREDE) )
						{
							if (matriz[linha_obstaculo][coluna_obstaculo] != OBSTACULO)
								matriz[linha_obstaculo][coluna_obstaculo] = OBSTACULO;

							else if (matriz[linha_obstaculo][coluna_obstaculo] == OBSTACULO)
								matriz[linha_obstaculo][coluna_obstaculo] = CAMPO_VAZIO;

							imprime_matriz(matriz, dificuldade);
						}
					}
				}
			} while( (pos_y >= 300) || (pos_y == 0) || ( (linha_obstaculo == linha_inicial) && (coluna_obstaculo == coluna_inicial) ) || ( (linha_obstaculo == linha_final) && (coluna_obstaculo == coluna_final) ) || (matriz[linha_obstaculo][coluna_obstaculo] == PAREDE) );
		}
		
		// Sai do bloco de comandos caso a opção obstaculo esteja desabilitada
		else
			break;

	} while (parar_obstaculos == false);

	//
	// -------------------------------------------------------------------------------------
	//  O seguinte for é utilizado para que o usuário entre com a posição dos bônus do jogo
	// -------------------------------------------------------------------------------------
	//

	
	// O seguinte bloco de comando será executado até que o usuário entre com o número correto de bônus
	do
	{
		pos_x = 0; // Zera variável

		pos_y = 0; // Zera variável

		al_draw_text(fonte_jogar, al_map_rgb(0, 186, 255), 250, 375, NULL, bonus); // Pinta o texto para melhor visualização

		al_flip_display(); // Atualiza tela

		inicializa_evento(); // Função utilizada para melhorar o programa em termos de visualização de código

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) // Ativa quando o usuário clicar com algum botão do mouse
		{
			if (ev.mouse.button & 1)
			{
				// quando o usuário clicar com o botão esquerdo do mouse as coordenadas serão armazenadas em variáveis
				pos_x = pos_x_mouse;

				pos_y = pos_y_mouse;
			}

			if (pos_y <= 300)
			{
				coluna_bonus = (pos_x * tamanho_matriz) / 600; // Converte a coordenada do mouse em coluna

				// Precisa ser verificado o nível de dificuldade 4, pois o tamanho correto seria 15x7.5 px, mas não é possível
				if (dificuldade == 4)
					linha_bonus = (pos_y * tamanho_matriz) / 280; // Converte a coordenada do mouse em linha

				else
					linha_bonus = (pos_y * tamanho_matriz) / 300; // Converte a coordenada do mouse em linha

				// Determina a posição dos bônus e atualiza a matriz
				if ( ( ( ( (linha_bonus != linha_inicial) || (coluna_bonus != coluna_inicial) ) && ( (linha_bonus != linha_final) || (coluna_bonus != coluna_final) ) && ( (linha_bonus != linha_alvo) || (coluna_bonus != coluna_alvo) ) ) && (matriz[linha_bonus][coluna_bonus] != PAREDE) ) )
				{
					matriz[linha_bonus][coluna_bonus] = BONUS;

					imprime_matriz(matriz, dificuldade);
				}
			}
		}
	} while( (pos_y >= 300) || (pos_y == 0) || ( (linha_bonus == linha_inicial) && (coluna_bonus == coluna_inicial) ) || ( (linha_bonus == linha_final) && (coluna_bonus == coluna_final) ) || ( (linha_bonus == linha_alvo) && (coluna_bonus == coluna_alvo) ) || ( (matriz[linha_bonus][coluna_bonus] == PAREDE) ) );

	// O próximo bloco de comandos será realizado até que a variável fechar seja igual a true
	do
	{
		al_flip_display(); // Atualiza a tela

		inicializa_evento(); // Função utilizada para melhorar o programa em termos de visualização de código

		if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) // Quando o usuário apertar algum botão do mouse
		{
			if((ev.mouse.button & 1) && (pos_x_mouse >= 536) && (pos_x_mouse <= 595) && (pos_y_mouse >= 374) && (pos_y_mouse <= 393))
				break; // Volta para o menu principal Caso seja selecionado o botão "VOLTAR"
			
			else if((ev.mouse.button & 1) && (pos_x_mouse >= 536) && (pos_x_mouse <= 595) && (pos_y_mouse >= 306) && (pos_y_mouse <= 325))
			{
				custo_total = procura_caminho(tamanho_matriz, matriz, linha_inicial, coluna_inicial, linha_alvo, coluna_alvo, false) + procura_caminho(tamanho_matriz, matriz, linha_alvo, coluna_alvo, linha_final, coluna_final, false); // Caso seja selecionado o botão "INICIAR"
				custo_aux = procura_caminho(tamanho_matriz, matriz, linha_inicial, coluna_inicial, linha_alvo, coluna_alvo, false) + procura_caminho(tamanho_matriz, matriz, linha_alvo, coluna_alvo, linha_bonus, coluna_bonus, false) + procura_caminho(tamanho_matriz, matriz, linha_bonus, coluna_bonus, linha_final, coluna_final, false) - (BONUS * 10 * dificuldade);
				custo_aux2 = procura_caminho(tamanho_matriz, matriz, linha_inicial, coluna_inicial, linha_bonus, coluna_bonus, false) + procura_caminho(tamanho_matriz, matriz, linha_bonus, coluna_bonus, linha_alvo, coluna_alvo, false) + procura_caminho(tamanho_matriz, matriz, linha_alvo, coluna_alvo, linha_final, coluna_final, false) - (BONUS * 10 * dificuldade);
			}
		
			if ( (custo_total <= custo_aux) && (custo_total <= custo_aux2) )
			{
				procura_caminho(tamanho_matriz, matriz, linha_alvo, coluna_alvo, linha_inicial, coluna_inicial, true);
				procura_caminho(tamanho_matriz, matriz, linha_final, coluna_final, linha_alvo, coluna_alvo, true);
			}

			else if ( (custo_aux <= custo_total) && (custo_aux <= custo_aux2) )
			{
				procura_caminho(tamanho_matriz, matriz, linha_alvo, coluna_alvo, linha_inicial, coluna_inicial, true);
				procura_caminho(tamanho_matriz, matriz, linha_bonus, coluna_bonus, linha_alvo, coluna_alvo, true);
				procura_caminho(tamanho_matriz, matriz, linha_final, coluna_final, linha_bonus, coluna_bonus, true);
			}

			else
			{
				procura_caminho(tamanho_matriz, matriz, linha_bonus, coluna_bonus, linha_inicial, coluna_inicial, true);
				procura_caminho(tamanho_matriz, matriz, linha_alvo, coluna_alvo, linha_bonus, coluna_bonus, true);
				procura_caminho(tamanho_matriz, matriz, linha_final, coluna_final, linha_alvo, coluna_alvo, true);
			}
		}
	} while(!fechar);

	al_destroy_font(fonte_jogar); // Finaliza a fonte
}