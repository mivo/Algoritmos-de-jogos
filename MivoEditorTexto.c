/* UNIVERSIDADE ESTADUAL DE MONTES CLAROS - UNIMONTES
    Centro de Ciências Exatas e Tecnológicas
    Departamento de Ciências da Computação
    Curso de Sistemas de Informação
    Acadêmica: Mariéllen Ivo
    Descrição: Editor de Textos
*/

#include <stdio.h>
#include <stdlib.h>

#define MAXTAM 100

//===============================
//Estruturas Utilizadas pela Pilha

typedef int TipoApontador;

typedef struct {
    char letra;
}TipoItem;

typedef struct {
    TipoItem Item[MAXTAM] ;
    TipoApontador Topo;
}TipoPilha;

//=================================
// Prototipos das Funções do Programa

void Inicializa(TipoPilha *Pilha);
int TestaVazia(TipoPilha Pilha);
void Empilha(char x , TipoPilha *Pilha);
void Desempilha(TipoPilha *Pilha);
void Imprime(TipoPilha Pilha);
void VerificaCaractere(char letra, TipoPilha *Pilha, int *verifica);
void Desaloca(TipoPilha *Pilha);
void Menu(void);
void Tutorial(void);

//========================================
//  Função Principal

int main(){
    // Declaração de variaveis
    TipoPilha Texto;
    int opcao=0, verificador=0;
    char letra_digitada;

    // inicializando a pilha
    Inicializa(&Texto);

    // loop principal
    do{
        system("cls");
        Menu();

        printf("\n\t\t\t >> Opcao: ");

        // armazenando a opção do usuario
        if(scanf("%d", &opcao)== 0){
            opcao=0;
            printf("\n Opcao invalida. Digite novamente.\n");
            system("pause"); fflush(stdin); system("cls");
            continue;
        }
        // digitar texto
        if(opcao==1){
            opcao=0;
            system("cls");
            Tutorial(); fflush(stdin);
            // sempre pegar todos os caracteres, exceto ENTER (13)
            do{
                letra_digitada = getche();
                VerificaCaractere(letra_digitada, &Texto, &verificador);
                if(verificador==1) break;
            }while(letra_digitada!=13);
        }
        // visualizar texto
        else if(opcao==2){
            opcao=0;
            system("cls");
            Imprime(Texto);
        }
        // novo texto (elimina toda a pilha)
        else if(opcao==3){
            opcao=0;
            Desaloca(&Texto);
//            Inicializa(&Texto);
        }
        // sair do editor de textos
        else if(opcao==4){
            return(0);
        }
    }while(opcao>=0 && opcao<=4);

    system("pause"); fflush(stdin);
    return(0);
}


//=========================================
//=========================================
// Corpo das Funções

// cria a pilha
void Inicializa(TipoPilha *Pilha){
    Pilha->Topo = 0;
}

// verifica se a pilha esta vazia
int TestaVazia(TipoPilha Pilha){
    return (Pilha.Topo == 0);
}

// insere elementos na estrutura
void Empilha(char x , TipoPilha *Pilha){
    if (Pilha->Topo == MAXTAM){
        system("color 1f");
        printf ( "\n\n\t Erro : Numero maximo de caracteres atingido. \n\n\t" );
        system("pause"); fflush(stdin);
    }
    else{
        Pilha->Topo++;
        Pilha->Item[Pilha->Topo - 1].letra = x ;
    }
}

// retira elementos da estrutura
void Desempilha(TipoPilha *Pilha){
    if (TestaVazia(*Pilha)){
        system("color 1f");
        printf ( "\n\n\t Erro : Nao ha caracteres disponiveis. \n\n\t" );
        system("pause"); fflush(stdin);
    }
    else{
        Pilha->Item[Pilha->Topo].letra = '\0';
        Pilha->Topo = Pilha->Topo --;
    }
}

// calculando o tamanho da pilha
int Tamanho(TipoPilha Pilha){
    return(Pilha.Topo);
}

// Imprime ordenadamente os dados da estrutura
void Imprime(TipoPilha Pilha){
    int i;

    if (TestaVazia(Pilha)){
        system("color 1f");
        printf ( "\n\n\t Erro : Nao ha caracteres disponiveis. \n\n\t" );
        system("pause"); fflush(stdin);
        return;
    }

    for(i=0; i<Pilha.Topo; i++){
        printf("%c", Pilha.Item[i].letra);
    }
    printf("\n\n\t\t >> Fim do texto << \n\n");
    system("pause"); fflush(stdin);
}

// retira todos os elementos da estrutura ate que ela esteja vazia
void Desaloca(TipoPilha *Pilha){
    int parada;
    parada = Tamanho(*Pilha);
    while(Pilha->Topo != 0){
        Desempilha(Pilha);
    }
}

// confere caractere a caractere o que foi digitado
void VerificaCaractere(char carac, TipoPilha *Pilha, int *verifica){
        int letra=carac, i=Pilha->Topo;

        // caractere: # apaga o caractere anterior
        if(letra == 35 && (!TestaVazia(*Pilha)) ){
            Desempilha(Pilha);
        }
        // caractere: \ apaga a linha anterior
        else if(letra == 92 && TestaVazia(*Pilha)!=0){
            while(Pilha->Item[i].letra != 10){
                Desempilha(Pilha);
                i--;
            }
        }
        // caractere * salta uma linha
        else if(letra == 42){
            Empilha(10, Pilha);
        }
        // caractere ~ encerra a digitacao
        else if(letra == 126){
            *verifica=1;
            return;
        }
        // insere os demais caracteres na pilha
        else{
            Empilha(letra, Pilha);
        }
        *verifica=0;
}

void Menu(void){
    int i;

    system("color f0");
    printf("\n\t");
    printf("    \xc9");
    for(i=0;i<45;i++)
        printf("\xcd");

    printf("\xbb\n");
    printf("\t    \xba \t\tEDITOR DE TEXTO vers\x84o 1.0  \t  \xba\n\n");
    printf("\t    \xba \t\t Menu Principal              \t  \xba\n\n");
    printf("\t    \xba \t\t [1] Digitar Texto             \t  \xba\n");
    printf("\t    \xba \t\t [2] Visualizar                \t  \xba\n");
    printf("\t    \xba \t\t [3] Novo Texto                \t  \xba\n");
    printf("\t    \xba \t\t [4] Sair do Editor            \t  \xba\n");
    printf("            \xc8");
    for(i=0;i<45;i++)
        printf("\xcd");
    printf("\xbc\n");

}

void Tutorial(void){
    int i;

    system("color f0");
    printf("\n\t");
    printf("    \xc9");
    for(i=0;i<52;i++)
        printf("\xcd");

    printf("\xbb\n");
    printf("\t    \xba \t\t          TUTORIAL                       \xba\n\n");
    printf("\t    \xba [#] Cancela o caractere anterior na linha corrente \xba\n");
    printf("\t    \xba [\\] Cancela todos caracteres da linha corrente \t \xba\n");
    printf("\t    \xba [*] Salta linha \t\t\t\t\t \xba\n");
    printf("\t    \xba [ENTER] ou [~] Fim do texto      \t\t\t \xba\n");
    printf("            \xc8");
    for(i=0;i<52;i++)
        printf("\xcd");
    printf("\xbc\n");

}
