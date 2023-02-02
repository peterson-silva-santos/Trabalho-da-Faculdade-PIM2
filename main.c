#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#define CONTEUDO " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789  " // vai  ser usado para procura conteudo digitado
#define CONVERTE " 1234567890)(*&¨$#@!><:?}^`{zxcçvbnmasdfghjklqwertyuiop/;.,~|\-  "
//legenda:
//nome - Usuario
//password - senha
//acesso - nivel de acesso ao app (1 ADM, 2 Gerencia, 3 Funcionario)
//salarioph - salario por hora
//horaspm - horas por mes
//cargo - nome do cargo ocupado
//nivel - junior, senior, pleno, estagiario etc
//cpf - cpf do funcionario
//score - valor administrativo
//lido - variavel para o sistema de mensagens
struct user
{
    char nome[25];
    char password[25];
    int acesso;
    float salarioph;
    float horaspm;
    char cargo[25];
    char nivel[10];
    char cpf[15];
    int score;
    int lido;

} funcionario[100];
//legenda:
//telefone - telefone da empresa
//nomeempresa - nome da empresa
//telefonepessoal - contato pessoal com a empresa
//endereco - endereço da empresa
//email - email para contato
//cpnj - cpnj da empresa
typedef struct cliente
{
    char telefone[15];
    char nomeempresa[25];
    char telefonepessoal[15];
    char endereco[100];
    char email[100];
    char cnpj[15];
    char cpf[15];
    char nomedocliente[25];
    struct cliente *prox;
} clientes;
//variaveis globais para o programa
char passwordcryph[25];
char texto[1000];
clientes *inicioCli = NULL; //ponteiro para a lista clientes
int funTam; //variavel auxiliar para o vetor de funcionarios

//Função Main
int main(void)
{
    setlocale(LC_ALL,"");
    int op_main=0; //variavel para seleção de opção no menu
    int indice; //variavel o "indice" para localizar qual funcionario está logado dentre os funcionarios no vetor a struct funcionarios

    //declaração de funções
    char * criptografar(char * p_texte);
    char *decriptografia( char * p_texte);

    CriaCliLista();
    databaseFill();//chama a função que faz a leitura dos arquivos de cadastros para preencher as variaveis de funcionários e clientes
    //criptografar();
    do
    {
        system("cls");
        printf ("========================================================================================================================\n");
        printf ("\t\t\t\t\t\t1 - Logar\n\t\t\t\t\t\t2 - Sair\n");
        printf ("========================================================================================================================\n");
        scanf ("%d", &op_main);
        fflush(stdin);
        switch(op_main)
        {
        case 1:
            system("cls");
            //chama o login para enviar o resultado em "indice"
            indice = login(funcionario);
            if (funcionario[indice].acesso==1)
            {
                menuAdm(indice, funcionario);
            }
            if (funcionario[indice].acesso==2)
            {
                menuGen(indice, funcionario);
            }
            if (funcionario[indice].acesso==3)
            {
                menuFun(indice, funcionario);
            }
            break;

        case 2:
            printf("\n\t\t\t\t\t\t...Saindo do sistema...");
            exit(0);
            break;
        default:
            printf("\n\t\t\t\t\t\tOpção inválida!");

        }
    }
    while(op_main!=2);
}



//função para login do usuario
int login(struct user login[])
{
    char loginuser[25];
    char loginsenha[25];
    char c;
    int i=0;
    int a = 0;
    fflush(stdin);
    printf ("\t\t\t\t\t\tLOGIN: ");
    gets (loginuser);
    printf("\n\t\t\t\t\t\tSENHA: ");
    do
    {
        c=getch();
        if(isprint(c))        //ANALISA SE O CARACTER DIGITADO É IMPRIMÍVEL
        {
            loginsenha[a]=c;         //ARMAZENA O CARACTER NO VETOR
            a++;                //INCREMENTA A POSIÇÃO DO VETOR
            printf("*");        //IMPRIME O *
        }
        else if(c==8&&a)      //8 É O CARACTER ESPAÇO NA TABELA ASCII; VERIFICA SE O CARACTER DIGITADO É ESPAÇO E SE ESTÁ NA PRIMEIRA POSIÇÃO DO VETOR
        {
            loginsenha[a]='\0';
            a--;
            printf("\b \b");       //APAGA O CARACTER DIGITADO
        }
    }
    while(c!=13);              //13 É O VALOR DE ENTER NA TABELA ASCII

    loginsenha[a]='\0';   //INSERE O /0 NO FINAL DO VETOR
    //loop para testar a senha e login dentro do vetor, usando como limitador a variavel "funTam", que recebe quantos funcionarios estão registrados no arquivo de cadastro
    while(i!=funTam)
    {
        if ((strcmp(login[i].nome, loginuser) == 0) && (strcmp(login[i].password, loginsenha) == 0))
        {
            printf("\n\t\t\t\t\t\tLogado com suscesso!");
            return i;
        }
        else
        {
            i++;
        }
    }
    printf ("\n\t\t\t\t\t\tUsuario ou senha incorretos!");
    return -1;
}

