#include "circuito.h"
#include <fstream>

using namespace std;
///
/// CLASSE CIRCUITO
///

/// ***********************
/// Inicializacao e finalizacao
/// ***********************

// Construtor por copia
Circuito::Circuito(const Circuito &C)
    : Nin_circ(C.Nin_circ), ports(), out_circ(), id_in(), id_out() {

  for (auto p : C.ports) {
    this->ports.push_back(p->clone());
  }

  this->out_circ = C.out_circ;
  this->id_in = C.id_in;
  this->id_out = C.id_out;
}

// Construtor por movimento
Circuito::Circuito(Circuito &&C)
    : Nin_circ(), ports(), out_circ(), id_in(), id_out() {
  swap(this->Nin_circ, C.Nin_circ);
  swap(this->ports, C.ports);
  swap(this->out_circ, C.out_circ);
  swap(this->id_in, C.id_in);
  swap(this->id_out, C.id_out);
}
// Limpa todo o conteudo do circuito.
void Circuito::clear() {
  Nin_circ = 0;
  for (auto p : this->ports) {
    delete p;
  }
  ports.clear();
  out_circ.clear();
  id_in.clear();
  id_out.clear();
}

// Operador de atribuicao por copia
Circuito &Circuito::operator=(const Circuito &C) {
  if (this != &C) {
    clear();
    Nin_circ = C.Nin_circ;
    for (auto p : C.ports) {
      this->ports.push_back(p->clone());
    }
    this->out_circ = C.out_circ;
    this->id_in = C.id_in;
    this->id_out = C.id_out;
  }
  return *this;
}

// Operador de atribuicao por movimento
Circuito &Circuito::operator=(Circuito &&C) {
  clear();
  swap(this->Nin_circ, C.Nin_circ);
  swap(this->ports, C.ports);
  swap(this->out_circ, C.out_circ);
  swap(this->id_in, C.id_in);
  swap(this->id_out, C.id_out);
  return *this;
}

// Redimensiona o circuito
// Redimensiona o circuito para passar a ter NI entradas, NO saidas e NP portas,
// com todos os vetores com valores iniciais nulos.

void Circuito::resize(int NI, int NO, int NP) {
  if (NI < 0 && NO < 0 && NP < 0) {
    return;
  }
  this->clear();

  this->Nin_circ = NI;

  this->ports.resize(NP);
  for (int i = 0; i < NP; i++) {
    if (ports[i] != nullptr) {
      this->ports[i] = ports[i]->clone();
    }
  }

  this->out_circ.resize(NO);
  this->id_in.resize(NP);
  for (auto ii : id_in) {
    ii.resize(NP);
  }
  this->id_out.resize(NO);
}

/// ***********************
/// Funcoes de testagem
/// ***********************

// Testa igualdade entre circuitos
bool Circuito::operator==(const Circuito &C) const {
  // Testa a igualdade do numero de entradas, saidas e portas
  if (getNumInputs() != C.getNumInputs() ||
      getNumOutputs() != C.getNumOutputs() || getNumPorts() != C.getNumPorts())
    return false;
  // Testa a igualdade das portas
  for (int id = 1; id <= getNumPorts(); ++id) {
    if (definedPort(id)) {
      if (!C.definedPort(id))
        return false;
      if (getNamePort(id) != C.getNamePort(id))
        return false;
      if (getNumInputsPort(id) != C.getNumInputsPort(id))
        return false;
      for (int j = 0; j < getNumInputsPort(id); ++j) {
        if (getIdInPort(id, j) != C.getIdInPort(id, j))
          return false;
      }
    } else if (C.definedPort(id))
      return false;
  }
  // Testa a igualdade das saidas
  for (int id = 1; id <= getNumOutputs(); ++id) {
    if (getIdOutput(id) != C.getIdOutput(id))
      return false;
  }
  // Tudo igual!
  return true;
}

