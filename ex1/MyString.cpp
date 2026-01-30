#include "MyString.h"

MyString::MyString(const char* str) {
	string = new char[strlen(str) + 1];
	strcpy(string, str);
}

// Construtor por Copia 
MyString::MyString(const MyString& outra) {
	// Exemplo: MyString a("Ola"); MyString b(a);
	// Dentro deste construtor:
	//   - 'this' aponta para o objeto NOVO que esta sendo criado ('b')
	//   - 'outra' e uma referencia para o objeto JA EXISTENTE ('a')
	//   - 'string' (ou 'this->string') e o campo do objeto NOVO ('b')
	//   - 'outra.string' e o campo do objeto JA EXISTENTE ('a')

	// Aloca memoria para o campo 'string' do objeto NOVO (b)
	// usando o tamanho da string do objeto JA EXISTENTE (a)
	string = new char[strlen(outra.string) + 1];
	// Equivalente a: this->string = new char[strlen(outra.string) + 1];

	// Copia o CONTEUDO de 'a.string' para 'b.string'
	strcpy(string, outra.string);
	// Equivalente a: strcpy(this->string, outra.string);

	/*
	ANTES do construtor:
	a.string ->[O][l][a][\0](0x1000) - JA EXISTE
	b.string -> ? ? ? -AINDA NAO EXISTE(esta sendo criado)

	DENTRO do construtor por copia :
	'this' = &b(ponteiro para b)
	'outra' = a(referencia para a)
	string = new char[4];  // Aloca para 'b.string' (o NOVO)
	b.string ->[](0x2000) - NOVA memoria alocada
	a.string ->[O][l][a][\0](0x1000) - nao mudou

	DEPOIS de strcpy :
	b.string ->[O][l][a][\0](0x2000) - objeto NOVO com copia
	a.string ->[O][l][a][\0](0x1000) - objeto original(nao mudou)
	*/
}

MyString& MyString::operator=(const MyString& outra) {
	// Verifica auto-atribuicao (a = a)
	// 'this' e um ponteiro para o objeto do lado ESQUERDO (ex: 'b' em b=a)
	// '&outra' e o endereco do objeto do lado DIREITO (ex: endereco de 'a' em b=a)
	// Se os enderecos forem iguais, entao e o MESMO objeto (a = a)
	if (this == &outra) {
		return *this; 
		// Retorna o objeto em si (*this) por isso temos que desreferenciar, e nao o endereco (this)
		// Se retornassemos 'this' (sem *), estariamos a retornar um endereço do objeto, e não é isso que queremos
	}
	delete[] string; 
	//Liberta a memoria antiga do 'this' (lado esquerdo)
	// Libertamos 'string' (que pertence a 'this'), NAO 'outra.string'!
	// Se libertassemos 'outra.string' (lado direito), perderiamos os dados a copiar.
	// Sem este delete[] haveria MEMORY LEAK (memoria antiga ficaria perdida).
	string = new char[strlen(outra.string) + 1]; 
	// Aloca NOVA memoria para 'this' (lado esquerdo)
	// Aloca com o tamanho da string de 'outra' (lado direito) + '\0'
	// porque vamos copiar os dados de 'outra' para 'this'
	strcpy(string, outra.string); 
	// Copia o CONTEUDO de 'outra' (direito) para 'this' (esquerdo)
	// strcpy(destino, origem) || strcpy(this->string, outra.string)
	// Copia de 'outra' (lado direito/origem) para 'string' (lado esquerdo/destino)
	return *this; 
	// Retorna* this para permitir atribuicoes em cadeia(a = b = c)
	// Retorna o objeto em si (*this) por isso temos que desreferenciar, e nao o endereco (this)
	// Se retornassemos 'this' (sem *), estariamos a retornar um endereço do objeto, e não é isso que queremos
}

void MyString::acrescenta(const char* str) {
	// MyString a("Ola"); a.acrescenta("Mundo"); 
	// Resultado esperado: "OlaMundo"
	// Calcular o tamanho total necessario que é a soma entre a (string atual + string a acrescentar)
	int tamanho = strlen(str) + strlen(string);

	// Aloca NOVA memoria temporária para armazenar a string concatenada (tamanho + '\0'), em C++ não existe realloc, logo temos que criar uma nova memória temporária para armazenar a string concatenada
	char* novaString = new char[tamanho + 1];

	// Copia a string ATUAL ("Ola") para a nova memoria
	strcpy(novaString, string);  // novaString contém agora "Ola"

	// Concatena a novaString que contém "Olá" com a str que contém "Mundo"
	strcat(novaString, str);     // novaString = "OlaMundo"

	// 1) Liberta a memoria ANTIGA de 'string', se não o conteúdo fica perdido para sempre logo memory leak
	delete[] string;

	// 2) Faz 'string' apontar para a novaString
	string = novaString;

	// NAO libertar 'novaString' aqui, pois 'string' esta a usar a memória novaString.

	/*
	Visualizacao:

	ANTES:
	  string -> [O][l][a][\0] (0x1000)
	  str = "Mundo"

	DURANTE:
	  string -> [O][l][a][\0] (0x1000) - memoria antiga
	  novaString -> [O][l][a][M][u][n][d][o][\0] (0x2000) - memoria nova

	DEPOIS:
	  (0x1000) foi libertado
	  string -> [O][l][a][M][u][n][d][o][\0] (0x2000) - aponta para a nova memoria
	*/
}

bool MyString::mudaCharAt(int indice, char c) {
	// Obtem o tamanho da string (sem contar o '\0')
	int tamanho = strlen(string);

	// Validamos se o indice esta dentro dos limites [0, tamanho-1]
	if (indice >= 0 && indice < tamanho) {
		string[indice] = c;
		return true;
	}
	return false;
}

MyString::~MyString() {
	delete[] string;
}