//Menu dos usuarios com acesso Administrativo
void menuAdm(int indice)
{
    FILE *file = fopen("mensagem.txt", "r");
    fflush(stdin);
    int op=0;
    char mensagem[255];
    do //inicio do loop de menu
    {
        fflush(stdin);
        system("cls");
        int titulo = 0;
        while ((fgets(mensagem, 255, file))!=NULL)
        {
            if (funcionario[indice].lido == 1 && mensagem != NULL)
            {
                if (titulo!=1)
                {
                    printf ("========================================================================================================================\n\t\t\t\t\t\t!NOVA MENSAGEM!\n\n");
                    titulo = 1;
                }
                printf("\t\t\t\t\t\t%s", mensagem);
            }
        }
        funcionario[indice].lido = 0;
        UpdateCadastro();//chamando a variavel de atualização de cadastro para atualizar que o usuario logado recebeu a mensagem
        printf ("========================================================================================================================\n");
        printf ("\t\t\t\t\t\t1 - Cadastrar Cliente\n\t\t\t\t\t\t2 - Cadastrar Funcionário\n\t\t\t\t\t\t3 - Cadastrar Horas Trabalhadas\n\t\t\t\t\t\t4 - Cadastrar Advertencia\n\t\t\t\t\t\t5 - Relatórios\n\t\t\t\t\t\t6 - Perfil\n\t\t\t\t\t\t7 - Mensagens\n\t\t\t\t\t\t8 - Trocar Senha\n\t\t\t\t\t\t9 - Sair\n");
        printf ("========================================================================================================================\n");
        scanf ("%d", &op);
        //switch-case que utiliza a variavel "op" para enviar o usuario para as outras funções do programa.
        switch(op)
        {
        case 1:
            CadastrarCli();
            break;
        case 2:
            CadastrarFun(indice);
            break;
        case 3:
            CadastrarHoras();
            break;
        case 4:
            CadastrarAdv(indice);
            break;
        case 5:
            Relatorios(indice);
            break;
        case 6:
            Perfil(indice);
            break;
        case 7:
            mensagens();
            break;
        case 8:
            TrocarSenha(indice);
            break;
        case 9:
            exit(0);
            break;
        default:
            printf("\t\t\t\t\t\tPor favor, digite uma opção válida!");
            system("pause");
        }
    }
    while(op!=8); //fim e teste do loop de menu.
}

//menu de usuarios com acesso de gerencia
void menuGen(int indice)
{
    FILE *file = fopen("mensagem.txt", "r");
    fflush(stdin);
    int op=0;
    char mensagem[255];
    do //inicio do loop de menu
    {
        fflush(stdin);
        system("cls");
        int titulo = 0;
        while ((fgets(mensagem, 255, file))!=NULL)
        {
            if (funcionario[indice].lido == 1 && mensagem != NULL)
            {
                if (titulo!=1)
                {
                    printf ("========================================================================================================================\n\t\t\t\t\t\t!NOVA MENSAGEM!\n\n");
                    titulo = 1;
                }
                printf("\t\t\t\t\t\t%s", mensagem);
            }
        }
        funcionario[indice].lido = 0;
        UpdateCadastro();
        printf ("========================================================================================================================\n");
        printf ("\t\t\t\t\t\t1 - Cadastrar Cliente\n\t\t\t\t\t\t2 - Cadastrar Funcionário\n\t\t\t\t\t\t3 - Cadastrar Horas Trabalhadas\n\t\t\t\t\t\t4 - Cadastrar Advertencia\n\t\t\t\t\t\t5 - Relatórios\n\t\t\t\t\t\t6 - Perfil\n\t\t\t\t\t\t7 - Mensagens\n\t\t\t\t\t\t8 - Trocar Senha\n\t\t\t\t\t\t9 - Sair\n");
        printf ("========================================================================================================================\n");
        scanf ("%d", &op);
        //switch-case que utiliza a variavel "op" para enviar o usuario para as outras funções do programa.
        switch(op)
        {
        case 1:
            op = CadastrarCli();
            break;
        case 2:
            op = CadastrarFun(indice);
            break;
        case 3:
            op = CadastrarHoras();
            break;
        case 4:
            op = CadastrarAdv(indice);
            break;
        case 5:
            op = Relatorios(indice);
            break;
        case 6:
            op = Perfil(indice);
            break;
        case 7:
            op = mensagens();
            break;
        case 8:
            op = TrocarSenha(indice);
            break;
        case 9:
            exit(0);
            break;
        default:
            printf("\t\t\t\t\t\tPor favor, digite uma opção válida!");
            system("pause");
        }
    }
    while(op!=8); //fim e teste do loop de menu.
}

