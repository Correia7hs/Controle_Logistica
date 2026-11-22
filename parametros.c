#include "parametros.h"
#include <stdio.h>
#include <direct.h>
#include <errno.h>
#include <string.h>

const char* Veiculos[] = {"Moto", "Carro", "Caminhão"};// constante usada para atribuição do tipo do veiculo
const char* Status[] = {"ATIVO", "INATIVO", "SUSPENSO"};//consante usada para atribuição de status do motorista
const char* Status_encomenda[] = {"PENDENTE", "EM ROTA", "ENTREGUE", "DEVOLVIDA"};// constante usada para atribuição de status da encomenda
const int Carga[] = {50, 200, 1000};// constante usada para a atribuição da carga máxima pelo tipo do veículo em KG
const int km_l[] = {60,13,5};//Constante usada para definir km/l de cada tipo de veículo, dados são ficticios

/**
 * @brief Função para pausar
 */
void pausar(){
    printf("\nPressione Enter para continuar...");
    getchar();
}

/**
 * @brief Limpa a tela e posiciona o cursor no topo
 */
void limpar_tela() {
    printf("\033[2J\033[H");  
}

/**
 * @brief Função para limpar o buffer deixar pelo scanf 
 */
void limpar_buffer(){
	int c;
	while((c = getchar()) != '\n' && c != EOF ){}
}

/**
 * @brief Garante a criação de pastas
 * 
 * Caso as pastas não estejam criadas assim que o progama inicia, a função identifica e cria as pastas
 * 
 */
void garantir_pastas(){
    
    if(_mkdir(PASTA_ROTAS) != 0 && errno != EEXIST){
        perror(PASTA_ROTAS);
    }
    if(_mkdir(PASTA_PRODUTIVIDADE) !=0 && errno != EEXIST){
        perror(PASTA_PRODUTIVIDADE);
    }
    
}

/**
 * @brief Função para acessar qualquer pasta
 */
// void acessar_pasta(const char *pasta_c){
//  if(_chdir(pasta_c) != 0){
//     perror("Não foi possível acessar a pasta");
//     return 1;
//  }
// }


/**
 *@brief Garante a criação de arquivos
 * 
 * Caso os arquivos não estejam criados assim que o progama inicia, a função identifica e cria os arquivos
 */
void garantir_arquivos(){ 
    FILE *motorista = fopen(ARQUIVO_MOTORISTA, "r");
    FILE *entregatr = fopen(ARQUIVO_ENTREGASTR, "r");

    if(motorista == NULL){
        motorista = fopen(ARQUIVO_MOTORISTA, "w");
        if(motorista != NULL){
            fprintf(motorista, "id;nome;veiculo;placa;carga_kg;status\n");
        }
        else perror(ARQUIVO_MOTORISTA);
    }
    if(entregatr == NULL){     
        entregatr = fopen(ARQUIVO_ENTREGASTR, "w");
        if(entregatr != NULL){
            fprintf(entregatr, "id;cliente;origem;destino;peso;status\n"); 
        }
        else perror(ARQUIVO_ENTREGASTR);
    }
    
    if(motorista != NULL) fclose(motorista);
    if(entregatr != NULL) fclose(entregatr);
}

/**
 * @brief Função para criar próximo id
 * 
 * A função localiza o maior id dentro do arquivo, e retorna um id novo com base em: maior id + 1.
 * Se não possuir nenhum registro no arquivo, a função retorna 99 + 1, onde será o primeiro id.
 * 
 */
int proximo_id(const char *arquivo_c) {
    FILE *arquivo = fopen(arquivo_c, "r");
    if (arquivo == NULL) {
        return 100; // arquivo ainda não existe
    }

    char linha[300];
    int id;
    int maior = 99;

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return 100; // arquivo vazio
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (sscanf(linha, "%d;", &id) == 1 && id > maior) {
            maior = id;
        }
    }

    fclose(arquivo);
    return maior + 1;
}

/**
 * @brief Função para relacionar o id do motorista com seu nome
 * 
 * A função recebe o id do motorista e pesquisa seu id no arquivo de motoristas, retornando seu nome
 */
const char *relacionar_id_motorista(int id){
    Motorista m;
    char linha[LINHA];
    static char nome[MAX_NOME];
    nome[0] = '\0';
    
    FILE *motorista = fopen(ARQUIVO_MOTORISTA, "r");
    if(motorista == NULL) return nome;

    fgets(linha, sizeof(linha), motorista);

    while(fscanf(motorista, "%d;%79[^;];%14[^;];%14[^;];%d;%14[^\n]\n", &m.id, m.nome, m.v.tipo_veiculo, m.v.placa, &m.v.carga_max, m.status) == 6){
        if(id == m.id){
            strcpy(nome, m.nome);
            break;
        }
    }
    fclose(motorista);
    return nome;
}

/**
 * @brief Função de cadastro global
 * 
 * A função funcionará por tipos, onde, 1 = Motorista, 2 = Encomenda, 3 = Rotas
 * 
 */