// Testa circuito valido
bool Circuito::valid() const {
  // Testa o numero de entradas, saidas e portas
  if (getNumInputs() <= 0 || getNumOutputs() <= 0 || getNumPorts() <= 0)
    return false;
  // Testa cada porta
  for (int id = 1; id <= getNumPorts(); ++id) {
    if (!definedPort(id))
      return false;
    for (int j = 0; j < getNumInputsPort(id); ++j) {
      if (!validIdOrig(getIdInPort(id, j)))
        return false;
    }
  }
  // Testa cada saida
  for (int id = 1; id <= getNumOutputs(); ++id) {
    if (!validIdOrig(getIdOutput(id)))
      return false;
  }
  // Tudo valido!
  return true;
}

/// ***********************
/// Funcoes de modificacao
/// ***********************

// Altera uma porta

// A porta cuja id eh IdPort passa a ser do tipo Tipo (NT, AN, etc.), com Nin
// entradas. Tambem altera o numero de conexoes dessa porta no vetor id_in, para
// ser igual ao novo numero de entradas da porta. Caso necessario, converte os
// caracteres da string para maiusculas. Se der tudo certo, retorna true. Se
// algum parametro for invalido, retorna false.

bool Circuito::setPort(int IdPort, std::string Tipo, int Nin) {
  // Chegagem dos parametros
  if (!validIdPort(IdPort))
    return false;
  if (Tipo.size() != 2)
    return false;
  Tipo.at(0) = toupper(Tipo.at(0));
  Tipo.at(1) = toupper(Tipo.at(1));

  if (Tipo != "NT" && Tipo != "AN" && Tipo != "NA" && Tipo != "OR" &&
      Tipo != "NO" && Tipo != "XO" && Tipo != "NX")
    return false;
  if (Tipo == "NT" && Nin != 1)
    return false;
  if (Tipo != "NT" && Nin < 2)
    return false;

  // Altera a porta

  if (Tipo == "NT") {
    delete ports.at(IdPort - 1);
    ports.at(IdPort - 1) = new PortaNOT();
  }
  if (Tipo == "AN") {
    delete ports.at(IdPort - 1);
    ports.at(IdPort - 1) = new PortaAND(Nin);
  }
  if (Tipo == "NA") {
    delete ports.at(IdPort - 1);
    ports.at(IdPort - 1) = new PortaNAND(Nin);
  }
  if (Tipo == "OR") {
    delete ports.at(IdPort - 1);
    ports.at(IdPort - 1) = new PortaOR(Nin);
  }
  if (Tipo == "NO") {
    delete ports.at(IdPort - 1);
    ports.at(IdPort - 1) = new PortaNOR(Nin);
  }
  if (Tipo == "XO") {
    delete ports.at(IdPort - 1);
    ports.at(IdPort - 1) = new PortaXOR(Nin);
  }
  if (Tipo == "NX") {
    delete ports.at(IdPort - 1);
    ports.at(IdPort - 1) = new PortaNXOR(Nin);
  }
  // //Redimensiona o vetor de conexoes da porta
  id_in.at(IdPort - 1).resize(Nin);

  return true;
}

// Altera a origem de uma entrada de uma porta
bool Circuito::setIdInPort(int IdPort, int I, int IdOrig) {
  // Chegagem dos parametros
  if (!definedPort(IdPort))
    return false;
  if (!ports.at(IdPort - 1)->validIndex(I))
    return false;
  if (!validIdOrig(IdOrig))
    return false;
  // Fixa a origem da entrada
  id_in.at(IdPort - 1).at(I) = IdOrig;
  return true;
}

// Altera a origem de uma saida
bool Circuito::setIdOutput(int IdOut, int IdOrig) {
  if (!validIdOutput(IdOut) || !validIdOrig(IdOrig))
    return false;
  id_out.at(IdOut - 1) = IdOrig;
  return true;
}

/// ***********************
/// E/S de dados
/// ***********************

