#ifndef NSGA2_H_
#define NSGA2_H_

#include "problem_info.h"
#include "generic_individual.h"
#include "multiobjective.h"

#include <limits>

/**
 * Esta estrutura representa um front.
 * Possui quatro atributos: index (rótulo) da fronteira;
 * contador de indivíduos na fronteira; início (index) e fim
 * da fronteira no vetor da população.
 *
 */
struct front {

	/**
	 * Construtor de front.
	 *
	 * @param int index (rótulo)
	 * @param int counter (contador de indivíduos)
	 * @param int begin (posição inicial da fronteira no vetor da população)
	 * @parma int end (posição final da fronteira no vetor da população)
	 */
	front (int i, int c, int b, int e = 0)
		: index(i),	counter(c), begin(b), end(e) {}

	int index;
	int counter;
	int begin;
	int end;

};

/**
 * Função que compara dois indivíduos utilizando o operador
 * Crowded-Compararison Operator que é definido no artigo que
 * propõe o algoritmo.
 *
 * Este operador é utilizado para ordenar o último front que vai entrar
 * na população, se ouver mais indivíduos no front que vagas na população.
 */
bool compareByCrownding (GenericIndividual * ind1, GenericIndividual * ind2){

	return ind1->fitness < ind2->fitness ||
			(ind1->fitness == ind2->fitness && ind1->crownding > ind2->crownding);
}

/**
 * Classe que implementa o Non-Dominated Sort Genetic Algoritmo 2.
 * Esta implementação é baseada no artigo de Deb et al (2002).
 */
class Nsga2 {

public:
	Nsga2 (int popsize = 10, int max_gen = 100,
			double p_cross = 0.5, double p_mut = 0.5);

	~Nsga2 ();

	void run ();
	void recombination ();
	void printPop ();
	void printPopAsPisa ();

	void printArc (std::fstream &file);


private:
	/**
	 * Torneio binário utilizada para escolha de indivíduos
	 * a serem utilizados em operadores de recombinação.
	 */
	int binary_tournament ();

	/**
	 * Procedimento para separação de indivíduos em fronteiras
	 * de indivíduos não domiandos como definido no artigo que
	 * propõe o algoritmo.
	 *
	 * O mecanismo utilizado aqui é diferente do que é proposto
	 * no artigo, em um único loop com outro loop interno
	 * todos os indivíduos tem seus ranks definidos.
	 *
	 * Em seguida a população é ordenada pelo rank. O que garante
	 * a ordenação pelo rank como definido no artigo.
	 */
	void fast_nom_dominated_sort ();

	/**
	 * Método utilizado para criar os fronts. Este método tem complexidade
	 * O(N). Ele cria os fronts com as seguintes informações:
	 * rótulo, quantidade de indivíduos, início e fim do front.
	 *
	 * O rótulo de um front funciona da seguinte forma. O primeiro front
	 * recebe rótulo 0, o segundo front recebe rótulo 1, e assim segue.
	 *
	 * O início e fim do front indica os índices no vetor da população
	 * onde o front inicia e termina.
	 */
	void create_fronts ();

	/**
	 * Método utilizado para cálcular a crownding_distance dos indivíduos de
	 * todos os fronts. O mecanismo de cálculo da crownding_distance foi
	 * implementado como definido no artigo.
	 *
	 * Este método recebe dois parâmetros:
	 * o primeiro parâmetro indica início do front no vetor que quarda a população
	 * e o segundo indica fim do front.
	 *
	 * @param int
	 * @param int
	 */
	void crownding_distance (int,int);

	/**
	 * Método utilizado para inicializar a população
	 * O usuário da classe deve definir a forma de criação de individuos
	 * em GenericIndividual.
	 * @see GenericIndividual
	 */
	void initialization ();

	/**
	 * Método utilizado para escolher os indivíduos da geração (i + 1).
	 * O método consiste em adicionar os fronts iniciado do front de indivíduos
	 * não dominados da população (rótulo 0) até que a população de N indivíduos
	 * esteja formada.
	 *
	 * O procedimento usa a crownding_distance no último front caso o mesmo possua
	 * mais indivíduos que a população suporte como é indicado no artigo.
	 */
	void nextPopulation ();

private:
	int m_popsize;
	int m_max_gen;
	int gen;
	int m_curr_popsize;
	double m_prob_cross;
	double m_prob_mut;
	GenericIndividual **m_population;

