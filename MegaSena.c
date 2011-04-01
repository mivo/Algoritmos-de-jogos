/* UNIVERSIDADE ESTADUAL DE MONTES CLAROS - UNIMONTES
   Mega-Sena Universitária Implementada com Lista Duplamente Encadeada
   Desenvolvido por: Mariéllen Ivo e Hênio Carlos
   30/03/2011
*/

#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>


//==============================================================================
//==============================================================================
// Criaçao do tipo 'apostador' que armazenará os 'itens' das celulas.
typedef struct{
    //int chave;
    int dezena;
    int bilhete;
    char nome[50];
    int telefone;
    char e_mail[50];
    int cartela[15];
    float valor;
    int numeroAcertado;
    float premiacao;
}Apostador;

//================== Definição das Estruturas de Fila =============================

// criaçao de uma estrutura chamada apontador2 que fara referencia às cell da
// fila, no caso celula2, que sera posteriormente renomeada.
typedef struct Celula2 *Apontador2;

// Estrutura da cell da fila.
typedef struct Celula2 {
  Apostador item;
  Apontador2 prox;
} CelulaFila;

// criaçao dos controladores do tamanho da fila.
typedef struct TipoFila {
  Apontador2 frente, tras;
} PosicaoFila;

//====================== Definição das Estruturas de Lista ========================


// declaraçao do ponteiro apontador do tipo celula1.
typedef struct Celula1 *Apontador;

// criaçao da minha celula que tera um item (variavel pessoa do tipo apostador)
// e dois ponteiros do tipo apontador que indicarão as celulas anteriores e posteriores.
typedef struct Celula1{
        Apostador pessoa;            // struct para armazenar 'itens'
        Apontador prox, ant;        // ponteiros que sinalizam as celulas: anteriores e posteriores
}Celula;

// Criaçao de uma estruta que tera tres apontadores ( farão o controle do tamanho
// da minha lista ).
typedef struct {
        Apontador primeiro, ultimo, atual;
}Posicao;

// ============== Declaração de todas as Funções ==========================

// Prototipo para lista.
int Vazia ( Posicao Lista );
void CriarLista (Posicao *Lista);
void InserirCelula (Apostador item, Posicao *Lista );
void RetirarCelula (Posicao *Lista /*, int chave*/);
void VisualizarLista ( Posicao Lista );

// Prototipo para fila.
void FFVazia ( PosicaoFila *Fila );
int Vazia2 ( PosicaoFila Fila );
void Enfileira(Apostador x, PosicaoFila *Fila);
void Desenfileira( PosicaoFila *Fila, Apostador *item );
void Imprime( PosicaoFila Fila );

// Prototipo para funçoes diversas.
void MostrarMenu ( void );
void MensagemDEerro ( void );
void TabelaDEapostas ( void );
float valorAposta( int numeroTotal);
void sorteioAleatorio( int vet[], int limite );
void CompararCartelas ( Posicao *lista, int sorteio[] );
void CalcularAcertos( Posicao *Lista, float quadra, float quina, float sena, int *acertoQuadra, int *acertoQuina, int*acertoSena, float *acumulado );


//==========================================================================
//========================= Função Principal ===================================
//==========================================================================