// Entrada dos dados de um circuito via teclado
void Circuito::digitar() {
  int NI, NO, NP;
  int id, I;
  Circuito prov;

  // Lendo as dimensoes do circuito
  std::cout << "CIRCUITO:\n";
  do {
    std::cout << "Numero de entradas do circuito (>0): ";
    std::cin >> NI;
  } while (NI <= 0);
  do {
    std::cout << "Numero de saidas do circuito (>0): ";
    std::cin >> NO;
  } while (NO <= 0);
  do {
    std::cout << "Numero de ports do circuito (>0): ";
    std::cin >> NP;
  } while (NP <= 0);
  // Redimensionando o circuito
  resize(NI, NO, NP);

  // Lendo as portas
  std::cout << "PORTAS:\n";
  for (id = 1; id <= getNumPorts(); ++id) {
    std::cout << "Definindo a porta id=" << id << std::endl;
    // Lendo o tipo e o numero de entradas de uma porta
    string tipo;
    int Nin;

    do {
      std::cout << "Tipo (NT, AN, NA, OR, NO, XO, NX): ";
      std::cin >> tipo;
      std::cout << "Número de entradas: ";
      std::cin >> Nin;
    } while (!setPort(id, tipo, Nin));
  }
  // Lendo a conectividade das portas
  std::cout << "CONEXOES DAS PORTAS:\n";
  for (id = 1; id <= getNumPorts(); ++id) {
    std::cout << "Definindo a conexao da porta id=" << id << std::endl;
    for (I = 0; I < getNumInputsPort(id); ++I) {
      // Lendo a id de origem de uma das entradas de uma porta do circuito
      int IdOrig;
      do {
        std::cout << "Id da origem da entrada " << I << ":";
        std::cin >> IdOrig;
      } while (!validIdOrig(IdOrig));
      setIdInPort(id, I, IdOrig);
    }
  }

  // Lendo a conectividade das saidas
  std::cout << "CONEXOES DAS SAIDAS:\n";
  for (id = 1; id <= getNumOutputs(); ++id) {
    // Lendo a id de origem de uma saida do circuito
    std::cout << "Definindo a conexao da saida id=" << id << "\n";
    int IdOrig;
    do {
      std::cout << "Id da origem de uma saida: ";
      std::cin >> IdOrig;
    } while (!validIdOrig(IdOrig));
    setIdOutput(id, IdOrig);
  }
}

// Entrada dos dados de um circuito via arquivo
bool Circuito::ler(const std::string &arq) {
  // Novo circuito provisorio a ser lido do arquivo
  Circuito prov;
  // A stream do arquivo a ser lido
  std::ifstream myfile(arq);

  try {
    if (!myfile.is_open())
      throw 1;

    // Variaveis temporarias para leitura
    std::string pS, type;
    int NI, NO, NP, n_in;
    int i, id, I;
    char o;
    double IdOrig;
    // Lendo as dimensoes do circuito
    myfile >> pS >> NI >> NO >> NP;
    if (!myfile.good() || pS != "CIRCUITO" || NI <= 0 || NO <= 0 || NP <= 0)
      throw 2;
    // Redimensionando o novo circuito
    prov.resize(NI, NO, NP);

    // Lendo as portas do circuito
    myfile >> pS;
    if (!myfile.good() || pS != "PORTAS")
      throw 3;
    for (i = 0; i < prov.getNumPorts(); ++i) {
      // Lendo o tipo e o numero de entradas de uma porta
      std::string type;
      myfile >> id >> o >> type >> n_in;
      if (!myfile.good() || o != ')' || (id != (i + 1)) ||
          !prov.setPort(id, type, n_in))
        throw 4;
    }
    // Lendo a conectividade das portas
    myfile >> pS;
    if (!myfile.good() || pS != "CONEXOES")
      throw 5;

    for (i = 0; i < prov.getNumPorts(); ++i) {
      // Lendo a id da porta
      myfile >> id >> o;
      if (!myfile.good() || o != ')' || !prov.validIdPort(id))
        throw 6;
      // // Lendo as ids das entradas da porta
      for (I = 0; I < prov.getNumInputsPort(id); ++I) {
        myfile >> IdOrig;
        if (!myfile.good() || !prov.validIdOrig(IdOrig))
          throw 7;
        prov.setIdInPort(id, I, IdOrig);
      }
    }

    // Lendo as saidas do circuito
    myfile >> pS;
    if (!myfile.good() || pS != "SAIDAS")
      throw 8;

    for (i = 0; i < prov.getNumOutputs(); ++i) {
      int IdOrig_saidas;
      myfile >> id >> o >> IdOrig_saidas;

      if (!myfile.good() || o != ')' || (id != (i + 1)) || !prov.validIdOrig(IdOrig_saidas))
        throw 9;
      prov.setIdOutput(id, IdOrig_saidas);
    }
  } catch (int erro) {
    // Mensagem de erro para debug.
    std::cerr << "ERRO Circuito::ler - arquivo (" << arq
              << ") invalido - erro tipo " << erro << std::endl;

    return false;
  }

  // Leitura OK
  // Faz o circuito assumir as caracteristicas lidas do arquivo
  *this = std::move(prov);
  return true;
}

