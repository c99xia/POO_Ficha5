#pragma once
#include "Cliente.h"

class ArmarioFichas
{
	Cliente** clientes;		// clientes um Array dinâmicos de ponteiros do tipo Cliente
	
	// Cliente* seria um array de OBJETOS Cliente, o que exigiria:
	//   - new Cliente[n] chamaria o construtor default Cliente() n vezes
	//   - MAS a classe Cliente NAO tem construtor default, logo não compila
	//   - Apenas tem Cliente(string, int) que requer parametros
	//   - E o enunciado diz que a classe Cliente NAO e alteravel
	//
	// SOLUCAO: Cliente** (array de ponteiros)
	//   - new Cliente*[n] cria apenas n PONTEIROS (nao objetos)
	//   - Cada Cliente e criado INDIVIDUALMENTE quando necessario:
	//     clientes[i] = new Cliente(nome, nif);
	//   - Permite criar so quando temos dados validos (nome, NIF)
	//   - Posicoes vazias podem ser nullptr
	
	/*
	SEM construtor default Cliente () → Cliente** (obrigatório)
	COM construtor default Cliente ()→ Depende:
	- Tamanho fixo/pequeno → Cliente* (array de objetos)
	- Tamanho variável/grande → Cliente** (array de ponteiros)
	*/
	
	int numClientes;		// Número atual de clientes 

	class InfoCliente {
		std::string nomeCliente;
		int numConsultas;

	public:
		//Construtor
		InfoCliente(const std::string& nomeClienteP, int numConsultasP);

		//Getters
		std::string getNomeCliente() const { return nomeCliente; }
		int getNumConsultas() const { return numConsultas; }
	};

public:
	//Construtor da Classe
	ArmarioFichas();

	//Construtor por Cópia
	ArmarioFichas(const ArmarioFichas& outra);

	//Operador de Atribuição
	ArmarioFichas& operator=(const ArmarioFichas& outra);

	//Destrutor
	~ArmarioFichas();

	//Acrescentar clientes
	bool acrescentarClientes(const std::string& nome, int nif); // dadas as informações necessárias a um novo cliente, logo são os parâmetros que são necessários para "construir" um cliente

	//Apagar cliente
	bool apagarCliente(int nif);

	//Registar uma nova consulta dado NIF
	bool registarConsulta(int nif);

	//Obter nome e número de consultas de um cliente dado NIF
	InfoCliente obterDados(int nif) const;

	//Esvaziar o conjunto de clientes
	void esvaziar();

	//Obter a listagem de clientes
	std::string listagem() const;

	//Getter
	int getNumClientes() const { return numClientes; }
};

