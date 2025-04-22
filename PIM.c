#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>


#define MAX_PRODUTOS 100
#define MAX_PEDIDOS 100
#define MAX_NOME 50
#define MAX_PRODUTOS_POR_PEDIDO 30

// Estrutura para armazenar informaÃ§Ãµes de um produto
typedef struct
{
    int id;
    char nome[MAX_NOME];
    double preco;
    float quantidade;
    char tipo[15];
} Produto;

// Estrutura para armazenar produtos em um pedido
typedef struct {
    int produtoId;
    float quantidade;
} ProdutoPedido;

// Estrutura para armazenar informações de um pedido
typedef struct {
    int id;
    ProdutoPedido produtos[MAX_PRODUTOS_POR_PEDIDO];
    int numProdutos;
    double total;
} Pedido;

Produto produtos[MAX_PRODUTOS];
Pedido pedidos[MAX_PEDIDOS];
int numProdutos = 0;
int numPedidos = 0;

struct user
{
    char usuario[100];
    char senha[100];
    char nivelAcesso[10];
};

int numUsuarios = 0;

typedef struct user usuario;
usuario login;
usuario cadastro_login;

// Função para mostrar menu para User
void mostrarMenuUser()
{
    printf("\n --------------\n |Menu Usuário| \n --------------\n");
    printf("\n 1 - Menu de Vendas");
    printf("\n 2 - Menu de Produtos");
    printf("\n 0 - Sair");
    printf("\n Entre com o código: ");
}

// Função para mostrar menu para ADM
void mostrarMenuAdmin()
{
    printf("\n -----------------------\n |Menu do Administrador| \n -----------------------\n");
    printf("\n 3 - Menu de Produtos");
    printf("\n 4 - Menu de Vendas");
    printf("\n 5 - Menu de Usuários");
    printf("\n 0 - Sair\n");
    printf("\n Entre com o código: ");
}

// Função para gerar o login inicial, criando um arquivo se não existir
void gerarLoginInicial(char *nome_arquivo_login) {
    FILE *arquivo = fopen(nome_arquivo_login, "r");
    if (!arquivo) {
        printf("Arquivo %s não encontrado. Criando novo arquivo...\n", nome_arquivo_login);
        arquivo = fopen(nome_arquivo_login, "w");
        if (arquivo == NULL) {
            perror("Erro ao criar o arquivo de login");
            exit(1);
        }
        
        // Cabeçalho para organizar as colunas no Excel
        fprintf(arquivo, "Usuario,Senha,Nivel de Acesso\n");
        
        // Dados de login padrão
        fprintf(arquivo, "admin,123,ADM\n");
        printf("Login padrão (admin, 123) criado com sucesso.\n");
        fclose(arquivo);
    } else {
        fclose(arquivo);
    }
}

// Função para comparar o usuário e a senha informados com os dados do arquivo
int compararUsuarioSenha(char *nome_arquivo, char *usuario, char *senha) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de login");
        exit(1);
    }

    char linha[1000];
    int autenticado = 0;

    // Ignora a primeira linha de cabeçalho
    fgets(linha, sizeof(linha), arquivo);

    // Lê cada linha do arquivo
    while (fgets(linha, sizeof(linha), arquivo)) {
        char usuarioArquivo[100], senhaArquivo[100], nivelArquivo[10];

        // Extrai os valores de cada linha usando vírgula como delimitador
        sscanf(linha, "%[^,],%[^,],%s", usuarioArquivo, senhaArquivo, nivelArquivo);

        // Remove quebras de linha das strings lidas
        usuarioArquivo[strcspn(usuarioArquivo, "\n")] = 0;
        senhaArquivo[strcspn(senhaArquivo, "\n")] = 0;
        nivelArquivo[strcspn(nivelArquivo, "\n")] = 0;

        // Compara o usuário e a senha
        if (strcmp(usuario, usuarioArquivo) == 0 && strcmp(senha, senhaArquivo) == 0) {
            autenticado = 1;
            strcpy(login.nivelAcesso, nivelArquivo);
            break;
        }
    }

    fclose(arquivo);
    return autenticado;
}