	std::vector<front> fronts;

};

Nsga2::Nsga2(int popsize, int max_gen, double p_cross, double p_mut)
	: m_popsize(popsize), m_max_gen(max_gen),
	  m_prob_cross(p_cross), m_prob_mut (p_mut)
{
	gen = 1;
	m_curr_popsize = m_popsize;
	m_population = new GenericIndividual*[ 2 * m_popsize ];

}

Nsga2::~Nsga2() {

	for (int i=0; i < 2 * m_popsize; ++i) {
		delete m_population[i];
	}
	delete [] m_population;

}

void Nsga2::run() {

#ifdef DEBUG
	printf ("\nFunction: %s\n",__PRETTY_FUNCTION__);
#endif

	initialization();
	recombination();

	do {

		fast_nom_dominated_sort();
		nextPopulation();
		recombination();

	} while (++gen <= m_max_gen);

}

void Nsga2::fast_nom_dominated_sort() {

#ifdef DEBUG
	printf ("\nFunction: %s\n",__PRETTY_FUNCTION__);
#endif

	for (int i = 0; i < (2 * m_popsize); ++i) {

		//associação de crownding distance igual a 0
		//sempre realizada antes do cálculo de crowndig
		m_population[i]->crownding = 0.0;
		m_population[i]->fitness = 0.0;

		for (int j = 0; j < i; ++j) {
			if (i == j) continue;

			if (MultiObjective::dominate(
					m_population[i]->obj,
					m_population[j]->obj) )
			{
				m_population[j]->fitness++;

			} else if (MultiObjective::dominate(
					m_population[j]->obj,
					m_population[i]->obj) ) {

				m_population[i]->fitness++;
			}
		}
	}

	std::sort (m_population,m_population + (2 * m_popsize),compareByFitness);

#ifdef DEBUG
	printPop ();
#endif

	create_fronts();



}

void Nsga2::create_fronts() {

#ifdef DEBUG
	printf ("\nFunction: %s\n",__PRETTY_FUNCTION__);
#endif

	fronts.clear();

	int f = 0;
	fronts.push_back(front(f,1,f));
	int rank = 0;

	for (int i = 1; i < (2 * m_popsize); ++i) {

		if (rank != m_population[i]->fitness) {
			f++;

			rank = m_population[i]->fitness;

			fronts.push_back( front(f,1,i) ); //cria novo front
			fronts[f-1].end = i-1; //adiciona fim de front anterior
		} else {
			fronts[f].counter++;
		}
	}

	fronts[f].end = (2 * m_popsize) - 1;

#ifdef DEBUG
	cout << "Fronts" << endl;
	for (unsigned i = 0; i < fronts.size(); ++i) {
		cout << fronts[i].index << " ";
		cout << fronts[i].counter << " ";
		cout << "Begin: " << fronts[i].begin;
		cout << " End: " << fronts[i].end << endl;
	}
#endif

}


void Nsga2::crownding_distance(int begin, int end) {
#ifdef DEBUG
	printf ("\nFunction: %s\n",__PRETTY_FUNCTION__);
#endif


	for (int objective = 0; objective < Info::OBJECTIVES; ++objective) {

		//usa função que compara por objetivo
		//end + 1 indica o fim da front
		std::sort (m_population + begin, m_population + (end+1), compareByObjective( objective ) );

		double max = m_population[ end ]->obj[ objective ];
		double min = m_population[ begin ]->obj[ objective ];

		//m_population[ begin ]->crownding = numeric_limits<double>::max ();
		m_population[ begin ]->crownding = 100000;
		m_population[ end ]->crownding = m_population[ begin ]->crownding;

		double denom = max - min;

		for (int i = (begin + 1); i < end; ++i) {
			m_population[i]->crownding += ( m_population[ i+1 ]->obj[ objective ] - m_population[ i-1 ]->obj[ objective ] ) / denom;
		}
	}

#ifdef DEBUG

	cout << "Crownding Cal\n";
	for (int i=begin; i <= end; ++i) {
		cout << "Idx: " << m_population[i]->index;
		cout << " Crownding: " << m_population[i]->crownding << endl;
	}

#endif

}

