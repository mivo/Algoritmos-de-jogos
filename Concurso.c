/* UNIVERSIDADE ESTADUAL DE MONTES CLAROS - UNIMONTES
    Centro de Ciências Exatas e Tecnológicas
    Departamento de Ciências da Computação
    Curso de Sistemas de Informação

    Professor: Renato Cota
    Academicos: Mariellen Ivo e Antônio Oliveira
    Descrição: Vestibular Unimontes 2011
    Data: 05/05/2011
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


//Estrutura dos dados do candidato
typedef struct{
    char nome[51];
    int idade;
    int numero_de_inscricao;
    int pontuacao;
    char *gabarito;
    char cargo[51];
}candidato;

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

//==============================================================
//                  PROTOTIPOS  de LISTA

void IniciarLista ( TipoLista *Lista );
int TesteListaVazia ( TipoLista Lista );
void InserirCelula (TipoLista *Lista ,candidato X);
void RetirarCelula ( TipoLista *Lista, candidato *X);
void ImprimeLista ( TipoLista Lista );
int Desalocar ( TipoLista *Lista, TipoFila *Fila, int *verifica_concurso );


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
/********************* FUNÇÃO PRINCIPAL ************************/
/*****************************************************************/

int main (void){

        system("title VESTIBULAR 2011");
        system("color 0f");


// Declaração de variáveis comuns
    int numero_concurso, numero_vagas, numero_questoes;
    int confirma, op;
    char data[15];
    char nome_cargo[51], promotor_concurso[51];
    char *gabarito_oficial;
    int flag=0, verifica_concurso=0;

// Declaração de contadores
    int i,j, k, l=0, cont_linha=0, hash=0;
    char c;

// Declaração de variáveis para arquivo
    FILE *configuracao_concurso, *ultimo_concurso;
    FILE *arq_lista, *arq_fila;
    FILE *arquivo2;

    char string_tmp[100], caractere[10];

// Declaração de variáveis para lista e fila
    candidato ficha_pessoal, X;
    TipoLista relacao_candidatos_lista;
    TipoFila relacao_candidatos_fila;

//===============================================================
//                           INICIO DO PROGRAMA

    // Inicia as lista/fila para poder carregar dados nelas
    IniciarLista(&relacao_candidatos_lista);
    IniciarFila (&relacao_candidatos_fila);

    ficha_pessoal.numero_de_inscricao=100000;
    numero_concurso = 0;

    // Carregando o numero do ultimo concurso
    if( (ultimo_concurso = fopen("ultimo_concurso.txt", "r") ) != NULL){
        fgets( caractere, 10, ultimo_concurso);
        sscanf( caractere, "%d", &numero_concurso);
        fclose(ultimo_concurso);
    }

    //Salva uma # na primeira linha do arquivo dos candidatos
    if((arquivo2 =fopen("Ficha dos Candidatos.txt","a+"))!=NULL){
        fclose(arquivo2);
        // Abre o arquivo para leitura/escrita
        if((arquivo2 =fopen("Ficha dos Candidatos.txt","r+"))!=NULL){
            // Caso houver um espaço em branco no início do arquivo
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

/******* Carregando Configurações do Concurso ********/

    //Carregando os dados das configurações do último concurso
    if( (configuracao_concurso = fopen("configuracao_concurso.txt","r") ) != NULL ){

        while ((c = fgetc(configuracao_concurso)) != EOF ){
            // contando as # ate chegar na do ultimo concurso
            l=0;
            if(c == '#'){
                hash++;
                // Pára na posicao da ultima #
                if( hash == numero_concurso){
                    l=1;
                    break;
                }
            }
        }

        if(l ==1){
            // Coloca o cursor no arquivo de onde parou de ler as #
            fseek (configuracao_concurso,0, SEEK_CUR);

            // Pega os dados de configuração do arquivo e passa para as variáveis
            // Pega linha a linha com o fgets e passa para a variável com o sscanf
            // Obs: a primeira linha é um #, então, saltamo-la
            fgets(string_tmp, 100, configuracao_concurso); // #
            fgets(string_tmp, 100, configuracao_concurso); // promotor do concurso
            sscanf(string_tmp,"%s",&promotor_concurso); fflush(stdin);
            fgets(string_tmp, 100, configuracao_concurso); // data do concurso
            sscanf(string_tmp,"%s",&data); fflush(stdin);
            fgets(string_tmp, 100, configuracao_concurso); // cargo do concurso
            sscanf(string_tmp,"%s",&nome_cargo); fflush(stdin);
            fgets(string_tmp, 100, configuracao_concurso); // numero de vagas
            sscanf(string_tmp,"%d",&numero_vagas); fflush(stdin);
            fgets(string_tmp, 100, configuracao_concurso); // numero de questões
            sscanf(string_tmp,"%d",&numero_questoes); fflush(stdin);

            // Aloca o espaço para o gabarito
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

   // Caso o arquivo não exista, a condição acima será ignorada, pois não há dados a serem carregados

    //  Preenchendo a lista com dados dos candidatos, abre o arquivo somente leitura
    if( (arq_lista = fopen("Ficha dos Candidatos.txt","r") ) != NULL ){
        hash=0;

        while ((c = fgetc(arq_lista)) != EOF ){
            // contando as # ate chegar nos dados do primeiro candidato do ultimo concurso
            if(c == '#'){
                hash++;
                // Pára na posicao da ultima #
                if( hash == numero_concurso){
                    break;
                }
            }
        }
        // Coloca o cursor no arquivo de onde parou de ler as #
		fseek (arq_lista,0, SEEK_CUR);

        // Conta linhas até o fim do arquivo
        while ((c = fgetc(arq_lista)) != EOF ){
            if(c == '\n'){
                cont_linha++;
            }
        }

        // Volta ao inicio do arquivo
        c=0;
        fseek(arq_lista,0, SEEK_SET);
        // Contagem do número de candidatos com base no numero de linhas
        cont_linha /=(6+numero_questoes);

        // Percorre novamente o arquivo desde o inicio
        hash=0;

        while ((c = fgetc(arq_lista)) != EOF ){
            // contando as # ate chegar nos dados do primeiro candidato do ultimo concurso
            if(c == '#'){
                hash++;
                // Pára na posicao da ultima #
                if( hash == numero_concurso){
                    break;
                }
            }
        }

        // Coloca o cursor no arquivo de onde parou de ler as #
		//fseek (arq_lista,0, SEEK_CUR);
        for(l=0; l<cont_linha; l++){

            // Pega os dados de configuração do arquivo e passa para as variáveis
            // Pega linha a linha com o fgets e passa para a variável com o sscanf
            // Obs: a primeira linha é um #, no caso do primeiro concurso, é um espaço, então, saltamo-la
            fgets(string_tmp, 100, arq_lista);  fflush(stdin);// # ou <espaço>

            fgets(ficha_pessoal.nome, 100, arq_lista); // Nome completo do candidato
            ficha_pessoal.nome[strlen(ficha_pessoal.nome)-1]='\0';
            fgets(string_tmp, 100, arq_lista); // Idade do candidato
            sscanf(string_tmp,"%d",&ficha_pessoal.idade);
            fgets(string_tmp, 100, arq_lista); // Cargo do Candidato
            sscanf(string_tmp,"%s",&ficha_pessoal.cargo);
            fgets(string_tmp, 100, arq_lista); // Numero de inscrição do candidato
            sscanf(string_tmp,"%d",&ficha_pessoal.numero_de_inscricao);
            fgets(string_tmp, 100, arq_lista); // Pontuação do candidato
            sscanf(string_tmp,"%d",&ficha_pessoal.pontuacao);

            // Aloca o espaço para o gabarito
            ficha_pessoal.gabarito = (char*) malloc (numero_questoes*sizeof(char));

            // Preenche o gabarito
            for( k=0; k<numero_questoes; k++){
                fgets(string_tmp, 100, arq_lista); // Gabarito do candidato
                string_tmp[strlen(string_tmp)-1]='\0';
                sscanf(string_tmp,"%c",&ficha_pessoal.gabarito[k]); fflush(stdin);
            }

            // Insere os dados numa celula da lista
            InserirCelula(&relacao_candidatos_lista, ficha_pessoal);
        }
        // Depois de carregar todos os dados, fecha o arquivo
        fclose(arq_lista);
    }
    // Caso o arquivo não exista, a condição acima será ignorada, pois não há dados a serem carregados

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

            // verificar se há um concurso iniciado (dados carregados)
            flag = Desalocar(&relacao_candidatos_lista, &relacao_candidatos_fila, &verifica_concurso);
           //A flag retorna 1 caso os dados sejam deletados, do contrário retorna 0

            // Caso nao haja nenhuma inscricao para o concurso
            if(flag==1 && verifica_concurso == 1){

                system("color 40");
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

                printf("            \xc9");
                for(i=0;i<20;i++)
                    printf("\xcd");
                printf("\xbb\n");
                printf("\t    \xba \xfa\xfa\xfa CONCURSO %d \xfa\xfa\xfa \xba\n", numero_concurso);
                printf("            \xc8");
                for(i=0;i<20;i++)
                    printf("\xcd");
                printf("\xbc\n");

                // Cria as lista e fila para armazenar os dados
                IniciarLista(&relacao_candidatos_lista);
                IniciarFila (&relacao_candidatos_fila);

                // Armazenando as configurações do novo concurso
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

                // Alocando espaço para armazenar o gabarito oficial
                gabarito_oficial = (char*) malloc ( numero_questoes*sizeof(char) );

                // Armazenando o gabarito oficial
                for( i=0; i<numero_questoes; i++ ){
                    fflush(stdin);
                    printf(" \n\t Resposta %i: ", i+1);
                    gabarito_oficial[i] = VerificaCaracter(gabarito_oficial[i]);
                }

                // Salvando em arquivo as configurações do concurso, os dados serão sempre salvos
                // no final do arquivo, e o concurso não poderá ser acessado se não for o ultimo
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
                }

                else{
                    printf("\n Ocorreu um erro inesperado, os dados nao puderam ser salvos. \n");
                }
            }

        // Caso o usuário opte por continuar um concurso, as opções já estarão disponíveis
        // pois os dados são automaticamente carregados no início do programa. Então,
        // basta que ele continue normalmente a utilizar o programa, passando para as opções
        // seguintes.
        NovaTecla();
        }

        //=================== OPCAO 2 ============
        if( op == 2){

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

                // Alocando espaço para armazenar o gabarito do candidato
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

                // Confirmação dos dados

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

            //  Corrigir provas, ou seja, calcular pontuaçoes e modificar estes valores na lista
            // Lembrando que:
            //- cada questão certa o candidato ganha 3 pontos;
            //- cada questão errada o candidato perde 1 ponto;
            //- cada questão em branco o candidato não perde nem ganha pontos.
            ficha_pessoal.pontuacao = CompararGabarito ( ficha_pessoal.gabarito, gabarito_oficial, numero_questoes );

            // Preenche uma celula da lista com os dados do candidato
            InserirCelula(&relacao_candidatos_lista, ficha_pessoal);

            //Grava os dados do candidato em arquivo
            GravarArquivoLista( ficha_pessoal, numero_questoes );
            printf("\n\n\n\t  Inscricao concluida com exito! ");
            printf("\n\n\t  Seu cadastro foi salvo com sucesso! \n\n");
            NovaTecla();
        }

        // ========= OPÇÃO 3 =====================
        if ( op == 3 ){
        // Caso nao haja nenhuma inscricao para o concurso
            if(TesteListaVazia(relacao_candidatos_lista)== 1 ){
                c=99; // 99 = lista vazia
            }
            else{
              c=11;
            }
            verifica_concurso=0;
            // caso nao haja inscriçoes nada é salvo.
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
        }
        // === OPÇÃO 4 =================
        if ( op == 4 ){
            system("cls");
            // Imprime os dados da fila
            printf(" \n\n\n\t\t -------- Lista de Aprovados --------\n\n\n");
            printf("Concurso %d\n\n", numero_concurso);
            ImprimeFila(relacao_candidatos_fila);
            NovaTecla();
        }

        // === OPÇÃO 5 =================
        if ( op == 5 ){
            system("cls");
            // Imprime os dados da lista
            printf(" \n\n\n\t\t -------- Lista de Espera --------\n\n\n");
            ImprimeLista(relacao_candidatos_lista);
            NovaTecla();
        }

        // === OPÇÃO 6  =================
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


        // === OPÇÃO 7  =================
        if ( op == 7 ){
                break;
        }

        // === DEFAULT ================
        if( op <1 || op >7 ){
            system("cls");
            printf(" \n\n Opcao invalida, digite novamente. \n\n");
            NovaTecla();
        }

    }while(1);

    return(0);
}

/*****************************************************************/
/******************* CORPO DAS FUNÇÕES **************************/
/****************************************************************/

//                          FUNÇÕES DE LISTA

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

// Função que chama um candidato da lista de espera do vestibular
void RetirarCelula( TipoLista *Lista, candidato *X){
    Apontador aux, aux2; // são ponteiros auxiliares para percorrer a lista

    if(TesteListaVazia(*Lista) == 1){
        printf("\n\n Erro: Lista Vazia!\n\n");
        return;
    }

    aux = Lista -> primeiro -> prox;

    // Se só houver uma célula além da "cabeça", quer dizer que seu prox é NULL
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
        system("color 40");
        printf("\n\n Erro: Lista Vazia!\n\n");
        return;
    }

    aux = Lista.primeiro -> prox;

    while ( aux != NULL ){
        printf("\n Inscricao: %d \n Candidato: %s \t Idade: %d \t Pontuacao: %d \n\n", aux->item.numero_de_inscricao, aux->item.nome, aux->item.idade, aux->item.pontuacao);
        aux = aux-> prox;
    }

}

