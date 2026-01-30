#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

class Cliente {
    std::string nome;
    int nif;
    int numConsultas;
public:
    Cliente(std::string _nome, int _nif);
    std::string obtemNome() const { return nome; }
    int obtemNIF() const { return nif; }
    int obtemNumConsultas() const { return numConsultas; }
    void novaConsulta();
    std::string obtemDesc() const;
};