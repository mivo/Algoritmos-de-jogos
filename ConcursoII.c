/* UNIVERSIDADE ESTADUAL DE MONTES CLAROS - UNIMONTES
    Centro de Ci�ncias Exatas e Tecnol�gicas
    Departamento de Ci�ncias da Computa��o
    Curso de Sistemas de Informa��o

    Professor: Renato Cota
    Academicos: Mariellen Ivo e Ant�nio Oliveira
    Descri��o: Vestibular Unimontes 2011
    Data: 30/06/2011
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE  1
#define FALSE 0
#define MAX 10

//Estrutura dos dados do candidato
// Usada em todas as estruturas do codigo
typedef struct{
    char nome[51];
    int idade;
    int numero_de_inscricao;
    int pontuacao;
    char *gabarito;
    char cargo[51];
}candidato;

//===============================================================================
//                  ESTRUTURAS DE �RVORE SBB

typedef enum {
  Vertical, Horizontal
} Inclinacao;

typedef struct no* ApontadorNo;

typedef struct no {
  candidato Reg;
  ApontadorNo Esq, Dir;
  Inclinacao BitE, BitD;
} No;


//===============================================================================
//                  ESTRUTURAS DE LISTA

typedef struct celula *Apontador;

typedef struct celula{
    candidato item;
    Apontador prox;
    Apontador ant;
}Celula;

typedef struct{
    Apontador primeiro;
    Apontador ultimo;
}TipoLista;

//===============================================================================
//                  ESTRUTURAS DE FILA

typedef struct celulaFila *ApontadorFila;

typedef struct celulaFila{
    candidato itemFila;
    ApontadorFila prox;
}CelulaFila;

typedef struct{
    ApontadorFila primeiroFila;
    ApontadorFila ultimoFila;
}TipoFila;


//====================== FUNCOES PARA HASH ===========================

void PesquisaHash( TipoLista *Hash, candidato *x, int numero_questoes );
int CalculoPosicao(int x);
void PesquisaHashNome( TipoLista *Hash, candidato *x, int numero_questoes );
int CalculoPosicaoNome(candidato x);
void DesalocaHash (TipoLista *Hash);

//==============================================================
// PROTOTIPOS DE ARVORE SBB - usadas em ambas estruturas

void EE(ApontadorNo *Ap);
void ED(ApontadorNo *Ap);
void DD(ApontadorNo *Ap);
void DE(ApontadorNo *Ap);
void DesalocaArvore ( ApontadorNo *Arvore);
void Inicializa(ApontadorNo *Arvore);
void EsqCurto(ApontadorNo *Ap, short *Fim);
void DirCurto(ApontadorNo *Ap, short *Fim);
void Antecessor(ApontadorNo q, ApontadorNo *r, short *Fim);

//==============================================================
// PROTOTIPOS DE ARVORE SBB - PESQUISA POR NUMERO DE INSCRICAO

void IInsere(candidato x, ApontadorNo *Ap, Inclinacao *IAp, short *Fim);
void Insere(candidato x, ApontadorNo *Ap);
void Retira(candidato x, ApontadorNo *Ap);
void IRetira(candidato x, ApontadorNo *Ap, short *Fim);
void Pesquisa(candidato *x, ApontadorNo *p, int numero_questoes, int *achou);
void Testa1(ApontadorNo p, int nivel, int *NivelFolhas, short *PrimeiraFolha);
void Testa2(ApontadorNo p, int *NivelFolhas, short *PrimeiraFolha);
void Testa(ApontadorNo Arvore);
void Em_Ordem(ApontadorNo No);
void Pre_Ordem(ApontadorNo No);
void Pos_Ordem(ApontadorNo No);

//==============================================================
//          PROTOTIPOS DE ARVORE SBB - PESQUISA POR NOME

void IInsere_nome(candidato x, ApontadorNo *Ap, Inclinacao *IAp, short *Fim);
void Insere_nome(candidato x, ApontadorNo *Ap);
void IRetira_nome(candidato x, ApontadorNo *Ap, short *Fim);
void Retira_nome(candidato x, ApontadorNo *Ap);
void Pesquisa_nome(candidato *x, ApontadorNo *p, int numero_questoes, int *achou);
void Testa1_nome(ApontadorNo p, int nivel, int *NivelFolhas, short *PrimeiraFolha);
void Testa2_nome(ApontadorNo p, int *NivelFolhas, short *PrimeiraFolha);
void Testa_nome(ApontadorNo Arvore);
void Em_Ordem_nome(ApontadorNo No);
void Pre_Ordem_nome(ApontadorNo No);
void Pos_Ordem_nome(ApontadorNo No);

//==============================================================
//                  PROTOTIPOS  de LISTA

void IniciarLista ( TipoLista *Lista );
int TesteListaVazia ( TipoLista Lista );
void InserirCelula (TipoLista *Lista ,candidato X);
void RetirarCelula ( TipoLista *Lista, candidato *X);
void ImprimeLista ( TipoLista Lista );
int Desalocar ( TipoLista *Lista, TipoFila *Fila, int *verifica_concurso, ApontadorNo *Arvore_inscricao, ApontadorNo *Arvore_nomes );


//                PROTOTIPOS de FILA

void IniciarFila ( TipoFila *Fila );
int TesteFilaVazia ( TipoFila Fila );
void InserirCelulaFila ( TipoFila *Fila, candidato X  );
void RetirarCelulaFila ( TipoFila *Fila );
void ImprimeFila ( TipoFila Fila );


//==============================================================
//                 OUTROS PROTOTIPOS

void ImprimeMenu(void);
void apresentacao(void);
void NovaTecla(void);
char VerificaCaracter( char var);
int VerificaInteiro(int var );
char VerificarGabaritoCandidato( char gabarito );
void GravarArquivoLista(candidato x, int n_questoes);
int CompararGabarito ( char gabarito_candidato[], char gabarito_oficial[], int n_quetoes );
int ContarCelulas( TipoLista Lista);
void Ordena(int esq, int dir, TipoLista *Lista);
void Particao(int esq, int dir, int *i, int *j, TipoLista *Lista);
void QuickSortLista( TipoLista *Lista, int tamanho);

/*******************************************************************/
/************************ FUN��O PRINCIPAL ************************/
/*****************************************************************/