int Desalocar( TipoLista *Lista, TipoFila *Fila, int *verifica_concurso ){
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
                    printf("\n A fila esta vazia! \n");
                    return 0;
                }
                aux4 = aux4 -> prox;
            }
            Fila -> primeiroFila = NULL;
            Fila -> ultimoFila = NULL;

            // Parte para desalocar fila FIM
            return(1);
        }

        if( op == 2 ){
          *verifica_concurso=1;
            return(0);
        }
    }

}

/*****************************************************************/
//                                  FUNÇÕES DE FILA

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
     printf("\n\n Erro: A fila esta  vazia! \n");
     return;
  }

  aux = Fila->primeiroFila;
  aux2= aux -> prox;

  if( aux2!=NULL ){
        aux -> prox = aux2 -> prox;
        free(aux2);
        if( aux->prox != NULL ){
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
     system("color 40");
     printf("\n\n Erro: A fila esta  vazia! \n");
     return;
  }

  aux = Fila.primeiroFila->prox;

  while (aux != NULL){
      printf("\n Inscricao: %d \n Candidato: %s \t Idade: %d \t Pontuacao: %d \n\n", aux->itemFila.numero_de_inscricao, aux->itemFila.nome, aux->itemFila.idade, aux->itemFila.pontuacao);
      aux = aux->prox;
  }
}


/**************************************************************/
//                          Outras funções

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
    printf("\t    \xba       7) Sair                                          \xba\n");
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
//============== Funções para o Quicksort ================================

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

    // Comparações
    do{

        // Confere se o item da esquerda é maior que o pivo
        while(pivo < aux->item.pontuacao){
            (*i)++;
            aux=aux->prox;
        }

        // Confere se o item da direita é menor que o pivo
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

void apresentacao(){
    int lin,col;

    system("color 90");
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