int main()
{
    // Variaveis Comuns.
    int escolha, dezena, apostaAleatoria,  confirmacao, escolhaRelatorio;
    int sorteio[6], limiteApostas=0, concurso=0;
    int acertoQuadra=0, acertoQuina=0, acertoSena=0;
    float valorBruto=0, porcentagem, quadra, quina, sena ;
    float acumulado, premiacao;
    int sorteioFeito=0,contadorLimiteAposta = 1;

    // Variaveis para se trabalhar com lista e fila.
    Apostador jogador;
    Posicao lista;
    Apostador item;
    PosicaoFila fila;

    // Contadores:
    int i, j, k;

//=================== Começo do programa. =======================

    // Inicializaçao de variaveis.
    acumulado=0;
    premiacao=0;
    limiteApostas=0;
    item.bilhete=10000;
//    item.chave=0;
    item.numeroAcertado=0;

    // Criaçao de fila e lista.
    CriarLista( &lista );
    FFVazia( &fila );

    // Inicializaçao do vetor que armazenara os numeros do sorteio final.
    for( i=0; i<6; i++){
         sorteio[i]=0;
    }

    // Titulo.
    system("color 1f");
    system("title MEGA-SENA UNIVERSITARIA 2010 ");
    printf("\n\n\t\t===== MEGA-SENA UNIVERSITARIA 2010 =====\n\n\n");
    printf("\t\t\t Faca suas apostas!! \n\n\n");
    printf("\n( E necessario iniciar um concurso antes de realizar apostas. )\n\n");

    //Sempre mostrar o menu.
    while(1){
         escolha = 0;

         MostrarMenu();
         scanf("%d", &escolha); fflush(stdin);
         system("cls");

         // caso escolha invalida.
         if ( escolha<1 || escolha>8 ){
             while( escolha<1 || escolha>8 ){
                escolha=0;
                MensagemDEerro();
                system("cls");
                MostrarMenu();
                scanf("%d", &escolha); fflush(stdin);
                system("cls");
             }
         }

//______________________________________________________________________________
         // Caso escolha 1: Iniciando um concurso: obrigatorio.

         // se o concurso nao estiver em andamento.
         if ( escolha == 1 ){
             if( limiteApostas == 0 && sorteioFeito == 0 ){
                sorteioFeito = 1;
                concurso += 1;
                printf("\n\n\n\n\t ********* Concurso %d iniciado.   ********\n\n", concurso);
                system("pause"); fflush(stdin);
                system("cls");
             }
             // caso contrario.
             else if( limiteApostas<contadorLimiteAposta ){
                system("cls");
                printf("\n\n\n\t **** Concurso em andamento! **** \n\n\n");
                system("pause"); fflush(stdin);
                system("cls");
                continue;
             }
         }
         system("cls");

//______________________________________________________________________________

        if ( escolha ==2){

             // Implementaçao de fila. Os dados serao armazenados em uma celulaFila e,
             // posteriormente transferidos para uma lista.

             // Quando concurso tiver sido iniciado e nao tiver sido encerrado.
             if( concurso>0 && sorteioFeito==1 && limiteApostas<contadorLimiteAposta ){

                 system("cls");
                 printf("\n\n\t DADOS INICIAIS:\n\n");
                 printf("\t Para efetuar uma aposta, colocaremos seu nome\n\t e sua aposta na fila de apostadores.\n\n");
                 printf(" Insira seus dados:\n");

                 printf(" Nome: ");
                 gets(item.nome); fflush(stdin);
                 printf("\n\n");
                 TabelaDEapostas();

                 // zera o vetor de apostas
                 for(i=0;i<15;i++){
                    item.cartela[i]=0;
                 }

                 printf(" Insira a quantidade de dezenas que deseja apostar: ");
                 scanf("%d", &dezena);

                 // Se quantidade invalida.
                 if( dezena<6 || dezena>15 ){
                     while( dezena>15 || dezena<6 ){
                        system("cls");
                        printf("\n\n");
                        MensagemDEerro();
                        system("cls");
                        TabelaDEapostas();
                        printf("\n\n\t Digite outra quantidade: ");
                        scanf("%d", &dezena); fflush(stdin);
                     }
                   }
                   item.dezena = dezena;
                   while(1){
                      apostaAleatoria=0;
                      printf(" Digite:\n [1] Para numeros aleatorios.\n [2] Para escolher numeros manualmente.\n");
                      printf("\n\n >> ");
                      scanf("%d", &apostaAleatoria); fflush(stdin);
                      if(apostaAleatoria==1 || apostaAleatoria==2){
                         break;
                      }
                      MensagemDEerro();
                      system("cls");
                   }

                   // Caso numeros aleatorios.
                   if( apostaAleatoria == 1){
                      apostaAleatoria=0;
                      printf("\n\n Sua aposta: ");
                      sorteioAleatorio( item.cartela, item.dezena );
                      for(i=0; i<item.dezena; i++){
                             printf("\n\t %d",item.cartela[i]);
                      }
                      printf("\n\n");
                      system("pause"); fflush(stdin);
                      system("cls");
                   }

                   // Caso numeros manualmente.
                   if( apostaAleatoria == 2){
                      apostaAleatoria=0;
                      printf("\n Sua aposta: \n\n");
                      i=0;
                      while( i<item.dezena ){
                             printf("\n\n\t Dezena %d: ", i+1);
                             scanf("%d", &item.cartela[i]); fflush(stdin);

                             // numero invalido.
                             if( item.cartela[i]<1 || item.cartela[i]>60 ){
                                 printf("\n\n Numero Invalido!!\n\n");
                                 continue;
                             }
                             for( k=1; k<item.dezena; k++){
                                  // numero repetido.
                                  if(item.cartela[i]== item.cartela[i-k] && (i-k >=0) && (i!=0) ){
                                        printf("\n\n Numero repetido!!\n\n");
                                        i--;
                                  }
                             }
                             i++;
                      }
                   }
                   item.bilhete += 1;
//                   item.chave += 1;
                   Enfileira( item, &fila );
                   limiteApostas++;
             }
             else if( concurso==0 || limiteApostas>=contadorLimiteAposta){
                   system("cls");
                   MensagemDEerro();
                   printf("\n\n\t Inicie um novo concurso, ou \n");
                   printf("\t insira apostadores na lista.\n\n\n\n");
                   system("pause"); fflush(stdin);
                   system("cls");
                   escolha=0;
                   continue;
             }

             system("cls");
        }

//______________________________________________________________________________
// Caso realisar sorteio
         if( escolha == 3 ){

             // Se o concurso nao tiver sido encerrado.
             if ( limiteApostas<contadorLimiteAposta ){
                   system("cls");
                   printf("\n\n\n\t **** O concurso ainda nao foi iniciado/encerrado! ****\n\n\n\n");
                   system("pause");
                   system("cls");
                   continue;
             }

             if (limiteApostas == contadorLimiteAposta && !Vazia(lista) ){
                   acumulado=0;
                   // caso concurso encerrado.
                   printf("\n\n\t **** APOSTAS ENCERRADAS !! **** \n\n\n");
                   system("pause"); fflush(stdin);
                   system("cls");
                   printf("\n\n\n\t\tNumeros do Sorteio:\n\n");
                   sorteioAleatorio( sorteio, 6 );
                   for(i=0; i<6; i++){
                                      //sorteio[i]= i+1;
                                      printf("\n\t%d", sorteio[i]);
                   }

                   // Calculo das porcentagens.
                   porcentagem = ( valorBruto * 0.7 )+ acumulado;   // 70% do valor arrecadado vai para a premiação.*******
                   quadra = porcentagem * 0.15;                     // 15% dos 70% vai para a premiaçao da quadra.
                   quina = porcentagem * 0.25;                      // 25% dos 70% vai para a premiação da quina.
                   sena = porcentagem * 0.6;                        // 60% dos 70% vai para a premiaçao da sena.

                   // Comparando cartelas.
                   CompararCartelas ( &lista, sorteio );
                   CalcularAcertos( &lista, quadra, quina, sena, &acertoQuadra, &acertoQuina, &acertoSena, &acumulado );

                   // Relatorio sobre os valores monetarios.
                   printf("\n\n\t >>>> RELATORIO GERAL <<<< \n\n");

                   printf("\n ..Valor bruto arrecadado: %.2f", valorBruto);
                   printf("\n ..Valor destinado a premiacao: %.2f", porcentagem);
                   printf("\n ..Quantidade de Ganhadores:\n\n"),
                   printf(" ..Quadra: %d.\n\n ..Quina: %d.\n\n ..Sena: %d.\n\n", acertoQuadra, acertoQuina, acertoSena);
                   printf("\n ..Valor acumulado: %.2f", acumulado);

                   // Lista com nomes e pontuaçoes.
                   printf("\n\n\t >>>> LISTA CLASSIFICATORIA <<<< \n\n");
                   VisualizarLista( lista );

                   // Zera as variaveis para um novo concurso.
                   if( sorteioFeito == 1 ){
                       sorteioFeito=0;
                   }
                   escolha=0;
                   limiteApostas=0;
                   system("pause"); fflush(stdin);
                   system("cls");
                   continue;
             }
             else{
                  printf("\n\n Termine de inserir apostadores na fila ou passe-os para a lista. \n\n");
                  MensagemDEerro();
                  system("cls");
                  continue;
             }
         }
//______________________________________________________________________________

         // Caso sair do programa.
         if( escolha == 4 ){

             system("cls");
             printf("\n\n\n\t Obrigada por sua participacao!!\n\n");
             printf("\n\t BOA SORTE!!\n\n\n\n");
             system("pause");
             exit(0);
            }

//______________________________________________________________________________

        // Inserir na lista os dados dos apostadores.
        if ( escolha== 5 ){

            if( Vazia2(fila) == 1 ){
                printf("\n Insira dados na fila! \n");
                system("pause"); fflush(stdin);
                system("cls");
            }
            else if (limiteApostas>=contadorLimiteAposta){

                while ( !Vazia2(fila) ){
                   // colocar os itens restantes na fila e passar para a lista.
                   Desenfileira( &fila, &item );
                   printf("\n Jogador %s :\n\n", item.nome );
                   printf("\n Insira seu telefone: ");
                   scanf("%d", &item.telefone); fflush(stdin);
                   printf("\n Insira seu e_mail: ");
                   gets(item.e_mail); fflush(stdin);

                   // Calculando o valor da aposta.
                   item.valor = valorAposta( item.dezena );
                   valorBruto += ( item.valor + acumulado );
                   printf("\n\n Valor da aposta: %.2f\n\n", item.valor);

                   InserirCelula( item, &lista );
                   system("pause"); fflush(stdin);
                   system("cls");
                }
            }
            else{
                 MensagemDEerro();
                 printf("\n Termine de inserir apostadores na fila.\n\n( pressione <enter> )\n");
                 getch(); fflush(stdin);
                 system("cls");
            }
        }

//______________________________________________________________________________

        if ( escolha == 6 ){
               printf("\n\n");
               VisualizarLista( lista );
               printf("\n\n");
               system("pause"); fflush(stdin);
               system("cls");
        }

//______________________________________________________________________________

        if ( escolha == 7 ){
           if( !Vazia2(fila)){
               Imprime( fila );
               //printf("\n\n Lista vazia!\n\n");
               system("pause"); fflush(stdin);
               system("cls");
           }
           else{
                system("cls");
                printf("\n\n Fila vazia!\n\n");
                system("pause"); fflush(stdin);
                system("cls");
           }
        }
    }
    system("pause");
    return(0);
}