int main (void){

        system("title VESTIBULAR 2011");
        system("color 0f");


// Declara��o de vari�veis comuns
    int numero_concurso, numero_vagas, numero_questoes;
    int confirma, op, select, var, achou;
    char data[15];
    char nome_cargo[51], promotor_concurso[51];
    char *gabarito_oficial;
    int flag=0, verifica_concurso=1;

// Declara��o de contadores
    int i,j, k, l=0, cont_linha=0, hash=0;
    char c;

// Declara��o de vari�veis para arquivo
    FILE *configuracao_concurso, *ultimo_concurso;
    FILE *arq_lista, *arq_fila;
    FILE *arquivo2;

    char string_tmp[100], caractere[10];

// Declara��o de vari�veis para lista e fila
    candidato ficha_pessoal, X;
    TipoLista relacao_candidatos_lista;
    TipoFila relacao_candidatos_fila;

// Declara��o de vari�veis para Arvores SBB
    ApontadorNo Arvore_inscricao;
    ApontadorNo Arvore_nomes;

// Declara��o de vari�veis para Tabela Hash
    TipoLista Hash_inscricao[MAX];
    TipoLista Hash_nome[MAX];
    int posicao;


//===============================================================
//                           INICIO DO PROGRAMA

    // Inicia as lista/fila/arvores para poder carregar dados nelas
    IniciarLista(&relacao_candidatos_lista);
    IniciarFila (&relacao_candidatos_fila);
    Inicializa(&Arvore_inscricao);
    Inicializa(&Arvore_nomes);

    // Inicializa as tabelas hash
    for( i=0; i<MAX; i++ ){
        IniciarLista(&Hash_inscricao[i]);
        IniciarLista(&Hash_nome[i]);
    }

    // Inicializando as variaveis-contadores
    ficha_pessoal.numero_de_inscricao=100;
    numero_concurso = 0;

    // Carregando o numero do ultimo concurso
    if( (ultimo_concurso = fopen("ultimo_concurso.txt", "a+") ) != NULL){
        fgets( caractere, 10, ultimo_concurso);
        sscanf( caractere, "%d", &numero_concurso);
        fclose(ultimo_concurso);
    }

    //Salva uma # na primeira linha do arquivo dos candidatos
    if((arquivo2 =fopen("Ficha dos Candidatos.txt","a+"))!=NULL){
        fclose(arquivo2);
        // Abre o arquivo para leitura/escrita
        if((arquivo2 =fopen("Ficha dos Candidatos.txt","r+"))!=NULL){
            // Caso houver um espa�o em branco no in�cio do arquivo
            while ((c = fgetc(arquivo2)) != EOF ){
                if( c == '\n'){
                    // Salva uma hash na primeira linha do arquivo
                    fseek(arquivo2,0, SEEK_SET);
                    fprintf(arquivo2, "#");
                    fclose(arquivo2);
                }
                fclose(arquivo2);
                break;
            }
            fclose(arquivo2);
        }
    }

            /******* Carregando Configura��es do Concurso ********/

    //Carregando os dados das configura��es do �ltimo concurso
    if(((configuracao_concurso = fopen("configuracao_concurso.txt","r") ) != NULL ) && ultimo_concurso!=0){

        while ((c = fgetc(configuracao_concurso)) != EOF ){
            // contando as # ate chegar na do ultimo concurso
            l=0;
            if(c == '#'){
                hash++;
                // P�ra na posicao da ultima #
                if( hash == numero_concurso){
                    l=1;
                    break;
                }
            }
        }

        if(l ==1){

            // Coloca o cursor no arquivo de onde parou de ler as #
            fseek (configuracao_concurso,0, SEEK_CUR);

            // Pega os dados de configura��o do arquivo e passa para as vari�veis
            // Pega linha a linha com o fgets e passa para a vari�vel com o sscanf
            // Obs: a primeira linha � um #, ent�o, saltamo-la
            fgets(string_tmp, 100, configuracao_concurso); // #
            fgets(string_tmp, 100, configuracao_concurso); // promotor do concurso
            sscanf(string_tmp,"%s",&promotor_concurso); fflush(stdin);
            fgets(string_tmp, 100, configuracao_concurso); // data do concurso
            sscanf(string_tmp,"%s",&data); fflush(stdin);
            fgets(string_tmp, 100, configuracao_concurso); // cargo do concurso
            sscanf(string_tmp,"%s",&nome_cargo); fflush(stdin);
            fgets(string_tmp, 100, configuracao_concurso); // numero de vagas
            sscanf(string_tmp,"%d",&numero_vagas); fflush(stdin);
            fgets(string_tmp, 100, configuracao_concurso); // numero de quest�es
            sscanf(string_tmp,"%d",&numero_questoes); fflush(stdin);

            // Aloca o espa�o para o gabarito
            gabarito_oficial = (char*) malloc (numero_questoes*sizeof(char));

            // Preenche o gabarito
            for( k=0; k<numero_questoes; k++){
                fgets(string_tmp, 100, configuracao_concurso); // gabarito oficial do concurso
                string_tmp[strlen(string_tmp)-1]='\0';
                sscanf(string_tmp,"%c",&gabarito_oficial[k]); fflush(stdin);
            }

            fgets(string_tmp, 100, configuracao_concurso); // flag para verifica_concurso
            sscanf(string_tmp,"%d",&verifica_concurso);

            // Depois de carregar todos os dados, fecha o arquivo
            fclose(configuracao_concurso);
        }
        fclose(configuracao_concurso);
    }

   // Caso o arquivo n�o exista, a condi��o acima ser� ignorada, pois n�o h� dados a serem carregados

    //  Preenchendo a lista com dados dos candidatos, abre o arquivo somente leitura
    if( (arq_lista = fopen("Ficha dos Candidatos.txt","r") ) != NULL && numero_concurso!=0 ){
        hash=0;

        while ((c = fgetc(arq_lista)) != EOF ){
            // contando as # ate chegar nos dados do primeiro candidato do ultimo concurso
            if(c == '#'){
                hash++;
                // P�ra na posicao da ultima #
                if( hash == numero_concurso){
                    break;
                }
            }
        }

        // Coloca o cursor no arquivo de onde parou de ler as #
		fseek (arq_lista,0, SEEK_CUR);

        // Conta linhas at� o fim do arquivo
        while ((c = fgetc(arq_lista)) != EOF ){
            if(c == '\n'){
                cont_linha++;
            }
        }

        // Volta ao inicio do arquivo
        c=0;
        fseek(arq_lista,0, SEEK_SET);
        // Contagem do n�mero de candidatos com base no numero de linhas
        cont_linha /=(6+numero_questoes);

        // Percorre novamente o arquivo desde o inicio
        hash=0;

        while ((c = fgetc(arq_lista)) != EOF ){
            // contando as # ate chegar nos dados do primeiro candidato do ultimo concurso
            if(c == '#'){
                hash++;
                // P�ra na posicao da ultima #
                if( hash == numero_concurso){
                    break;
                }
            }
        }

        // Coloca o cursor no arquivo de onde parou de ler as #
		//fseek (arq_lista,0, SEEK_CUR);
        for(l=0; l<cont_linha; l++){

            // Pega os dados de configura��o do arquivo e passa para as vari�veis
            // Pega linha a linha com o fgets e passa para a vari�vel com o sscanf
            // Obs: a primeira linha � um #, no caso do primeiro concurso,
            // � um espa�o, ent�o, saltamo-la
            fgets(string_tmp, 100, arq_lista);  fflush(stdin);// # ou <espa�o>

            fgets(ficha_pessoal.nome, 100, arq_lista); // Nome completo do candidato
            ficha_pessoal.nome[strlen(ficha_pessoal.nome)-1]='\0';
            fgets(string_tmp, 100, arq_lista); // Idade do candidato
            sscanf(string_tmp,"%d",&ficha_pessoal.idade);
            fgets(string_tmp, 100, arq_lista); // Cargo do Candidato
            sscanf(string_tmp,"%s",&ficha_pessoal.cargo);
            fgets(string_tmp, 100, arq_lista); // Numero de inscri��o do candidato
            sscanf(string_tmp,"%d",&ficha_pessoal.numero_de_inscricao);
            fgets(string_tmp, 100, arq_lista); // Pontua��o do candidato
            sscanf(string_tmp,"%d",&ficha_pessoal.pontuacao);

            // Aloca o espa�o para o gabarito
            ficha_pessoal.gabarito = (char*) malloc (numero_questoes*sizeof(char));

            // Preenche o gabarito
            for( k=0; k<numero_questoes; k++){
                fgets(string_tmp, 100, arq_lista); // Gabarito do candidato
                string_tmp[strlen(string_tmp)-1]='\0';
                sscanf(string_tmp,"%c",&ficha_pessoal.gabarito[k]); fflush(stdin);
            }

            // Insere os dados numa celula da lista
            InserirCelula(&relacao_candidatos_lista, ficha_pessoal);

            // Insere os dados na arvore de pesquisa por num. de insc.
            Insere(ficha_pessoal,&Arvore_inscricao);

            // Insere os dados na arvore de pesquisa por nome
            Insere_nome(ficha_pessoal,&Arvore_nomes);

            // Insere os dados na tabela hash de pesquisa por num. de insc. e nomes
            posicao = CalculoPosicao(ficha_pessoal.numero_de_inscricao);
            InserirCelula(&Hash_inscricao[posicao], ficha_pessoal);
            posicao=0;

            // Insere os dados na tabela hash de pesquisa por nome
            posicao = CalculoPosicaoNome(ficha_pessoal);
            InserirCelula(&Hash_nome[posicao], ficha_pessoal);
            posicao=0;

        }
        // Depois de carregar todos os dados, fecha o arquivo
        fclose(arq_lista);
    }
    // Caso o arquivo n�o exista, a condi��o acima ser� ignorada, pois n�o h� dados a serem carregados

/**************** Fim do processo de carregamento de Dados ******************/
    apresentacao();
    do{

        system("cls");
        ImprimeMenu();

        while(1){
            if( scanf("%d",&op) == 0){
                printf("\n Opcao invalida, digite novamente. \n");
                NovaTecla();
                ImprimeMenu();
                continue;
            }
            break;
        }

        //================ OPCAO 1 ===================

        if( op == 1 ){

            // verificar se h� um concurso iniciado (dados carregados)
            flag = Desalocar(&relacao_candidatos_lista, &relacao_candidatos_fila, &verifica_concurso, &Arvore_inscricao, &Arvore_nomes);
            //A flag retorna 1 caso os dados sejam deletados, do contr�rio retorna 0
            // Assim, a tabela tamb�m ser� desalocada
            if(flag==1){
                for( i=0; i<MAX; i++ ){
                    DesalocaHash(&Hash_inscricao[i]);
                    DesalocaHash(&Hash_nome[i]);
                }
            }

            // Caso nao haja nenhuma inscricao para o concurso
            if(flag==1 && verifica_concurso == 1){

                system("color 4f");
                printf("            \xc9");
                for(i=0;i<55;i++)
                    printf("\xcd");
                printf("\xbb\n");
                printf("\t    \xba Nao houveram inscricoes para o concurso em andamento. \xba\n");
                printf("\t    \xba Deseja continuar?           [1]Sim   [2]Nao           \xba\n");
                printf("            \xc8");
                for(i=0;i<55;i++)
                  printf("\xcd");
                printf("\xbc\n");
                printf("\n\t Opcao:  ");
            while(1){
                if( scanf("%d",&op) == 0){
                    printf("\n Opcao invalida, digite novamente. \n");
                    NovaTecla();
                    ImprimeMenu();
                    continue;
                }
                break;
            }

                printf("\n\t\t    Pressione qualquer tecla . . .");
                getch();
                fflush(stdin);
                system("cls");
                if(op==1){
                    verifica_concurso=0;
                }
                else{
                    verifica_concurso=1;
                }
                continue;
            }


            // Caso novo concurso
            if( flag == 1 && verifica_concurso == 0){
                numero_concurso+=1;
                verifica_concurso=1; // concurso iniciado
                system("cls");

                // Salva o numero do ultimo concurso em arquivo
                // o que evita bugs ao carregar um concurso nao terminado
                if( (ultimo_concurso = fopen("ultimo_concurso.txt","w+") ) != NULL ){
                    fprintf(ultimo_concurso, "%d", numero_concurso);
                    fclose(ultimo_concurso);
                }
                else{
                    printf("\n Ocorreu um erro inesperado. \n");
                    NovaTecla();
                    continue;
                }

                printf("            \xc9");
                for(i=0;i<20;i++)
                    printf("\xcd");
                printf("\xbb\n");
                printf("\t    \xba \xfa\xfa\xfa CONCURSO %d \xfa\xfa\xfa \xba\n", numero_concurso);
                printf("            \xc8");
                for(i=0;i<20;i++)
                    printf("\xcd");
                printf("\xbc\n");

                // Cria as lista/arvore/fila para armazenar os dados
                IniciarLista(&relacao_candidatos_lista);
                IniciarFila (&relacao_candidatos_fila);
                Inicializa(&Arvore_inscricao);
                Inicializa(&Arvore_nomes);

                // Inicializa as tabelas hash
                for( i=0; i<MAX; i++ ){
                    IniciarLista(&Hash_inscricao[i]);
                    IniciarLista(&Hash_nome[i]);
                }

                // Armazenando as configura��es do novo concurso
                fflush(stdin);
                system("color 0f");
                printf("\n Preencha os dados abaixo: \n");
                printf("\n Promotor do concurso: ");
                scanf("%20[^\n]", &promotor_concurso); fflush(stdin);
                printf(" Data de realizacao do concurso: ");
                scanf("%15[^\n]", &data); fflush(stdin);
                printf(" Cargo: ");
                scanf("%20[^\n]",nome_cargo); fflush(stdin);
                printf("\n Numero de vagas: ");
                numero_vagas = VerificaInteiro(numero_vagas);fflush(stdin);
                printf("\n Numero de Questoes: ");
                numero_questoes = VerificaInteiro(numero_questoes);fflush(stdin);
                printf("\n\xc9");
                for(i=0;i<20;i++)
                    printf("\xcd");
                printf("\xbb\n");
                printf("\xba\xfa\xfaGabarito Oficial\xfa\xfa\xba\t\n");
                printf("\xc8");
                for(i=0;i<20;i++)
                    printf("\xcd");
                printf("\xbc\n");

                // Alocando espa�o para armazenar o gabarito oficial
                gabarito_oficial = (char*) malloc ( numero_questoes*sizeof(char) );

                // Armazenando o gabarito oficial
                for( i=0; i<numero_questoes; i++ ){
                    fflush(stdin);
                    printf(" \n\t Resposta %i: ", i+1);
                    gabarito_oficial[i] = VerificaCaracter(gabarito_oficial[i]);
                }

                // Salvando em arquivo as configura��es do concurso, os dados ser�o sempre salvos
                // no final do arquivo, e o concurso n�o poder� ser acessado se n�o for o ultimo
                if( (configuracao_concurso = fopen("Configuracao_concurso.txt", "a")) !=NULL ){

                   fprintf( configuracao_concurso, "#");
                   fprintf( configuracao_concurso, "\n%s", promotor_concurso);
                   fprintf( configuracao_concurso, "\n%s", data);
                   fprintf( configuracao_concurso, "\n%s ", nome_cargo);
                   fprintf( configuracao_concurso, "\n%d", numero_vagas );
                   fprintf( configuracao_concurso, "\n%d\n", numero_questoes);
                   for( j=0; j< numero_questoes; j++){
                        fprintf( configuracao_concurso, "%c\n", gabarito_oficial[j]);
                   }
                   fprintf( configuracao_concurso, "%d\n", verifica_concurso);
                   fclose(configuracao_concurso);
                }

                else{
                    printf("\n Ocorreu um erro inesperado, os dados nao puderam ser salvos. \n");
                }
            }

        // Caso o usu�rio opte por continuar um concurso, as op��es j� estar�o dispon�veis
        // pois os dados s�o automaticamente carregados no in�cio do programa. Ent�o,
        // basta que ele continue normalmente a utilizar o programa, passando para as op��es
        // seguintes.
        NovaTecla();
        }

        //=================== OPCAO 2: Realizar uma inscricao ============
        if( op == 2){
            // caso nao haja um concurso aberto
            if(verifica_concurso==0){
                system("cls");
                printf("\n  \xc9");
                for(i=0;i<69;i++)
                    printf("\xcd");
                printf("\xbb\n");
                printf("  \xba\t\t\t\t\t\t\t\t\t\xba\n");
                printf("  \xba E necessario que um concurso esteja em andamento para inscrever-se. \xba\n");
                printf("  \xba\t\t\t\t\t\t\t\t        \xba\n");
                printf("  \xc8");
                for(i=0;i<69;i++)
                    printf("\xcd");
                printf("\xbc\n");
                NovaTecla();
                continue;
            }

            // caso haja um concurso em andamento
            do{
                op=2;
                (ficha_pessoal.numero_de_inscricao)+=1;
                (ficha_pessoal.pontuacao)=0;

                system("cls");
                strcpy(ficha_pessoal.cargo, nome_cargo);
                printf("\n\n Cargo: %s ", ficha_pessoal.cargo);
                printf("\n Digite: \n\n Nome Completo: "); fflush(stdin);
                scanf("%50[^\n]",ficha_pessoal.nome); fflush(stdin);
                printf("\n Idade: ");fflush(stdin);
                ficha_pessoal.idade = VerificaInteiro(ficha_pessoal.idade); fflush(stdin);
                printf("\n\n Cada questao possui apenas uma resposta,\n compreendida entre: a, b, c, d, e.");
                printf("\n Uma questao em branco ou outra resposta\n qualquer invalidara a mesma. \n\n");
                printf("Digite o seu gabarito: ");

                // Alocando espa�o para armazenar o gabarito do candidato
                ficha_pessoal.gabarito = (char*) malloc (numero_questoes*sizeof(char));

                // Armazenando o gabarito do candidato
                for( i=0; i<numero_questoes; i++ ){
                    fflush(stdin);
                    printf(" \n\t Questao %i: ", i+1);
                    scanf("%c", &ficha_pessoal.gabarito[i]);
                    fflush(stdin);
                    ficha_pessoal.gabarito[i] = VerificarGabaritoCandidato(ficha_pessoal.gabarito[i]);
                }
                NovaTecla();

                system("cls");

                // Confirma��o dos dados

                printf("\n Cargo: %s\t\t",ficha_pessoal.cargo);
                printf("\n Nome: %s",ficha_pessoal.nome);
                printf("\n Idade: %d \n Numero de inscricao: %d ",ficha_pessoal.idade,ficha_pessoal.numero_de_inscricao);
                printf("\n Gabarito do Candidato: \n");

                for( i=0; i<numero_questoes; i++ ){
                        printf("  %c  ", ficha_pessoal.gabarito[i] );
                }

                printf("\n\n\n\n\n\n  \xc9");
                for(i=0;i<33;i++)
                    printf("\xcd");
                printf("\xbb\n");
                printf("  \xba Deseja confirmar sua inscricao? \xba\n  \xba\t\t\t\t    \xba\n  \xba   [1] - Sim\t\t\t    \xba\n  \xba   [2] - Nao\t\t\t    \xba\n");
                printf("  \xc8");
                for(i=0;i<33;i++)
                    printf("\xcd");
                printf("\xbc\n");
                printf("  Opcao: ");

                scanf("%d",&op); fflush(stdin);
            }while(op==2);

            //  Corrigir provas, ou seja, calcular pontua�oes e modificar estes valores na lista
            // Lembrando que:
            //- cada quest�o certa o candidato ganha 3 pontos;
            //- cada quest�o errada o candidato perde 1 ponto;
            //- cada quest�o em branco o candidato n�o perde nem ganha pontos.
            ficha_pessoal.pontuacao = CompararGabarito ( ficha_pessoal.gabarito, gabarito_oficial, numero_questoes );

            // Preenche uma celula da lista com os dados do candidato
            InserirCelula(&relacao_candidatos_lista, ficha_pessoal);

            // Preenche as arvores com os dados dos candidatos
            Insere(ficha_pessoal, &Arvore_inscricao);
            Insere_nome(ficha_pessoal, &Arvore_nomes);

            // Preenche as tabelas hash com os dados
            posicao = CalculoPosicao(ficha_pessoal.numero_de_inscricao);
            InserirCelula(&Hash_inscricao[posicao], ficha_pessoal);
            posicao=0;

            posicao= CalculoPosicaoNome(ficha_pessoal);
            InserirCelula(&Hash_nome[posicao], ficha_pessoal);
            posicao=0;

            //Grava os dados do candidato em arquivo
            GravarArquivoLista( ficha_pessoal, numero_questoes );
            printf("\n\n\n\t  Inscricao concluida com exito! ");
            printf("\n\n\t  Seu cadastro foi salvo com sucesso! \n\n");
            NovaTecla();
        }

        // ========= OP��O 3: Encerrar o concurso =====================
        if ( op == 3 ){
        // Caso nao haja nenhuma inscricao para o concurso
            if(TesteListaVazia(relacao_candidatos_lista)== 1 ){
                c=99; // 99 = lista vazia
            }
            else{
              c=11;
            }
            verifica_concurso=0;
            // caso nao haja inscri�oes nada � salvo.
            if( c==11){
                // Salva o numero do ultimo concurso
                if( (ultimo_concurso = fopen("ultimo_concurso.txt","w+") ) != NULL ){
                    fprintf(ultimo_concurso, "%d", numero_concurso);
                    fclose(ultimo_concurso);
                }
                else{
                    printf("\n Ocorreu um erro inesperado. \n");
                    NovaTecla();
                    continue;
                }

                //Salva uma # depois de salvar o ultimo cadastro do concurso
                if((arquivo2 =fopen("Ficha dos Candidatos.txt","a"))!=NULL){
                    fprintf(arquivo2, "#");
                    fclose(arquivo2);
                }
                else{
                    printf("\n Ocorreu um erro inesperado. \n");
                    NovaTecla();
                    continue;
                }

                /****  Ordenar a lista: Quicksort ****/
                c = ContarCelulas( relacao_candidatos_lista);
                QuickSortLista( &relacao_candidatos_lista, c);

                // Salvar um historico dos candidatos aprovados
                if( (arq_fila = fopen("Lista_de_aprovados.txt","a+")) != NULL){
                  fprintf(arq_fila, "\n\n\t -- Concurso: %d -- \n\n", numero_concurso);
                  system("pause");
                //  Inserir os melhores colocados na fila, lembrando que sao 3 vezes o numero de vagas
                for(k=0; (k<(numero_vagas*3) ) && (TesteListaVazia(relacao_candidatos_lista)==0 ); k++){
                    RetirarCelula(&relacao_candidatos_lista, &X);
                    InserirCelulaFila(&relacao_candidatos_fila, X);

                    //  Salvar dados em arquivo
                    if( (arq_fila = fopen("Lista_de_aprovados.txt","a+")) != NULL){
                        fprintf(arq_fila,"\tCargo: %s\n", nome_cargo);
                        fprintf(arq_fila,"\tCandidato: %s",X.nome);
                        fprintf(arq_fila,"\n\tIdade: %d",X.idade);
                        fprintf(arq_fila,"\n\tInscricao: %d\n",X.numero_de_inscricao);
                        fprintf(arq_fila, "\tPontuacao: %d\n", X.pontuacao);
                        fprintf(arq_fila, "\tGabarito: \n");
                        for(i=0; i<numero_questoes; i++ ){
                            fprintf(arq_fila, "\t%c\n", X.gabarito[i] );
                        }
                        fprintf(arq_fila, "\n");
                    }
                }
                fclose(arq_lista);
                printf("\n\n\t A lista de aprovados ja esta disponivel na opcao 4 do menu principal. \n\n");
                NovaTecla();
            }
            else if(c==99){
              printf("\n\n\t O concurso foi encerrado, mas nao houveram inscricoes para ele. \n\n");
              NovaTecla();
            }
          }
          verifica_concurso=0;
        }

        // === OP��O 4: Visualizar lista de Aprovados =================
        if ( op == 4 ){
            system("cls");
            // Imprime os dados da fila
            printf(" \n\n\n\t\t -------- Lista de Aprovados --------\n\n\n");
            printf("Concurso %d\n\n", numero_concurso);
            ImprimeFila(relacao_candidatos_fila);
            NovaTecla();
        }

        // === OP��O 5: Visualizar lista de Espera =================
        if ( op == 5 ){
            system("cls");
            // Imprime os dados da lista
            printf(" \n\n\n\t\t -------- Lista de Espera --------\n\n\n");
            ImprimeLista(relacao_candidatos_lista);
            NovaTecla();
        }

        // === OP��O 6: Visualizar os historicos de arquivo  =================
        if ( op == 6 ){

            if( (arq_fila = fopen("Lista_de_aprovados.txt","r")) != NULL){
              system("cls");
              printf("\n\n\t LISTA DE APROVADOS \n\n");
              while ( (c = fgetc(arq_fila) ) != EOF ){
                putchar(c);
              }
              NovaTecla();
            }
            else{
              printf("\n\n Os dados nao estao disponiveis. \n\n\n");
              NovaTecla();
            }
        }

        // === OPCAO 7: Pesquisar candidato por nome ==================
        if ( op == 7 ){
            system("cls");
            printf("\n Selecione a estrutura que deseja utilizar: \n\t 1] Arvore SBB \t 2] HASH\n\t Opcao: ");
            // Confere se a opcao foi valida
            while(1){
                if( scanf("%d",&select) == 0){
                    printf("\n Opcao invalida, digite novamente. \n");
                    NovaTecla();
                    ImprimeMenu();
                    continue;
                }
                break;
            }
            // Caso pesquisa em arvore binaria SBB
            if (select == 1 ){
                printf("\n\n\n Digite o nome que deseja encontrar: "); fflush(stdin);
                scanf("%50[^\n]",X.nome); fflush(stdin);
                Pesquisa_nome(&X, &Arvore_nomes, numero_questoes, &achou);
                if (achou==1){
                    printf("\n\n Deseja remove-lo? 1[sim]   2[nao]\n\t Opcao: ");
                    scanf("%d", &var); fflush(stdin);

                    // Retira o nome de todas as estruturas
                    if( var == 1 ){
                        Retira_nome(X, &Arvore_nomes);
                        Retira(X, &Arvore_inscricao);
                    }
                    else{
                        continue;
                    }
                    NovaTecla();
                }
            }

            // Caso pesquisa na tabela HASH
            else if( select == 2){
                system("cls");
                printf("\n Digite o nome que deseja pesquisar: "); fflush(stdin);
                scanf("%50[^\n]",ficha_pessoal.nome); fflush(stdin);

                posicao=CalculoPosicaoNome(ficha_pessoal);
                PesquisaHashNome(&Hash_nome[posicao], &ficha_pessoal, numero_questoes);
                posicao=0;
            }
            else{
                printf("\n\n\t Opcao invalida. \n");
                NovaTecla();
            }
        }

        // === OPCAO 8: Pesquisar candidato por numero de inscricao ===
        if (op == 8 ){
            system("cls");
            printf("\n Selecione a estrutura que deseja utilizar: \n\t 1] Arvore SBB \t 2] HASH\n\t Opcao: ");
            // Confere se a opcao foi valida
            while(1){
                if( scanf("%d",&select) == 0){
                    printf("\n Opcao invalida, digite novamente. \n");
                    NovaTecla();
                    ImprimeMenu();
                    continue;
                }
                break;
            }
            // Caso pesquisa em arvore binaria SBB
            if (select == 1 ){
                printf("\n\n\n Digite o numero de inscricao que deseja encontrar: ");
                scanf("%d", &X.numero_de_inscricao); fflush(stdin);
                Pesquisa(&X, &Arvore_inscricao, numero_questoes, &achou);
                if(achou==1){
                    printf("\n\n Deseja remove-lo? 1[sim]   2[nao]\n\t Opcao: ");
                    scanf("%d", &var); fflush(stdin);

                    // Retira de todas as estruturas
                    if( var == 1 ){
                        Retira(X, &Arvore_inscricao);
                        Retira_nome(X, &Arvore_nomes);
                    }
                    else{
                        continue;
                    }
                    NovaTecla();
                }
            }

            // Caso pesquisa na tabela HASH
            else if( select == 2){
                system("cls");
                printf("\n Digite o numero de inscricao que deseja encontrar: ");
                op=VerificaInteiro(op);
                ficha_pessoal.numero_de_inscricao = op;

                posicao=CalculoPosicao(op);
                PesquisaHash(&Hash_inscricao[posicao], &ficha_pessoal, numero_questoes);
                posicao=0;
                continue;
            }
            else{
                printf("\n\n\t Opcao invalida. \n");
                NovaTecla();
            }
        }

        // === OP��O 9: Sair do Programa  =================
        if ( op == 9 ){
                break;
        }

        // === DEFAULT ================
        if( op <1 || op >9 ){
            system("cls");
            printf(" \n\n Opcao invalida, digite novamente. \n\n");
            NovaTecla();
        }

    }while(1);

    return(0);
}

