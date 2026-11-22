#include "parametros.h"
#include <stdio.h>
#include <direct.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(){
    garantir_pastas();
    garantir_arquivos();

    int opcao;
    while(1){

        printf("----[GV LOGS]----\n"
               "1 - Gestão de Motoristas\n"
               "2 - Gestão de Encomendas\n"
               "3 - Gestão de Rotas\n"
               "4 - Relatórios\n"
               "5 - Sair\n"    
        );
        printf("Selecione uma opção 🔼\n");
        if(scanf(" %d", &opcao) != 1){
            limpar_buffer();
            continue; 
        }

        switch (opcao){

            case 1:
            limpar_tela();
            printf("1 - Cadastrar Motorista\n"
                   "2 - Listar Motoristas\n");
            scanf(" %d", &opcao);
            limpar_tela();
            if(opcao == 1) cadastro(1);
            if(opcao == 2) listar(1);
            // if(opcao == 3) atualizar
            // if(opcao == 4) excluir
            break;

            case 2:
            limpar_tela();
            printf("1 - Cadastrar encomenda\n"
                   "2 - Listar Encomendas\n");
            scanf(" %d", &opcao);
            limpar_tela();
            if(opcao == 1) cadastro(2);
            if(opcao == 2) listar(2);
            // if(opcao == 3) atualizar
            // if(opcao == 4) excluir
            break;

            case 3:
            limpar_tela();
            printf("1 - Gerar rota \n");
            scanf(" %d", &opcao);
            limpar_tela();
            if(opcao == 1) cadastro(3);
            // if(opcao == 2) listar
            // if(opcao == 3) atualizar
            // if(opcao == 4) excluir
            break;

            case 4:
            //terminar função
            break;

            case 5:
            printf("Saindo do programa...");
            sleep(2);
            return 0;

            default:
            printf("Opção inválida!");
            sleep(2);
            limpar_tela();
    
        }

    }

}
