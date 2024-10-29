#include "registro.h"
#include "cabecalho.h"


Registro *registro_readbin(FILE* entrada) {
      Registro *registro = cria_registro();

    if (!fread(&registro->removido, sizeof(char), 1, entrada)) {
        registro->removido = 'E'; // Indica que não foi possível ler o registro

        return registro;
    }

    if (registro->removido == '1') {
        return registro;
    }
 //freads para os campos de tamanho fixo
    fread(&registro->encadeamento, sizeof(int), 1, entrada);
    fread(&registro->populacao, sizeof(int), 1, entrada);
    fread(&registro->tamanho, sizeof(float), 1, entrada);
    fread(&registro->uniMedida, sizeof(char), 1, entrada);
    fread(&registro->velocidade, sizeof(int), 1, entrada);
// char*linha = calloc (160, sizeof(char)); para alocar memoria
    char* linha = calloc(160, sizeof(char));
    fgets(linha, 161 - sizeof(int) * 4 - sizeof(char) * 2, entrada); // lê o restante da linha

    strcpy (registro->nome , strsep(&linha, "#")); // strtok para separar os campos e delimitador
    strcpy (registro->nEspecie , strsep(&linha, "#"));
    strcpy (registro->habitat , strsep(&linha, "#"));
    strcpy (registro->tipo , strsep(&linha, "#"));
    strcpy (registro->dieta , strsep(&linha, "#"));
    strcpy (registro->alimento , strsep(&linha, "#"));

    //free(linha);

    return registro;
}

Registro* cria_registro ()
{
    Registro *registro  = malloc(sizeof(Registro));
    registro -> removido = '0';
    registro -> encadeamento = -1;
    registro -> populacao = -1;
    registro -> tamanho = -1;
    registro -> uniMedida = '$';
    registro -> velocidade = -1;
    registro -> nome = calloc (160, sizeof(char));
    registro -> nEspecie = calloc (160, sizeof(char));
    registro -> habitat = calloc (160, sizeof(char));
    registro -> tipo = calloc (160, sizeof(char));
    registro -> dieta = calloc (160, sizeof(char));
    registro -> alimento = calloc (160, sizeof(char));

    return registro;

}