/*****************************************************************/
/******************* CORPO DAS FUN��ES **************************/
/****************************************************************/

//                          FUN��ES DE LISTA

void IniciarLista( TipoLista *Lista ){
    Lista -> primeiro = (Apontador)malloc(sizeof(Celula));
    Lista -> primeiro -> prox = NULL;
    Lista -> primeiro -> ant = NULL;
    Lista -> ultimo = Lista -> primeiro;
}

int TesteListaVazia( TipoLista Lista ){
    return( Lista.primeiro == Lista.ultimo);
}

void InserirCelula(TipoLista *Lista ,candidato X){
    Apontador aux;

    aux = Lista -> ultimo;
    Lista -> ultimo -> prox = (Apontador)malloc(sizeof(Celula));
    Lista -> ultimo = Lista -> ultimo-> prox;
    Lista -> ultimo -> ant = aux;
    Lista -> ultimo ->item = X;
    Lista -> ultimo -> prox = NULL;
}

// Fun��o que chama um candidato da lista de espera do vestibular
void RetirarCelula( TipoLista *Lista, candidato *X){
    Apontador aux, aux2; // s�o ponteiros auxiliares para percorrer a lista

    if(TesteListaVazia(*Lista) == 1){
        system("color 4f");
        printf("\n\n Erro: Lista Vazia!\n\n");
        return;
    }

    aux = Lista -> primeiro -> prox;

    // Se s� houver uma c�lula al�m da "cabe�a", quer dizer que seu prox � NULL
    if( aux -> prox == NULL ){
        *X = aux -> item;
        Lista -> ultimo = Lista -> primeiro;
        free(aux);
        return;
    }

    aux2 = aux -> prox;
    *X = aux -> item;
    Lista -> primeiro -> prox = aux -> prox;
    aux2 -> ant = aux -> ant;
    free(aux);
}


