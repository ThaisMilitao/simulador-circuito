#include "porta.h"

/*Porta NOT*/
// Simula a porta logica
bool PortaNOT::simular(const std::vector<bool3S> &in_port) {
  if (in_port.size() > 0 && in_port.size() == getNumInputs()) {
    out_port = ~(in_port.at(0));
    return true;
  }
  out_port = bool3S::UNDEF;
  return false;
}

/*Porta AND*/
// Simula a porta logica
bool PortaAND::simular(const std::vector<bool3S> &in_port) {
  if (in_port.size() > 0 && in_port.size() == getNumInputs()) {
    out_port = in_port.at(0);
    for (int i = 1; i < in_port.size(); i++) {
      out_port &= in_port.at(i);
    }
    return true;
  }
  out_port = bool3S::UNDEF;
  return false;
}

/*Porta NAND*/
// Simula a porta logica
bool PortaNAND::simular(const std::vector<bool3S> &in_port) {
  if (in_port.size() > 0 && in_port.size() == getNumInputs()) {
    out_port = in_port.at(0);
    for (int i = 1; i < in_port.size(); i++) {
      out_port &= in_port.at(i);
    }
    out_port = ~out_port;
    return true;
  }
  out_port = bool3S::UNDEF;
  return false;
}

/*Porta OR*/
// Simula a porta logica
bool PortaOR::simular(const std::vector<bool3S> &in_port) {
  if (in_port.size() > 0 && in_port.size() == getNumInputs()) {
    out_port = in_port.at(0);
    for (int i = 1; i < in_port.size(); i++) {
      out_port |= in_port.at(i);
    }
    return true;
  }
  out_port = bool3S::UNDEF;
  return false;
}

/*Porta NOR*/
// Simula a porta logica
bool PortaNOR::simular(const std::vector<bool3S> &in_port) {
  if (in_port.size() > 0 && in_port.size() == getNumInputs()) {
    out_port = in_port.at(0);
    for (int i = 1; i < in_port.size(); i++) {
      out_port |= in_port.at(i);
    }
    out_port = ~out_port;
    return true;
  }
  out_port = bool3S::UNDEF;
  return false;
}

/*Porta XOR*/
// Simula a porta logica
bool PortaXOR::simular(const std::vector<bool3S> &in_port) {
  if (in_port.size() > 0 && in_port.size() == getNumInputs()) {
    out_port = in_port.at(0);
    for (int i = 1; i < in_port.size(); i++) {
      out_port ^= in_port.at(i);
    }
    return true;
  }
  out_port = bool3S::UNDEF;
  return false;
}

/*Porta XOR*/
// Simula a porta logica
bool PortaNXOR::simular(const std::vector<bool3S> &in_port) {
  if (in_port.size() > 0 && in_port.size() == getNumInputs()) {
    out_port = in_port.at(0);
    for (int i = 1; i < in_port.size(); i++) {
      out_port ^= in_port.at(i);
    }
    out_port = ~out_port;
    return true;
  }
  out_port = bool3S::UNDEF;
  return false;
}