void cadastro(int objeto){
    int opcao;
    switch (objeto){
        case 1:{
         //cadastro do motorista
         Motorista m;

         FILE *motorista = fopen(ARQUIVO_MOTORISTA, "a");
         if(motorista == NULL) return;

         printf("Digite o nome do motorista\n");
         scanf(" %79[^\n]", m.nome);
         do{        
             printf("Qual a categoria do seu veículo?\n"
                    "1 - Moto\n"
                    "2 - Carro\n"
                    "3 - Caminhão\n");
             if(scanf(" %d", &opcao) != 1){
                   limpar_buffer();
                   continue; 
                }   
             else{ 
                 strcpy(m.v.tipo_veiculo, Veiculos[opcao - 1]);
                 m.v.carga_max = Carga[opcao - 1];
                }
            }while(opcao < 1 || opcao > 3);
         printf("Digite a placa do Veículo (Ex: VGB-1108)\n");
         scanf(" %14[^\n]", m.v.placa);
        
         m.id = proximo_id(ARQUIVO_MOTORISTA);
         strcpy(m.status, Status[0]);//Status ativo padrão para cada motorista novo cadastrado

         fprintf(motorista, "%d;%s;%s;%s;%d;%s\n", m.id, m.nome, m.v.tipo_veiculo, m.v.placa, m.v.carga_max,m.status);
         fclose(motorista);
         limpar_tela();
         break;
        }
        case 2:{
         //triagem da encomenda
         Entrega e;

         FILE *entregatr = fopen(ARQUIVO_ENTREGASTR, "a");
         if(entregatr == NULL) return;

         printf("Digite o nome do cliente\n");
         scanf(" %79[^\n]", e.cliente);

         printf("Digite a origem da encomenda\n");
         scanf(" %79[^\n]", e.origem);

         printf("Digite o destino da encomenda\n");
         scanf(" %79[^\n]", e.destino);

         printf("Digite o peso da encomenda\n");
         scanf(" %f", &e.peso);

         e.id = proximo_id(ARQUIVO_ENTREGASTR);
         strcpy(e.status, Status_encomenda[0]);

         fprintf(entregatr, "%d;%s;%s;%s;%.2f;%s\n", e.id, e.cliente, e.origem, e.destino, e.peso, e.status);
         fclose(entregatr);
         limpar_tela();
         break;
        }
        case 3:{
         //Função para gerar rotas
         char nome[MAX_NOME];
         char cidade[MAX_NOME];
         char data[MENOR];
         int condutor;

        
         printf("Digite a cidade de destino\n");
         scanf(" %79[^\n]", cidade);
        
         limpar_tela();
         listar(3);
         printf("Digite o id do motorista que deseja alocar a rota (Digite 0 se não o achou) \n");
         scanf(" %d", &condutor);
         if(condutor == 0){ 
             limpar_tela();
             cadastro(1);
             condutor = proximo_id(ARQUIVO_MOTORISTA) - 1;   
            }
         limpar_tela();
         printf("Data da rota (Ex: 29/08/2006)");
         scanf(" %14[^\n]", data);

         strcpy(nome,relacionar_id_motorista(condutor));
         limpar_tela();
         break;

         // terminar função
        }
        default:
        break;
        
    


    }
 
 



}

void listar(int objeto){
    char linha[LINHA];
    switch (objeto){
        case 1:{
         Motorista m;
         
         FILE *motorista = fopen(ARQUIVO_MOTORISTA, "r");
         if(motorista == NULL) return;

         fgets(linha, sizeof(linha), motorista);

         while(fscanf(motorista, "%d;%79[^;];%14[^;];%14[^;];%d;%14[^\r\n]\n", &m.id, m.nome, m.v.tipo_veiculo, m.v.placa, &m.v.carga_max, m.status) == 6 ){

             printf("\n========================================\n");
             printf(" MOTORISTA #%d\n", m.id);
             printf("----------------------------------------\n");
             printf(" Nome         : %s\n", m.nome);
             printf(" Veículo      : %s\n", m.v.tipo_veiculo);
             printf(" Placa        : %s\n", m.v.placa);
             printf(" Carga máxima : %d kg\n", m.v.carga_max);
             printf(" Status       : %s\n", m.status);
             printf("========================================\n");
         }

         limpar_buffer();
         fclose(motorista);
         pausar();
         limpar_tela();
        }
        break;

        case 2:{
         Entrega e;

         FILE *entregatr = fopen(ARQUIVO_ENTREGASTR, "r");
         if(entregatr == NULL) return;

         fgets(linha, sizeof(linha), entregatr);

         while(fscanf(entregatr, "%d;%79[^;];%79[^;];%79[^;];%f;%14[^\r\n]\n", &e.id, e.cliente, e.origem, e.destino, &e.peso, e.status) == 6){
             printf("\n========================================\n");
             printf(" ENTREGA #%d\n", e.id);
             printf("----------------------------------------\n");
             printf(" Cliente : %s\n", e.cliente);
             printf(" Origem  : %s\n", e.origem);
             printf(" Destino : %s\n", e.destino);
             printf(" Peso    : %.2f kg\n", e.peso);
             printf(" Status  : %s\n", e.status);
             printf("========================================\n");
         }
         
         limpar_buffer();
         fclose(entregatr);
         pausar();
         limpar_tela();
        }
        break;

        case 3:
        // terminar função
        break;
        
    
    }
     










}
