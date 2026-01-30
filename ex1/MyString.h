#pragma once
#include <cstring>

class MyString
{
	char* string;

public:
	//Construtor com parâmetro por omissão
	MyString(const char* str = "");

	//Construtor por Cópia
	// Construtor por Copia (Deep Copy)
	// ----------------------------------
	// Cria uma copia independente de outro objeto MyString.
	// 
	// PROBLEMA: Sem este construtor, o compilador faz uma copia superficial
	// (shallow copy), copiando apenas o ponteiro 'string'. Isso faz com que
	// ambos os objetos apontem para a MESMA memoria dinamica, causando:
	//   1) Destruir um objeto invalida o ponteiro do outro (dangling pointer)
	//   2) Alterar um objeto afeta o outro
	//   3) Double-delete quando ambos forem destruidos (crash!)
	//
	// SOLUCAO: Este construtor aloca NOVA memoria e copia o CONTEUDO da
	// string, garantindo que cada objeto tem a sua propria memoria independente.
	//
	// Parametro: outro - objeto MyString a ser copiado (passado por const ref)
	// MyString a ("Ola"); 
	// MyString b(a) || MyString b = a; chama aqui o Construtor por Cópia 
	// const MyString& outro = a; -> Isso é feito pelo Compilador

	MyString(const MyString& outro);

	// Operador de Atribuicao (Deep Copy)
	// -----------------------------------
	// Atribui o conteudo de um objeto MyString JA EXISTENTE a outro objeto
	// JA EXISTENTE, garantindo uma copia independente.
	// MyString a ("Ola"), b ("Mundo");
	// b = a;
	//
	// QUEM E QUEM na expressao: b = a;
	//   - 'this'  = ponteiro para o objeto 'b' (LADO ESQUERDO do =)
	//               e o objeto que RECEBE a atribuicao e sera MODIFICADO
	//   - 'outro' = referencia para o objeto 'a' (LADO DIREITO do =)
	//               e o objeto que vai ser COPIADO (nao e modificado)
	//
	// DIFERENCA do Construtor por Copia:
	//   - Construtor por Copia: cria um NOVO objeto a partir de outro
	//     Exemplo: MyString b = a; ou MyString b(a);
	//   - Operador de Atribuicao: altera um objeto que JA EXISTE
	//     Exemplo: b = a; (ambos 'b' e 'a' ja foram criados antes)
	//
	// PROBLEMA: Sem este operador, o compilador faz uma copia superficial
	// (shallow copy), copiando apenas o ponteiro 'string'. Isso causa:
	//   1) MEMORY LEAK! A memoria antiga de 'this' (lado esquerdo 'b') 
	//      nunca e libertada. Exemplo: em b=a, "Mundo" fica perdido na memoria!
	//   2) Ambos os objetos apontam para a MESMA memoria (shallow copy),
	//      ou seja, 'b' passa a apontar para a string de 'a' (lado direito).
	//      Agora b.string e a.string apontam para [O][l][a][\0] (mesmo endereco!)
	//   3) Destruir um objeto invalida o ponteiro do outro (dangling pointer)
	//   4) Double-delete quando ambos forem destruidos (crash!)
	//
	// SOLUCAO: Este operador realiza os seguintes passos:
	//   1) Verifica auto-atribuicao (a = a) para evitar problemas
	//   2) LIBERTA a memoria dinamica antiga de 'this' (lado esquerdo) - b
	//   3) Aloca NOVA memoria para 'this' (lado esquerdo) - b
	//   4) Copia o CONTEUDO da string de 'outro' (lado direito) - a
	//   5) Retorna *this para permitir atribuicoes em cadeia (a = b = c)
	//
	// Exemplo detalhado:
	//   MyString a("Ola");      // a.string -> [O][l][a][\0]  (endereco 0x1000)
	//   MyString b("Mundo");    // b.string -> [M][u][n][d][o][\0] (endereco 0x2000)
	//   
	//   // COM operador padrao (ERRADO - shallow copy):
	//   b = a;  // b.string = a.string (copia o ponteiro!)
	//           // ANTES: b.string = 0x2000, a.string = 0x1000
	//           // DEPOIS: b.string = 0x1000, a.string = 0x1000 (MESMO ENDERECO!)
	//           // "Mundo" (0x2000) perdido na memoria! (MEMORY LEAK)
	//           // Ambos apontam para [O][l][a][\0] (0x1000)
	//
	//   // COM este operador customizado (CORRETO - deep copy):
	//   b = a;  // 1) Liberta "Mundo" (0x2000) - b ou 'this'
	//           // 2) Aloca nova memoria (0x3000) para 'this' ou b
	//           // 3) Copia "Ola" para (0x3000) - strcpy(this->string, outro.string) ou seja, copia o conteudo de 'a' para 'b'
	//			 //	   Ou se preferir strcpy(b,a) -> 'b' é o destino e 'a' a origem
	//           // Resultado: b.string = 0x3000 -> [O][l][a][\0]
	//           //            a.string = 0x1000 -> [O][l][a][\0]
	//           //            (enderecos DIFERENTES!)
	//
	// RETORNO: MyString& (referencia para MyString)
	// ----------------------------------------------
	// Por que retornar 'MyString&' e nao 'void'?
	//   - Para permitir atribuicoes em cadeia: a = b = c;
	//   
	// Por que retornar '*this' e nao 'this'?
	//   - 'this' e um PONTEIRO (MyString*) que aponta para o objeto 'b'
	//     Exemplo: Em b=a, 'this' contem o endereco de 'b' (ex: 0x2000)
	//   - '*this' e o OBJETO em si (MyString) - desreferencia o ponteiro
	//     Exemplo: Em b=a, '*this' e o proprio objeto 'b'
	//   - Retornamos '*this' para devolver o objeto 'b' (lado esquerdo)
	//
	// Exemplo de retorno:
	//   MyString a("A"), b("B"), c("C");
	//   a = b = c;  // Como funciona:
	//               // 1) Primeiro: b = c  (retorna 'b' via *this)
	//               // 2) Depois:  a = b   (usa o 'b' retornado)
	//               // Resultado: a, b, c todos com "C"
	//
	// Resumo: return *this; devolve o objeto do lado esquerdo (b) por referencia
	MyString& operator=(const MyString& outro);

	//Acrescenta
	void acrescenta(const char* str);

	//Mudar Carácter
	bool mudaCharAt(int indice, char c);

	//Obter o Tamanho
	int getTamanho() const { return strlen(string); }

	//Obter o CString
	const char* obtemCString() const { return string; }

	//Destrutor
	~MyString();
};

