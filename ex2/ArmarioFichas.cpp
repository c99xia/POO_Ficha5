#include "ArmarioFichas.h"

// Construtor Default
ArmarioFichas::ArmarioFichas() : numClientes(0), clientes(nullptr) {}

// ============================================================================
// CONSTRUTOR POR COPIA (Deep Copy)
// ============================================================================
// Cria um NOVO objeto ArmarioFichas copiando todos os dados de outro objeto
// JA EXISTENTE, garantindo que sao objetos INDEPENDENTES (deep copy).
//
// Exemplo de uso:
//   ArmarioFichas a;           // Cria armario 'a'
//   a.acrescentarClientes(...); // Adiciona clientes em 'a'
//   ArmarioFichas b(a);        // Cria armario 'b' COPIANDO 'a'
//   ArmarioFichas c = a;       // Tambem chama o construtor por copia!
//
// Depois da copia, 'a' e 'b' sao INDEPENDENTES:
//   - Modificar 'a' NAO afeta 'b'
//   - Destruir 'a' NAO afeta 'b'
// ============================================================================
ArmarioFichas::ArmarioFichas(const ArmarioFichas& outra) {
	// PASSO 1: Copiar o numero de clientes (valor simples)
	numClientes = outra.numClientes;
	// 'numClientes' e um int, entao e uma copia simples

	// PASSO 2: Alocar NOVA memoria para o array de ponteiros
	clientes = new Cliente * [outra.numClientes];
	// 'clientes' (tipo Cliente**) aponta para um NOVO array
	// Este array tera o MESMO tamanho que o array de 'outra'
	// MAS e um array DIFERENTE (memoria INDEPENDENTE)
	//
	// Visualizacao:
	//   outra.clientes -> [ptrA][ptrB][ptrC]  (array existente)
	//                       ↓     ↓     ↓
	//                    ClienteA ClienteB ClienteC
	//
	//   this->clientes -> [   ][   ][   ]  (NOVO array, ainda vazio)

	// PASSO 3: Copiar cada Cliente INDIVIDUALMENTE (deep copy)
	for (int i = 0; i < outra.numClientes; i++) {
		// Sub-passo 3.1: Criar NOVO objeto Cliente copiando dados
		clientes[i] = new Cliente(
			outra.clientes[i]->obtemNome(),
			outra.clientes[i]->obtemNIF()
		);
		// 'new Cliente(...)' cria um NOVO objeto Cliente na memoria
		// Com os MESMOS dados do Cliente original (nome, NIF)
		// MAS e um objeto DIFERENTE (endereco diferente)
		//
		// Exemplo: Se outra.clientes[0] esta em 0x1000,
		//          this->clientes[0] estara em 0x2000 (endereco diferente!)

		// Sub-passo 3.2: Copiar o numero de consultas (dado adicional)
		for (int j = 0; j < outra.clientes[i]->obtemNumConsultas(); j++) {
			clientes[i]->novaConsulta();
			// Como Cliente nao tem um setter para numConsultas,
			// chamamos novaConsulta() repetidamente para igualar o contador
		}
	}

	// Visualizacao FINAL:
	//   outra.clientes -> [ptrA][ptrB][ptrC]  (array original)
	//                       ↓     ↓     ↓
	//                    ClienteA ClienteB ClienteC
	//
	//   this->clientes -> [ptr0][ptr1][ptr2]  (NOVO array)
	//                       ↓     ↓     ↓
	//                    Cliente0 Cliente1 Cliente2 (NOVOS objetos, dados iguais!)
	//
	// IMPORTANTE: Arrays DIFERENTES, Objetos DIFERENTES, Dados IGUAIS!
	// Modificar 'this' NAO afeta 'outra' (independentes!)
}