// Função para cadastrar um novo administrador
void CadastrarADM() {
    char *nome_arquivo_login = "login.csv";
    printf(" Cadastro de ADM\n");
    printf("\n Digite o usuario do ADM: ");
    scanf("%s", cadastro_login.usuario);
    printf("\n Senha: ");
    scanf("%s", cadastro_login.senha);

    FILE *arquivo_login = fopen(nome_arquivo_login, "a+");
    if (arquivo_login == NULL) {
        perror(" Erro ao abrir o arquivo de login");
        exit(1);
    }

    fprintf(arquivo_login, "%s,%s,ADM\n", cadastro_login.usuario, cadastro_login.senha);
    fclose(arquivo_login);
    printf("\n ADM %s adicionado com sucesso.\n", cadastro_login.usuario);
}

// Função para cadastrar um novo usuário comum
void CadastrarUsuario() {
    char *nome_arquivo_login = "login.csv";
    printf(" Cadastro de Usuários\n");
    printf("\n Usuário: ");
    scanf("%49s", cadastro_login.usuario);
    printf("\n Senha: ");
    scanf("%49s", cadastro_login.senha);

    FILE *arquivo_login = fopen(nome_arquivo_login, "a+");
    if (arquivo_login == NULL) {
        perror(" Erro ao abrir o arquivo de login");
        exit(1);
    }

    fprintf(arquivo_login, "%s,%s,USER\n", cadastro_login.usuario, cadastro_login.senha);
    fclose(arquivo_login);
    printf("\n Usuário %s adicionado com sucesso.\n", cadastro_login.usuario);
}

// Função para ler o conteúdo do arquivo e exibi-lo formatado
void lerArquivo(char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror(" Erro ao abrir o arquivo de login");
        return;
    }

    char linha[1000];
    printf("--------------------------------------------------\n");

    // Lê e exibe cada linha do arquivo formatada
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (strlen(linha) <= 1) continue;

        char *token = strtok(linha, ",");
        int contadorToken = 0;
        while (token) {
            printf("%-16s", token);
            token = strtok(NULL, ",");
            contadorToken++;
        }
        printf("\n");
    }

    fclose(arquivo);
}


// Função para salvar os produtos em um arquivo
void salvarProdutos() {
    setlocale(LC_NUMERIC, "C");

    FILE *arquivo = fopen("produtos.csv", "w");
    if (arquivo) {
        // Adiciona um cabeçalho para facilitar a leitura no Excel
        fprintf(arquivo, "ID,Nome,Preço,Tipo,Quantidade\n");

        // Salva os produtos no arquivo com a formatação correta
        for (int i = 0; i < numProdutos; i++) {
            fprintf(arquivo, "%d,%s,%.2lf,%s,%.2f\n",
                    produtos[i].id, produtos[i].nome, produtos[i].preco,
                    produtos[i].tipo, produtos[i].quantidade);
        }
        fclose(arquivo);
    } else {
        printf(" Erro ao salvar produtos!\n");
    }
}

// Função para carregar os produtos de um arquivo CSV
void carregarProdutos() {
    setlocale(LC_NUMERIC, "C");


    FILE *arquivo = fopen("produtos.csv", "r");
    if (arquivo) {
        char linha[200];
        
        // Ignora a primeira linha do cabeçalho
        fgets(linha, sizeof(linha), arquivo);

        // Lê cada linha do arquivo
        while (fgets(linha, sizeof(linha), arquivo)) {
            // Faz o parsing da linha lida
            if (sscanf(linha, "%d,%49[^,],%lf,%49[^,],%f", 
                       &produtos[numProdutos].id, 
                       produtos[numProdutos].nome, 
                       &produtos[numProdutos].preco, 
                       produtos[numProdutos].tipo, 
                       &produtos[numProdutos].quantidade) == 5) {
                numProdutos++;
            }
        }
        fclose(arquivo);
    } else {
        //printf(" Erro ao carregar produtos!\n");
    }
}


// Função para salvar os pedidos em um arquivo com múltiplos produtos por pedido
void salvarPedidos() {
    setlocale(LC_NUMERIC, "C");
    FILE *arquivo = fopen("pedidos.csv", "w");
    if (arquivo) {
        for (int i = 0; i < numPedidos; i++) {
            fprintf(arquivo, "Pedido ID: %d, Valor Total: %.2lf\n", pedidos[i].id, pedidos[i].total);  // Salva o total
            for (int j = 0; j < pedidos[i].numProdutos; j++) {
                Produto *produto = &produtos[pedidos[i].produtos[j].produtoId - 1];
                fprintf(arquivo, "Produto ID: %d, Nome: %s, Quantidade: %.2f\n",  // Alterado para float
                        produto->id, produto->nome, pedidos[i].produtos[j].quantidade);
            }
            fprintf(arquivo, "\n");
        }
        fclose(arquivo);
    } else {
        printf("Erro ao salvar venda!\n");
    }
}


