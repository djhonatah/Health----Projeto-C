#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "patient.h"
#include "add_patient.h"

int calculateAge(char* birthDate) {
    // Calcula a idade do paciente a partir da data de nascimento

    time_t now;
    struct tm* currentDate;
    int currentDay, currentMonth, currentYear;
    int birthDay, birthMonth, birthYear;
    int age;

    // Obtém a data atual
    time(&now);
    currentDate = localtime(&now);
    currentDay = currentDate->tm_mday;
    currentMonth = currentDate->tm_mon + 1; // tm_mon é base 0
    currentYear = currentDate->tm_year + 1900; // tm_year é baseado em anos desde 1900

    // Extrai dia, mês e ano da data de nascimento
    sscanf(birthDate, "%d/%d/%d", &birthDay, &birthMonth, &birthYear);

    // Calcula a idade
    age = currentYear - birthYear;

    // Verifica se o aniversário já ocorreu este ano
    if (currentMonth < birthMonth || (currentMonth == birthMonth && currentDay < birthDay)) {
        age--;
    }

    return age;
}

float calculateTMB(struct Pacient* pacient) {
    // Calcula a Taxa Metabólica Basal (TMB) do paciente considerando o fator de atividade

    int age = calculateAge(pacient->birth);
    float weight = pacient->antropometria.peso;
    float height = pacient->antropometria.altura;
    float bmr;
    float activityFactor;

    if (strcmp(pacient->gender, "masculino") == 0) {
        bmr = 66.5 + (13.75 * weight) + (5 * height) - (6.75 * age);
    } else {
        bmr = 655.1 + (9.56 * weight) + (1.85 * height) - (4.68 * age);
    }

    // Solicitação e verificação do nível de atividade
    int validActivityLevel = 0;
    while (!validActivityLevel) {


        if (strcmp(pacient->antropometria.activityLevel, "s") == 0) {
            activityFactor = 1.2;
            validActivityLevel = 1;
        } else if (strcmp(pacient->antropometria.activityLevel, "l") == 0) {
            activityFactor = 1.375;
            validActivityLevel = 1;
        } else if (strcmp(pacient->antropometria.activityLevel, "m") == 0) {
            activityFactor = 1.55;
            validActivityLevel = 1;
        } else if (strcmp(pacient->antropometria.activityLevel, "a") == 0) {
            activityFactor = 1.725;
            validActivityLevel = 1;
        } else if (strcmp(pacient->antropometria.activityLevel, "ma") == 0) {
            activityFactor = 1.9;
            validActivityLevel = 1;
        } else {
            printf("Opção inválida. Digite novamente.\n");
        }
    }

    // Aplica o fator de atividade ao resultado da TMB
    float bmrWithActivity = bmr * activityFactor;

    return bmrWithActivity;
}


float calculateIMC(float peso, float altura) {
    // Verifica se a altura é válida (diferente de zero)
    if (altura == 0) {
        printf("Altura inválida!\n");
        return -1; // Retorna um valor inválido para indicar erro
    }

    // Calcula o IMC
    float imc = peso / (altura * altura);
    return imc;
}

int showCalculadora(struct Pacient* pacient) {
    system("clear");
    printf("\n");
    printf("##### Calculadora Metabólica #####\n");
    printf("\n");
     printf("\n");

    float bmr = calculateTMB(pacient);
            printf("\n");
            printf("Taxa Metabólica Basal (TMB): %.2f calorias\n", bmr);
    
    float imc = calculateIMC(pacient->antropometria.peso, pacient->antropometria.altura);
            printf("\n");
            printf("IMC (Índice de Massa Corporal): %.2f\n", imc);
    
    printf("\n");
    printf("\n");
    
    printf("Pressione Enter para Voltar...");
    clearInputBuffer();    
    // Chama a função novamente para permanecer na calculadora
    return showPacientPage(pacient);
}

int dadosFisicos(struct Pacient* pacient) {
    system("clear");

    // Construir o nome do arquivo de dados físicos do paciente
    char filename[100];
    snprintf(filename, sizeof(filename), "%s_dados.txt", pacient->name);

    // Tentar abrir o arquivo em modo de leitura
    FILE *arquivo = fopen(filename, "r");
    if (arquivo != NULL) {
        // Ler os dados do arquivo
        fscanf(arquivo, "Peso: %f\n", &(pacient->antropometria.peso));
        fscanf(arquivo, "Altura: %f\n", &(pacient->antropometria.altura));
        fscanf(arquivo, "Nível de Atividade: %s\n", pacient->antropometria.activityLevel);

        // Fechar o arquivo
        fclose(arquivo);
    }

    printf("###### DADOS FISICOS ######\n");

    // Exibir os valores lidos do arquivo
    printf("Peso (kg): %.2f\n", pacient->antropometria.peso);
    printf("Altura (0.00m): %.2f\n", pacient->antropometria.altura);
    printf("Nível de Atividade: %s\n", pacient->antropometria.activityLevel);

    printf("Novo Peso (kg): ");
    scanf("%f", &(pacient->antropometria.peso));
    printf("Nova Altura (0.00m): ");
    scanf("%f", &(pacient->antropometria.altura));
    printf("Novo Nível de Atividade: (s - Sedentário)  (l - Leve)  (m - Moderado)  (a - Ativo)  (ma - Muito Ativo):  ");
    scanf("%s", pacient->antropometria.activityLevel);
    printf("Pressione Enter para continuar...");
    clearInputBuffer();
    system("clear");
    printf("Dados Alterados Com Sucesso...");

    // Abrir o arquivo em modo de gravação
    arquivo = fopen(filename, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return -1; // Indica erro
    }

    // Gravar os dados no arquivo
    fprintf(arquivo, "Peso: %.2f\n", pacient->antropometria.peso);
    fprintf(arquivo, "Altura: %.2f\n", pacient->antropometria.altura);
    fprintf(arquivo, "Nível de Atividade: %s\n", pacient->antropometria.activityLevel);

    // Fechar o arquivo
    fclose(arquivo);

    return showPacientPage(pacient);
}