//menu de usuarios com acesso de funcionario
void menuFun(int indice)
{
    FILE *file = fopen("mensagem.txt", "r");
    fflush(stdin);
    int op=0;
    char mensagem[255];
    do //inicio do loop de menu
    {
        fflush(stdin);
        system("cls");
        int titulo = 0;
        while ((fgets(mensagem, 255, file))!=NULL)
        {
            if (funcionario[indice].lido == 1 && mensagem != NULL)
            {
                if (titulo!=1)
                {
                    printf ("========================================================================================================================\n\t\t\t\t\t\t!NOVA MENSAGEM!\n\n");
                    titulo = 1;
                }
                printf("\t\t\t\t\t\t%s", mensagem);
            }
        }
        funcionario[indice].lido = 0;
        UpdateCadastro();
        printf ("========================================================================================================================\n");
        printf ("\t\t\t\t\t\t1 - Cadastrar Cliente\n\t\t\t\t\t\t2 - Relatórios\n\t\t\t\t\t\t3 - Perfil\n\t\t\t\t\t\t4 - Trocar Senha\n\t\t\t\t\t\t5 - Sair\n");
        printf ("========================================================================================================================\n");
        scanf ("%d", &op);
        switch(op)
        {
        case 1:
            op = CadastrarCli();
            break;
        case 2:
            op = Relatorios(indice);
            break;
        case 3:
            op = Perfil(indice);
            break;
        case 4:
            op = TrocarSenha(indice);
            break;
        case 5:
            exit(0);
            break;
        default:
            printf("\t\t\t\t\t\tPor favor, digite uma opção válida!");
            system("pause");
        }

    }
    while(op!=5); //fim e teste do loop de menu.

}

//função para cadastrar clientes
int CadastrarCli()
{
    fflush(stdin);
    clientes *p; //criação do ponteiro para cadastrar clientes na lista
    p =  (clientes *) malloc(sizeof(clientes)); //ponteiro recebe um espaço de memória do tamanho certo para uma variavel do tipo Funcionario
    //teste de alocação
    if (p==NULL)  //SE RECEBER NULL A ALOCAÇÃO NÃO PODE SER REALIZADA
    {
        printf("ERRO DE ALOCACAO\n\n");
        return 0;  //não foi possível acrescentar conteúdo
    }
    system ("cls");
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t\tCadastro de Clientes\n");
    printf ("========================================================================================================================\n");
    printf("\t\t\t\t\t\ Razão social: ");
    gets(p->nomeempresa);
    printf("\t\t\t\t\t\ CPNJ: ");
    scanf("%s", p->cnpj); //inicio do recebimento das variaveis de clientes
    fflush(stdin);
    printf("\t\t\t\t\t\ Endereço: ");
    gets(p->endereco);
    fflush(stdin);
    printf("\t\t\t\t\t\ Telefone: ");
    gets(p->telefone);
    fflush(stdin);
    printf("\t\t\t\t\t\ E-mail: ");
    gets(p->email);
    printf("\t\t\t\t\t\ Nome do resposavel: ");
    gets(p->nomedocliente);
    printf("\t\t\t\t\t\ CPF: ");
    gets(p->cpf);
    printf("\t\t\t\t\t\Telefone do Responsavel: ");
    gets(p->telefonepessoal);
    //fim do recebimento das variaveis de clientes
    //impressão dos dados no arquivo de cadastro de clientes
    FILE *file = fopen("cadastro_clientes.txt", "a");//abertura do arquivo de cadastro de clientes na forma de addicionar
    fprintf (file, "\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n------------", p->nomeempresa, p->cnpj, p->endereco, p->telefone, p->email, p->nomedocliente, p->cpf, p->telefonepessoal);
    fclose(file);
    p->prox = NULL;//criação do proximo espaço da lista
    if (inicioCli==NULL)
    {
        inicioCli = p;
    }
    else
    {
        p->prox = inicioCli;
        inicioCli = p;
    }

    return 0;
}