void ImprimeLista ( TipoLista Lista ){
    Apontador aux;

    if(TesteListaVazia(Lista) == 1){
        system("color 4f");
        printf("\n\n Erro: Lista Vazia!\n\n");
        return;
    }

    aux = Lista.primeiro -> prox;

    while ( aux != NULL ){
        printf("\n Inscricao: %d \n Candidato: %s \t Idade: %d \t Pontuacao: %d \n\n", aux->item.numero_de_inscricao, aux->item.nome, aux->item.idade, aux->item.pontuacao);
        aux = aux-> prox;
    }

}

int Desalocar( TipoLista *Lista, TipoFila *Fila, int *verifica_concurso, ApontadorNo *Arvore_inscricao, ApontadorNo *Arvore_nomes ){
    Apontador aux, aux2;
    ApontadorFila aux3, aux4;
    int op,i;

    if( (TesteListaVazia(*Lista) == 1) && (TesteFilaVazia( *Fila ) == 1) )
        return(1);

//==
    if((TesteListaVazia(*Lista) == 0) && ( (TesteFilaVazia( *Fila ) == 0) || (TesteFilaVazia(*Fila) == 1) )){

        system("color 4f");
        printf("       \xc9");
        for(i=0;i<65;i++){
        printf("\xcd");
        }
        printf("\xbb\n");
        printf("       \xba\t     Ha um concurso em andamento.\t\t\t \xba\n");
        printf("       \xba   Se optar por iniciar outro concurso os dados serao apagados.  \xba\n");
        printf("       \xba\t\t\t\t\t\t\t\t \xba");
        printf("   \n       \xba\t    Deseja continuar?\t\t\t\t\t \xba\n");
        printf("       \xba\t\t\t\t\t\t\t\t \xba");
        printf("\n       \xba\t    (1)Sim \t (2)Nao \t\t\t\t \xba");
        printf("             \xc8");
        for(i=0;i<65;i++)
            printf("\xcd");
        printf("\xbc\n");
        printf("         Opcao: ");

        while( scanf("%d",&op) == 0 ){
            fflush(stdin);
            printf("\n Digite novamente: ");
        }

        // Opcao para desalocar as estruturas
        if( op == 1 ){

            *verifica_concurso=0;
            // Parte para desalocar lista INICIO
            aux = Lista->ultimo;
            aux2 = aux -> ant;

            while ( aux2 != Lista -> primeiro ){
                Lista -> ultimo = aux2;
                aux2 = aux2 -> ant;
                free(aux);
                aux = Lista -> ultimo;
            }
            Lista-> ultimo = aux2;
            free(aux);
            aux2 = NULL;

// Parte para desalocar lista FIM
//==================================
// Parte para desalocar fila INICIO

            aux3 = Fila -> primeiroFila;
            aux4 = aux3 -> prox;

            if( aux4!=NULL ){
                aux3 -> prox = aux4 -> prox;
                free(aux4);
                while( aux4 -> prox != NULL ){
                    system("color 4f");
                    printf("\n A fila esta vazia! \n");
                    return 0;
                }
                aux4 = aux4 -> prox;
            }
            Fila -> primeiroFila = NULL;
            Fila -> ultimoFila = NULL;

            // Parte para desalocar fila FIM
            //==============================
            // Parte de desalocar as �rvores SBB inicio
            DesalocaArvore(Arvore_inscricao);
            DesalocaArvore(Arvore_nomes);
            // Parte de desalocar as �rvores SBB fim

            return(1);
        }

        if( op == 2 ){
          *verifica_concurso=1;
            return(0);
        }
    }

}

