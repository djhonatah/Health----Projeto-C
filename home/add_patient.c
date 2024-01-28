#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "login.h"
#include "home.h"
#include "patient.h"
#include "calculator.c"
// Função para exibir a página de Antropometria
void showCalculatorPage(struct Pacient* pacient);
int showPacientPage(struct Pacient* pacient);
int dadosFisicos(struct Pacient* pacient);
int showCalculadora(struct Pacient* pacient);




void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void readInputLine(char* buffer, size_t bufferSize) {

    fgets(buffer, bufferSize, stdin);
    // Remover o caractere de nova linha ('\n') do buffer
    size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }
}


void addAlteration(struct Alteration** history, float value) {
    struct Alteration* newAlteration = (struct Alteration*)malloc(sizeof(struct Alteration));
    newAlteration->value = value;
    newAlteration->date = time(NULL);
    newAlteration->next = *history;
    *history = newAlteration;
    
    FILE* file = fopen("alteration_history.txt", "a");
    if (file != NULL) {
        fprintf(file, "Value: %.2f, Date: %s", value, ctime(&newAlteration->date));
        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo de histórico.\n");
    }
}


// Função para exibir o histórico de alterações
void showAlterationHistory(struct Alteration* history) {
    int input;
    struct Alteration* current = history;

    while (1) {
        system("clear");
        printf("Histórico de alterações:\n");

        while (current != NULL) {
            printf(" - Valor: %.2f, Data: %s", current->value, asctime(localtime(&(current->date))));
            current = current->next;
        }

        printf("\nDigite '0' para voltar à página anterior\n");
        printf("Digite '1' para exibir o gráfico\n");
        printf("Digite sua escolha: ");
        scanf("%d", &input);
        clearInputBuffer();

        if (input == 0) {
            break;
        } else if (input == 1) {
            // Chamada da função para exibir o gráfico
            printf("\nExibindo o gráfico...\n");
            // ... lógica para exibir o gráfico ...
        } else {
            printf("\nOpção inválida. Tente novamente.\n");
        }
    }
}



void showCadastroPaciente(const struct Pacient* pacient) {
    system("clear");
    printf("\n");
    printf("##### Informações de Cadastro do Paciente #####\n");
    printf("\n");
    printf("Nome: %s\n", pacient->name);
    printf("Email: %s\n", pacient->email);
    printf("Número de telefone: %s\n", pacient->number);
    printf("CPF: %s\n", pacient->cpf);
    printf("Data de nascimento: %s\n", pacient->birth);

    int selectedOption;
    printf("\n");
    printf("\nDigite 0 para voltar à página anterior ou 1 para encerrar o programa: ");
    scanf("%d", &selectedOption);
    clearInputBuffer();

    switch (selectedOption) {
        case 0:
            // Voltar à página anterior
            system("clear");
            return;
        case 1:
            // Encerrar o programa
            exit(0);
        default:
            printf("Opção inválida.\n");
            break;
    }
}


int showPacientPage(struct Pacient* pacient) {
    system("clear");
    printf("\n");
    printf("######## Página do Paciente ########\n");
    printf("\n");
    printf("Paciente:  %s\n", pacient->name);
    printf("\n");

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

    printf("Peso (kg): %.2f\n", pacient->antropometria.peso);
    printf("Altura (0.00m): %.2f\n", pacient->antropometria.altura);
    printf("Nível de Atividade: %s\n", pacient->antropometria.activityLevel);
    printf("\n");
    printf("\n");

    printf("1. Alterar Dados Fisicos\n");
    printf("2. Calculadoras\n");
    printf("3. Dieta\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("3. Informações de Cadastro\n");
    printf("0. Voltar\n");

    int selectedOption;
    
    printf("\nOpcao: ");
    scanf("%d", &selectedOption);
    clearInputBuffer();

    switch (selectedOption) {
        case 0:
            printf("Voltando...\n");
            return 0;
        case 1:
            dadosFisicos(pacient);
            break;
        case 2:
            showCalculadora(pacient);
            break;
        case 3:
            exibirDieta(pacient);
            break;
        default:
            printf("Opção inválida.\n");
            break;
    }
}

void showPacientList(const char* username) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s.txt", username);

    FILE* userFile = fopen(filename, "r");
    if (userFile == NULL) {
        printf("Erro ao abrir o arquivo do usuário.\n");
        return;
    }

    struct Pacient pacient;

    system("clear");
    printf("\n");
    printf("##### Lista de Pacientes #####\n");
    printf("\n");

    int number = 1;
    while (fread(&pacient, sizeof(struct Pacient), 1, userFile)) {
        printf("%d. %s\n", number, pacient.name);
        number++;
    }

    fclose(userFile);

    int selectedNumber;
    printf("\nEscolha o paciente ou (0 para voltar): ");
    scanf("%d", &selectedNumber);
    clearInputBuffer();

    if (selectedNumber == 0) {
        printf("Voltando...\n");
        return;
    } else if (selectedNumber >= 1 && selectedNumber < number) {
        userFile = fopen(filename, "r");
        if (userFile == NULL) {
            printf("Erro ao abrir o arquivo do usuário.\n");
            return;
        }

        int count = 1;
        while (fread(&pacient, sizeof(struct Pacient), 1, userFile)) {
            if (count == selectedNumber) {
                showPacientPage(&pacient);
                break;
            }
            count++;
        }

        fclose(userFile);
    } else {
        printf("Opção inválida.\n");
    }
}