// Função para carregar os pedidos de um arquivo
void carregarPedidos() {
    setlocale(LC_NUMERIC, "C");
    FILE *arquivo = fopen("pedidos.csv", "r");
    if (arquivo) {
        int pedidoId, produtoId;
        float quantidade;  // Alterado para float
        double total;
        char linha[100];

        Pedido novoPedido;
        int novoPedidoFlag = 0;

        while (fgets(linha, sizeof(linha), arquivo)) {
            if (sscanf(linha, "Pedido ID: %d, Valor Total: %lf", &pedidoId, &total) == 2) {
                if (novoPedidoFlag) {
                    pedidos[numPedidos++] = novoPedido;
                }

                novoPedido.id = pedidoId;
                novoPedido.numProdutos = 0;
                novoPedido.total = total;
                novoPedidoFlag = 1;
            } else if (sscanf(linha, "Produto ID: %d, Nome: %*[^,], Quantidade: %f", &produtoId, &quantidade) == 2) {  // Alterado para float
                if (novoPedido.numProdutos < MAX_PRODUTOS_POR_PEDIDO) {
                    ProdutoPedido novoProdutoPedido;
                    novoProdutoPedido.produtoId = produtoId;
                    novoProdutoPedido.quantidade = quantidade;

                    novoPedido.produtos[novoPedido.numProdutos++] = novoProdutoPedido;
                }
            }
        }

        if (novoPedidoFlag) {
            pedidos[numPedidos++] = novoPedido;
        }

        fclose(arquivo);
        printf("Vendas carregadas com sucesso.\n");
    } else {
        //printf("Erro ao abrir o arquivo de pedidos!\n");
    }
}

// Função para cadastrar um novo produto
void cadastrarProduto() {
    if (numProdutos >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido!\n");
        return;
    }

    Produto novoProduto;
    novoProduto.id = numProdutos + 1;

    printf("\nCadastro de Produto:\n");
    printf("Digite o nome do produto: ");
    scanf(" %[^\n]s", novoProduto.nome);
    printf("Digite o preço do produto: ");
    scanf("%lf", &novoProduto.preco);
    printf("Digite o tipo do produto (KG ou Unid): ");
    scanf(" %[^\n]s", novoProduto.tipo);
    printf("Digite a quantidade do produto: ");
    scanf("%f", &novoProduto.quantidade);  // Alterado para float

    produtos[numProdutos++] = novoProduto;
    printf("Produto cadastrado com sucesso!\n");

    salvarProdutos();
}

// Função para listar os produtos a partir do arquivo "produtos.csv"
void listarProdutos() {
    FILE *arquivo = fopen("produtos.csv", "r");
    if (arquivo == NULL) {
        printf(" Erro ao abrir o arquivo de produtos!\n");
        return;
    }

    char linha[200];

    // Ignora a primeira linha (cabeçalho)
    fgets(linha, sizeof(linha), arquivo);

    // Lê e exibe os produtos a partir do arquivo
    int produtosCarregados = 0;
    while (fscanf(arquivo, "%d,%49[^,],%lf,%49[^,],%f\n",
                  &produtos[produtosCarregados].id, produtos[produtosCarregados].nome,
                  &produtos[produtosCarregados].preco, produtos[produtosCarregados].tipo,
                  &produtos[produtosCarregados].quantidade) == 5) {
        // Exibe os dados dos produtos no console
        printf(" ID: %d, Nome: %s, Preço: %.2lf, Tipo: %s, Quantidade: %.3lf\n",
               produtos[produtosCarregados].id, produtos[produtosCarregados].nome,
               produtos[produtosCarregados].preco, produtos[produtosCarregados].tipo,
               produtos[produtosCarregados].quantidade);
        produtosCarregados++;  // Incrementa o contador de produtos carregados
    }

    fclose(arquivo);

    // Caso não haja produtos cadastrados
    if (produtosCarregados == 0) {
        printf(" Nenhum produto encontrado no arquivo.\n");
    }
}