//=========================================================================
//======================== Estruturas das Funçoes ==============================
//=========================================================================

// ____________________ Funções de lista _______________________________________


int Vazia ( Posicao Lista ){
     return ( Lista.primeiro == Lista.ultimo );
}

void CriarLista ( Posicao *Lista ){
     Lista-> primeiro = ( Apontador ) malloc ( sizeof ( Celula ));
     Lista-> ultimo = Lista -> primeiro;
     Lista-> atual = Lista-> primeiro;
     Lista-> primeiro-> prox = NULL;
     Lista-> primeiro-> ant = NULL;
}

void InserirCelula ( Apostador item, Posicao *Lista ){
     Lista -> ultimo -> prox = ( Apontador ) malloc ( sizeof ( Celula ));
     Lista -> ultimo = Lista -> ultimo -> prox;
     Lista -> ultimo -> ant =  Lista -> atual;
     Lista -> atual = Lista -> ultimo;
     Lista -> ultimo -> pessoa = item;
     Lista -> ultimo -> prox = NULL;
}

// Esta função retira todas as células, exceto a célula cabeça
void RetirarCelula( Posicao *Lista/*, int chave*/ ){
     Apontador aux, q;
     aux = Lista -> primeiro;
     q = aux -> prox;

     if( Vazia(*Lista) || aux==NULL || aux->prox==NULL ){
         printf("\n\n Erro! A lista esta vazia ! \n\n");
         return;
     }

     while( q!=NULL ){
        aux -> prox = q -> prox;
		q -> prox -> ant = aux -> prox -> ant;
        if (aux->prox == NULL){
            Lista->ultimo = aux;
        }
        free(q);
		q = q->prox;
		aux = aux -> prox;
	 }
}