void adicionarDieta(struct Pacient* pacient) {
    while (1) {
        system("clear");

        printf("###### ADICIONAR DIETA ######\n");
        printf("\n");
        printf("Paciente: %s\n", pacient->name);
        printf("\n");

        // Construir o nome do arquivo de dieta do paciente
        char filename[100];
        snprintf(filename, sizeof(filename), "%s_dieta.txt", pacient->name);

        // Tentar abrir o arquivo em modo de leitura
        FILE* arquivo = fopen(filename, "r");
        if (arquivo != NULL) {
            // Ler os dados do arquivo e preencher a matriz de dieta do paciente
            fread(pacient->dieta, sizeof(pacient->dieta), 1, arquivo);

            // Fechar o arquivo
            fclose(arquivo);
        }

        // Matriz para armazenar a nova dieta
        char novaDieta[100];

        int dia; // Variável para armazenar o número do dia da semana escolhido
        printf("Digite o número do dia da semana (1-7): ");
        scanf("%d", &dia);
        clearInputBuffer();

        if (dia < 1 || dia > 7) {
            printf("Dia da semana inválido.\n");
            continue; // Reinicia o loop para solicitar novamente a dieta
        }

        dia--; // Ajustar para índice da matriz

        int periodo; // Variável para armazenar o número do período do dia escolhido
        printf("Digite o número do período do dia (1-3):\n");
        printf("1. Manhã\n");
        printf("2. Tarde\n");
        printf("3. Noite\n");
        printf("Opção: ");
        scanf("%d", &periodo);
        clearInputBuffer();

        if (periodo < 1 || periodo > 3) {
            printf("Período do dia inválido.\n");
            continue; // Reinicia o loop para solicitar novamente a dieta
        }

        periodo--; // Ajustar para índice da matriz

        printf("Digite a nova dieta para o dia %d, período %d: ", dia + 1, periodo + 1);
        fgets(novaDieta, sizeof(novaDieta), stdin);
        novaDieta[strcspn(novaDieta, "\n")] = '\0';

        // Atribuir a nova dieta ao paciente
        strncpy(pacient->dieta[dia][periodo], novaDieta, sizeof(pacient->dieta[dia][periodo]));

        // Abrir o arquivo em modo de gravação
        arquivo = fopen(filename, "w");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo!\n");
            return;
        }

        // Gravar os dados da dieta no arquivo
        fwrite(pacient->dieta, sizeof(pacient->dieta), 1, arquivo);

        // Fechar o arquivo
        fclose(arquivo);

        printf("Dieta adicionada com sucesso!\n");

        printf("\nPressione Enter para adicionar outra dieta ou digite 0 para voltar: ");
        int option;
        scanf("%d", &option);
        clearInputBuffer();

        if (option == 0) {
            printf("Voltando...\n");
            break; // Sai do loop e retorna para a função anterior
        }
    }

    printf("\nPressione Enter para continuar...");
    clearInputBuffer();
    getchar();
    system("clear");
}

void exibirDieta(struct Pacient* pacient) {
    system("clear");

    printf("###### EXIBIR DIETA ######\n");
    printf("\n");
    printf("Paciente: %s\n", pacient->name);
    printf("\n");

    // Construir o nome do arquivo de dieta do paciente
    char filename[100];
    snprintf(filename, sizeof(filename), "%s_dieta.txt", pacient->name);

    // Tentar abrir o arquivo em modo de leitura
    FILE* arquivo = fopen(filename, "r");
    if (arquivo != NULL) {
        // Ler os dados do arquivo e preencher a matriz de dieta do paciente
        fread(pacient->dieta, sizeof(pacient->dieta), 1, arquivo);

        // Fechar o arquivo
        fclose(arquivo);
    }

    // Exibir a dieta por dia da semana e período do dia
    char* diasSemana[7] = {"Domingo", "Segunda-feira", "Terça-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sábado"};
    char* periodosDia[3] = {"Manhã", "Tarde", "Noite"};

    int dia, periodo;

    for (dia = 0; dia < 7; dia++) {
        printf("%s:\n", diasSemana[dia]);

        for (periodo = 0; periodo < 3; periodo++) {
            printf("- %s: %s\n", periodosDia[periodo], pacient->dieta[dia][periodo]);
        }

        printf("\n");
    }

    printf("Pressione 1 para adicionar dieta, 0 para voltar: ");
    int opcao;
    scanf("%d", &opcao);
    clearInputBuffer();

    switch (opcao) {
        case 1:
            adicionarDieta(pacient);
            break;
        case 0:
            return;
        default:
            printf("Opção inválida.\n");
            break;
    }

    exibirDieta(pacient); // Chamar novamente a função para exibir a dieta atualizada
}