// ============================================================================
// OPERADOR DE ATRIBUICAO (Deep Copy)
// ============================================================================
// Atribui o conteudo de um ArmarioFichas JA EXISTENTE a outro ArmarioFichas
// JA EXISTENTE, garantindo uma copia INDEPENDENTE (deep copy).
//
// DIFERENCA do Construtor por Copia:
//   - Construtor: Cria um NOVO objeto a partir de outro
//     Exemplo: ArmarioFichas b = a;  ou  ArmarioFichas b(a);
//   - Operador: Modifica um objeto que JA EXISTE
//     Exemplo: b = a;  (tanto 'b' quanto 'a' ja foram criados antes)
//
// Exemplo de uso:
//   ArmarioFichas a, b;        // Cria dois armarios vazios
//   a.acrescentarClientes(...); // Adiciona clientes em 'a'
//   b = a;                     // OPERADOR DE ATRIBUICAO! Copia 'a' para 'b'
//
// IMPORTANTE: 'b' tinha dados antigos que precisam ser LIBERTADOS primeiro!
// ============================================================================
ArmarioFichas& ArmarioFichas::operator=(const ArmarioFichas& outra) {
	// PASSO 1: Verificar auto-atribuicao (a = a)
	if (this == &outra) {
		// 'this' e um ponteiro para o objeto do LADO ESQUERDO (ex: 'b' em b=a)
		// '&outra' e o endereco do objeto do LADO DIREITO (ex: endereco de 'a' em b=a)
		// Se os enderecos forem iguais, entao e o MESMO objeto!
		//
		// Por que verificar?
		// Se fizermos: a = a;
		// Sem esta verificacao, iriamos:
		//   1) Deletar a memoria de 'a' (Passo 2)
		//   2) Tentar copiar de 'a' (mas 'a' ja foi deletado!) <- ERRO!
		//
		return *this;  // Retorna o objeto sem fazer nada
		// '*this' = objeto em si (desreferenciado)
		// 'this' = ponteiro para o objeto
	}

	// PASSO 2: Libertar TODOS os objetos Cliente do array atual
	for (int i = 0; i < numClientes; i++) {
		delete clientes[i];
		// 'clientes[i]' e um ponteiro (tipo Cliente*)
		// 'delete' liberta o OBJETO Cliente apontado
	}
	// Visualizacao:
	//   this->clientes -> [ptr0][ptr1][ptr2]
	//                       X     X     X
	//                       ✗     ✗     ✗  (objetos destruidos!)

	// PASSO 3: Libertar o ARRAY de ponteiros antigo
	delete[] clientes;
	// Liberta apenas o ARRAY em si (os ponteiros)
	// Os objetos ja foram libertados no Passo 2
	//
	// Visualizacao:
	//   this->clientes -> [LIBERTADO]  (array antigo destruido)

	// PASSO 4: Copiar o numero de clientes (valor simples)
	numClientes = outra.numClientes;

	// PASSO 5: Alocar NOVA memoria para o array de ponteiros
	clientes = new Cliente * [outra.numClientes];
	// Cria um NOVO array com o tamanho de 'outra'
	//
	// Visualizacao:
	//   outra.clientes -> [ptrA][ptrB][ptrC]  (array de 'outra')
	//                       ↓     ↓     ↓
	//                    ClienteA ClienteB ClienteC
	//
	//   this->clientes -> [   ][   ][   ]  (NOVO array vazio)

	// PASSO 6: Copiar cada Cliente INDIVIDUALMENTE (deep copy)
	for (int i = 0; i < outra.numClientes; i++) {
		// Sub-passo 6.1: Criar NOVO objeto Cliente copiando dados
		clientes[i] = new Cliente(
			outra.clientes[i]->obtemNome(),
			outra.clientes[i]->obtemNIF()
		);
		// Cria NOVO objeto com os mesmos dados

		// Sub-passo 6.2: Copiar numero de consultas
		for (int j = 0; j < outra.clientes[i]->obtemNumConsultas(); j++) {
			clientes[i]->novaConsulta();
		}
	}

	// Visualizacao FINAL:
	//   outra.clientes -> [ptrA][ptrB][ptrC]  (nao mudou)
	//                       ↓     ↓     ↓
	//                    ClienteA ClienteB ClienteC
	//
	//   this->clientes -> [ptr0][ptr1][ptr2]  (novo array)
	//                       ↓     ↓     ↓
	//                    Cliente0 Cliente1 Cliente2 (novos objetos!)

	// PASSO 7: Retornar *this (para permitir atribuicoes em cadeia)
	return *this;
	// Permite: a = b = c;
	// Execucao: (a = (b = c))
	//   1) Primeiro: b = c  (retorna 'b')
	//   2) Depois:  a = b   (usa o 'b' retornado)
	//
	// Por que '*this' e nao 'this'?
	// - 'this' = ponteiro (tipo ArmarioFichas*)
	// - '*this' = objeto em si (tipo ArmarioFichas&)
	// - O retorno e 'ArmarioFichas&', entao precisamos de '*this'
}

