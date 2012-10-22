#ifndef _MY_INDIVIDUAL_
#define _MY_INDIVIDUAL_

#include <iostream>
#include <fstream>


//ADICICIONE OS CABEÇALHOS DO SEU PROBLEMA

/*
#include "../network/reader.h"
#include "../network/group.h"
#include "../network/network.h"
#include "../individual/multicastindividual.h"
#include "../individual/mpackingoperator.h"
#include "../algorithms/util.h"
*/

using namespace rca;

namespace Info {

/*
* DEFINA OUTROS ELEMENTOS DO SEU PROBLEMA.
* NO MEU CASO, OBJETOS UTILIZADAS AO LONGO DO PROCESSO DE
* EXECUÇÃO E SÓ PRECISA SER CRIADO UMA VEZ FOI COLOCADO AQUI
* POR EXEMPLO, O OBJETO QUE REPRESENTA A MINHA REDE FICA AQUI
* SENDO ACESSÍVEL DURANTE O PROCESSO DE EXECUÇÃO.
*/
int * objconf; //indica como manipular os objetivos
int OBJECTIVES;

/**
 * Classe que armazena infomações sobre o problema.
 * Esta classe é utilizada para guardar informações de
 * um problema estudado. Tais informações incluem
 * a instância a ser testada, como também o número de
 * objetivos.
 */
class ProblemInfo {
	
public:

	/**
	 * Construtor da classe recebe duas strings representado
	 * nomes de arquivos de texto.
	 *
	 * O primeiro representa a instância do problema.
	 * O segundo representa um arquivo que indica a configuração
	 * dos objetivos.
	 *
	 * O segundo arquivo é independente de problema, pois indica
	 * o número de objetivos e se os mesmos são max/min.
	 * O formato do arquivo é:
	 *
	 * 						1 1 -1
	 *
	 * Um arquivo de configuração de objetivos definido desta maneira
	 * indica que há três objetivos e que os dois primeiros são de
	 * minimização e o último é maximização.
	 *
	 * @param %string file_instance
	 * @param %string objectives_configuration
	 * @author Romerito Campos
	 * @date 10/10/2012
	 */
	ProblemInfo (std::string file, std::string obj_conf);

	/**
	 * Destrói objetos alocados por pelo construtor da classe.
	 * Os objetos deletados são objetos globais necessários ao
	 * problema estudado.
	 */
	~ProblemInfo () {
		//delete Info::network; network = NULL;
		//delete Info::mproblem; mproblem = NULL;
		//delete Info::objconf; objconf = NULL;
	}

	/**
	 * Imprime a configuração dos objetivos.
	 */
	void printObjectives ();
	
private:
	void readerObj (std::string &);
};

ProblemInfo::ProblemInfo (std::string file, std::string obj_conf) {


//Configurando os objetos que utilizo como globais
/*
	Reader reader(file);
	network = new Network;
	reader.configNetwork(network);
	mproblem = new MulticastPacking(network);
	reader.configProblem(mproblem);
	readerObj (obj_conf);
*/
	
}


/**
* Esta função lê as configurações dos objetivos
* usando um arquivo de texto passado pelo usuário.
* Tal arquivo deve conter uma sequência de números 1,
* podendo ser negativo ou positivo. 
*
* Se for 1 indica que objetivo é minimizar, se for -1
* indica que é maximizar.
*
* Esta informação é utiliza na avaliação de dominância
* onde todos os objetivos são tratados como sendo de 
* minimização.
*/
void ProblemInfo::readerObj (std::string & obj_conf) {
	
	std::ifstream file_(obj_conf.c_str(), ifstream::in);	
	
	if (file_.fail()) exit (1);
	
	std::string str;
	int count = 0;
	file_ >> str;
	do {		
		count++;
		file_ >> str;
	} while (!file_.eof ());
		
	
	file_.close ();
	file_.open (obj_conf.c_str(), ifstream::in);
	objconf = new int[count];
	OBJECTIVES = count;
	for (int i=0; i < count; i++) {
		file_ >> str;
		objconf[i] = atoi(str.c_str());
	}
}

void ProblemInfo::printObjectives () {
	
	printf("Número de Objetivos: %d \n",OBJECTIVES);
	for (int i=0; i < OBJECTIVES; i++) {
		if (objconf[i] == 1)	printf("%s ","Min");
			else printf("%s ","Max");
	}
	printf("\n");
}

}

#endif