// Função para editar um produto existente
void editarProduto() {
    setlocale(LC_NUMERIC, "C");
    listarProdutos();  // Lista os produtos cadastrados

    int id;
    printf("\n Edição de Produtos:\n\n");
    printf(" Digite o ID do produto a ser editado: ");
    scanf("%d", &id); // Lê o ID do produto a ser editado

    if (id < 1 || id > numProdutos) { // Verifica se o ID é válido
        printf(" Produto não encontrado!\n");
        return;
    }

    Produto *produto = &produtos[id - 1]; // Encontra o produto a ser editado
    printf(" Novo nome (atual: %s): ", produto->nome);
    scanf(" %[^\n]s", produto->nome); // Edita o nome do produto
    printf(" Novo preço (atual: %.2lf): ", produto->preco);
    scanf("%lf", &produto->preco); // Edita o preço do produto
    printf(" Novo tipo (atual: %s): ", produto->tipo);
    scanf(" %[^\n]s", produto->tipo); // Edita o tipo do produto
    printf(" Nova quantidade (atual: %f): ", produto->quantidade);
    scanf("%f", &produto->quantidade); // Edita a quantidade do produto

    printf(" Produto editado com sucesso!\n");

    // Após editar o produto, salva as alterações no arquivo CSV
    salvarProdutos();
}

// Função para excluir um produto e atualizar os IDs
void excluirProduto() {
    listarProdutos();  // Exibe os produtos cadastrados

    int id;
    printf("\n Exclusão de Produtos:\n\n");
    printf(" Digite o ID do produto a ser excluído: ");
    scanf("%d", &id); // Lê o ID do produto a ser excluído

    if (id < 1 || id > numProdutos) { // Verifica se o ID é válido
        printf(" Produto não encontrado!\n");
        return;
    }

    // Move todos os produtos após o ID excluído para preencher o espaço
    int i;
    for (i = id - 1; i < numProdutos - 1; i++) {
        produtos[i] = produtos[i + 1];
    }

    numProdutos--; // Reduz o contador de produtos

    // Atualiza os IDs dos produtos para garantir que sejam sequenciais
    for (i = 0; i < numProdutos; i++) {
        produtos[i].id = i + 1;  // Atualiza o ID para começar de 1 novamente
    }

    // Atualiza o arquivo CSV removendo o produto excluído e com IDs atualizados
    salvarProdutos();

    printf(" Produto excluído com sucesso!\n");
}

void menuProdutosUser()
{
    int opcao;
    while (1)
    {
        // Menu de opÃ§Ãµes para gerenciamento de produtos
        printf("\n Menu de Produtos:\n\n");
        printf(" 1. Listar Produtos\n");
        printf(" 2. Voltar\n");
        printf(" Escolha uma opção: ");
        scanf("%d", &opcao); // LÃª a opÃ§Ã£o do usuÃ¡rio

        // Executa a funÃ§Ã£o correspondente Ã  opÃ§Ã£o escolhida
        switch (opcao)
        {
        case 1:
        system("cls");
            listarProdutos();
            break;
        case 2:
        system("cls");
            return; // Retorna ao menu principal
        default:
            printf(" Opção inválida! Tente novamente.\n");
        }
    }
}

void menuProdutosAdm()
{
    int opcao;
    while (1)
    {
        // Menu de opções para gerenciamento de produtos
        printf("\n Menu de Produtos:\n\n");
        printf(" 1. Cadastrar Produto\n");
        printf(" 2. Listar Produtos\n");
        printf(" 3. Editar Produto\n");
        printf(" 4. Excluir Produto\n");
        printf(" 5. Salvar e Sair\n");
        printf(" Escolha uma opção: ");
        scanf("%d", &opcao); // Lê a opção do usuário

        // Executa a função correspondente á  opção escolhida
        switch (opcao)
        {
        case 1:
        system("cls");
            cadastrarProduto();
            break;
        case 2:
        system("cls");
            listarProdutos();
            break;
        case 3:
        system("cls");
            editarProduto();
            break;
        case 4:
        system("cls");
            excluirProduto();
            break;
        case 5:
            system("cls");
            salvarProdutos();
            return;
        default:
            printf(" Opção inválida! Tente novamente.\n");
        }
    }
}