/*****************************************************************/
//                   FUN��ES DE FILA

void IniciarFila ( TipoFila *Fila ){
    Fila->primeiroFila = (ApontadorFila) malloc(sizeof(CelulaFila));
    Fila->ultimoFila = Fila->primeiroFila;
    Fila->primeiroFila->prox = NULL;
}

int TesteFilaVazia( TipoFila Fila ){
    return ( Fila.primeiroFila == Fila.ultimoFila );
}


void InserirCelulaFila( TipoFila *Fila, candidato x){
  Fila-> ultimoFila -> prox = (ApontadorFila) malloc(sizeof(CelulaFila));
  Fila-> ultimoFila = Fila -> ultimoFila -> prox;
  Fila-> ultimoFila -> itemFila = x;
  Fila-> ultimoFila -> prox = NULL;
}

void RetirarCelulaFila( TipoFila *Fila ){
  ApontadorFila aux,aux2;

  if (TesteFilaVazia( *Fila ) ){
     system("cls");
     system("color 4f");
     printf("\n\n Erro: A fila esta  vazia! \n");
     return;
  }

  aux = Fila->primeiroFila;
  aux2= aux -> prox;

  if( aux2!=NULL ){
        aux -> prox = aux2 -> prox;
        free(aux2);
        if( aux->prox != NULL ){
            system("color 4f");
            printf("\n\n Erro: A fila esta vazia! \n");
            return;
        }
        aux2 = aux -> prox;
  }
}


void ImprimeFila( TipoFila Fila ){
  ApontadorFila aux;
  int i;

  if (TesteFilaVazia( Fila ) ){
     system("cls");
     system("color 4f");
     printf("\n\n Erro: A fila esta  vazia! \n");
     return;
  }

  aux = Fila.primeiroFila->prox;

  while (aux != NULL){
      printf("\n Inscricao: %d \n Candidato: %s \t Idade: %d \t Pontuacao: %d \n\n", aux->itemFila.numero_de_inscricao, aux->itemFila.nome, aux->itemFila.idade, aux->itemFila.pontuacao);
      aux = aux->prox;
  }
}


//======================================================================
//================ FUN��ES PARA LAYOUT E OUTRAS ========================

void ImprimeMenu(void){
    int i;
    system("color 0f");
    printf("\n\n");
    printf("            \xc9");
    for(i=0;i<56;i++)
        printf("\xcd");
    printf("\xbb\n");
    printf("\t    \xba\t\t\t\t\t\t\t     \xba\n");
    printf("\t    \xba       \xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa MENU PRINCIPAL \xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\t     \xba\n");
    printf("\t    \xba\t\t\t\t\t\t\t     \xba\n");
    printf("\t    %c",204);
    for(i=0;i<56;i++)
        printf("\xcd");
    printf("%c\n",185);
    printf("\t    \xba\t\t\t\t\t\t\t     \xba\n");
    printf("\t    \xba\t\t\t\t\t\t\t     \xba\n");
    printf("\t    \xba       1) Cadastrar concurso                            \xba\n");
    printf("\t    \xba       2) Inscreva-se                                   \xba\n");
    printf("\t    \xba       3) Corrigir provas e encerrar concurso           \xba\n");
    printf("\t    \xba       4) Imprimir lista de aprovados                   \xba\n");
    printf("\t    \xba       5) Imprimir lista de espera                      \xba\n");
    printf("\t    \xba       6) Imprimir historico dos aprovados              \xba\n");
    printf("\t    \xba       7) Pesquisar candidatos por nome                 \xba\n");
    printf("\t    \xba       8) Pesquisar candidatos por numero de inscricao  \xba\n");
    printf("\t    \xba       9) Sair                                          \xba\n");
    printf("\t    \xba\t\t\t\t\t\t\t     \xba\n");
    printf("\t    \xba\t\t\t\t\t\t\t     \xba\n");
    printf("\t    \xba\t\t\t\t\t\t\t     \xba\n");
    printf("            \xc8");
    for(i=0;i<56;i++)
        printf("\xcd");
    printf("\xbc\n");
    printf("\n\t        Opcao: ");

}

void NovaTecla(void){
    printf("\n\n  Pressione qualquer tecla . . .");
    getch();
    fflush(stdin);
    system("cls");
}

int VerificaInteiro( int var ){
    while(1){
            if( scanf("%d",&var) == 0){
                printf("\n\n Opcao invalida, digite novamente..\n");
                fflush(stdin);
                continue;
            }
            else if(var ==0){
                printf("\n\n Opcao invalida, digite novamente.. \n");
                continue;
            }
            return(var);
    }
}

char VerificaCaracter( char var){
    scanf("%c", &var ); fflush(stdin);
    if( var == 'a' || var == 'b' || var == 'c' ||var == 'd' || var == 'e' ){
           return(var);
       }
    if( var == 'A' )
        return('a');
    if( var == 'B' )
        return('b');
    if( var == 'C' )
        return('c');
    if( var == 'D' )
        return('d');
    if( var == 'E' )
        return('e');
    else{
            printf("\n\n Opcao invalida, digite novamente..\n");
            VerificaCaracter ( var );
        }
}


void GravarArquivoLista(candidato x, int n_questoes ){
    FILE *arquivo;
    candidato ficha_pessoal;
    int i;

    fflush(stdin);

    if((arquivo=fopen("Ficha dos Candidatos.txt","a"))!=NULL){

        fprintf(arquivo,"\n%s",x.nome);
        fprintf(arquivo,"\n%d",x.idade);
        fprintf(arquivo,"\n%s ",x.cargo);
        fprintf(arquivo,"\n%d\n",x.numero_de_inscricao);
        fprintf(arquivo, "%d\n", x.pontuacao);
        for(i=0; i<n_questoes; i++ ){
            fprintf(arquivo, "%c\n", x.gabarito[i] );
        }

        fclose(arquivo);
        return;
    }
    printf("\n Ocorreu um erro inesperado e sua inscricao nao pode ser salva.\n");
    NovaTecla();
    return;
}