// Destrutor
ArmarioFichas::~ArmarioFichas() {
	// PASSO 1: Liberta cada OBJETO Cliente individualmente
	for (int i = 0; i < numClientes; i++) {
		delete clientes[i];
		// 'clientes[i]' e um PONTEIRO para Cliente (tipo Cliente*)
		// 'delete clientes[i]' liberta a MEMORIA do OBJETO Cliente apontado
		// Exemplo: Se clientes[0] aponta para endereco 0x1000,
		//          delete liberta o objeto Cliente que esta em 0x1000
	}

	// PASSO 2: Liberta o ARRAY de ponteiros
	delete[] clientes;
	// 'clientes' e um PONTEIRO para ponteiro (tipo Cliente**)
	// Aponta para um ARRAY de ponteiros (cada elemento e do tipo Cliente*)
	// 'delete[] clientes' liberta apenas o ARRAY em si, NAO os objetos
	// (os objetos ja foram libertados no Passo 1)
	//
	// Visualizacao:
	// ANTES:
	//   clientes -> [ptr0][ptr1][ptr2]  <- Array de ponteiros (sera libertado aqui)
	//                 |     |     |
	//                 v     v     v
	//             Cliente Cliente Cliente  <- Objetos (ja libertados no Passo 1)
	//
	// DEPOIS:
	//   Toda a memoria foi libertada (sem memory leaks!)
}

bool ArmarioFichas::acrescentarClientes(const std::string& nome, int nif) {
	// Verificar se ja existe cliente com o mesmo NIF
	for (int i = 0; i < numClientes; i++) {
		if (clientes[i]->obtemNIF() == nif) {
			return false;  // NIF duplicado, nao acrescenta
		}
	}

	// Criar ARRAY TEMPORARIO de ponteiros com tamanho aumentado
	Cliente** clientesTemp = new Cliente * [numClientes + 1];
	// 'clientesTemp' e um PONTEIRO para ponteiro (tipo Cliente**)
	// Aponta para um NOVO array com (numClientes + 1) posicoes
	// Cada posicao do array e do tipo Cliente* (ponteiro para Cliente)
	//
	// Visualizacao:
	//   clientesTemp -> [   ][   ][   ][   ]  <- Array com 4 posicoes (se era numClientes=3)
	//                   Cada posicao guardara um Cliente* (ponteiro para Cliente)

	// Copiar PONTEIROS para Cliente do array antigo para o novo array (clientesTemp)
	for (int i = 0; i < numClientes; i++) {
		clientesTemp[i] = clientes[i];
		// NAO estamos a copiar OBJETOS Cliente!
		// Estamos a copiar PONTEIROS (enderecos de memoria)
		// Exemplo: Se clientes[0] contem 0x1000, agora clientesTemp[0] tambem contem 0x1000
		//          Ambos apontam para o MESMO objeto Cliente
	}

	// Criar NOVO objeto Cliente na ultima posicao do array novo (clientesTemp)
	clientesTemp[numClientes] = new Cliente(nome, nif);
	// 'new Cliente(nome, nif)' cria um NOVO objeto Cliente na memoria
	// Retorna um PONTEIRO para esse objeto (tipo Cliente*)
	// Esse ponteiro e guardado em clientesTemp[numClientes]
	//
	// Visualizacao:
	//   clientesTemp -> [ptr0][ptr1][ptr2][ptrNovo]
	//                     |     |     |       |
	//                     v     v     v       v
	//                 Cliente Cliente Cliente NovoCliente <- Criado aqui!

	// Libertar o ARRAY de ponteiros para Cliente antigo
	delete[] clientes;
	// Liberta apenas o ARRAY em si (os ponteiros dentro dele)
	// NAO liberta os objetos Cliente! (eles ainda sao necessarios)
	// Os objetos Cliente continuam a existir porque clientesTemp aponta para eles
	//
	// Visualizacao:
	//   clientes -> [LIBERTADO] (array antigo foi destruido)
	//   clientesTemp -> [ptr0][ptr1][ptr2][ptrNovo]  (array novo ainda existe)
	//                     |     |     |       |
	//                     v     v     v       v
	//                 Cliente Cliente Cliente NovoCliente (objetos ainda existem!)

	// Fazer 'clientes' apontar para o array novo
	clientes = clientesTemp;
	// Agora 'clientes' aponta para o mesmo array que 'clientesTemp'
	// 'clientesTemp' e apenas uma variavel local (sera destruida no fim da funcao)
	// mas o array que ela aponta continuara a existir atraves de 'clientes'
	//
	// Visualizacao:
	//   clientes -------> [ptr0][ptr1][ptr2][ptrNovo]
	//   clientesTemp ---> (aponta para o mesmo lugar)

	// Incrementar o contador de clientes
	numClientes++;
	// Agora numClientes = 4 (se era 3 antes)

	return true;
}