// Função para cadastrar um novo pedido com múltiplos produtos
void cadastrarPedido() {
    if (numPedidos >= MAX_PEDIDOS) {
        printf(" Limite de Vendas atingido!\n");
        return;
    }

    Pedido novoPedido;
    novoPedido.id = numPedidos + 1;
    novoPedido.numProdutos = 0;
    novoPedido.total = 0.0; // Inicializa o total do pedido

    listarProdutos();

    int adicionarMais;
    do {
        
        
        if (novoPedido.numProdutos >= MAX_PRODUTOS_POR_PEDIDO) {
            printf(" Limite de produtos por pedido atingido!\n");
            break;
        }

        ProdutoPedido novoProdutoPedido;
        printf(" \n Digite o ID do produto: ");
        scanf("%d", &novoProdutoPedido.produtoId);

        if (novoProdutoPedido.produtoId < 1 || novoProdutoPedido.produtoId > numProdutos) {
            printf(" Produto não encontrado!\n");
            continue;
        }

        printf(" Digite a quantidade do produto: ");
        scanf("%f", &novoProdutoPedido.quantidade);

        Produto *produto = &produtos[novoProdutoPedido.produtoId - 1];
        if (novoProdutoPedido.quantidade > produto->quantidade) {
            printf(" Quantidade indisponível em estoque!\n");
            continue;
        }

        // Adiciona o produto ao pedido
        novoPedido.produtos[novoPedido.numProdutos++] = novoProdutoPedido;
        produto->quantidade -= novoProdutoPedido.quantidade;

        // Atualiza o total do pedido
        novoPedido.total += produto->preco * novoProdutoPedido.quantidade;

        printf(" Produto adicionado ao pedido com sucesso!\n");
        printf(" Deseja adicionar outro produto ao pedido? (1-Sim, 0-Não): ");
        scanf("%d", &adicionarMais);

    } while (adicionarMais);


    // Mostrar lista de produtos comprados
    printf("\n Compra:\n");
    for (int i = 0; i < novoPedido.numProdutos; i++) {
        Produto *produto = &produtos[novoPedido.produtos[i].produtoId - 1];
        printf(" Produto: %s, Quantidade: %.3lf %s, Preço: %.2lf %s, Total: %.2lf\n",
               produto->nome, novoPedido.produtos[i].quantidade, produto->tipo, produto->preco, produto->tipo,
               produto->preco * novoPedido.produtos[i].quantidade);
    }

    // Exibe o valor total do pedido antes de finalizar
    printf("\n Valor total do pedido: %.2lf reais\n", novoPedido.total);

    // Escolha do método de pagamento
    int metodoPagamento;
    printf("\n Escolha o método de pagamento:\n");
    printf(" 1 - PIX\n");
    printf(" 2 - Dinheiro\n");
    printf(" Escolha uma opção: ");
    scanf("%d", &metodoPagamento);

    // Chave PIX
    const char *chavePix = " Naturezaviva@gmail.com";

    switch (metodoPagamento) {
        case 1:
            printf(" \n Chave PIX: %s\n", chavePix);
            char confirmacaoPix;
            printf(" \n Cliente realizou o pagamento via PIX? (s/n): ");
            scanf(" %c", &confirmacaoPix);

            if (confirmacaoPix == 's' || confirmacaoPix == 'S') {
                printf(" \n Pagamento via PIX confirmado. Total Pago: %.2lf reais.\n", novoPedido.total);
            } else {
                printf(" Pagamento via PIX não confirmado. O pedido será cancelado.\n");
                return;
            }
            break;
        case 2: {
            double valorNota;
            printf(" Total a pagar: %.2lf reais.\n", novoPedido.total);
            printf("\n Informe o valor da nota recebida: ");
            scanf("%lf", &valorNota);
            
            if (valorNota < novoPedido.total) {
                printf(" Valor da nota insuficiente para cobrir o total do pedido!\n");
                return; // Retorna se o valor da nota for insuficiente
            }

            double troco = valorNota - novoPedido.total;
            printf("\n Troco: %.2lf reais.\n", troco);
            break;
        }
        default:
            printf(" Método de pagamento inválido! O pedido será cancelado.\n");
            return;
    }

    // Adiciona o pedido à lista de pedidos
    pedidos[numPedidos++] = novoPedido;
    salvarPedidos();
    salvarProdutos();
    printf("\n Compra Finalizada\n");
}