//função de cadastro de funcionários
void CadastrarFun(int indice)
{
    fflush(stdin);
    int acesso;
    system ("cls");
    char nomeaux[25];
    fflush(stdin);
    char *criptografar( char * p_texte);
    FILE *file = fopen("cadastro_funcionarios.txt", "a");//abertura do arquivo de cadastro de funcionários
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t\tCadastro de Funcionários\n");
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t\tNome: ");//inicio do recebimento das variaveis de funcionário, utilizando a variavel "funTam" para saber qual a próxima area aberta no vetor de funcionários
    gets(nomeaux);
    strcpy(funcionario[funTam].nome, nomeaux);

    printf ("\n\t\t\t\t\t\tSenha: ");
    scanf("%s", funcionario[funTam].password);
    strcpy(passwordcryph, funcionario[funTam].password);
    char * resultado = criptografar(passwordcryph);

    if (funcionario[indice].acesso==1) //estrutura de decisão - apenas administradores podem escolher acesso, caso a pessoa registrando seja apenas um gerente, vai para o else
    {

        printf ("\n\t\t\t\t\t\t[1] - Gerente ");
        printf ("\n\t\t\t\t\t\t[2] - Funcionário ");
        do
        {
            printf ("\n\t\t\t\t\t\tAcesso: ");
            scanf("%d", &acesso);
            funcionario[funTam].acesso = acesso+1;
            if((funcionario[funTam].acesso<2) || (funcionario[funTam].acesso>3))
            {
                printf("\n\t\t\t\tOpção inválida! Insira novamente!\n");
            }
        }
        while ((funcionario[funTam].acesso<2) || (funcionario[funTam].acesso>3));

    }
    else  //caso o usuario não tenha acesso de administração, a variavel recebe o valor "3" para acesso de funcionário automaticamente
    {
        funcionario[funTam].acesso = 3;
    }
    fflush(stdin);
    printf ("\n\t\t\t\t\t\tSalário: ");
    scanf("%f", &funcionario[funTam].salarioph);
    fflush(stdin);
    printf ("\t\t\t\t\t\tCargo: ");
    scanf("%s", funcionario[funTam].cargo);
    fflush(stdin);
    printf ("\t\t\t\t\t\tNivel: ");
    scanf("%s", funcionario[funTam].nivel);
    fflush(stdin);
    printf ("\t\t\t\t\t\tCPF: ");
    gets(nomeaux);
    strcpy(funcionario[funTam].cpf, nomeaux);
    funcionario[funTam].horaspm = 0;
    funcionario[funTam].score = 100;
    funcionario[funTam].lido = 1;
    printf("\n");
    //impressão dos valores recebidos no arquivo de cadastro
    fprintf(file, "\n%s\n%s\n%d\n%f\n%f\n%s\n%s\n%s\n%d\n%d\n--------", funcionario[funTam].nome, resultado, funcionario[funTam].acesso, funcionario[funTam].salarioph, funcionario[funTam].horaspm, funcionario[funTam].cargo, funcionario[funTam].nivel, funcionario[funTam].cpf, funcionario[funTam].score, funcionario[funTam].lido);
    funTam++;
    fclose(file);
    fflush(stdin);
    system ("pause");
}

void CadastrarHoras()
{
    system("cls");
    fflush(stdin);
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t Cadastrar Horas:\n");
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t - Selecione o Funcionário -\n");
    int i, aux, aux2;
    float horas;
    for (i=0; i<funTam; i++)
    {
        printf ("\t\t %d - %s\n", i+1, funcionario[i].nome);
    }
    scanf("%d", &aux);
    aux=aux-1;
    system("cls");
    fflush(stdin);
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t Cadastrar Horas: %s\n", funcionario[aux].nome);
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t %s tem %.0f horas.\n", funcionario[aux].nome, funcionario[aux].horaspm);
    printf ("\t\t\t\t\t 1 - Adicionar Horas\n");
    printf ("\t\t\t\t\t 2 - Substituir Horas\n");
    scanf ("%d", &aux2);
    switch (aux2)
    {
    case 1:
        printf ("\t\t\t\t Digite quantas horas deseja adicionar: ");
        scanf("%f", &horas);
        funcionario[aux].horaspm = funcionario[aux].horaspm+horas;
        printf ("\t\t\t\t %s tem %.0f horas.\n", funcionario[aux].nome, funcionario[aux].horaspm);
        break;
    case 2:
        printf ("\t\t\t\tATENÇÃO: Essa ação vai substituir completamente as horas do funcionario!\n\t\t\t\tDigite o numero de horas: ");
        scanf("%f", &horas);
        funcionario[aux].horaspm = horas;
        printf ("\t\t\t\t%s tem %.0f horas.\n", funcionario[aux].nome, funcionario[aux].horaspm);
        break;
    default:
        printf ("\t\t\t\tOpção Inválida!");
        break;
    }

    UpdateCadastro();
    system("pause");
    return 0;

}

void CadastrarAdv(int indice)
{
    system("cls");
    fflush(stdin);
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t Cadastrar Advertencia:\n");
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t - Selecione o Funcionário -\n");
    int i, aux, aux2;
    for (i=0; i<funTam; i++)
    {
        printf ("\t\t %d - %s\n", i+1, funcionario[i].nome);
    }
    fflush(stdin);
    scanf("%d", &aux);
    aux=aux-1;
    system("cls");
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t Cadastrar Advertencia:\n");
    printf ("========================================================================================================================\n");
    if (funcionario[aux].acesso <= funcionario[indice].acesso)
    {
        printf ("\t\tVocê não tem permissão para cadastrar advertencia para um usuário dessa hierarquia!\n");
        system("pause");
        return 0;
    }
    printf ("\t\t\t\t\t - Qual o nivel da infração? -\n");
    printf ("\t\t 1 - Leve\n\t\t 2 - Media\n\t\t 3 - Grave\n");
    scanf ("%d", &aux2);
    switch (aux2)
    {
    case 1:
        funcionario[aux].score = funcionario[aux].score - 10;
        break;
    case 2:
        funcionario[aux].score = funcionario[aux].score - 25;
        break;
    case 3:
        funcionario[aux].score = funcionario[aux].score - 35;
        break;
    default:
        printf ("\t\t\t\tOpção Inválida!");
        system ("pause");
        return 0;
    }
    fflush(stdin);
    char advertencia[350];
    printf ("\t\t\t - Digite uma breve descrição da infração -\n");
    gets(advertencia);
    FILE *file = fopen("advertencias.txt", "a");
    fprintf(file, "Funcionário: \n%s\nCPF: \n%s\n%s\n\n", funcionario[aux].nome, funcionario[aux].cpf, advertencia);
    if (funcionario[aux].score <=0)
    {
        printf("\t\tRECOMENDAÇÃO DE DEMISSÃO: SCORE BAIXO DETECTADO");
    }
    fclose(file);
    UpdateCadastro();
    system ("pause");
    return 0;

}

