//___________________________________________________________________________________
// PROJETO PRÁTICO - SISTEMA DE CADASTRO EM ARQUIVOS
// DISCIPLINA: PROGAMAÇÃO DE COMPUTADORES I
// DOCENTE: PAULA CHRISTINA FIGUEIRA CARDOSO
// DISCENTES: ALICE FERNANDA SILVA BARROS (202511140025)
//            EDIMAX OLIVEIRA BASTOS JUNIOR (202511140023)
// TEMA: PAÍSES
//___________________________________________________________________________________

#include <fstream>
#include <iostream>
using namespace std;

// Registro de país
struct paises {
  string nome, idioma, continente;
  int populacao, id;
};

// verificar se o vetor está cheio e se precisa aumentar (vetor dinâmico)
// usa o & para alterar a capacidade e o vetor, não só uma cópia e a quantidade atual é a posição atual do vetor
void tamanhoVetor(paises *&vetor, int &capacidade, int quantidadeAtual) { 
  int incremento = 10; // redimensionamento de 10 em 10
  if (quantidadeAtual >= capacidade) { // verifica se a capacidade foi atingida 
    cout << "Vetor cheio, aumentando capacidade...\n";
    int novaCapacidade = capacidade + incremento; // aumenta a capacidade
    // cria um novo vetor com a nova capacidade, maior que a anterior
    paises *novoVetor = new paises[novaCapacidade];

    // copiar os dados do vetor antigo para o novo (para não perder)
    for (int i = 0; i < capacidade; i++) {
      novoVetor[i] = vetor[i];
    }
    delete[] vetor; // libera a memória do vetor antigo, pra não lotar a memória ram
    // atualiza o vetor e a capacidade, agora com o novo tamanho
    vetor = novoVetor;
    capacidade = novaCapacidade; // Atualiza só no final, para não dar erro 
    cout << "Capacidade aumentada para " << capacidade << endl;
  }
}

// Menu
void menu() {
  cout << "_____________________________________________\n\n";
  cout << "1 - Cadastrar país\n";
  cout << "2 - Imprimir lista de países\n";
  cout << "3 - Buscar por id\n";
  cout << "4 - Buscar por nome\n";
  cout << "5 - Buscar por continente\n"; 
  cout << "6 - Remover país\n";
  cout << "7 - Salvar alterações no arquivo\n";
  cout << "Digite -1 para encerrar o programa\n";
}

// Função para imprimir a lista de países (agora completa ou um trecho
// especificado pelo usuário)
void imprimir(paises *vetor, int quantidade) {

  int opcao;
  cout << "\nDigite 1 para imprimir a lista completa ou 2 para imprimir um trecho: \n";
  cin >> opcao;

  cout << "_____________________________________________\n\n";
  cout << "ID - NOME - POPULAÇÃO - IDIOMA(S) - CONTINENTE\n";

  if (opcao == 1) { // imprime a lista completa
    for (int i = 0; i < quantidade; i++) { 
      if (vetor[i].id != -1){ // agora não imprime os países removidos logicamente (id -1)
        cout << vetor[i].id << " - ";
        cout << vetor[i].nome << " - ";
        cout << vetor[i].populacao << " - ";
        cout << vetor[i].idioma << " - ";
        cout << vetor[i].continente << endl;
      } 
    }
  }
  else if (opcao == 2) { // imprime um trecho especificado pelo usuário
    int posInicial, posFinal;
    cout << "Digite a posição inicial do trecho: \n";
    cin >> posInicial;
    cout << "Digite a posição final do trecho: \n";
    cin >> posFinal;

    int inicio = posInicial - 1; // porque o vetor começa em 0
    int fim = posFinal - 1;

    // tratamento de erro para intervalo inválido
    if (inicio < 0)
      inicio = 0; // se digitar um número negativo, o início será 0
    if (fim > quantidade - 1)
      fim = quantidade - 1; // se extrapolar a quantidade de países, o fim será
                            // a última posição

    if (inicio <= fim) {
      for (int i = inicio; i <= fim; i++) {
        if (vetor[i].id != -1){ 
          cout << vetor[i].id << " - ";
          cout << vetor[i].nome << " - ";
          cout << vetor[i].populacao << " - ";
          cout << vetor[i].idioma << " - ";
          cout << vetor[i].continente << endl;
        }
      }
    }
    else {
      cout << "Intervalo inválido!\n";
    }
  }
  else {
    cout << "Opção inválida!\n";
  }
}