void registro_writebin(FILE *nomebin, Registro *registro) {
    // Função para escrever o registro no arquivo binário

    // Obter o tamanho das strings de tamanho variável
    int tam = strlen(registro->nome);
    int tamA = strlen(registro->alimento);
    int tamD = strlen(registro->dieta);
    int tamE = strlen(registro->nEspecie);
    int tamT = strlen(registro->tipo);
    int tamH = strlen(registro->habitat);

    // Escreve campos de tamanho fixo
    fwrite(&registro->removido, sizeof(char), 1, nomebin);
    fwrite(&registro->encadeamento, sizeof(int), 1, nomebin);
    fwrite(&registro->populacao, sizeof(int), 1, nomebin);
    fwrite(&registro->tamanho, sizeof(float), 1, nomebin);
    fwrite(&registro->uniMedida, sizeof(char), 1, nomebin);
    fwrite(&registro->velocidade, sizeof(int), 1, nomebin);

    // Escreve campos de tamanho variável com delimitador #
    if (registro->nome != NULL && strcmp(registro->nome, "#") != 0) {
        fwrite(registro->nome, sizeof(char), tam, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    if (registro->nEspecie != NULL && strcmp(registro->nEspecie, "#") != 0) {
        fwrite(registro->nEspecie, sizeof(char), tamE, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    if (registro->habitat != NULL && strcmp(registro->habitat, "#") != 0) {
        fwrite(registro->habitat, sizeof(char), tamH, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    if (registro->tipo != NULL && strcmp(registro->tipo, "#") != 0) {
        fwrite(registro->tipo, sizeof(char), tamT, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    if (registro->dieta != NULL && strcmp(registro->dieta, "#") != 0) {
        fwrite(registro->dieta, sizeof(char), tamD, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    if (registro->alimento != NULL && strcmp(registro->alimento, "#") != 0) {
        fwrite(registro->alimento, sizeof(char), tamA, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    // Verifica quanto falta para completar os 160 bytes
    int preenche = registro->tam_preenchimento;
    char aux[preenche];
    memset(aux, REGISTRO_FILL, preenche);  // Preenche com o caractere de preenchimento
    fwrite(aux, sizeof(char), preenche, nomebin);
}

Registro *registro_readcsv(FILE *csv) {
    Registro *registro = malloc(sizeof(Registro));
    char linha[300];
    fgets(linha, sizeof(linha), csv);

    // Remove os caracteres de nova linha (tanto \n quanto \r)
    linha[strcspn(linha, "\n")] = '\0';
    linha[strcspn(linha, "\r")] = '\0';

    char *linha_copy = linha;
    //alocando memoria
    registro->nome = calloc(160,sizeof(registro->nome));
    registro->nEspecie = calloc(160,sizeof(registro->nEspecie));
    registro->dieta = calloc(160,sizeof(registro->dieta));
    registro->habitat = calloc(160,sizeof(registro->habitat));
    registro->alimento = calloc(160,sizeof(registro->alimento));
    registro->tipo = calloc(160,sizeof(registro->tipo));

    // Nome
    strcpy(registro->nome,strsep(&linha_copy, ","));

    // Dieta
    strcpy(registro->dieta,strsep(&linha_copy, ","));

    // Habitat
    strcpy(registro->habitat,strsep(&linha_copy, ","));

    // População
    char *populacao = strsep(&linha_copy, ",");
    if (strlen(populacao) == 0) {
        registro->populacao = -1;
    }
    else{
        registro->populacao = atoi(populacao);
    }

    // Tipo
    strcpy(registro->tipo,strsep(&linha_copy, ","));


    // Velocidade
    char *velocidade = strsep(&linha_copy, ",");
    if (strlen(velocidade) == 0) {
        registro->velocidade = -1;
    }
    else{
        registro->velocidade = atoi(velocidade);
    }

    // Unidade de medida
    char *unidade = strsep(&linha_copy, ",");
    if (strlen(unidade) == 0) {
        registro->uniMedida = '$';
    }
    else{
        registro->uniMedida = unidade[0];
    }

    // Tamanho
    char *tamanho = strsep(&linha_copy, ",");
    if (strlen(tamanho) == 0) {
        registro->tamanho = -1;
    }
    else{
        registro->tamanho = atof(tamanho);
    }

    // Espécie
    strcpy(registro->nEspecie,strsep(&linha_copy, ","));

    // Alimento
    strcpy(registro->alimento,strsep(&linha_copy, ","));

    // Configurações adicionais para o registro
    registro->removido = REGISTRO_REMOVIDO_FALSE;
    registro->encadeamento = 0;

    // Calcular bytes restantes para completar os 160 bytes
    // calcular tam_preenchimento corretamente com base nos tamanhos das strings
    int tam = strlen(registro->nome);
    int tamE = strlen(registro->nEspecie);
    int tamA = strlen(registro->alimento);
    int tamH = strlen(registro->habitat);
    int tamD = strlen(registro->dieta);
    int tamT = strlen(registro->tipo);

    // 18 bytes para os campos fixos (população, tamanho, velocidade, uniMedida, removido e encadeamento) e 6 bytes para delimitadores '#'
    registro->tam_preenchimento = 160 - ((tam + tamD + tamH + tamT + tamE + tamA) * sizeof(char) + 18 +6);

    return registro;
}


void registro_print(Registro *registro){

    printf("Nome: %s\n", registro->nome);
    printf("Especie: %s\n", registro->nEspecie);

    if(strcmp(registro->tipo, "") != 0){
        printf("Tipo: %s\n", registro->tipo);
    }


    printf("Dieta: %s\n", registro->dieta);

    if(strcmp(registro->habitat, "") != 0){
        printf("Lugar que habitava: %s\n", registro->habitat);
    }

    if(registro->tamanho != -1){
        printf("Tamanho: %.1f m\n", registro->tamanho);
    }
    if (registro->velocidade != -1 && registro->uniMedida != '$'){
        printf("Velocidade: %d %cm/h\n", registro->velocidade, registro->uniMedida);
    }
    printf("\n");

}
void registro_busca_elemento(char *valor, int valorint, float valorf, Registro *registro) {
    if (valor != NULL && strcmp(registro->nome, valor) == 0) {
        registro_print(registro);
    } else if (valor != NULL && strcmp(registro->nEspecie, valor) == 0) {
        registro_print(registro);
    } else if (valor != NULL && strcmp(registro->alimento, valor) == 0) {
        registro_print(registro);
    } else if (valor != NULL && strcmp(registro->dieta, valor) == 0) {
        registro_print(registro);
    } else if (valor != NULL && strcmp(registro->tipo, valor) == 0) {
        registro_print(registro);
    } else if (valor != NULL && strcmp(registro->habitat, valor) == 0) {
        registro_print(registro);
    } else if (registro->populacao == valorint) {
        registro_print(registro);
    } else if (registro->velocidade == valorint) {
        registro_print(registro);
    } else if (registro->tamanho == valorf) {
        registro_print(registro);
    }
    else
        return;
}

int verificacaoString(char *campo) {
    // Verifica o tipo do campo e retorna a posição
    if (strcmp("nome", campo) == 0) {
        return 0; // Retorna 0 para 'nome'
    } else if (strcmp("especie", campo) == 0) {
        return 1; // Retorna 1 para 'especie'
    } else if (strcmp("habitat", campo) == 0) {
        return 2; // Retorna 2 para 'habitat'
    } else if (strcmp("tipo", campo) == 0) {
        return 3; // Retorna 3 para 'tipo'
    } else if (strcmp("dieta", campo) == 0) {
        return 4; // Retorna 4 para 'dieta'
    } else if (strcmp("alimento", campo) == 0) {
        return 5; // Retorna 5 para 'alimento'
    } else if (strcmp("velocidade", campo) == 0) {
        return 6;
    } else if (strcmp("populacao", campo) == 0) {
        return 7;
    } else if (strcmp("tamanho", campo) == 0) {
        return 8;
    } else {
        printf("Campo inexistente.\n");
        return -1; // Campo inexistente
    }
}


void registro_remove(Registro *registro){
    // remove logicamente um registro
}

bool registro_isValid(Registro *registro){
    // retorna true caso o registro não seja removido
    return registro->removido == REGISTRO_REMOVIDO_FALSE;
}




int registro_getPop(Registro *registro){
    // verifica o valor do campo topo
    return registro->populacao;
}
float registro_getTam(Registro *registro){
    // verifica o valor do campo proximo a ser removido
    return registro->tamanho;
}

char registro_getUnimedida(Registro *registro){
    // verifica a quantidade de paginas
    return registro->uniMedida;
}

int registro_getVelocidade(Registro *registro){
    // verifica quantas vezes foi compactado
    return registro->velocidade;
}

char* registro_getNome(Registro *registro){
    return registro->nome;
}

char* registro_getNEspecie(Registro *registro){
    return registro->nEspecie;
}

char* registro_getHabitat(Registro *registro){
    return registro->habitat;
}

char* registro_getTipo(Registro *registro){
    return registro->tipo;
}

char* registro_getDieta(Registro *registro){
    return registro->dieta;
}
char* registro_getAlimenta(Registro *registro){
    return registro->alimento;
}
char registro_getRemovido(Registro *registro){
    return registro->removido;
}

int registro_getEncadeamento(Registro *registro){
    return registro->encadeamento;
}








void registro_setPop(Registro *registro, int pop){
    // verifica o valor do campo topo
    registro->populacao = pop;
}
void registro_setTam(Registro *registro, float tam){
    // verifica o valor do campo proximo a ser removido
    registro->tamanho = tam;
}

void registro_setUnimedida(Registro *registro, char unidade){
    // verifica a quantidade de paginas
    registro->uniMedida = unidade;
}

void registro_setVelocidade(Registro *registro, int velocidade){
    // verifica quantas vezes foi compactado
    registro->velocidade = velocidade;
}

void registro_setNome(Registro *registro, char *nome){
    strcpy(registro->nome, nome);
}

void registro_setNEspecie(Registro *registro, char *especie){
    strcpy(registro->nEspecie, especie);
}

void registro_setHabitat(Registro *registro, char *habitat){
    strcpy(registro->habitat, habitat);
}

void registro_setTipo(Registro *registro,char *tipo){
    strcpy(registro->tipo, tipo);
}

void registro_setDieta(Registro *registro, char *dieta){
    strcpy(registro->dieta, dieta);
}
void registro_setAlimenta(Registro *registro, char *alimento){
    strcpy(registro->alimento, alimento);
}

void registro_setRemovido(Registro *registro, bool removido){
    registro->removido=removido;
}

void registro_setEncadeamento(Registro *registro, int encadeamento) {
    registro->encadeamento = encadeamento;
}

Registro leitura(){

    Registro registro;

    char *populacao, *tamanho, *velocidade, *medidaVelocidade;

    cria_registro();              // Inicializa um registro

    // Aloca espaço para as variáveis que precisam ser manipuladas
    populacao = calloc(10, sizeof(char));
    tamanho = calloc(10, sizeof(char));
    velocidade = calloc(10, sizeof(char));
    medidaVelocidade = calloc(10, sizeof(char));


    // Lê todos os campos do teclado
    scan_quote_string(registro.nome);
    scan_quote_string(registro.dieta);
    scan_quote_string(registro.habitat);
    scan_quote_string(populacao);
    scan_quote_string(registro.tipo);
    scan_quote_string(velocidade);
    scan_quote_string(medidaVelocidade);
    scan_quote_string(tamanho);
    scan_quote_string(registro.nEspecie);
    scan_quote_string(registro.alimento);

    if(strcmp(populacao, "")==0)                    // Caso o campo seja nulo, atualiza o valor da variável para -1
        registro.populacao = -1;
    else registro.populacao = atoi(populacao);      // Caso contrário, a variável recebe o valor lido

    if(strcmp(velocidade, "")==0)                   // Caso o campo seja nulo, atualiza o valor da variável para -1
        registro.velocidade = -1;
    else registro.velocidade = atoi(velocidade);    // Caso contrário, a variável recebe o valor lido

    if(strcmp(medidaVelocidade, "")==0)            // Caso o campo seja nulo, atualiza o valor da variável para '$'
        registro.uniMedida = '$';
    else registro.uniMedida = medidaVelocidade[0];  // Caso contrário, a variável recebe o valor lido

    if(strcmp(tamanho, "")==0)                      // Caso o campo seja nulo, atualiza o valor da variável para -1
        registro.tamanho = -1;
    else registro.tamanho = atof(tamanho);          // Caso contrário, a variável recebe o valor lido

    registro.removido = '0';                        // Certifica que registro.removido = '0'

    return registro;
}

int tamanhoString(char *string)
{
    int i=0;
    while(string[i]!='\0')
        i++;

    return i;
}
int escreve_registro(FILE *arquivonbin, Registro registro, int qtd_registro) {
    long int psc_atual;
    int psc_final;
    char delimitador = '#';
    int tamNome, tamEspecie, tamHabitat, tamTipo, tamDieta, tamAlimento;

    if (arquivonbin == NULL) {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        return -1;
    }

    // Calcula os tamanhos das strings
    tamNome = tamanhoString(registro.nome);
    tamEspecie = tamanhoString(registro.nEspecie);
    tamHabitat = tamanhoString(registro.habitat);
    tamTipo = tamanhoString(registro.tipo);
    tamDieta = tamanhoString(registro.dieta);
    tamAlimento = tamanhoString(registro.alimento);

    // Escreve os campos binários no arquivo
    fwrite(&registro.removido, sizeof(char), 1, arquivonbin);
    fwrite(&registro.encadeamento, sizeof(int), 1, arquivonbin);
    fwrite(&registro.populacao, sizeof(int), 1, arquivonbin);
    fwrite(&registro.tamanho, sizeof(float), 1, arquivonbin);
    fwrite(&registro.uniMedida, sizeof(char), 1, arquivonbin);
    fwrite(&registro.velocidade, sizeof(int), 1, arquivonbin);

    // Escreve as strings com os tamanhos calculados e delimitadores
    fwrite(registro.nome, sizeof(char), tamNome, arquivonbin);
    fwrite(&delimitador, sizeof(char), 1, arquivonbin);

    fwrite(registro.nEspecie, sizeof(char), tamEspecie, arquivonbin);
    fwrite(&delimitador, sizeof(char), 1, arquivonbin);

    fwrite(registro.habitat, sizeof(char), tamHabitat, arquivonbin);
    fwrite(&delimitador, sizeof(char), 1, arquivonbin);

    fwrite(registro.tipo, sizeof(char), tamTipo, arquivonbin);
    fwrite(&delimitador, sizeof(char), 1, arquivonbin);

    fwrite(registro.dieta, sizeof(char), tamDieta, arquivonbin);
    fwrite(&delimitador, sizeof(char), 1, arquivonbin);

    fwrite(registro.alimento, sizeof(char), tamAlimento, arquivonbin);
    fwrite(&delimitador, sizeof(char), 1, arquivonbin);

    // ve a posicao
    psc_atual = ftell(arquivonbin);
    psc_final = 1600 + (qtd_registro * 160);

    // $$$$$ ate completar
    for (int i = psc_atual; i < psc_final; i++) {
        fwrite("$", sizeof(char), 1, arquivonbin);
    }

    return 0;  // Sucesso
}
int arquivo_ok(FILE *arquivobin, char *arquivo)
{
    if(arquivobin == NULL)
    {
        printf("Foi impossível de abrir o arquivo: %s\n", arquivo);
        return -1;
    }

    return 0;
}

Registro ler_registro(FILE *arquivobin, char * arquivo){
    Registro reg_;
    char data[142] /*por que este tamanho??*/, *linha;

    if(arquivo_ok(arquivobin, arquivo) <0)
        return reg_;

    if(fread(&reg_.removido,sizeof(char),1,arquivobin)==0){
        reg_.removido = '2';
        return reg_;
    }

    if(reg_.removido == '1')
    {
        fread(&reg_.encadeamento,sizeof(int),1,arquivobin);
        return reg_;
    }

    if( reg_.removido == '0')
    {
         {
        fread(&reg_.encadeamento, sizeof(int),1,arquivobin);
        fread(&reg_.populacao, sizeof(int),1,arquivobin);
        fread(&reg_.tamanho, sizeof(float),1,arquivobin);
        fread(&reg_.uniMedida, sizeof(char),1,arquivobin);
        fread(&reg_.velocidade, sizeof(int), 1,arquivobin);

        if(fread(data,sizeof(char),142, arquivobin)==0)
        {
            reg_.removido = '2';
            return reg_;
        }

        linha - strdup(data);
        reg_.nome = strsep(&linha , "#");
        reg_.tipo = strsep(&linha, "#");
        reg_.tamanho = atoi(strsep(&linha, "#"));

        reg_.nEspecie =strsep(&linha, "#");
        reg_.habitat = strsep(&linha, "#");
        reg_.dieta = strsep(&linha, "#");

        return reg_;
}}

void sub_registro (FILE *arquivobin, Registro registro)
{ char delim = '#';

    // Escrevendo campos binários
    fwrite(&registro.removido, sizeof(char), 1, arquivobin);
    fwrite(&registro.encadeamento, sizeof(int), 1, arquivobin);
    fwrite(&registro.populacao, sizeof(int), 1, arquivobin);
    fwrite(&registro.tamanho, sizeof(float), 1, arquivobin);
    fwrite(&registro.uniMedida, sizeof(char), 1, arquivobin);
    fwrite(&registro.velocidade, sizeof(int), 1, arquivobin);

    // Escrevendo strings e delimitadores
    if (registro.nome) {
        fwrite(registro.nome, sizeof(char), strlen(registro.nome), arquivobin);
    }
    fwrite(&delim, sizeof(char), 1, arquivobin);

    if (registro.nEspecie) {
        fwrite(registro.nEspecie, sizeof(char), strlen(registro.nEspecie), arquivobin);
    }
    fwrite(&delim, sizeof(char), 1, arquivobin);

    if (registro.habitat) {
        fwrite(registro.habitat, sizeof(char), strlen(registro.habitat), arquivobin);
    }
    fwrite(&delim, sizeof(char), 1, arquivobin);

    if (registro.tipo) {
        fwrite(registro.tipo, sizeof(char), strlen(registro.tipo), arquivobin);
    }
    fwrite(&delim, sizeof(char), 1, arquivobin);

    if (registro.dieta) {
        fwrite(registro.dieta, sizeof(char), strlen(registro.dieta), arquivobin);
    }
    fwrite(&delim, sizeof(char), 1, arquivobin);

    if (registro.alimento) {
        fwrite(registro.alimento, sizeof(char), strlen(registro.alimento), arquivobin);
    }
    fwrite(&delim, sizeof(char), 1, arquivobin);


    return 0;
}