void VisualizarLista ( Posicao Lista ){
     int i;
     Apontador aux;
	    aux = Lista.primeiro->prox;
	    if (Vazia(Lista)){
		      printf("Erro: Lista vazia!\n");
		      return;
	    }
	    while (aux != NULL){
        printf("\n\n\n---NOME: %s \n", aux->pessoa.nome );
	       printf("\n\n---BILHETE: %d \n", aux->pessoa.bilhete);
        printf("\n\n---TELEFONE: %d \n", aux->pessoa.telefone);
        printf("\n\n---E_MAIL: %s \n", aux->pessoa.e_mail);
        printf("\n\n---CARTELA:\n");
        for ( i=0; i< aux->pessoa.dezena; i++ ){
          printf("\n %d ", aux->pessoa.cartela[i] );
        }
        printf("\n\n---VALOR DA APOSTA: %.2f \n", aux->pessoa.valor);
        printf("\n\n---NUMERO DE ACERTOS: %d \n", aux->pessoa.numeroAcertado);
        printf("\n\n---PREMIACAO DO APOSTADOR: %.2f \n", aux->pessoa.premiacao);
        aux = aux->prox;
     }
}


//========================== Funções de fila ========================================

// gera a primeira cell da fila( FFVazia = faz fila vazia ).
void FFVazia( PosicaoFila *Fila){
  Fila->frente = (Apontador2) malloc(sizeof(CelulaFila));
  Fila->tras = Fila->frente;
  Fila->frente->prox = NULL;
}