// Função para listar todos os pedidos
void listarPedidos() {
   if (numPedidos == 0) {
        printf(" Nenhum venda feita!\n");
        return;
    }

    printf("\n Lista de Vendas:\n");
    for (int i = 0; i < numPedidos; i++) {
        printf(" Venda ID: %d, Total: %.2lf reais\n", pedidos[i].id, pedidos[i].total);
        for (int j = 0; j < pedidos[i].numProdutos; j++) {
            int produtoId = pedidos[i].produtos[j].produtoId - 1;
            double totalProduto = produtos[produtoId].preco * pedidos[i].produtos[j].quantidade;
            printf("   Produto: %s, Quantidade: %.3lf %s, Total: %.2lf reais\n",
                   produtos[produtoId].nome, pedidos[i].produtos[j].quantidade, produtos[produtoId].tipo, totalProduto);
        }
        printf("\n");
    }
}

// Função para editar um pedido
void editarPedido() {
    listarPedidos();
    int id;
    printf(" Digite o ID da Venda a ser editado: ");
    scanf("%d", &id);  // Lê o ID do pedido
    if (id < 1 || id > numPedidos) {  // Verifica se o ID é válido
        printf(" Venda não encontrada!\n");
        return;
    }

    Pedido *pedido = &pedidos[id - 1];  // Encontra o pedido a ser editado
    double totalAnterior = pedido->total;  // Armazena o total anterior
    pedido->total = 0.0;  // Reseta o total para recalcular

    // Loop para editar as quantidades dos produtos no pedido
    for (int i = 0; i < pedido->numProdutos; i++) {
        int produtoId = pedido->produtos[i].produtoId;  // ID do produto
        float quantidadeAnterior = pedido->produtos[i].quantidade;  // Quantidade anterior
        float novaQuantidade;

        printf(" Produto ID: %d, Nome: %s, Quantidade anterior: %.3lf. Digite nova quantidade: ",
               produtoId, produtos[produtoId - 1].nome, quantidadeAnterior);
        scanf("%f", &novaQuantidade);  // Lê a nova quantidade

        // Atualiza a quantidade do produto no estoque
        produtos[produtoId - 1].quantidade += quantidadeAnterior;  // Devolve a quantidade anterior
        if (novaQuantidade <= produtos[produtoId - 1].quantidade) {  // Verifica se a nova quantidade é válida
            pedido->produtos[i].quantidade = novaQuantidade;  // Atualiza a quantidade no pedido
            produtos[produtoId - 1].quantidade -= novaQuantidade;  // Retira a nova quantidade do estoque

            // Recalcula o total do pedido
            pedido->total += produtos[produtoId - 1].preco * novaQuantidade;
        } else {
            printf(" Quantidade em estoque insuficiente para o pedido!\n");
            pedido->produtos[i].quantidade = quantidadeAnterior;  // Retorna à quantidade anterior
            produtos[produtoId - 1].quantidade -= quantidadeAnterior;  // Retira a quantidade anterior do estoque
            pedido->total += produtos[produtoId - 1].preco * quantidadeAnterior;  // Recalcula o total
        }
    }
    salvarPedidos();
    salvarProdutos();
    printf(" Venda editada com sucesso! Novo total: %.2lf reais\n", pedido->total);  // Exibe o novo total
}

// Função para excluir um pedido
void excluirPedido() {
    listarPedidos();  // Mostra os pedidos atuais
    int id;
    printf(" Digite o ID da Venda a ser excluído: ");
    scanf("%d", &id);
    if (id < 1 || id > numPedidos) {
        printf(" Venda não encontrada!\n");
        return;
    }

    // Devolve a quantidade dos produtos ao estoque antes de excluir
    for (int j = 0; j < pedidos[id - 1].numProdutos; j++) {
        int produtoId = pedidos[id - 1].produtos[j].produtoId - 1;
        produtos[produtoId].quantidade += pedidos[id - 1].produtos[j].quantidade;
    }

    // Move todos os pedidos após o ID excluído para preencher o espaço
    for (int i = id; i < numPedidos; i++) {
        pedidos[i - 1] = pedidos[i];  // Move o pedido para a posição anterior
        pedidos[i - 1].id = i;  // Atualiza o ID do pedido
    }
    numPedidos--;  // Reduz o contador de pedidos
    salvarPedidos();  // Salva os pedidos atualizados
    salvarProdutos();
    printf(" Venda excluída com sucesso!\n");
}