char VerificarGabaritoCandidato( char gabarito ){
    if( gabarito == 'a' || gabarito == 'A' )
           return('a');
    else if( gabarito == 'b' || gabarito == 'B' )
           return('b');
    else if( gabarito =='c' || gabarito == 'C' )
           return('c');
    else if( gabarito == 'd' || gabarito == 'D' )
           return('d');
    else if( gabarito == 'e' || gabarito == 'E' )
           return('e');
    else
            return (' ');
}


int CompararGabarito ( char gabarito_candidato[], char gabarito_oficial[], int n_quetoes ){
    int i;
    int pontuacao=0;

    for(i=0; i<n_quetoes; i++ ){

        if( gabarito_candidato[i] == ' ' ){
            pontuacao +=0;
            continue;
        }
        if( gabarito_candidato[i] == gabarito_oficial[i] ){
            pontuacao += 3;
            continue;
        }
        if( gabarito_candidato[i] != gabarito_oficial[i]){
            pontuacao -= 1;
            continue;
        }
    }

    if( pontuacao < 0){
        return(0);
    }
    return(pontuacao);
}

//================================================================================
//====================== Fun��es para o Quicksort ================================

int ContarCelulas ( TipoLista Lista){
    int i=0;
    Apontador aux;
    aux= Lista.primeiro->prox;
    while( aux!=NULL ){
        i++;
        aux=aux->prox;
   }
   return(i);
}

void QuickSortLista( TipoLista *Lista, int tamanho){
    Ordena( 1, tamanho,Lista);
}

void Ordena( int esq, int dir, TipoLista *Lista ){
    int i, j;

    Particao( esq, dir, &i, &j, Lista );

    if( esq<j){
        Ordena( esq, j, Lista);
    }
    if(i<dir){
        Ordena(i, dir, Lista);
    }
}


void Particao(int esq, int dir, int *i, int *j, TipoLista *Lista){

    int val1, val2;
    Apontador aux, aux2, aux_pivo;
    int pivo, prim,ulti, piv;
    candidato tmp;

    *i=esq;
    *j=dir;

    // Inicializa a posicao dos ponteiros
    aux=Lista->primeiro->prox;
    aux2= Lista->primeiro->prox;

    //Faz a busca para chegar na cell 'i'
    prim=1;
    while(prim != (*i)){
            aux=aux->prox;
            prim++;
    }

    // Faz a busca para chegar na cell 'j'
    ulti=1;
    while(ulti!=(*j)){
            aux2=aux2->prox;
            ulti++;
    }

    // Calcula a posicao do pivo
    pivo = ( *i +*j )/2;

    piv=*i;
    aux_pivo = aux;
    while((piv!=pivo)){
            aux_pivo = aux_pivo ->prox;
            piv++;
        }

    pivo=aux_pivo->item.pontuacao;

    // Compara��es
    do{

        // Confere se o item da esquerda � maior que o pivo
        while(pivo < aux->item.pontuacao){
            (*i)++;
            aux=aux->prox;
        }

        // Confere se o item da direita � menor que o pivo
        while(pivo > aux2->item.pontuacao){
            (*j)--;
            aux2= aux2->ant;
        }

        // se o da direita for menor que o da esquerda, troca.
        if( (*i)<=(*j)){
            tmp = aux->item;
            aux->item = aux2->item;
            aux2->item = tmp;

            // o da direita passa pro proximo da esquerda
            (*i)++;
            aux=aux->prox;

            // o da esquerda volta para o anterior da direita
            (*j)--;
            aux2= aux2->ant;
        }

    }while((*i)<=(*j));

}

//=====================================================================
//===================== FUNCOES DE ARVORE SBB =========================

void DesalocaArvore(ApontadorNo *No){
    if((*No) == NULL){
       return;
    }
    DesalocaArvore(&((*No)->Esq));
    DesalocaArvore(&((*No)->Dir));
    free(*No);
    return;
}

void EE(ApontadorNo *Ap) {
  ApontadorNo Ap1;

  Ap1 = (*Ap)->Esq;
  (*Ap)->Esq = Ap1->Dir;
  Ap1->Dir = *Ap;
  Ap1->BitE = Vertical;
  (*Ap)->BitE = Vertical;
  *Ap = Ap1;
}

void ED(ApontadorNo *Ap) {
  ApontadorNo Ap1, Ap2;

  Ap1 = (*Ap)->Esq;
  Ap2 = Ap1->Dir;
  Ap1->BitD = Vertical;
  (*Ap)->BitE = Vertical;
  Ap1->Dir = Ap2->Esq;
  Ap2->Esq = Ap1;
  (*Ap)->Esq = Ap2->Dir;
  Ap2->Dir = *Ap;
  *Ap = Ap2;
}

void DD(ApontadorNo *Ap) {
  ApontadorNo Ap1;

  Ap1 = (*Ap)->Dir;
  (*Ap)->Dir = Ap1->Esq;
  Ap1->Esq = *Ap;
  Ap1->BitD = Vertical;
  (*Ap)->BitD = Vertical;
  *Ap = Ap1;
}

void DE(ApontadorNo *Ap) {
  ApontadorNo Ap1, Ap2;

  Ap1 = (*Ap)->Dir;
  Ap2 = Ap1->Esq;
  Ap1->BitE = Vertical;
  (*Ap)->BitD = Vertical;
  Ap1->Esq = Ap2->Dir;
  Ap2->Dir = Ap1;
  (*Ap)->Dir = Ap2->Esq;
  Ap2->Esq = *Ap;
  *Ap = Ap2;
}

void Inicializa(ApontadorNo *Arvore) {
  *Arvore = NULL;
}

void EsqCurto(ApontadorNo *Ap, short *Fim) {
  /* Folha esquerda retirada => arvore curta na altura esquerda */
  ApontadorNo Ap1;
  if ((*Ap)->BitE == Horizontal) {
	(*Ap)->BitE = Vertical;
	*Fim = TRUE;
	return;
  }
  if ((*Ap)->BitD == Horizontal) {
	Ap1 = (*Ap)->Dir;
    (*Ap)->Dir = Ap1->Esq;
    Ap1->Esq = *Ap;
    *Ap = Ap1;
    if ((*Ap)->Esq->Dir->BitE == Horizontal) {
    	DE(&(*Ap)->Esq);
    	(*Ap)->BitE = Horizontal;
	}
    else
      if ((*Ap)->Esq->Dir->BitD == Horizontal) {
      	DD(&(*Ap)->Esq);
      	(*Ap)->BitE = Horizontal;
	  }
    *Fim = TRUE;
    return;
  }
  (*Ap)->BitD = Horizontal;
  if ((*Ap)->Dir->BitE == Horizontal) {
	DE(Ap);
	*Fim = TRUE;
	return;
  }
  if ((*Ap)->Dir->BitD == Horizontal) {
	DD(Ap);
	*Fim = TRUE;
  }
}

void DirCurto(ApontadorNo *Ap, short *Fim) {
  /* Folha direita retirada => arvore curta na altura direita */
  ApontadorNo Ap1;
  if ((*Ap)->BitD == Horizontal) {
	(*Ap)->BitD = Vertical;
	*Fim = TRUE;
	return;
  }
  if ((*Ap)->BitE == Horizontal) {
  	Ap1 = (*Ap)->Esq;
  	(*Ap)->Esq = Ap1->Dir;
  	Ap1->Dir = *Ap;
  	*Ap = Ap1;
    if ((*Ap)->Dir->Esq->BitD == Horizontal) {
		ED(&(*Ap)->Dir);
		(*Ap)->BitD = Horizontal;
	}
    else
      if ((*Ap)->Dir->Esq->BitE == Horizontal) {
		EE(&(*Ap)->Dir);
		(*Ap)->BitD = Horizontal;
	  }
    *Fim = TRUE;
    return;
  }
  (*Ap)->BitE = Horizontal;
  if ((*Ap)->Esq->BitD == Horizontal) {
  	ED(Ap);
  	*Fim = TRUE;
  	return;
  }
  if ((*Ap)->Esq->BitE == Horizontal) {
  	EE(Ap);
  	*Fim = TRUE;
  }
}

void Antecessor(ApontadorNo q, ApontadorNo *r, short *Fim) {
  if ((*r)->Dir != NULL) {
  	Antecessor(q, &(*r)->Dir, Fim);
    if (!*Fim)
		DirCurto(r, Fim);
    return;
  }
  q->Reg = (*r)->Reg;
  q = *r;
  *r = (*r)->Esq;
  free(q);
  if (*r != NULL)
    *Fim = TRUE;
}

// ============= SBB PARA PESQUISA POR NUMERO DE INSCRICAO ===============

void IRetira(candidato x, ApontadorNo *Ap, short *Fim) {
  No *Aux;
  if (*Ap == NULL) {
    system("color 4f");
	printf("\n\n\t Numero de inscricao invalido ou inexistente. \n");
	NovaTecla();
	*Fim = TRUE;
	return;
	}
  if (x.numero_de_inscricao < (*Ap)->Reg.numero_de_inscricao) {
  	IRetira(x, &(*Ap)->Esq, Fim);
    if (!*Fim)
		EsqCurto(Ap, Fim);
    return;
  }
  if (x.numero_de_inscricao > (*Ap)->Reg.numero_de_inscricao) {
  	IRetira(x, &(*Ap)->Dir, Fim);
    if (!*Fim)
		DirCurto(Ap, Fim);
    return;
  }
  *Fim = FALSE;
  Aux = *Ap;
  if (Aux->Dir == NULL) {
	*Ap = Aux->Esq;
    free(Aux);
    if (*Ap != NULL)
      *Fim = TRUE;
    return;
  }
  if (Aux->Esq == NULL) {
	*Ap = Aux->Dir;
	free(Aux);
    if (*Ap != NULL)
	  *Fim = TRUE;
    return;
  }
  Antecessor(Aux, &Aux->Esq, Fim);
  if (!*Fim)
	EsqCurto(Ap, Fim); /* Encontrou chave */
}