// Função para cadastrar país
void cadastrar(paises *vetor, int &posicao) {
  // gerando o id automaticamente agora
  int maiorId = 0;
  // percorre o vetor para encontrar o maior id
  for (int i = 0; i < posicao; i++) {
    if (vetor[i].id > maiorId)
      maiorId = vetor[i].id;
  }
  // o id do novo país vai ser o maior id + 1, pra não repetir e ir na ordem
  vetor[posicao].id = maiorId + 1;
  
  // pegando as informações pelo usuário
  cout << "_____________________________________________\n\n";
  cout << "Digite as informações do país para cadastrar: \n";
  cout << "Digite o nome, a população, o idioma e o continente: \n";
  cin.ignore();
  getline(cin, vetor[posicao].nome);
  cin >> vetor[posicao].populacao;
  cin.ignore();
  getline(cin, vetor[posicao].idioma);
  getline(cin, vetor[posicao].continente);

  posicao++;
  cout << "País cadastrado com sucesso!\n";
}

// Procedimento de remover dados (logicamente pelo id)
void remover(paises *vetor, int quantidade) {
  int idRemover;
  cout << "\nDigite o id do país para remover: \n";
  cin >> idRemover;
  int achou = 0; // verifica se o id foi encontrado (0 = não, 1 = sim)

  for (int i = 0; i < quantidade && achou == 0; i++) {
    if (vetor[i].id == idRemover) {
      vetor[i].id = -1; // marca como removido
      cout << "País removido!\n";
      achou = 1; // marca como encontrado
    }
  }
  if (achou == 0) {
    cout << "Id não encontrado!(ou removido)\n\n";
  }
}

// procedimento pra regravar o arquivo apagando os dados removidos
void salvarArquivo (paises* vetor, int quantidade){
  ofstream arquivo("paises.csv"); 
  arquivo << "IDENTIFICADOR;PAÍS;POPULAÇÃO;IDIOMA(S);CONTINENTE\n"; // grava o cabeçalho

  for(int i = 0; i < quantidade; i++){
    if (vetor[i].id != -1){ // grava só os países que não foram removidos
      arquivo << vetor[i].id << ";";
      arquivo << vetor[i].nome << ";";
      arquivo << vetor[i].populacao << ";";
      arquivo << vetor[i].idioma << ";";
      arquivo << vetor[i].continente << endl;
    }
  }
  arquivo.close();
  cout << "Alterações salvas!\n";
}
      
// Busca por id
void buscarId(paises* vetor, int quantidade){
  int idBusca;
  cout << "Digite o id que deseja buscar: \n";
  cin >> idBusca;
  int achou = 0;

  for(int i = 0; i < quantidade && achou == 0; i++){
    if (vetor[i].id == idBusca && vetor[i].id != -1){// verifica também se o id não foi removido (-1)
      cout << "País encontrado!\n";
      cout << "ID - NOME - POPULAÇÃO - IDIOMA(S) - CONTINENTE\n";
      cout << vetor[i].id << " - ";
      cout << vetor[i].nome << " - ";
      cout << vetor[i].populacao << " - ";
      cout << vetor[i].idioma << " - ";
      cout << vetor[i].continente << endl;
      achou = 1;
    }
  }
  if (achou == 0){
    cout << "Id não encontrado!(ou removido)\n\n";
  }
}

// Busca por nome
void buscarNome(paises* vetor, int quantidade){
  string nomeBusca;
  cout << "Digite o nome que deseja buscar: \n";
  cin.ignore();
  getline(cin, nomeBusca);
  int achou = 0;

  for(int i = 0; i < quantidade && achou == 0; i++){
    if (vetor[i].nome == nomeBusca && vetor[i].id != -1){
      cout << "País encontrado!\n";
      cout << "ID - NOME - POPULAÇÃO - IDIOMA(S) - CONTINENTE\n";
      cout << vetor[i].id << " - ";
      cout << vetor[i].nome << " - ";
      cout << vetor[i].populacao << " - ";
      cout << vetor[i].idioma << " - ";
      cout << vetor[i].continente << endl;
      achou = 1;
    }
  }
  if (achou == 0)
    cout << "Nome não encontrado!(ou removido)\n\n";
}