void menuPedidosAdm()
{
    int opcao;
    while (1)
    {
        printf("\n Menu de Vendas:\n\n");
        printf(" 1 - Cadastrar Venda\n");
        printf(" 2 - Listar Vendas\n");
        printf(" 3 - Editar Vendas\n");
        printf(" 4 - Excluir Venda\n");
        printf(" 5 - Salvar e Sair\n");;
        printf(" Escolha uma Opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            system("cls");
            cadastrarPedido();
            break;
        case 2:
            system("cls");
            listarPedidos();
            break;
        case 3:
            system("cls");
            editarPedido();
            break;
        case 4:
            system("cls");
            excluirPedido();
            break;
        case 5:
            system("cls");
            return; // Retorna ao menu principal
            salvarPedidos();
            salvarProdutos();
        default:
            printf(" Opção inválida! Tente novamente.\n");
        }
    }
}

// Menu de pedidos
void menuPedidosUser()
{
    int opcao;
    while (1)
    {
        printf("\n Menu de Vendas:\n\n");
        printf(" 1 - Cadastrar Venda\n");
        printf(" 2 - Listar Vendas\n");
        printf(" 3 - Salvar e Sair\n");
        printf(" Escolha uma Opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            system("cls");
            cadastrarPedido();
            break;
        case 2:
            system("cls");
            listarPedidos();

            break;
        case 3:
            system("cls");
            return; // Retorna ao menu principal
            salvarPedidos();
            salvarProdutos();
        default:
            printf(" Opção Inválida! Tente novamente.\n");
        }
    }
}

// Função para listar os usuários cadastrados
void ListarUsuario() {
    char *nome_arquivo_login = "login.csv";  // Nome do arquivo de login
    printf("\n Listar logins:\n\n");
    lerArquivo(nome_arquivo_login);  // Chama a função para ler e exibir os dados do arquivo
}