bool ArmarioFichas::apagarCliente(int nif) {
	// Procurar o cliente com o NIF especificado
	for (int i = 0; i < numClientes; i++) {
		if (clientes[i]->obtemNIF() == nif) {
			// Cliente encontrado na posicao 'i'!

			// Libertar a MEMORIA do OBJETO Cliente encontrado
			delete clientes[i];
			// 'clientes[i]' e um PONTEIRO para Cliente (tipo Cliente*)
			// 'delete clientes[i]' liberta o OBJETO Cliente apontado
			// Exemplo: Se clientes[2] apontava para 0x5000, o objeto em 0x5000 e destruido
			//
			// Visualizacao:
			//   clientes -> [ptr0][ptr1][ptr2][ptr3]
			//                 ↓     ↓     X     ↓
			//             Cliente Cliente ✗ Cliente  <- Objeto em ptr2 foi destruido!

			// Preencher o "buraco" com o ULTIMO elemento (swap-and-pop)
			clientes[i] = clientes[numClientes - 1];
			// Move o PONTEIRO do ultimo elemento para a posicao do elemento apagado
			// NAO move o objeto, apenas o PONTEIRO!
			// Isto evita deixar "buracos" (nullptr) no meio do array
			//
			// Exemplo: Se i=1 e numClientes=4:
			//   clientes[1] = clientes[3];
			//   Copia o ponteiro da posicao 3 para a posicao 1
			//
			// Visualizacao ANTES:
			//   clientes -> [ptr0][ptrX][ptr2][ptr3]  <- ptrX foi libertado (X no objeto)
			//                 ↓     X     ↓     ↓
			//             Cliente  ✗  Cliente Cliente
			//
			// Visualizacao DEPOIS:
			//   clientes -> [ptr0][ptr3][ptr2][ptr3]  <- ptr3 agora aparece 2 vezes!
			//                 ↓     ↓     ↓     ↓
			//             Cliente Cliente Cliente Cliente
			//                      (copiado)      (original)

			// Diminuir o contador de clientes
			numClientes--;
			// Agora numClientes = 3 (era 4)
			// Logicamente "esconde" o ultimo elemento (que foi movido para cima)
			//
			// Visualizacao LOGICA:
			//   clientes -> [ptr0][ptr3][ptr2]|[ptr3]  <- Elemento depois de | e ignorado
			//                 ↓     ↓     ↓    |
			//             Cliente Cliente Cliente|     <- Ultimo nao e mais usado
			//                                    |
			//                      (fronteira: numClientes=3)

			// Criar ARRAY TEMPORARIO com tamanho REDUZIDO
			Cliente** clientesTemp = new Cliente * [numClientes];
			// 'clientesTemp' e um PONTEIRO para ponteiro (tipo Cliente**)
			// Aponta para um NOVO array menor: tamanho = numClientes (ja decrementado!)
			// Exemplo: Se numClientes=4 antes agora numClientes=3, cria array com 3 posicoes
			//
			// Visualizacao:
			//   clientesTemp -> [   ][   ][   ]  <- Array com 3 posicoes

			// Copiar PONTEIROS do array antigo para o novo (apenas elementos validos)
			for (int j = 0; j < numClientes; j++) {
				clientesTemp[j] = clientes[j];
				// Copia apenas os primeiros 'numClientes' ponteiros
				// NAO copia o ponteiro duplicado no final!
				// NAO copia OBJETOS, apenas PONTEIROS (enderecos)
			}
			// Visualizacao:
			//   Array ANTIGO:
			//     clientes -> [ptr0][ptr3][ptr2][ptr3]  <- 4 posicoes, ultima e duplicada
			//                   ↓     ↓     ↓     ↓
			//               Cliente Cliente Cliente Cliente
			//
			//   Array NOVO:
			//     clientesTemp -> [ptr0][ptr3][ptr2]  <- 3 posicoes, SEM duplicacao!
			//                       ↓     ↓     ↓
			//                   Cliente Cliente Cliente

			// Libertar o ARRAY de ponteiros antigo
			delete[] clientes;
			// Liberta apenas o ARRAY em si (os 4 ponteiros)
			// NAO liberta os objetos Cliente! (eles ainda sao necessarios)
			// Os objetos continuam a existir porque clientesTemp aponta para eles
			//
			// Visualizacao:
			//   clientes -> [LIBERTADO]  (array antigo destruido)
			//
			//   clientesTemp -> [ptr0][ptr3][ptr2]  (array novo ainda existe)
			//                     ↓     ↓     ↓
			//                 Cliente Cliente Cliente (objetos ainda existem!)

			// Fazer 'clientes' apontar para o array novo
			clientes = clientesTemp;
			// Agora 'clientes' aponta para o mesmo array que 'clientesTemp'
			// 'clientesTemp' e apenas uma variavel local (sera destruida no fim da funcao)
			// mas o array que ela aponta continuara a existir atraves de 'clientes'
			//
			// Visualizacao:
			//   clientes -------> [ptr0][ptr3][ptr2]
			//   clientesTemp ---> (aponta para o mesmo lugar)
			//                       ↓     ↓     ↓
			//                   Cliente Cliente Cliente

			return true;  // Cliente apagado com sucesso!

			// RESUMO DO QUE ACONTECEU:
			// - Objeto Cliente destruido (delete clientes[i])
			// - "Buraco" preenchido com ultimo elemento (swap-and-pop)
			// - Array redimensionado de 4 para 3 posicoes
			// - numClientes decrementado de 4 para 3
		}
	}

	// Cliente nao encontrado
	return false;
}

