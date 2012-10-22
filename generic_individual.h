#ifndef _IND_OBJ_
#define _IND_OBJ_

#include "problem_info.h"

/**
 * Esta estrutura é uma representação genérica
 * de um indivíduo. Nela é possível descrever a
 * representação de um indivíduo para um problema
 * específico ou importar uma implementação definida
 * em outra arquivo de cabeçalho.
 *
 */
typedef struct individual_t {
	
	~individual_t () {
		if (obj != NULL)
			delete obj;
	}

	/**
	 * Construtor padrão da Classe. Este construtor é utilizado
	 * para definir objetos nulos. O parâmetro é definido apenas
	 * para diferenciar do construtor que criar indivíduos para
	 * problema definidos pelo usuário.
	 */
	individual_t (bool);

	/**
	 * Construtor utilizado para criar indivíduos através de um mecanismo
	 * fornecido pelo usuário. O mecanismo é dependende do problema e deve
	 * ser implementado no escopo do construtor ou incluido através de um
	 * arquivo de cabeçalho.
	 */
	individual_t ();

	/**
	 * Esta função é utilizada para associar o indivíduo passado como
	 * parâmetro ao indivíduo que invoca a função. Este procedimento
	 * é util quando a necessidade de trocar indivíduos de posição no
	 * vetor que armazena a população.
	 *
	 * As informaçãos de individual_t são atribuídas ao objeto chamador.
	 *
	 * Retorna verdadeiro caso a atribuição seja realizada com exito.
	 *
	 * @param individual_
	 * @return bool
	 */
	bool assign (individual_t *); 

	/**
	 * Usado para armazenar a posicao do indivíduo no vetor que
	 * representa a população
	 */
	int index;
	/**
	 * No Spea2 é usaudo como fitness. No Nsga2 é usado como rank.
	 */
	double fitness;

	/**
	 * Usado no Nsga2
	 */
	double crownding;


	double *obj;

	/**
	 * Objeto incluido na implementação do indivíduo genérico, este objeto
	 * indica que o indivíduo a ser utilizada é referente ao Multicast
	 * packing problem.
	 */
	MulticastIndividual mpp_ind;
	
		
} GenericIndividual;

GenericIndividual::individual_t (bool val) {

	index = 0;
	fitness = 0.0;
	crownding = 0.0;
	obj = new double[Info::OBJECTIVES];
	for (int i=0; i < Info::OBJECTIVES; i++) {
		obj[i] = 0;
	}

	this->mpp_ind = MulticastIndividual (2,Info::mproblem->getNumberGroups(),Info::mproblem);

}

GenericIndividual::individual_t () 
{
	fitness = 0.0;
	index = 0;
	crownding = 0.0;

	/**
	* Inicie a configuração de seu objeto aqui
	*/


	//obj[0] = <seu individuo>.get Objective (i)
	//obj[1] = <seu individuo>.get Objective (i)
}

bool GenericIndividual::assign (individual_t * ind)
{	
	//verifica se o objeto já está no arquivo
	if (ind->obj[0] == obj[0] && ind->obj[1] == obj[1]) return false;


	fitness = ind->fitness;
	index = ind->index;

	/**
	* ponha aqui alguma atribuição para o objeto que está
	* recebendo o ind.
	*/


	obj[0] = mpp_ind.getObjective (0);
	obj[1] = mpp_ind.getObjective (1);


	return true;
}

/**
 * Função para comparação de dois indivíduos pelo fitness.
 * Está função é util quando há necessidade de ordenar
 * os indivíduos de uma população através do fintess do mesmo.
 *
 * Nesta função os objetos são passados como ponteiros.
 *
 * @param GenericIndividual
 * @param GenericIndividual
 * @return bool
 */
bool compareByFitness(const GenericIndividual *ind1,
					 const GenericIndividual *ind2)
{
	return ind1->fitness < ind2->fitness;
}

/**
* Função para comparação de indivíduos por objetivo.
* Esta função é utilizada no Spea2 para cálculo da
* crowding distance.
* Pode se utiliza está função como objeto do algoritmo
* sort, por exemplo.
*/
struct compareByObjective {
	compareByObjective(int __obj) :objective(__obj) {	}

	bool operator() (GenericIndividual * ind1, GenericIndividual * ind2) {
		return ind1->obj[objective] < ind2->obj[objective];
	}
	int objective;
};

#endif