void Relatorios(int indice)
{
    int op;
    do
    {
        system("cls");
        printf ("========================================================================================================================\n");
        printf ("\t\t\t\t\t\t1 - Financeiro\n\t\t\t\t\t\t2 - Clientes\n\t\t\t\t\t\t3 - Funcionarios\n\t\t\t\t\t\t4 - Voltar\n");
        printf ("========================================================================================================================\n");
        fflush(stdin);
        scanf ("%d", &op);
        //switch-case que utiliza a variavel "op" para enviar o usuario para as outras funções do programa.
        switch(op)
        {
        case 1:
            if (funcionario[indice].acesso==1 || funcionario[indice].acesso==2)
            {
                RelFinanceiro(indice);
            }
            else
            {
                //usuarios de acesso 3 não tem acesso as informações financeiras
                printf("\n\t\t\t\t\tVocê não tem permissão para acessar essa página!\n");
                system("pause");
            }
            break;
        case 2:
            RelClientes();
            break;
        case 3:
            if (funcionario[indice].acesso==1 || funcionario[indice].acesso==2)
            {
                RelFuncionarios(indice);
            }
            else
            {
                //usuarios de acesso 3 não tem acesso as informações de outros funcionários
                printf("\n\t\t\t\t\tVocê não tem permissão para acessar essa página!\n");
                system("pause");
            }
            break;
        case 4:
            //caso "voltar" retorna ao menu certo de acordo com seu nivel de acesso
            if (funcionario[indice].acesso==1)
            {
                menuAdm(indice);
            }
            if (funcionario[indice].acesso==2)
            {
                menuGen(indice);
            }
            if (funcionario[indice].acesso==3)
            {
                menuFun(indice);
            }
            break;
        default:
            printf("\t\t\t\t\t\tPor favor, digite uma opção válida!");
            system("pause");
        }
    }
    while(op!=4); //fim e teste do loop de menu.
}

// pelo indice chamamos o usuario e entramos na tela de trocar a senha //
void TrocarSenha(int indice)
{
    fflush(stdin);
    char novasenha[25];
    system ("cls");
    system("cls");
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t\t Redefinição de Senha\n");
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t\t Digite uma nova senha: ");
    scanf ("%s", novasenha);
    // Esta chamando a senha pelo indice e redefinindo a senha do funcionario //
    strcpy(funcionario[indice].password, novasenha);
    UpdateCadastro();
    return 0;
}


//função de perfil do usuário
void Perfil(int indice)
{
    system("cls");
    fflush(stdin);
    char mensagem[256];
    FILE *file = fopen("mensagem.txt", "r");
    printf ("========================================================================================================================\n");
    printf ("\t\t\t\t\t\t\t PERFIL\n");
    printf ("========================================================================================================================\n");
    printf ("\t\t\t Nome do Funcionário: %s\n", funcionario[indice].nome);
    printf ("\t\t\t Cargo: %s\n", funcionario[indice].cargo);
    printf ("\t\t\t Nivel: %s\n", funcionario[indice].nivel);
    printf ("\t\t\t Salario por hora: %.2f\n", funcionario[indice].salarioph);
    printf ("\t\t\t Horas Registradas: %.0f\n", funcionario[indice].horaspm);
    printf ("\t\t\t Salario Esperado: %.2f\n", funcionario[indice].salarioph*funcionario[indice].horaspm);
    printf ("\t\t\t CPF: %s\n", funcionario[indice].cpf);
    printf ("\t\t\t Score: %d\n", funcionario[indice].score);
    printf ("\t\t\t Ultima Mensagem: \n");
    while ((fgets(mensagem, 255, file))!=NULL)
    {
        printf("\t\t\t\t\t\t%s", mensagem);
    }

    system("pause");
    return 0;


}