// Retorna se a lista está vazia
int Vazia2 (PosicaoFila Fila){
  return (Fila.frente == Fila.tras);
}

// Recebe o item q quero enfileirar e onde.
// sempre coloca atras.( enfileirar cell )
void Enfileira(Apostador x, PosicaoFila *Fila){
  Fila->tras->prox = (Apontador2) malloc(sizeof(CelulaFila));
  Fila->tras = Fila->tras->prox;
  Fila->tras->item = x;
  Fila->tras->prox = NULL;
}

// Funçao para retirar da fila, sempre retira o primeiro da fila, ou seja, quem entrou por ultimo.
void Desenfileira( PosicaoFila *Fila, Apostador *item ){
  Apontador2 q;
  if (Vazia2( *Fila ) ){
     system("cls");
     printf(" Erro! A fila esta  vazia.\n");
     return;
  }
  q = Fila->frente;
  Fila->frente = Fila->frente->prox;
  *item = Fila->frente->item;
  free(q);
}

// funçao que imprime a lista.
void Imprime( PosicaoFila Fila ){
  Apontador2 aux;
  int i;
  aux = Fila.frente->prox;

  while (aux != NULL){
      printf("\n\n DADOS INICIAIS: \n\n");
      printf(" BILHETE: ");
      printf("%d", aux->item.bilhete );
      printf("\n NOME: ");
      printf("\n %s \n", aux->item.nome );
      printf("\n CARTELA: ");
      for ( i=0; i< aux->item.dezena; i++ ){
          printf("\n %d ", aux->item.cartela[i] );
      }
      printf("\n\n");
      aux = aux->prox;
  }
}

//======================= Outras funções =============================

// Mostrar o menu principal.
void MostrarMenu( void ){

    printf("\n\n\t\t MENU \n\n");
    printf("\n\n\t >> Deseja iniciar novo concurso? << \n\n");
    printf("\t 1) Iniciar concurso.\n\t 2) Inserir na fila. (fazer bilhete)\n");
    printf("\t 3) Realizar sorteio.\n\t 4) Sair.\n\t 5) Inserir na lista.\n\t 6) Mostrar lista.\n\t 7) Mostrar fila. ");
    printf("\n\n\t Escolha: ");
}

// Imprimir mensagem de erro.
void MensagemDEerro(void){
    system("cls");
    printf("\n\n\n\t>>>>>>>>>> ACAO INVALIDA!! <<<<<<<<<<<<<\n\n\n");
    system("pause");
}

// Mostrar de tabela de preços para apostas.
void TabelaDEapostas ( void ){
    printf("\n\n");
    printf("\t\t TABELA DE PRECOS \n\n");
    printf(" ____________________________________________________________________\n");
    printf("| Qtde. de numeros | Valor aposta | Qtde. de numeros | Valor aposta  |\n");
    printf(" ____________________________________________________________________\n");
    printf("| 6 dezenas        |  2,00        |   11 dezenas     | 924,00        |\n");
    printf(" ____________________________________________________________________\n");
    printf("| 7 dezenas        |  14,00       |   12 dezenas     | 1848,00       |\n");
    printf(" ____________________________________________________________________\n");
    printf("| 8 dezenas        |  56,00       |   13 dezenas     | 3432,00       |\n");
    printf(" ____________________________________________________________________\n");
    printf("| 9 dezenas        |  168,00      |   14 dezenas     | 6006,00       |\n");
    printf(" ____________________________________________________________________\n");
    printf("| 10 dezenas       |  420,00      |   15 dezenas     | 10010,00      |\n");
    printf(" ____________________________________________________________________\n\n\n");
}