void editarUsuario() {
    char *nome_arquivo_login = "login.csv";
    char usuarioAlvo[100], novoNome[100], novaSenha[100], novoNivel[10];
    int encontrado = 0;
    ListarUsuario();

    printf("\n Edição de Usuários:\n\n");
    printf("\n Digite o usuário que deseja editar: ");
    scanf("%s", usuarioAlvo);

    FILE *arquivo = fopen(nome_arquivo_login, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (arquivo == NULL || temp == NULL) {
        //perror(" Erro ao abrir o arquivo");
        exit(1);
    }

    char linha[1000];

    // Copia o cabeçalho do arquivo original
    fgets(linha, sizeof(linha), arquivo);
    fprintf(temp, "%s", linha);

    // Lê cada linha do arquivo original
    while (fgets(linha, sizeof(linha), arquivo)) {
        char usuario[100], senha[100], nivel[10];

        // Usa sscanf para capturar os campos usando vírgula como delimitador
        if (sscanf(linha, "%[^,],%[^,],%[^,\n]", usuario, senha, nivel) != 3) {
            continue; // Pula linhas mal formatadas
        }

        // Verifica se o usuário a ser editado foi encontrado
        if (strcmp(usuarioAlvo, usuario) == 0) {
            encontrado = 1;
            printf("\n Digite um novo nome para %s: ", usuarioAlvo);
            scanf("%s", novoNome);
            printf(" Digite a nova senha para %s: ", usuarioAlvo);
            scanf("%s", novaSenha);
            printf(" Digite o novo nível de acesso para %s (ADM ou USER): ", usuarioAlvo);
            scanf("%s", novoNivel);

            // Escreve a linha editada no arquivo temporário
            fprintf(temp, "%s,%s,%s\n", novoNome, novaSenha, novoNivel);
        } else {
            // Escreve a linha original no arquivo temporário
            fprintf(temp, "%s,%s,%s\n", usuario, senha, nivel);
        }
    }

    fclose(arquivo);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    remove(nome_arquivo_login);
    rename("temp.csv", nome_arquivo_login);

    if (encontrado) {
        printf("\n Usuário editado com sucesso!\n");
    } else {
        printf("\n Usuário não encontrado.\n");
    }
}


// Função que apresenta o menu de gerenciamento de usuários
void MenuUsuarios() {
    int opcao;  // Variável para armazenar a opção do menu
    while (1) {
        printf("\n Menu de Usuários:\n\n");
        printf(" 1 - Cadastrar Usuário\n");
        printf(" 2 - Cadastrar ADM\n");
        printf(" 3 - Editar Usuário\n");
        printf(" 4 - Listar Usuários\n");
        printf(" 5 - Voltar ao Menu Principal\n");  // Atualizando a opção de voltar
        printf(" Escolha uma opção: ");
        scanf("%d", &opcao);  // Lê a opção escolhida

        // Executa a opção escolhida
        switch (opcao) {
        case 1:
        system("cls");
            CadastrarUsuario();  // Cadastra um novo usuário
            break;
        case 2:
        system("cls");
            CadastrarADM();  // Cadastra um novo administrador
            break;
        case 3:
        system("cls");
            editarUsuario();  // Edita um usuário existente
            break;
        case 4:
        system("cls");
            ListarUsuario();  // Lista todos os usuários
            break;
        case 5:
        system("cls");
            return;  // Retorna ao menu principal
        default:
            printf(" Opção inválida! Tente novamente.\n");  // Mensagem de erro para opção inválida
        }
    }
}

// Função para deslogar um usuário, limpando os dados de login
void logout() {
    strcpy(login.usuario, "");  // Limpa o nome de usuário
    strcpy(login.senha, "");    // Limpa a senha
    strcpy(login.nivelAcesso, "");  // Limpa o nível de acesso
}

// Função para realizar o login do usuário
void realizarLogin(char *nome_arquivo_login) {
    usuario usuario_login;  // Estrutura para armazenar os dados do usuário que está tentando logar
    int resposta = 0;  // Variável para armazenar o resultado da autenticação

    while (resposta != 1) {  // Loop até que o usuário esteja autenticado
        printf("\n ==== Hortifruti Natureza Pura ====\n\n");
        printf(" Login do Usuário:\n\n");
        printf(" Usuário: ");
        scanf("%s", usuario_login.usuario);  // Lê o nome de usuário
        printf(" Senha: ");
        scanf("%s", usuario_login.senha);  // Lê a senha do usuário

        system("cls");

        // Compara as credenciais do usuário com os dados do arquivo
        resposta = compararUsuarioSenha(nome_arquivo_login, usuario_login.usuario, usuario_login.senha);

        if (resposta != 1) {  // Se a autenticação falhar
            system("cls");
            printf(" Usuário ou senha incorretos! Por favor, tente novamente.\n\n");  // Mensagem de erro
        }
    }
    printf(" Olá %s\n", usuario_login.usuario);  // Mensagem de boas-vindas ao usuário autenticado
}


// Função principal do programa
int main() {
    setlocale(LC_ALL, "PORTUGUESE");  // Define a localidade para português
    char *nome_arquivo_login = "login.csv";  // Nome do arquivo de login

    carregarPedidos();
    carregarProdutos();

    gerarLoginInicial(nome_arquivo_login);  // Gera o login inicial
    while (1) {
        realizarLogin(nome_arquivo_login);  // Realiza o login do usuário
        while (1) {
            int itemMenu;  // Variável para armazenar a opção do menu
            // Mostra o menu apropriado com base no nível de acesso do usuário
            if (strcmp(login.nivelAcesso, "USER") == 0)
                mostrarMenuUser();  // Exibe menu do usuário
            else
                mostrarMenuAdmin();  // Exibe menu do administrador

            scanf("%d", &itemMenu);  // Lê a opção escolhida
            if (itemMenu == 0) {
                system("cls");
                logout();  // Se a opção for 0, desloga o usuário
                break;  // Sai do loop do menu
            }

            // Executa as ações baseadas no nível de acesso
            if (strcmp(login.nivelAcesso, "USER") == 0) {
                switch (itemMenu) {
                case 1:
                system("cls");
                    menuPedidosUser();  // Comentado, função para menu de pedidos do usuário
                    break;
                case 2:
                system("cls");
                    menuProdutosUser();  // Comentado, função para menu de produtos do usuário
                    break;
                default:
                system("cls");
                    printf(" Opção inválida!\n");  // Mensagem de erro para opção inválida
                }
            } else {
                switch (itemMenu) {
                case 3:
                system("cls");
                    menuProdutosAdm();  // Comentado, função para menu de produtos do administrador
                    break;
                case 4:
                system("cls");
                    menuPedidosAdm();  // Comentado, função para menu de pedidos do administrador
                    break;
                case 5:
                system("cls");
                    MenuUsuarios();  // Abre o menu de gerenciamento de usuários
                    break;
                default:
                system("cls");
                    printf(" Opção inválida!\n");  // Mensagem de erro para opção inválida
                }
            }
        }
    }
    return 0;  // Retorna 0 para indicar que o programa terminou com sucesso
}