//função que preenche a base de dados com os valores da .txt
int databaseFill()
{
    char aux[100];
    char *decriptografia( char * p_texte);
    FILE *file = fopen("cadastro_funcionarios.txt", "r");//abertura do arquivo de cadastro de funcionários em modo leitura
    //loop que lê o arquivo até o final
    do
    {
        fgets (funcionario[funTam].nome, 100, file);
        funcionario[funTam].nome[strcspn(funcionario[funTam].nome, "\n")] = 0;//função para apagar "/n" que tem no arquivo .txt
        fgets (passwordcryph, 100, file);
        passwordcryph[strcspn(passwordcryph, "\n")] = 0;
        char * resultado = decriptografia(passwordcryph);
        strcpy(funcionario[funTam].password, resultado);
        fscanf (file, "%d", &funcionario[funTam].acesso);
        fscanf (file, "%f", &funcionario[funTam].salarioph);
        fscanf (file, "%f", &funcionario[funTam].horaspm);
        fgets(aux, 100, file);
        fgets (funcionario[funTam].cargo, 100, file);
        funcionario[funTam].cargo[strcspn(funcionario[funTam].cargo, "\n")] = 0;
        fgets (funcionario[funTam].nivel, 100, file);
        funcionario[funTam].nivel[strcspn(funcionario[funTam].nivel, "\n")] = 0;
        fgets (funcionario[funTam].cpf, 100, file);
        funcionario[funTam].cpf[strcspn(funcionario[funTam].cpf, "\n")] = 0;
        fscanf (file, "%d", &funcionario[funTam].score);
        fscanf (file, "%d", &funcionario[funTam].lido);
        fgets(aux, 100, file);
        fgets(aux, 100, file);
        funTam++;
    }
    while(!feof(file));
    fclose(file);

    clientes *p; //criação do ponteiro para cadastrar clientes na lista
    file = fopen("cadastro_clientes.txt", "r");//abertura do arquivo de cadastro de clientes na forma de leitura
    while(!feof(file)) //loop que lê o arquivo até o final
    {
        p =  (clientes *) malloc(sizeof(clientes)); //ponteiro recebe um espaço de memória do tamanho certo para uma variavel do tipo Funcionario
        //teste de alocação
        if (p==NULL)  //SE RECEBER NULL A ALOCAÇÃO NÃO PODE SER REALIZADA
        {
            printf("ERRO DE PREENCHIMENTO DO BANCO DE DADOS\n\n");
            return 0;  //não foi possível acrescentar conteúdo
        }
        fgets(p->nomeempresa, 100, file);
        p->nomeempresa[strcspn(p->nomeempresa, "\n")] = 0;
        fgets(p->cnpj, 100, file);
        p->cnpj[strcspn(p->cnpj, "\n")] = 0;
        fgets(p->endereco, 100, file);
        p->endereco[strcspn(p->endereco, "\n")] = 0;
        fgets(p->telefone, 100, file);
        p->telefone[strcspn(p->telefone, "\n")] = 0;
        fgets(p->email, 100, file);
        p->email[strcspn(p->email, "\n")] = 0;
        fgets(p->nomedocliente, 100, file);
        p->nomedocliente[strcspn(p->nomedocliente, "\n")] = 0;
        fgets(p->cpf, 100, file);
        p->cpf[strcspn(p->cpf, "\n")] = 0;
        fgets(p->telefonepessoal, 100, file);
        p->telefonepessoal[strcspn(p->telefonepessoal, "\n")] = 0;
        fgets(aux, 100, file);
        p->prox = NULL;//criação do proximo espaço da lista
        if (inicioCli==NULL)
        {
            inicioCli = p;
        }
        else
        {
            p->prox = inicioCli;
            inicioCli = p;
        }
    }
    fclose(file);
}

//função que cria as mensagens para o sistema de mensagem
void mensagens()
{
    fflush(stdin);
    system("cls");
    char motivo[250];//vetor para armazenar o texto
    char texto[250];//vetor para armazenar o texto
    char teste[250];
    int op;
    printf ("========================================================================================================================\n");
    printf("\t\t\t\t\t\tRazão da Reunião (max 250 caracteres)\n");
    printf ("========================================================================================================================\n");
    gets (motivo);// escreve o relatorio
    fflush(stdin);
    printf ("========================================================================================================================\n");
    printf("\t\t\t\t\t\tInforme a DATA e HORA da Reunião (max 250 caracteres)\n");
    printf ("========================================================================================================================\n");
    gets (texto);// escreve o relatorio
    FILE *file;// define um ponteiro
    file = fopen("mensagem.txt","r");
    fgets(teste, 255, file);
    fclose(file);
    if (teste!=NULL)
    {
        printf("JÁ EXISTE UMA MENSAGEM, DESEJA SUBSTITUÍ-LA?\n\nMensagem: %s\n\n1 - Sim || 2 - Não\n", teste);
        fflush(stdin);
        scanf("%d", &op);
        if (op==1)
        {
            file = fopen("mensagem.txt","w");// criando o arquivo

            fprintf (file, "%s\n%s\n", motivo, texto);// joga a variavel no ponteiro
            fclose(file);// salva e fecha
            system("cls");
            printf ("========================================================================================================================\n");
            printf("\t\t\t\t\t\tMensagem salva com sucesso!\n");
            printf ("========================================================================================================================\n");
            int i=0;
            while(i!=funTam)
            {
                funcionario[i].lido = 1;
                i++;

            }
            UpdateCadastro();
            system("pause");
        }
        else
        {
            printf("MENSAGEM NÃO FOI SUBSTITUIDA!");
            system("pause");
            return;
        }
    }


}

