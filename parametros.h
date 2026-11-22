#ifndef PARAMETROS_H
#define PARAMETROS_H
#define LINHA 300 
#define MENOR 15 //Usado como delimitador para variáveis que precisam de menos tamanho de vetor
#define MAX_NOME 80
#define GASOLINA 5.20
#define PASTA_ROTAS "rotas"
#define PASTA_PRODUTIVIDADE "produtividade"
#define ARQUIVO_MOTORISTA "motoristas.csv"
#define ARQUIVO_ENTREGASTR "entregas_triagem.csv"
#define ARQUIVO_MULTIUSO "multi.csv" //Arquivo que poderá ser usado para qualquer finalidade no projeto

extern const char *Veiculos[];
extern const char *Status[];
extern const char *Status_encomenda[];
extern const int Carga[];
extern const int km_l[];

typedef struct{ 
    char tipo_veiculo[MENOR];
    char placa[MENOR];
    int carga_max;
}Veiculo;

typedef struct{
    char nome[MAX_NOME];
    int id;
    char status[MENOR];
    Veiculo v;
}Motorista;

typedef struct{
    int id; 
    char cliente[MAX_NOME];
    char origem[MAX_NOME];
    char destino[MAX_NOME];
    float peso;
    char status[MENOR];
}Entrega;

void pausar();
void limpar_tela();
void limpar_buffer();
void garantir_pastas();
void acessar_pasta(const char *pasta);
void garantir_arquivos();
int proximo_id(const char *arquivo_c);
const char *relacionar_id_motorista(int id); 
void cadastro(int x);
void listar(int objeto);

#endif