void CompararCartelas ( Posicao *Lista, int sorteio[] ){
     int i, j;
     Apontador aux;
     aux = Lista->primeiro->prox;

     while( (Vazia(*Lista)!=1) && (aux != NULL) ){
         aux->pessoa.numeroAcertado=0;
         for(i=0; i<aux->pessoa.dezena; i++){
             for(j=0; j<6; j++){
                 if(aux->pessoa.cartela[i]==sorteio[j]){
                       aux->pessoa.numeroAcertado++;
                 }
             }
         }
         aux = aux->prox;
     }
}

void CalcularAcertos( Posicao *Lista, float quadra, float quina, float sena, int *acertoQuadra, int *acertoQuina, int*acertoSena, float *acumulado ){

    Apontador aux;
    aux = Lista->primeiro->prox;
    while( !Vazia(*Lista) && (aux != NULL) ){

           // Calculando numero de ganhadores.
           if( aux->pessoa.numeroAcertado == 4 ){
               acertoQuadra++;
           }
           else if( aux->pessoa.numeroAcertado == 5 ){
                acertoQuina++;
           }
           else if( aux->pessoa.numeroAcertado == 6 ){
                acertoSena++;
           }
           else{
                *acumulado += (aux->pessoa.valor *0.70);
           }

           // Dividindo o premio.
           if( aux->pessoa.numeroAcertado == 4 ){
               aux->pessoa.premiacao = quadra/ *acertoQuadra;
           }
           else if( aux->pessoa.numeroAcertado == 5 ){
                aux->pessoa.premiacao = quina/ *acertoQuadra;
           }
           else if( aux->pessoa.numeroAcertado == 6 ){
                aux->pessoa.premiacao = sena/ *acertoSena;
           }
           else if( aux->pessoa.numeroAcertado == 0 ){
                aux->pessoa.premiacao = 0;
           }
           aux = aux->prox;
    }
}

float valorAposta( int numeroTotal){
    float valor;
    if( numeroTotal == 6){
             valor = 2.00;
    }
    else if ( numeroTotal == 7 ){
             valor = 14.00;
    }
    else if ( numeroTotal == 8 ){
             valor = 56.00;
    }
    else if ( numeroTotal == 9 ){
             valor = 168.00;
    }
    else if ( numeroTotal == 10 ){
             valor = 420.00;
    }
    else if ( numeroTotal == 11 ){
             valor = 924.00;
    }
    else if ( numeroTotal == 12 ){
             valor = 1848.00;
    }
    else if ( numeroTotal == 13 ){
             valor = 3432.00;
    }
    else if ( numeroTotal == 14 ){
             valor = 6006.00;
    }
    else if ( numeroTotal == 15 ){
             valor = 10010.00;
    }
    return( valor );
}


void sorteioAleatorio( int vet[], int limite ){
    int cont=0,tmp,cont2=0,i,j;
    srand(time(NULL));

    // Coloca 0 em todas posiçoes do vetor
    for(i=0;i<limite;i++){
		           vet[i]=0;
    }
	   // Sorteia o primeiro numero
	   vet[0] = rand()%60+1;

	   //Sorteio dos seguintes
    for (cont=1;cont<limite;cont++){
		           tmp=rand()%60+1;
		           cont2=0;
		      while(cont2<limite){
             // Se o numero sorteado for igual a algum no vetor faz novo sorteio
             if(tmp==vet[cont2]){
                    tmp=rand()%60+1;
                    // cont2=-1(incrementa depois e vira 0)  pois deve voltar ao inicio do while com cont2=0,
                    // para testar se esse novo numero tambem nao é repetido.
                           cont2=-1;
			          }
			          cont2++;
		     }
		     vet[cont]=tmp;
    }
}
