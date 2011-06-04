/*      Universidade Estadual de Montes Claros - UNIMONTES
         Descrição: Jogo da Velha Implementado
         Autora: Mivo
         Data:   28/03/2011
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <dos.h>

//=======================================================================
//           Protótipo de Funções

 void Menu( void );
 void NovaJanela( void );
 void ImprimeDesenho( char matriz [3][ 3] );
 int ResultadoGanhador ( int *jogador1Vitorias, int *jogador2Vitorias, char velha[3][3], char jogador1[], char jogador2[] );
 void SalvarDados ( int jogador1Vitorias, int jogador2Vitorias, char jogador1[], char jogador2[], FILE *GanhadoresJogoDaVelha );
 void Tutorial(void);
 void Modelo(void);
 void CarregarJogo(void);

//=======================================================================
//               Função Principal

 int main (void) {

    // Titulo
    CarregarJogo();
    system("title # Jogo da Velha Ultimate_by mivo ");
    system("color 0f");


    // Declaração de variáveis
    char velha[3][3], X, O, jogador1[20], jogador2[20];
	int i, j, escolha=0, vez=1, jogada=0, numPosicao=1, menu =0 ;
	int i1, j1,contador=1, flag=0;
    int jogador1Vitorias=0, jogador2Vitorias=0;
    int op=0, save=0;
    char c;
    FILE *GanhadoresJogoDaVelha;

    // Inicio do programa
	do{
	    // Imprime menu e armazena a opção do usuário
        Menu();
        scanf("%d", & menu); fflush(stdin);
        system("cls");

        // caso novo jogo
        if ( menu == 1){
            menu=0;

            // inicializa matriz com valores 'vazios'
            for ( i=0; i < 3; i++) {
                for ( j=0; j < 3; j++) {
                    velha[i][j] = ' ';
                }
            }

            // Armazena os dados dos jogadores
            printf("\n Jogador 1, qual o seu nome? ");
            gets(jogador1); fflush(stdin);
            printf("\n Jogador 2, qual o seu nome? ");
            gets(jogador2); fflush(stdin);

            printf("\n\n Certo, vamos jogar! ");
            printf("\n\n --\n\n ");
            NovaJanela();

            // Reinicialização de variáveis
            vez = 1;
            contador = 1;
            numPosicao = 1;

            // loop para o número total de jogadas (menores que dez, uma
            // vez que a matriz é de 3x3)
            while (contador < 10){
                system("cls");
                printf("\t\t\t");
                Modelo();
                printf("\t\t\t");
                ImprimeDesenho( velha );


                if (vez == 1) {
                    printf("\n\n %s digite sua jogada: \t",jogador1);
                }

                if (vez == 2) {
                    printf("\n %s digite sua jogada: \t",jogador2);
                }

                // Armazena a posicao da jogada
                jogada = 0;
                scanf("%d", &jogada); fflush(stdin);

                // Avaliando se as jogadas foram validas:
                if (jogada > 9 || jogada < 1) {
                    do {
                        system("cls");
                        printf("\t\t\t");
                        Modelo();
                        printf("\t\t\t");
                        ImprimeDesenho( velha );
                        printf(" Caractere invalido! \n Digite outra jogada: \t");
                        scanf("%d",&jogada); fflush(stdin);
                    } while ( (jogada > 9) || (jogada < 1) );
                }

                // Definindo a coordenada de linha da escolha
                if ((jogada == 1) || (jogada == 2) || (jogada == 3))
                    i1 = 0;
                if ((jogada == 4) || (jogada == 5) || (jogada == 6))
                    i1 = 1;
                if ((jogada == 7) || (jogada == 8) || (jogada == 9))
                    i1 = 2;

                // Definindo a coordenada de coluna da escolha
                if ((jogada == 1) || (jogada == 4) || (jogada == 7))
                    j1 = 0;
                if ((jogada == 2) || (jogada == 5) || (jogada == 8))
                    j1 = 1;
                if ((jogada == 3) || (jogada == 6) || (jogada == 9))
                    j1 = 2;

                // Preenchendo a matriz
                if (velha[i1][j1] == ' ') {
                    if (vez == 1) {
                        velha[i1][j1] = 'X';
                    }
                    else {
                        velha[i1][j1] = 'O';
                    }
                }

                // Caso o jogador determine uma coordenada ja usada anteriormente
                else if ((velha[i1][j1] == 'X') || (velha[i1][j1] == 'O')) {
					do {
					    system("cls");
                        printf("\t\t\t");
                        Modelo();
                        printf("\t\t\t");
                        ImprimeDesenho( velha );
						printf(" Posicao ocupada. \n Digite outra posicao: \t");
						scanf("%d",&jogada); fflush(stdin);

						// (OBS: optei por não usar o "goto")
						if ((jogada == 1) || (jogada == 2) || (jogada == 3))
							i1 = 0;
						if ((jogada == 4) || (jogada == 5) || (jogada == 6))
							i1 = 1;
						if ((jogada == 7) || (jogada == 8) || (jogada == 9))
							i1 = 2;
						if ((jogada == 1) || (jogada == 4) || (jogada == 7))
                            j1 = 0;
						if ((jogada == 2) || (jogada == 5) || (jogada == 8))
							j1 = 1;
                        if ((jogada == 3) || (jogada == 6) || (jogada == 9))
                            j1 = 2;

                        if (velha[i1][j1] == ' ') {
                            if (vez == 1) {
                                velha[i1][j1] = 'X';
                            }
                            else {
                                velha[i1][j1] = 'O';
                            }
                            break;
                        }
                    } while(1);
                }

                // Funçao que avalia se houve ganhadores
                flag = ResultadoGanhador( &jogador1Vitorias, &jogador2Vitorias, velha, jogador1, jogador2 );
                if ( flag == 10 ){
                    menu = 0 ;
                    NovaJanela();
                    contador=flag;
                    save=1;
                }

				// Inversao da ordem dos jogadores.
                if (vez == 1)
                    vez = 2;
                else if (vez == 2)
                    vez = 1;
                contador ++;
            }
                // Caso não haja ganhador
                if ( contador == 10 ) {
                    save=1;
				    system("cls");
					printf("\n\n\t DEU VELHA! ");
					NovaJanela();
				}
    }

//===============================
        //  caso salvar o jogo
        else if ( menu == 2 ){
            if( save==1 ){
            menu = 0;
            SalvarDados ( jogador1Vitorias, jogador2Vitorias, jogador1, jogador2, GanhadoresJogoDaVelha );
            continue;
            }
            else{
                printf("\n\n E necessario iniciar um novo jogo para salvar dados. \n\n");
                NovaJanela();
            }
        }

//====================================
        // caso visualizar pontuacoes
        else if ( menu == 3 ){
            menu = 0;
            printf("\n\n\n\t\t PONTUACOES \n\n\n");

            if( (GanhadoresJogoDaVelha = fopen("GanhadoresJogoDaVelha.txt", "r") ) != NULL){
                          while ((c = fgetc(GanhadoresJogoDaVelha) ) != EOF ){
                                putchar( c );
                          }
                          fclose(GanhadoresJogoDaVelha);
                   }
                   else{
                        printf("\t O arquivo nao pode ser aberto!\n\n");
                        printf("\t Salve suas informacoes primeiro.\n\n\n");
                   }
            NovaJanela();
            continue;
        }

//=======================================================================
        // Caso sair
        else if ( menu == 4 ){
            menu = 0;
            printf("\n\n\t\t\t Fim de Jogo ! \n\n ");
            NovaJanela();
            return(0);
        }

//========================================================================
        // Caso tutorial
        else if ( menu == 5){
            Tutorial();
            NovaJanela();
        }
//========================================================================
        // Caso reiniciar pontuações
        else if( menu ==6 ){
             printf("\n\n\n Deseja mesmo deletar as pontuacoes existentes? \n\n\t 1 [sim] \t 2 [nao] \n\t >");
             scanf("%d", &op); fflush(stdin);

            // caso um jogo já tenha sido iniciado
             if( op ==1){
                if( (remove("GanhadoresJogoDaVelha.txt" )) != 0 ){
					printf( "\nErro ao remover arquivo" );
					NovaJanela();
				}
				else{
                    printf("\n\n Arquivo removido com sucesso! \n\n");
                    NovaJanela();
				}
                continue;
             }
             // Caso contrário
            else if ( op == 2 ){
                continue;
            }
            else{
                printf("\n\n\n Opcao Invalida! \n\n\n");
            }
            NovaJanela();
             }
//==============================
        // Caso menu inválido
        else {
            menu = 0;
            system("cls");
            printf("\n\n\t MENU INVALIDO !! \n\n");
            printf("\t Escolha outra opcao. \t");
            NovaJanela();
            continue;
        }
    }while ( menu!=4 );
    return(0);
 }

//=======================================================================
//                          Corpo das Funções

// Menu Principal
void Menu(void){
    printf("\n\n\t\t Bem-Vindo (a) ao Jogo da Velha Ultimate !! \n");
    printf("\n\t\t -----------------------------------------");
    printf("\n\n\t O que deseja fazer ? ");
    printf("\n\n\t 1 > Novo Jogo \n\t 2 > Salvar Jogo \n\t 3 > Pontuacoes \n\t 4 > Sair  \n\t 5 > Tutorial \n\t 6 > Reiniciar pontuacoes \n\n\t Opcao > ");
}

// Limpar janela
void NovaJanela(void){
    printf("\n\n\t Pressione >ENTER< para continuar. ");
    getch(); fflush(stdin);
    system("cls");
}

// Desenho da matriz do jogo da velha
void ImprimeDesenho( char matriz[3][ 3] ){
    int i, j;
    printf("\n\n\t\t ");
        for (i=0; i < 3; i++) {
            printf("|");
            for (j=0; j < 3; j++) {
                printf("   %c   |", matriz[i][j]);
            }
            printf("\n\t\t ");
        }
        printf(" \n\n");
}

// Verificação se houve ganhadores
int ResultadoGanhador ( int *jogador1Vitorias, int *jogador2Vitorias, char velha[3][3], char jogador1[], char jogador2[] ){
    if (velha[0][0] == 'X' && velha[0][1] == 'X' && velha[0][2] == 'X' ||
        velha[1][0] == 'X' && velha[1][1] == 'X' && velha[1][2] == 'X' ||
        velha[2][0] == 'X' && velha[2][1] == 'X' && velha[2][2] == 'X' ||
        velha[0][0] == 'X' && velha[1][1] == 'X' && velha[2][2] == 'X' ||
        velha[0][2] == 'X' && velha[1][1] == 'X' && velha[2][0] == 'X' ||
        velha[0][0] == 'X' && velha[1][0] == 'X' && velha[2][0] == 'X' ||
        velha[0][1] == 'X' && velha[1][1] == 'X' && velha[2][1] == 'X' ||
        velha[0][2] == 'X' && velha[1][2] == 'X' && velha[2][2] == 'X') {
            printf("\n\n %s e o vencedor.\n", jogador1);
            (*jogador1Vitorias)++;
            return(10);
    }
	if (velha[0][0] == 'O' && velha[0][1] == 'O' && velha[0][2] == 'O' ||
        velha[1][0] == 'O' && velha[1][1] == 'O' && velha[1][2] == 'O' ||
		velha[2][0] == 'O' && velha[2][1] == 'O' && velha[2][2] == 'O' ||
		velha[0][0] == 'O' && velha[1][1] == 'O' && velha[2][2] == 'O' ||
		velha[0][2] == 'O' && velha[1][1] == 'O' && velha[2][0] == 'O' ||
		velha[0][0] == 'O' && velha[1][0] == 'O' && velha[2][0] == 'O' ||
		velha[0][1] == 'O' && velha[1][1] == 'O' && velha[2][1] == 'O' ||
		velha[0][2] == 'O' && velha[1][2] == 'O' && velha[2][2] == 'O') {
            printf("\n\n %s e o vencedor.\n",jogador2);
            (*jogador2Vitorias)++;
            return(10);
    }

}

// Salvar Dados em um arquivo
void SalvarDados ( int jogador1Vitorias, int jogador2Vitorias, char jogador1[], char jogador2[], FILE *GanhadoresJogoDaVelha ){
    int escrita = 0;

    GanhadoresJogoDaVelha = fopen("GanhadoresJogoDaVelha.txt","r+");

    if ((GanhadoresJogoDaVelha = fopen("GanhadoresJogoDaVelha.txt","a+")) != NULL) {

        fprintf( GanhadoresJogoDaVelha," %s ganhou %d vezes" , jogador1, jogador1Vitorias );
        fprintf( GanhadoresJogoDaVelha,"\n %s ganhou %d vezes" , jogador2, jogador2Vitorias );
        fprintf( GanhadoresJogoDaVelha, "\n");
        printf("\n\n Arquivo salvo com SUCESSO!!! \n\n --\n\n ");

        fclose( GanhadoresJogoDaVelha );
    }

    else {
        system("cls");
        printf("\n\n\t O arquivo nao pode ser aberto!!\n");
        system("pause"); fflush(stdin);
    }
    NovaJanela();
}

// Tutorial do Jogo da Velha
void Tutorial(void){
    char tutorial=0;
    int i, j, numposicao=1;
    int posicao[3][3]={0,0,0,0,0,0,0,0,0};

    printf("\n\n\t Bem-vindo(a) ao tutorial do Jogo da Velha!\n");
    printf("\n O que voce deseja saber? \n\t ");
    printf("\n \t 1)Como jogar \n\t 2)Como salvar o jogo \n\t 3)Pontuacoes \n\t 4)Nao preciso de tutorial\n");
    printf(" Opcao: ");
    scanf("%d", &tutorial);
    system("cls");

    switch(tutorial){
        case 1:
            printf(" Selecione um Novo Jogo no menu pressionando a tecla 1.");
            printf("\n Logo apos, aparecera um modelo da matriz do jogo da velha.\n Veja abaixo:\n\n");
            for (i=0;i<3;i++) {
                printf("|");
				for(j=0;j<3;j++) {
                    posicao[i][j]= numposicao++;
					printf("%d|\t",posicao[i][j]);
                }
                                                         printf("\n");
                                       }
				                       printf(" \n\n");
				                       printf(" Estes numeros representam as coordenadas da matriz.\n\n");
				                       printf(" Caso voce escolha o numero 1,\n sera impresso o caractere correspondente\n do jogador na coordenada 1, 'X' ou 'O'.");
				                       printf(" Por exemplo:\n Digitando a coordenada 1, na vez do jogador 1.\n\n");
				                       printf(" X |  |  \n   |  |  \n   |  |  \n");
				                       printf(" E assim sucessivamente.");
				                       printf("\n\n\t Isto encerra este tutorial.\n\n");

				                       break;
				                    case 2:
                                        printf(" Seu jogo sera salvo cada vez que voce optar \n no menu pela opcao correspondente.");
                                        printf("\n Note que, para salvar o jogo voce antes deve terminar uma partida.\n");
                                        printf("\n Ao digitar a opcao \t2) Salvar jogo, \n aparecera a frase na tela: \n");
                                        printf("\n\n Arquivo salvo com SUCESSO!!! \n\n Isso significa que tudo ocorreu como o planejado.");
                                        printf("\n\n Isto encerra este tutorial. \n\n");

                                        break;
                                    case 3:
                                        system("cls");
                                        printf("\n\n Aparecera uma lista com as pontuacoes de cada jogador. \n\n");
                                        break;
                                    case 4:
                                        break;
                                    default:
                                        break;
                                    }
                                }

// Matriz modelo
void Modelo (void){
    int numPosicao=1, i, j;
    char posicao[3][3]= {0,0,0,0,0,0,0,0,0};

    printf(" \n\n \tMODELO: \n\n");
	for (i=0;i<3;i++) {
        printf("|");
		for(j=0;j<3;j++) {
            posicao[i][j]=numPosicao++;
			printf("%d|\t",posicao[i][j]);
        }
		printf("\n");
    }
	printf(" \n");
}

// Carregar Jogo (de mentirinha)
void CarregarJogo(void){
    system("color 09");
    printf("\n\n\n\n\n\n\t\t\t\t Carregando \n\n\t\t\t\t");
	int t;
	for (t=0; t < 5; t++) {
        printf(" %c", 04);
        Sleep(500);
    }
	system("cls");
}