//função para atualizar o cadastro dentro do file .txt
void UpdateCadastro()
{
    char *criptografar( char * p_texte);
    remove("cadastro_funcionarios.txt");
    FILE *file = fopen("cadastro_funcionarios.txt", "a");
    strcpy(passwordcryph, funcionario[0].password);
    char * resultado = criptografar(passwordcryph);
    fprintf(file, "%s\n%s\n%d\n%f\n%f\n%s\n%s\n%s\n%d\n%d\n--------", funcionario[0].nome, resultado, funcionario[0].acesso, funcionario[0].salarioph, funcionario[0].horaspm, funcionario[0].cargo, funcionario[0].nivel, funcionario[0].cpf, funcionario[0].score, funcionario[0].lido);
    int i = 1;
    while(i<funTam)
    {
        strcpy(passwordcryph, funcionario[i].password);
        char * resultado = criptografar(passwordcryph);
        fprintf(file, "\n%s\n%s\n%d\n%f\n%f\n%s\n%s\n%s\n%d\n%d\n--------", funcionario[i].nome, resultado, funcionario[i].acesso, funcionario[i].salarioph, funcionario[i].horaspm, funcionario[i].cargo, funcionario[i].nivel, funcionario[i].cpf, funcionario[i].score, funcionario[i].lido);
        i++;
    }
    fclose(file);
}

//função para criar a lista de clientes
void CriaCliLista()
{
    inicioCli = NULL;
}


//RELATÓRIOS
//RELATÓRIOS
//RELATÓRIOS
void RelFinanceiro(int indice)
{
    FILE *file;
    char titulo[30] = "financeiro";
    char txt[5] = ".txt";
    char filename[30];
    float gastototal;
    int i;
    fflush(stdin);
    system("cls");
    printf ("========================================================================================================================\n");
    printf("\t\t\t\t\t\tRelatório Financeiro\n");
    printf ("========================================================================================================================\n");
    printf("\t\t\tInsira da data (Apenas Numeros): ");
    gets(filename);
    strcat(titulo, filename);
    strcat(titulo, txt);
    file = fopen(titulo, "w");
    system("cls");
    printf ("========================================================================================================================\n");
    printf("\t\t\t\t\t\tRelatório Financeiro\n");
    printf ("========================================================================================================================\n");
    for (i=0; i<funTam; i++)
    {
        if (funcionario[indice].acesso<=funcionario[i].acesso)
        {
            printf("\t\tFuncionário: %s\n\t\tGanhos: %.2f\n\n", funcionario[i].nome, (funcionario[i].salarioph*funcionario[i].horaspm));
            fprintf(file, "Funcionário: %s\nGanhos: %.2f\n\n", funcionario[i].nome, (funcionario[i].salarioph*funcionario[i].horaspm));
            gastototal = gastototal + (funcionario[i].salarioph*funcionario[i].horaspm);
        }
        else
        {
            gastototal = gastototal + (funcionario[i].salarioph*funcionario[i].horaspm);
        }
    }
    printf("\t\tTotal de gastos: %.2f\n\t\t(Atenção: Total de gastos inclue usuários que talvez você não tenha acesso)", gastototal);
    fprintf(file, "Total de gastos: %.2f\n(Atenção: Total de gastos inclue usuários que talvez você não tenha acesso)", gastototal);
    fclose(file);
    system("pause");
    return;
}

void RelClientes()
{
    system("cls");
    FILE *file;
    char titulo[30] = "clientes";
    char txt[5] = ".txt";
    char filename[30];
    clientes *p;
    fflush(stdin);
    system("cls");
    printf ("========================================================================================================================\n");
    printf("\t\t\t\t\t\tRelatório de Clientes\n");
    printf ("========================================================================================================================\n");
    printf("\t\t\tInsira da data (Apenas Numeros): ");
    gets(filename);
    strcat(titulo, filename);
    strcat(titulo, txt);
    file = fopen(titulo, "w");
    system("cls");
    printf ("========================================================================================================================\n");
    printf("\t\t\t\t\t\tRelatório de Clientes\n");
    printf ("========================================================================================================================\n");
    if (inicioCli == NULL)
    {
        printf("Sem Clientes Cadastrados!\n");
        system("pause");
        return;
    }
    p = inicioCli;
    while(p!=NULL)
    {
        if (p->nomeempresa != NULL)
        {
            printf ("\t\t\t Empresa: %s\n", p->nomeempresa);
            printf ("\t\t\t CNPJ: %s\n", p->cnpj);
            printf ("\t\t\t Endereço: %s\n", p->endereco);
            printf ("\t\t\t Telefone Publico: %s\n", p->telefone);
            printf ("\t\t\t E-mail: %s\n", p->email);
            printf ("\t\t\t Contato: %s\n", p->nomedocliente);
            printf ("\t\t\t CPF: %s\n", p->cpf);
            printf ("\t\t\t Telefone: %s\n\n", p->telefonepessoal);
            fprintf(file, "Empresa: %s\nCNPJ %s\nEndereço: %s\nTelefone Publico: %s\nE-mail: %s\nContato: %s\nCPF: %s\nTelefone: %s\n\n", p->nomeempresa, p->cnpj, p->endereco, p->telefone, p->email, p->nomedocliente, p->cpf, p->telefonepessoal);
        }
        p=p->prox;
    }
    printf("\t\t\tFIM DOS CLIENTES\n");
    fprintf(file, "FIM DOS CLIENTES");
    fclose(file);
    system("pause");
    return;
}