// Busca por continente 
void buscarContinente(paises* vetor, int quantidade){
  string continenteBusca;
  cout << "Digite o continente que deseja buscar: \n";
  cin.ignore();
  getline(cin, continenteBusca);
  int achou = 0; // agora é um contador, pois há mais de um país por continente

  cout << "Países encontrados no continente: " << continenteBusca << endl;
  for (int i = 0; i < quantidade; i++){
    if (vetor[i].continente == continenteBusca && vetor[i].id != -1){

      if (achou == 0)
        cout << "ID - NOME - POPULAÇÃO - IDIOMA(S) - CONTINENTE\n"; // o cabeçalho só aparece se houver países no continente

      cout << vetor[i].id << " - ";
      cout << vetor[i].nome << " - ";
      cout << vetor[i].populacao << " - ";
      cout << vetor[i].idioma << " - ";
      cout << vetor[i].continente << endl;
      achou++;
    }
  }
  if (achou == 0)
    cout << "Continente inválido!\n\n";
}

// Função principal
int main() {
  int capacidade = 40; // tamanho inicial do vetor, 40 posições
  int opcao = -1; 
  char lixo; // variável para pegar o ; do arquivo
  int posicao = 0; // posição atual do vetor, verifica quantos países já foram cadastrados
  paises *vetor = new paises[capacidade]; // vetor dinâmico, com capacidade inicial de 40 posições

  // Leitura automática do arquivo
  ifstream arquivo("paises.csv");
  if (!arquivo)
    cout << "Arquivo não encontrado!\n";
  else {
    string linhaCabecalho; /* string temporária pra pular a primeira linha
                              do arquivo (o cabeçalho)*/
    getline(arquivo, linhaCabecalho);
    while (arquivo >> vetor[posicao].id) {
      arquivo >> lixo;
      getline(arquivo, vetor[posicao].nome, ';');
      arquivo >> vetor[posicao].populacao;
      arquivo >> lixo;
      getline(arquivo, vetor[posicao].idioma, ';');
      getline(arquivo, vetor[posicao].continente);

      // tratamento de string para remover o tal do  \r do final da linha, que não está sendo removido 
      //pelo getline e tá causando erro na busca por continente
      string continenteLimpo = ""; // uma string vazia 
      // passa por cada caractere da string do continente
      for (int i = 0; i < vetor[posicao].continente.size(); i++){
        // se o caractere não for \r, adiciona na string vazia
        if (vetor[posicao].continente[i] != '\r'){
          continenteLimpo = continenteLimpo + vetor[posicao].continente[i];
        }
      }
      // atualiza o continente do país com a string limpa
      vetor[posicao].continente = continenteLimpo;
      
      posicao++;
      tamanhoVetor(vetor, capacidade, posicao); // Verifica se o arquivo é muito grande e precisa
                                                // de mais espaço!
    }
    arquivo.close();
    cout << "Fim da leitura do arquivo!\n";
  }

  // laço do menu
  do {
    menu();
    cin >> opcao;

    switch (opcao) {

    case -1: { // encerra o programa
      // perguntar se quer salvar as alterações antes de sair
      int salvar = 0;
      cout << "\nDeseja salvar as alterações antes de sair? (1 - sim, 0 - não)\n";
      cin >> salvar;
      if (salvar == 1) {
        salvarArquivo(vetor, posicao);
      }

      cout << "Programa encerrado!\n";
      break;
    } // Precisa de chaves, pois não da para declarar variáveis em um case sem
      // elas, caso contrário, dá erro.

    case 1: { // Cadastrar país (no arquivo)
      tamanhoVetor(vetor, capacidade, posicao);
      cadastrar(vetor, posicao);
      break;
    }

    case 2: // Imprimir lista de países
      imprimir(vetor, posicao);
      break;

    case 3: // Buscar por id
      buscarId(vetor, posicao);
      break;

    case 4: // Buscar por nome
      buscarNome(vetor, posicao);
      break;

    case 5: // Buscar por continente
      buscarContinente(vetor, posicao);
      break;

    case 6: // Remove o país pelo id (logicamente)
      remover(vetor, posicao);
      break;

    case 7: // Salva as alterações no arquivo (definitivo)
      salvarArquivo(vetor, posicao);
      break;

    default: 
      cout << "Opção inválida!\n";
      break;
    }
  } while (opcao != -1); 
  delete[] vetor; // Libera a memória alocada para o vetor, pra não lotar a memória ram

  return 0;
}