// ============================================================================
// REGISTAR CONSULTA
// ============================================================================
// Procura um cliente pelo NIF e, se encontrado, incrementa o seu contador
// de consultas.
//
// Exemplo de uso:
//   ArmarioFichas armario;
//   armario.acrescentarClientes("Joao", 123456789);
//   armario.registarConsulta(123456789);  // Incrementa consultas de Joao
//
// Parametros:
//   - nif: Numero de Identificacao Fiscal do cliente a procurar
//
// Retorno:
//   - true: Cliente encontrado, consulta registada com sucesso
//   - false: Cliente nao encontrado (NIF nao existe no armario)
// ============================================================================
bool ArmarioFichas::registarConsulta(int nif) {
	// Percorrer todos os clientes do armario
	for (int i = 0; i < numClientes; i++) {
		// Verificar se o cliente na posicao 'i' tem o NIF procurado
		if (clientes[i]->obtemNIF() == nif) {
			// Cliente encontrado!
			// 'clientes[i]' e um ponteiro (tipo Cliente*)
			// '->' e usado para aceder a metodos do objeto apontado

			// Incrementar o contador de consultas do cliente
			clientes[i]->novaConsulta();
			// Chama o metodo 'novaConsulta()' do objeto Cliente
			// Este metodo incrementa o contador interno 'numConsultas'
			//
			// Exemplo: Se o cliente tinha 5 consultas, agora tem 6

			return true;  // Sucesso! Consulta registada
		}
	}

	// Se chegou aqui, percorreu todo o array e nao encontrou o NIF
	return false;  // Cliente nao encontrado

	// Visualizacao do processo:
	//   clientes -> [Cliente0][Cliente1][Cliente2]
	//                  NIF:111   NIF:222   NIF:333
	//
	//   registarConsulta(222):
	//     - i=0: 111 != 222, continua
	//     - i=1: 222 == 222, ENCONTRADO! Incrementa consultas, retorna true
	//
	//   registarConsulta(999):
	//     - i=0: 111 != 999, continua
	//     - i=1: 222 != 999, continua
	//     - i=2: 333 != 999, continua
	//     - Fim do loop, retorna false (nao encontrado)
}