//usa o crownding distance se necessário
void Nsga2::nextPopulation() {
#ifdef DEBUG
	printf ("\nFunction: %s\n",__PRETTY_FUNCTION__);
#endif

	int size = 0;
	int f = 0;
	bool needCrownding = false;
	while (size < m_popsize) {

		crownding_distance(fronts[f].begin,fronts[f].end);

		if (size + fronts[f].counter > m_popsize) {
			needCrownding = true;
			break;
		}
		else if (size + fronts[f].counter == m_popsize){
			break;
		}

		size += fronts[f].counter;
		f++;
	}

	if (needCrownding) {

#ifdef DEBUG
		cout << "Aplicar crownding distante ao front: " << f << endl;
		cout << "Population size: " << size << endl;
		cout << "It needs of " << m_popsize - size << " elements\n";
#endif

		/* Ordena o font pela crowding_distance, isso garante a escolha dos indivíduos
		 * corretos para entrar na população */
		std::sort (m_population + fronts[f].begin,
				m_population + (fronts[f].end + 1), compareByCrownding);

	}

#ifdef DEBUG
	printPop();
#endif

}


void Nsga2::initialization() {

#ifdef DEBUG
	printf ("\nFunction: %s\n",__PRETTY_FUNCTION__);
#endif

	for (int var = 0; var < m_popsize; ++var) {
		m_population[var] = new GenericIndividual;
		m_population[var]->index = var;
	}

	for (int var = m_popsize; var < 2*m_popsize; ++var) {
		m_population[var] = new GenericIndividual(false);
		m_population[var]->index = var;
	}

}

void Nsga2::recombination() {

#ifdef DEBUG
	printf ("\nFunction: %s\n",__PRETTY_FUNCTION__);
#endif


	for (int i=0; i < m_popsize; i++) {

		GenericIndividual * ind = new GenericIndividual(false);

		int _p1 = binary_tournament();
		int _p2 = binary_tournament();
		while (_p1 == _p2) _p2 = binary_tournament();

		GenericIndividual * p1 = m_population[_p1];
		GenericIndividual * p2 = m_population[_p2];

		int prob_cross = rand () % 10 + 1;


		if ( ((double)prob_cross/10) <= m_prob_cross ) {
		//do something if prob_cross is less or equal than m_prob_cross
			

		} else {
		//do something else
			
		}

		ind->index = i + m_popsize;
		ind->obj[0] = ind->mpp_ind.getObjective(0);
		ind->obj[1] = ind->mpp_ind.getObjective(1);

		m_population[i + m_popsize]->assign(ind);

		delete ind;
	}

}

int Nsga2::binary_tournament() {

	int ind1 = rand () % (m_popsize);
	int ind2 = rand () % (m_popsize);
	while (ind1 == ind2) ind2 = rand () % (m_popsize);

	return (m_population[ind1]->fitness < m_population[ind2]->fitness ? ind1 : ind2);

}

void Nsga2::printPop() {
	printf("Current Population\n");
	for (int i=0; i < (2 * m_popsize); i++) {
		printf ("Index: %d  fitness: %f crownding: %.2f \t",
				m_population[i]->index,
				m_population[i]->fitness,
				m_population[i]->crownding);
		double obj1 = m_population[i]->mpp_ind.getObjective(0);
		double obj2 = m_population[i]->mpp_ind.getObjective(1);
		printf ("%f %f\n",obj1, obj2);
	}

}

void Nsga2::printPopAsPisa () {

	for (int i=0; i < (m_popsize); i++) {
		double obj1 = m_population[i]->mpp_ind.getObjective(0);
		double obj2 = m_population[i]->mpp_ind.getObjective(1);
		printf ("%f %f\n",obj1, obj2);
	}
	printf ("\n");
}

//print only non-dominated individuals do a file
void Nsga2::printArc(std::fstream& file) {

	for (int i=0; i < (m_popsize); i++) {

		if ((int)m_population[i]->fitness < 1) {
			double obj1 = m_population[i]->mpp_ind.getObjective(0);
			double obj2 = m_population[i]->mpp_ind.getObjective(1);
			file << obj1 << " " << obj2 << endl;
		}
	}
	file << endl;

}

#endif /* NSGA2_H_ */