void IInsere(candidato x, ApontadorNo *Ap, Inclinacao *IAp, short *Fim) {
  if (*Ap == NULL) {
  	*Ap = (ApontadorNo)malloc(sizeof(No));
    *IAp = Horizontal;
    (*Ap)->Reg = x;
    (*Ap)->BitE = Vertical;
    (*Ap)->BitD = Vertical;
    (*Ap)->Esq = NULL;
    (*Ap)->Dir = NULL;
    *Fim = FALSE;
    return;
  }
  if (x.numero_de_inscricao < (*Ap)->Reg.numero_de_inscricao) {
  	IInsere(x, &(*Ap)->Esq, &(*Ap)->BitE, Fim);
    if (*Fim)
		return;
    if ((*Ap)->BitE != Horizontal) {
    	*Fim = TRUE;
    	return;
	}
    if ((*Ap)->Esq->BitE == Horizontal) {
    	EE(Ap);
    	*IAp = Horizontal;
    	return;
	}
    if ((*Ap)->Esq->BitD == Horizontal) {
    	ED(Ap);
    	*IAp = Horizontal;
	}
    return;
  }
  IInsere(x, &(*Ap)->Dir, &(*Ap)->BitD, Fim);
  if (*Fim)
	return;
  if ((*Ap)->BitD != Horizontal) {
  	*Fim = TRUE;
  	return;
  }
  if ((*Ap)->Dir->BitD == Horizontal) {
  	DD(Ap);
  	*IAp = Horizontal;
  	return;
  }
  if ((*Ap)->Dir->BitE == Horizontal) {
  	DE(Ap);
  	*IAp = Horizontal;
  }
}

void Insere(candidato x, ApontadorNo *Ap) {
  short Fim;
  Inclinacao IAp;
  IInsere(x, Ap, &IAp, &Fim);
}

void Retira(candidato x, ApontadorNo *Ap) {
  short Fim;
  IRetira(x, Ap, &Fim);
}

void Pesquisa(candidato *x, ApontadorNo *p, int numero_questoes, int *achou) {
    int i;
  if (*p == NULL) {
    system("color 4f");
  	printf("\n\n\t Erro: Registro nao encontrado! \n");
  	NovaTecla();
    achou=0;
    return;
  }

  if (x->numero_de_inscricao < (*p)->Reg.numero_de_inscricao) {
  	Pesquisa(x, &(*p)->Esq, numero_questoes, achou);
    return;
  }

  if (x->numero_de_inscricao > (*p)->Reg.numero_de_inscricao)
	Pesquisa(x, &(*p)->Dir, numero_questoes, achou);
  else
    *x = (*p)->Reg;

    if( x->numero_de_inscricao == (*p)->Reg.numero_de_inscricao){
        printf("\n\n\t Registro Encontrado: \n");
        printf("\n Cargo: %s\t\t",x->cargo);
        printf("\n Nome: %s",x->nome);
        printf("\n Idade: %d \n Numero de inscricao: %d ",x->idade,x->numero_de_inscricao);
        printf("\n Gabarito do Candidato: \n");

        for( i=0; i<numero_questoes; i++ ){
            printf("  %c  ", x->gabarito[i] );
        }
        *achou=1;
        NovaTecla();
        return;
    }
}

void Testa1(ApontadorNo p, int nivel, int *NivelFolhas, short *PrimeiraFolha) {
  if (p == NULL)
	return;
  if (*PrimeiraFolha)
	if (*NivelFolhas < nivel)
	  *NivelFolhas = nivel;
	if (p->Esq == NULL && p->Dir == NULL) {
	  if (*PrimeiraFolha == TRUE)
		*PrimeiraFolha = FALSE;
	  else {
		if (nivel != *NivelFolhas) {
          system("color 4f");
		  printf("\n\n\t Erro: Folhas em niveis diferentes. \n");
		  exit(1);
		}
	  }
	}
  if (p->BitE == Horizontal)
	Testa1(p->Esq, nivel, NivelFolhas, PrimeiraFolha);
  else
    Testa1(p->Esq, nivel + 1, NivelFolhas, PrimeiraFolha);
  if (p->BitD == Horizontal)
	Testa1(p->Dir, nivel, NivelFolhas, PrimeiraFolha);
  else
    Testa1(p->Dir, nivel + 1, NivelFolhas, PrimeiraFolha);
}

void Testa2(ApontadorNo p, int *NivelFolhas, short *PrimeiraFolha) {
  if (p == NULL)
	return;
  if (p->Esq != NULL) {
  	if (p->Reg.numero_de_inscricao < p->Esq->Reg.numero_de_inscricao) {
  	  system("color 4f");
	  printf("\n Erro: %d < que filho a esquerda \n", p->Reg.numero_de_inscricao);
      exit(1);
    }
    Testa2(p->Esq, NivelFolhas, PrimeiraFolha);
  }
  if (p->Dir == NULL)
	return;
  if (p->Reg.numero_de_inscricao > p->Dir->Reg.numero_de_inscricao) {
    system("color 4f");
  	printf("\n Erro: %d > que filho a direita \n", p->Reg.numero_de_inscricao);
    exit(1);
  }
  Testa2(p->Dir, NivelFolhas, PrimeiraFolha);
}

void Testa(ApontadorNo Arvore) {
  int NivelFolhas = 0;
  short PrimeiraFolha = TRUE;
  Testa1(Arvore, 1, &NivelFolhas, &PrimeiraFolha);
  Testa2(Arvore, &NivelFolhas, &PrimeiraFolha);
}

void Em_Ordem(ApontadorNo No){
    if(No == NULL)
       return;
    Em_Ordem(No->Esq);
    printf("\n%d", No->Reg.numero_de_inscricao);
    Em_Ordem(No->Dir);
    return;
}

void Pre_Ordem(ApontadorNo No){
    if(No == NULL)
       return;
    printf("\n%d", No->Reg.numero_de_inscricao);
    Pre_Ordem(No->Esq);
    Pre_Ordem(No->Dir);
    return;
}

void Pos_Ordem(ApontadorNo No){
    if(No == NULL)
       return;
    Pos_Ordem(No->Esq);
    Pos_Ordem(No->Dir);
    printf("\n%d", No->Reg.numero_de_inscricao);
    return;
}

//====================== SBB PARA PESQUISA POR NOMES =================

void IInsere_nome(candidato x, ApontadorNo *Ap, Inclinacao *IAp, short *Fim) {
  if (*Ap == NULL) {
  	*Ap = (ApontadorNo)malloc(sizeof(No));
    *IAp = Horizontal;
    (*Ap)->Reg = x;
    (*Ap)->BitE = Vertical;
    (*Ap)->BitD = Vertical;
    (*Ap)->Esq = NULL;
    (*Ap)->Dir = NULL;
    *Fim = FALSE;
    return;
  }

  // se o primeiro nome for menor que o segundo nome
  // este ser� inserido a direita
  if (strcmpi(x.nome,(*Ap)->Reg.nome)> 0) {
  	IInsere_nome(x, &(*Ap)->Esq, &(*Ap)->BitE, Fim);
    if (*Fim)
		return;
    if ((*Ap)->BitE != Horizontal) {
    	*Fim = TRUE;
    	return;
	}
    if ((*Ap)->Esq->BitE == Horizontal) {
    	EE(Ap);
    	*IAp = Horizontal;
    	return;
	}
    if ((*Ap)->Esq->BitD == Horizontal) {
    	ED(Ap);
    	*IAp = Horizontal;
	}
    return;
  }

  // Caso o primeiro nome seja maior que o segundo
  // este sera inserido a sua esquerda
  IInsere_nome(x, &(*Ap)->Dir, &(*Ap)->BitD, Fim);
  if (*Fim)
	return;
  if ((*Ap)->BitD != Horizontal) {
  	*Fim = TRUE;
  	return;
  }
  if ((*Ap)->Dir->BitD == Horizontal) {
  	DD(Ap);
  	*IAp = Horizontal;
  	return;
  }
  if ((*Ap)->Dir->BitE == Horizontal) {
  	DE(Ap);
  	*IAp = Horizontal;
  }
}

void Insere_nome(candidato x, ApontadorNo *Ap) {
  short Fim;
  Inclinacao IAp;
  IInsere_nome(x, Ap, &IAp, &Fim);
}

void IRetira_nome(candidato x, ApontadorNo *Ap, short *Fim) {
  No *Aux;
  if (*Ap == NULL) {
    system("cls");
    system("color 4f");
	printf("\n\n\t Este nome nao esta na arvore. \n");
	NovaTecla();
	*Fim = TRUE;
	return;
	}
  // Caso o nome seja menor
  if (strcmpi(x.nome,(*Ap)->Reg.nome)>0) {
  	IRetira_nome(x, &(*Ap)->Esq, Fim);
    if (!*Fim)
		EsqCurto(Ap, Fim);
    return;
  }
  // Caso o nome seja maior
  if (strcmpi(x.nome, (*Ap)->Reg.nome)<0) {
  	IRetira_nome(x, &(*Ap)->Dir, Fim);
    if (!*Fim)
		DirCurto(Ap, Fim);
    return;
  }

  *Fim = FALSE;
  Aux = *Ap;
  if (Aux->Dir == NULL) {
	*Ap = Aux->Esq;
    free(Aux);
    if (*Ap != NULL)
      *Fim = TRUE;
    return;
  }
  if (Aux->Esq == NULL) {
	*Ap = Aux->Dir;
	free(Aux);
    if (*Ap != NULL)
	  *Fim = TRUE;
    return;
  }
  Antecessor(Aux, &Aux->Esq, Fim);
  if (!*Fim)
	EsqCurto(Ap, Fim); /* Encontrou chave */
}