// ============================================================================
// OBTER DADOS
// ============================================================================
// Procura um cliente pelo NIF e retorna os seus dados (nome e num consultas).
// Os dados sao retornados numa estrutura InfoCliente.
//
// Parametros:
//   - nif: Numero de Identificacao Fiscal do cliente a procurar
//
// Retorno:
//   - InfoCliente com nome e numConsultas (se cliente encontrado)
//   - InfoCliente("", 0) se cliente nao encontrado (dados vazios)
//
// Exemplo de uso:
//   ArmarioFichas armario;
//   armario.acrescentarClientes("Maria", 987654321);
//   armario.registarConsulta(987654321);  // 1 consulta
//   
//   auto dados = armario.obterDados(987654321);
//   // dados.nome = "Maria", dados.numConsultas = 1
// ============================================================================
ArmarioFichas::InfoCliente ArmarioFichas::obterDados(int nif) const {
	// Percorrer todos os clientes do armario
	for (int i = 0; i < numClientes; i++) {
		// Verificar se o cliente tem o NIF procurado
		if (clientes[i]->obtemNIF() == nif) {
			// Cliente encontrado! Retornar os seus dados
			return InfoCliente(
				clientes[i]->obtemNome(),           // Nome do cliente
				clientes[i]->obtemNumConsultas()    // Numero de consultas
			);
			// InfoCliente e uma estrutura (struct) que agrupa nome e numConsultas
			// Definida dentro da classe ArmarioFichas (nested struct)
		}
	}

	// Cliente nao encontrado - retornar dados VAZIOS
	return InfoCliente("", 0);
	// Nome vazio ("") e 0 consultas indicam que o cliente nao existe
	//
	// Visualizacao:
	//   clientes -> [Cliente0][Cliente1][Cliente2]
	//                NIF:111   NIF:222   NIF:333
	//
	//   obterDados(222):
	//     - i=0: 111 != 222, continua
	//     - i=1: 222 == 222, ENCONTRADO! Retorna InfoCliente("Maria", 3)
	//
	//   obterDados(999):
	//     - Nao encontrado, retorna InfoCliente("", 0)
}