void removePacient(const char* username) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s.txt", username);

    FILE* userFile = fopen(filename, "r");
    if (userFile == NULL) {
        printf("Erro ao abrir o arquivo do usuario.\n");
        return;
    }

    struct Pacient pacient;
    struct Pacient tempPacient;
    int found = 0;

    system("clear");
    printf("\n");
    printf("##### Lista de Pacientes #####\n");
    printf("\n");

    while (fread(&pacient, sizeof(struct Pacient), 1, userFile)) {
        printf("%s\n", pacient.name);
    }

    fclose(userFile);

    printf("\nDigite o nome do paciente a ser removido (ou 0 para voltar): ");
    char input[100];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "0") == 0) {
        printf("Voltando...\n");
        return;
    }

    userFile = fopen(filename, "r");
    if (userFile == NULL) {
        printf("Erro ao abrir o arquivo do usuario.\n");
        return;
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(userFile);
        return;
    }

    while (fread(&pacient, sizeof(struct Pacient), 1, userFile)) {
        if (strcmp(pacient.name, input) == 0) {
            found = 1;
            continue;
        }
        fwrite(&pacient, sizeof(struct Pacient), 1, tempFile);
    }

    fclose(userFile);
    fclose(tempFile);

    if (found == 0) {
        printf("Paciente nao encontrado.\n");
        remove("temp.txt");
        return;
    }

    if (remove(filename) != 0) {
        printf("Erro ao remover o paciente.\n");
        return;
    }

    if (rename("temp.txt", filename) != 0) {
        printf("Erro ao renomear arquivo temporario.\n");
        return;
    }

    printf("\nPaciente removido com sucesso!\n");
    printf("\n");
}

void addPacient(const char* userName) {
    struct Pacient pacient;

    system("clear");

    printf("-----Preencha os dados do paciente-----\n");
    printf("\n");

    printf("");
    readInputLine(pacient.name, sizeof(pacient.name));

    printf("Nome Completo: ");
    readInputLine(pacient.name, sizeof(pacient.name));

    printf("Genero (m/f): ");
    readInputLine(pacient.gender, sizeof(pacient.gender));

    printf("CPF: ");
    readInputLine(pacient.cpf, sizeof(pacient.cpf));

    printf("Email: ");
    readInputLine(pacient.email, sizeof(pacient.email));

    printf("Celular: ");
    readInputLine(pacient.number, sizeof(pacient.number));

    printf("Data de nascimento (dd/mm/aa): ");
    readInputLine(pacient.birth, sizeof(pacient.birth));

    char filename[100];
    snprintf(filename, sizeof(filename), "%s.txt", userName);

    FILE* userFile = fopen(filename, "a");
    if (userFile == NULL) {
        printf("Erro ao abrir o arquivo do usuário.\n");
        return;
    }

    fwrite(&pacient, sizeof(struct Pacient), 1, userFile);
    fclose(userFile);

    printf("\n");
    printf("Paciente cadastrado com sucesso!\n");
    printf("\n");
}