// Saida dos dados de um circuito
ostream &Circuito::imprimir(std::ostream &O) const {
  // Soh imprime se o circuito for valido
  if (!valid())
    return O;

  int i, I;

  O << "CIRCUITO " << getNumInputs() << " " << getNumOutputs() << " "
    << getNumPorts() << "\n";
  O << "PORTAS\n";
  for (i = 1; i <= getNumPorts(); ++i) {
    O << i << ") " << getNamePort(i) << " " << getNumInputsPort(i) << "\n";
  }
  O << "CONEXOES\n";
  for (i = 1; i <= getNumPorts(); ++i) {
    O << i << ") ";
    for (I = 0; I < getNumInputsPort(i); ++I) {
      O << getIdInPort(i, I) << " ";
    }
    O << "\n";
  }
  O << "SAIDAS\n";
  for (i = 1; i <= getNumOutputs(); ++i) {
    O << i << ") " << getIdOutput(i) << "\n";
  }
  return O;
}

// Salvar circuito em arquivo
bool Circuito::salvar(const std::string &arq) const {
  if (!valid())
    return false;

  std::ofstream myfile(arq);
  if (!myfile.is_open())
    return false;
  imprimir(myfile);
  return true;
}

/// ***********************
/// SIMULACAO (funcao principal do circuito)
/// ***********************

// Simula o circuito
bool Circuito::simular(const std::vector<bool3S> &in_circ) {
  // Soh simula se o cicuito e o parametro forem validos
  if (!valid() || int(in_circ.size()) != getNumInputs())
    return false;

  bool tudo_def, alguma_def;
  int id;
  // INICIALIZAÇÃO
  for (int i = 0; i < getNumPorts(); i++) {
    ports[i]->setOutput(bool3S::UNDEF);
  }

  // ALGORITMO ITERATIVO
  do {
    tudo_def = true;
    alguma_def = false;
    for (int i = 0; i < getNumPorts(); i++) {
      if (ports[i]->getOutput() == bool3S::UNDEF) {
        std::vector<bool3S> in_port(ports[i]->getNumInputs(), bool3S::UNDEF);
        for (int j = 0; j < ports[i]->getNumInputs(); j++) {
          id = getIdInPort((i + 1), j);
          in_port.at(j) =
              (id > 0) ? ports[id - 1]->getOutput() : in_circ.at(-id - 1);
        }
        ports[i]->simular(in_port);
        if (ports[i]->getOutput() == bool3S::UNDEF) {
          tudo_def = false;
        } else {
          alguma_def = true;
        }
      }
    }
  } while (!tudo_def && alguma_def);

  // DETERMINAÇÃO DAS SAÍDAS
  for (int k = 0; k < getNumOutputs(); k++) {
    id = getIdOutput(k + 1);
    out_circ[k] = (id > 0) ? ports[id - 1]->getOutput() : in_circ.at(-id - 1);
  }

  return true;
}