void Retira_nome(candidato x, ApontadorNo *Ap) {
  short Fim;
  IRetira_nome(x, Ap, &Fim);
}

void Pesquisa_nome(candidato *x, ApontadorNo *p, int numero_questoes, int *achou) {
  int i;
  if (*p == NULL) {
    system("color 4f");
    system("cls");
  	printf("\n\n\t Erro: Registro nao esta presente na arvore. \n");
  	NovaTecla();
  	achou=0;
    return;
  }
  // Caso o nome a ser procurado seja menor
  if (strcmpi(x->nome, (*p)->Reg.nome)>0) {
  	Pesquisa_nome(x, &(*p)->Esq, numero_questoes, achou);
    return;
  }
  if (strcmpi(x->nome, (*p)->Reg.nome)<0)
	Pesquisa_nome(x, &(*p)->Dir, numero_questoes, achou);
  else
    *x = (*p)->Reg;

    if( strcmpi(x->nome,(*p)->Reg.nome) == 0){
        printf("\n\t Registro encontrado: \n");
        printf("\n Cargo: %s\t\t",x->cargo);
        printf("\n Nome: %s",x->nome);
        printf("\n Idade: %d \n Numero de inscricao: %d ",x->idade,x->numero_de_inscricao);
        printf("\n Gabarito do Candidato: \n");

        for( i=0; i<numero_questoes; i++ ){
            printf("  %c  ", x->gabarito[i] );
        }
        *achou=1;
        NovaTecla();
        return;
    }
}

void Testa1_nome(ApontadorNo p, int nivel, int *NivelFolhas, short *PrimeiraFolha) {
  if (p == NULL)
	return;
  if (*PrimeiraFolha)
	if (*NivelFolhas < nivel)
	  *NivelFolhas = nivel;
	if (p->Esq == NULL && p->Dir == NULL) {
	  if (*PrimeiraFolha == TRUE)
		*PrimeiraFolha = FALSE;
	  else {
		if (nivel != *NivelFolhas) {
		  system("color 4f");
		  printf("\n Erro: Folhas em niveis diferentes. \n");
		  NovaTecla();
		  exit(1);
		}
	  }
	}
  if (p->BitE == Horizontal)
	Testa1_nome(p->Esq, nivel, NivelFolhas, PrimeiraFolha);
  else
    Testa1_nome(p->Esq, nivel + 1, NivelFolhas, PrimeiraFolha);
  if (p->BitD == Horizontal)
	Testa1_nome(p->Dir, nivel, NivelFolhas, PrimeiraFolha);
  else
    Testa1_nome(p->Dir, nivel + 1, NivelFolhas, PrimeiraFolha);
}

void Testa2_nome(ApontadorNo p, int *NivelFolhas, short *PrimeiraFolha) {
  if (p == NULL)
	return;
  if (p->Esq != NULL) {
  	if (strcmpi(p->Reg.nome,p->Esq->Reg.nome)>0) {
  	  system("color 4f");
	  printf("\n Erro: %d < que filho a esquerda. \n", p->Reg.nome);
      NovaTecla();
      exit(1);
    }
    Testa2_nome(p->Esq, NivelFolhas, PrimeiraFolha);
  }
  if (p->Dir == NULL)
	return;
  if (strcmpi(p->Reg.nome, p->Dir->Reg.nome)<0) {
    system("color 4f");
  	printf("\n Erro: %d > que filho a direita. \n", p->Reg.nome);
    exit(1);
  }
  Testa2_nome(p->Dir, NivelFolhas, PrimeiraFolha);
}

void Testa_nome(ApontadorNo Arvore) {
  int NivelFolhas = 0;
  short PrimeiraFolha = TRUE;
  Testa1_nome(Arvore, 1, &NivelFolhas, &PrimeiraFolha);
  Testa2_nome(Arvore, &NivelFolhas, &PrimeiraFolha);
}

void Em_Ordem_nome(ApontadorNo No){
    if(No == NULL)
       return;
    Em_Ordem_nome(No->Esq);
    printf("\n%s", No->Reg.nome);
    Em_Ordem_nome(No->Dir);
    return;
}

void Pre_Ordem_nome(ApontadorNo No){
    if(No == NULL)
       return;
    printf("\n%s", No->Reg.nome);
    Pre_Ordem_nome(No->Esq);
    Pre_Ordem_nome(No->Dir);
    return;
}

void Pos_Ordem_nome(ApontadorNo No){
    if(No == NULL)
       return;
    Pos_Ordem_nome(No->Esq);
    Pos_Ordem_nome(No->Dir);
    printf("\n%s", No->Reg.nome);
    return;
}

//================= FUNCOES DE TABELA HASH ============================

void PesquisaHash( TipoLista *Hash, candidato *x, int numero_questoes ){
    Apontador aux;
    int i;

    // caso a lista da tabela esteja vazia
    if( TesteListaVazia(*Hash) == 1){
        system("cls");
        system("color 4f");
        printf("\n Posicao vazia. \n");
        NovaTecla();
        return;
    }

    // aux recebe a primeira celula com conteudo
    aux = Hash->primeiro->prox;

    while(aux != NULL){

        // caso o item procurado seja o apontado por aux
        if( aux->item.numero_de_inscricao ==  (*x).numero_de_inscricao){
            *x = aux -> item;

            printf("\n Cargo: %s\t\t",x->cargo);
            printf("\n Nome: %s",x->nome);
            printf("\n Idade: %d \n Numero de inscricao: %d ",x->idade,x->numero_de_inscricao);
            printf("\n Gabarito do Candidato: \n");

            for( i=0; i<numero_questoes; i++ ){
                printf("  %c  ", x->gabarito[i] );
            }
            NovaTecla();
            return;
        }
        aux = aux -> prox;
    }
    system("cls");
    system("color 4f");
    printf("\n Posicao vazia. \n");
    NovaTecla();
}

int CalculoPosicao(int x){
    int posicao;

    posicao = (x%MAX);
    return(posicao);
}

void DesalocaHash(TipoLista *Hash){
    Apontador aux, aux2;

    if( (TesteListaVazia(*Hash) == 1))
        return;

        aux = Hash->ultimo;
        aux2 = aux -> ant;

        while ( aux2 != Hash -> primeiro ){
            Hash -> ultimo = aux2;
            aux2 = aux2 -> ant;
            free(aux);
            aux = Hash -> ultimo;
        }
        Hash-> ultimo = aux2;
        free(aux);
        aux2 = NULL;
}

int CalculoPosicaoNome(candidato x){
    int i, posicao;
    i = atoi(x.nome);
    posicao=i%MAX;
    return(posicao);
}

void PesquisaHashNome( TipoLista *Hash, candidato *x, int numero_questoes ){
    Apontador aux;
    int i;

    // caso a lista da tabela esteja vazia
    if( TesteListaVazia(*Hash) == 1){
        system("cls");
        system("color 4f");
        printf("\n Tabela vazia. \n");
        return;
    }

    // aux recebe a primeira celula com conteudo
    aux = Hash->primeiro->prox;

    while(aux != NULL){

        // caso o item procurado seja o apontado por aux
        if(strcmpi(aux->item.nome,(*x).nome)==0){
            *x = aux -> item;

            printf("\n Cargo: %s\t\t",x->cargo);
            printf("\n Nome: %s",x->nome);
            printf("\n Idade: %d \n Numero de inscricao: %d ",x->idade,x->numero_de_inscricao);
            printf("\n Gabarito do Candidato: \n");

            for( i=0; i<numero_questoes; i++ ){
                printf("  %c  ", x->gabarito[i] );
            }
            NovaTecla();
            return;
        }
        aux = aux -> prox;
    }
    system("cls");
    system("color 4f");
    printf("\n Posicao vazia. \n");
}

//======================================================================
//======================================================================

void apresentacao(){
    int lin,col;

    system("color f0");
    for(lin=1;lin<=7;lin++){
        for(col=1;col<=39;col++){
            if((lin+col)%2==0){
                printf("\xdb\xdb");
            }
            else{
                printf("  ");
            }
        }
    printf("\n");
    }
    printf("  \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\t\t\t\t              \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\n");
    printf("\xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\t\t\t\t            \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\n");
    printf("  \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\t\t\t\t              \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\n");
    printf("\xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb     Processo Seletivo - Unimontes 2011   \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\n");
    printf("  \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\t\t\t\t              \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\n");
    printf("\xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\t\t\t\t            \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\n");
    printf("  \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\t\t\t\t              \xdb\xdb  \xdb\xdb  \xdb\xdb  \xdb\xdb\n");
    for(lin=1;lin<=8;lin++){
        for(col=1;col<=39;col++){
            if((lin+col)%2==0){
                printf("\xdb\xdb");
            }
            else{
                printf("  ");
            }
        }
    printf("\n");
    }
getch();
}