// ============================================================================
// ESVAZIAR
// ============================================================================
// Remove TODOS os clientes do armario, libertando TODA a memoria dinamica.
// Deixa o armario no estado INICIAL (vazio), como se tivesse acabado de ser
// criado pelo construtor default.
//
// ATENCAO: Esta operacao e IRREVERSIVEL! Todos os dados sao perdidos.
//
// Exemplo de uso:
//   ArmarioFichas armario;
//   armario.acrescentarClientes("Joao", 111);
//   armario.acrescentarClientes("Maria", 222);
//   // armario tem 2 clientes
//   
//   armario.esvaziar();
//   // armario agora esta VAZIO (0 clientes)
// ============================================================================
void ArmarioFichas::esvaziar() {
	// PASSO 1: Libertar cada OBJETO Cliente individualmente
	for (int i = 0; i < numClientes; i++) {
		delete clientes[i];
		// Liberta a memoria do objeto Cliente apontado por clientes[i]
		// Exemplo: Se clientes[0] aponta para 0x1000, liberta o objeto em 0x1000
	}
	// Visualizacao:
	//   clientes -> [ptr0][ptr1][ptr2]
	//                 X     X     X
	//                 ✗     ✗     ✗  (objetos destruidos)

	// PASSO 2: Libertar o ARRAY de ponteiros
	delete[] clientes;
	// Liberta o array em si (os ponteiros)
	// Os objetos ja foram libertados no Passo 1
	//
	// Visualizacao:
	//   clientes -> [LIBERTADO]  (array destruido)

	// PASSO 3: Reinicializar para o estado INICIAL (vazio)
	numClientes = 0;
	// Contador volta a zero

	clientes = nullptr;
	// IMPORTANTE! Define o ponteiro como nullptr para evitar dangling pointer
	// Sem isto, 'clientes' apontaria para memoria ja libertada (perigoso!)
	//
	// Estado FINAL:
	//   numClientes = 0
	//   clientes = nullptr
	//   (equivalente ao estado apos construtor default)
}

// ============================================================================
// LISTAGEM
// ============================================================================
// Retorna uma string com a descricao completa de TODOS os clientes do armario.
// Cada cliente aparece numa linha separada.
// Usa o metodo obtemDesc() da classe Cliente para obter a descricao.
//
// Retorno:
//   - String com a listagem completa (uma linha por cliente)
//   - String VAZIA se o armario nao tiver clientes
//
// Exemplo de uso:
//   ArmarioFichas armario;
//   armario.acrescentarClientes("Joao", 111);
//   armario.acrescentarClientes("Maria", 222);
//   armario.registarConsulta(111);
//   
//   cout << armario.listagem();
//   // Saida:
//   // Joao / 111 / 1
//   // Maria / 222 / 0
// ============================================================================
std::string ArmarioFichas::listagem() const {
	// Criar um ostringstream para construir a string de forma eficiente
	std::ostringstream oss;
	// ostringstream permite "acumular" texto como se fosse um cout,
	// mas em vez de imprimir no ecra, guarda numa string

	// Percorrer todos os clientes do armario
	for (int i = 0; i < numClientes; i++) {
		// Adicionar a descricao do cliente 'i' ao stream
		oss << clientes[i]->obtemDesc() << std::endl;
		// obtemDesc() retorna: "nome / nif / numConsultas"
		// std::endl adiciona uma quebra de linha
		//
		// Exemplo: Se cliente[0] e "Joao / 111 / 1",
		//          oss contem: "Joao / 111 / 1\n"
	}

	// Converter o ostringstream para string e retornar
	return oss.str();
	// str() extrai a string completa do ostringstream
	//
	// Visualizacao do processo:
	//   clientes -> [Cliente0][Cliente1][Cliente2]
	//                "Joao/111" "Maria/222" "Pedro/333"
	//
	//   Loop:
	//     i=0: oss += "Joao / 111 / 1\n"
	//     i=1: oss += "Maria / 222 / 0\n"
	//     i=2: oss += "Pedro / 333 / 2\n"
	//
	//   Retorno: "Joao / 111 / 1\nMaria / 222 / 0\nPedro / 333 / 2\n"
	//
	// Se armario vazio (numClientes=0):
	//   Loop nao executa, oss fica vazio
	//   Retorna: "" (string vazia)
}