void RelFuncionarios(int indice)
{
    FILE *file;
    char titulo[30] = "funcionarios";
    char txt[5] = ".txt";
    char filename[30];
    int i;
    fflush(stdin);
    system("cls");
    printf ("========================================================================================================================\n");
    printf("\t\t\t\t\t\tRelatório de Funcionarios\n");
    printf ("========================================================================================================================\n");
    printf("\t\t\tInsira da data (Apenas Numeros): ");
    gets(filename);
    strcat(titulo, filename);
    strcat(titulo, txt);
    file = fopen(titulo, "w");
    system("cls");
    printf ("========================================================================================================================\n");
    printf("\t\t\t\t\t\tRelatório de Funcionarios\n");
    printf ("========================================================================================================================\n");
    for (i=0; i<funTam; i++)
    {
        if (funcionario[indice].acesso<=funcionario[i].acesso)
        {
            printf ("\t\t\tNome do Funcionário: %s \n\t\t\tCargo: %s\n\t\t\tNivel: %s \n\t\t\tSalario por hora: %.2f \n\t\t\tHoras Registradas: %.0f \n\t\t\tSalario Esperado: %.2f \n\t\t\tCPF: %s \n\t\t\tScore: %d \n\n", funcionario[i].nome, funcionario[i].cargo, funcionario[i].nivel, funcionario[i].salarioph, funcionario[i].horaspm, (funcionario[i].salarioph*funcionario[i].horaspm), funcionario[i].cpf, funcionario[i].score);
            fprintf(file, "Nome do Funcionário: %s\nCargo: %s\nNivel: %s\nSalario por hora: %.2f\nHoras Registradas: %.0f\nSalario Esperado: %.2f\nCPF: %s\nScore: %d\n\n", funcionario[i].nome, funcionario[i].cargo, funcionario[i].nivel, funcionario[i].salarioph, funcionario[i].horaspm, (funcionario[i].salarioph*funcionario[i].horaspm), funcionario[i].cpf, funcionario[i].score);
        }
    }
    printf("\t\t\tFIM DOS USUÁRIOS\n");
    fprintf(file, "FIM DOS USUÁRIOS");
    fclose(file);
    system("pause");
    return;
}

char * criptografar(char * p_texte)
{
    char * temp = p_texte;// pega o ponteiro do texto que foi passado  na variavel
    char * resultado =(char*)malloc(sizeof(char) *strlen(p_texte));
    if (!resultado)// testa para ver  se ponteito funcionou
    {
        printf("erro de memoria\n");// se não manda  rsposta
        return NULL;
    }
    memset (resultado, '\0', sizeof(char)* strlen(p_texte));// zera memoria
    for (; *temp; temp++)// faz com que a memoria  ande
    {
        char * str_pos = strchr(CONTEUDO, *temp ); // procurando a posição que ele esta  de acordo com caracter temp//
        int pos= str_pos - CONTEUDO; // aqui  da posição numerica dele //
        char temp[2]= { CONVERTE[pos],'\0'}; // aqui retorna o resultado da  converção//
        strncat(resultado, temp, 1);
    }
    return resultado;
}

char *decriptografia(char * p_texte) // função para decriptografa
{
    char * temp = p_texte; // pega o ponteiro do texto que foi passado  na variavel
    char * resultado =(char*)malloc(sizeof(char) *strlen(p_texte));// gera um resultado  no momento
    if (!resultado)// testa para ver  se ponteito funcionou
    {

        printf("erro de memoria\n");// se não manda  rsposta
        return NULL;
    }
    memset (resultado, '\0', sizeof(char)* strlen(p_texte)); // zera memoria
    for (; *temp; temp++)// faz com que a memoria  ande
    {
        char * str_pos = strchr(CONVERTE, *temp ); // procurando a posição que ele esta  de acordo com caracter temp//
        int pos= str_pos - CONVERTE; // aqui  da posição numerica dele //
        char temp[2]= { CONTEUDO[pos],'\0'}; // aqui retorna o resultado da  converção//
        strncat(resultado, temp, 1);
    }
    return resultado;
}
