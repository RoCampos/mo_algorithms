#ifndef _SPEA_H_
#define _SPEA_H_

#include <vector>
#include <cstdlib>
#include <iostream>
#include <limits>


// FORNEÇA O SEUS CABEÇALHOS
/*
#include "../container/matrix.h"
#include "generic_individual.h"
#include "multiobjective.h"
*/

string line = "--------------------------------------------------------------";

/**
 * Struct utilizada para armazenar a distância para um
 * indívuo. É utilizado para auxiliar no cálculo da
 * distância entre os indivíduos no SPEA2.s
 *
 * index representa o índice do indivíduo.
 * eDist representa a distância euclidiana.
 */
struct Ind {
	int index;
	double eDist; //euclidian distance	
	Ind (int i = 0, double d = 0):index(i) , eDist(d){}	
};

/**
 * Função  que compara dois indivíduos do tipo struct Ind
 * e indica se obj1 é menor que obj2.
 * Função utilizada para ordenar os indivíduos em ordem
 * crescente.
 */
bool compare (const struct Ind& obj1, const struct Ind& obj2)
{
	return obj1.eDist < obj2.eDist;
}

/**
* Esta classe contém a implementação do SPEA2 (Strenght Pareto
* evolutionary algoritihm 2 - 2001).
*
* @author Romerito Campos
* @date 10/10/2012
*/
class Spea2{
	
public: 
	/**
	 * Construtor da classe Spea2. Possui trẽs parâmetro default
	 * popzise que indica o tamanho da população utilizada.
	 * arc_size que indica o tamanho do arquivo utilizada.
	 * max_gen que indica a quantidade de geraçao.
	 *
	 * O construtor inicializa as estruturas de dados para armazenar
	 * a população e o arquivo. Além disso, inicializa a matriz de
	 * distância entre os indivíduos. Por fim, inicializa o valor
	 * do k-ésimo vizinho mais próximo.
	 *
	 * @param int popsize
	 * @param int arc_size
	 * @param int max_gen
	 */
	Spea2 (int popsize = 100, int arc_size = 50, int max_gen = 100);

	/**
	 * Destrutor da clase Spea2. Desaloca a memória utiliza para armazenar
	 * os indivíduos da população e arquivo.
	 */
	~Spea2 ();

	/**
	 * Método de execução do algoritmo Spea2.
	 * Este método executa o Spea2 segundo a definição dada pelos
	 * autores do mesmo.
	 */
	void run ();

	/**
	 * Este método inicializa a população inicial do algoritmo Spea2.
	 * Ele faz uso do construtor de soluções da classe GenericIndividual.
	 */
	void initialization (); //aberto

	/**
	 * Aqui é realizada a recombinação de indivíduos.
	 * Os indíviduos são escolhidos através de torneio binário.
	 *
	 * O usuário deve fornecer um mecanismo de recombinação de
	 * indivíduos.
	 */
	void recombination (); //aberto

	/**
	 * Método para realização de reparo em indivíduos da população
	 * Este método deve ser implementado pelo usuário do algoritmo.
	 */
	void repair ();

	//funções para impressão da população e do arquivo
	void printPop ();
	void printArc ();
	void printAsPisa ();

private:	

	/**
	 *
	 */
	void fitnessAssign ();

	/**
	 * Função para o cálculo da densidade de todos os
	 * indivíduos.
	 *
	 * Inicialmente faz o cálculo da distância euclidiana
	 * entre todos os pontos (soluções) para todos os outros.
	 *
	 * Após isto para cada ponto ordena os seus vizinhos (com
	 * base na distância euclidiana obtida) em ordem crescete
	 * Isto vai ser utilizado para o cálculo da densidade.
	 *
	 * @date 11/10/2012
	 * @uthor Romerito Campos
	 */
	void densityCalc ();

	/**
	 * Método que realiza a pressão ambiente sobre os indivíduos.
	 * Esta função invaca a funçao de truncamento de arquivo.
	 *
	 * Inicialmente, todos os indivíduos não dominados são
	 * avaliados. Se a quantidade de indivíduos é exatamente o
	 * tamanho do arquivo, então a função retorna vazio.
	 *
	 * Caso a quantidade de indivíduos seja maior ou menor
	 * que o tamanho do arquivo, então a função truncation2
	 * é invocada.
	 *
	 * @see truncation2
	 * @date 11/10/2012
	 * @author Romerito Campos
	 */
	void environmentSelection ();

	/**
	 * Método que retorna a densidade de um indivíduo.
	 * O cálculo é o seguinte: 1/(kth_dis + 2),
	 * onde kth_dis indica a distância euclidiana para
	 * o k-ésimo vizinho mais próximo.
	 * O k-ésimo vizinho mais próximo é definido como
	 * a raiz-quadrada do tamanho da popução mais o tamnho
	 * do arquivo.
	 *
	 * @param int
	 * @return double
	 */
	double getDensity (int);

	/**
	 * Método para truncar o arquivo. Este método
	 * possui dois casos:
	 *
	 * O primeiro caso ocorre  quando o arquivo não foi
	 * totalmente preenchido, neste caso organiza-se os
	 * indivíduos dominados da população pelo valor de
	 * fitness e os adiciona ao arquivo.
	 *
	 * O segundo caso ocorre quando há mais indivíduos
	 * não dominados que o tamanho do arquivo, então
	 * remove-se o indivíduo com a menor distância para
	 * outro indivíduo no arquivo. Isto é feito até a
	 * quantidade de indivíduos não dominados ser igual
	 * ao tamanho do arquivo.
	 */
	void truncation2 (int);

	/**
	 * Método para remoção de um indivíduo do arquivo.
	 * O primeiro parâmetro indica o início do arquivo,
	 * o segundo parâmetro indica o indivíduo a ser
	 * removido.
	 *
	 * O procedimento de remoção consiste em deslocar o
	 * indivíduo indicado no parâmetro index até o início
	 * do arquivo através da troca de de posição.
	 *
	 * Após isto, o início do arquivo é uma marcado para uma
	 * posição a frente de beginArc.
	 *
	 * @changePos
	 * @param int beginArc
	 * @param int index
	 */
	void removal (int,int);

	/**
	 * Faz a troca entre dois indivíduos no vetor que representa
	 * a população de indivíduos.
	 * Esta função é útil nos métodos de pressão ambiente e de
	 * truncar o arquivo de indivíduos não-dominados.
	 *
	 * O primeiro parâmetro indica a nova posição do indivíduo,
	 * o segundo parâmetro indica a antiga posição do indivíduo.
	 *
	 * @param int
	 * @param int
	 */
	void changePos (int,int);

	/**
	 * Torneio binário com reposição.
	 * Os indivíduos escolhidos para reprodução são
	 * sorteados dentre os indivíduos do arquivo, onde
	 * há reposição dos mesmos.
	 *
	 * O retorno é um inteiro indicando o índice que o
	 * indivíduo possui no vetor que armazena a população.
	 *
	 * @return int
	 */
	int binaryTournament  ();
	
private:	
	int POPSIZE;
	int ARCSIZE;
	int MAX_GEN;
	int gen;
	int all_pop;
	int kth;
	GenericIndividual **population;
	vector<struct Ind> distance;

};


Spea2::Spea2 (int popsize, int arc_size, int max_gen)
	: POPSIZE(popsize), ARCSIZE (arc_size), MAX_GEN (max_gen), gen(1)
{
	all_pop = POPSIZE+ARCSIZE;
	population = new GenericIndividual*[this->all_pop];
	distance = std::vector<Ind> (all_pop * all_pop);
	kth = trunc (sqrt(all_pop));

}

Spea2::~Spea2 ()
{	
	for (int i=0; i < all_pop; i++) {
		delete population[i];
	}

	delete [] population;
}

void Spea2::run () {
	
#ifdef DEBUG
	printf ("Function %s\n", __PRETTY_FUNCTION__ );
#endif
	
	cout << "Generation :" << gen << endl;
	initialization ();

#ifdef DEBUG
	printPop();
#endif

	densityCalc ();
	fitnessAssign ();
	environmentSelection ();
	POPSIZE = POPSIZE + ARCSIZE;

	while (++gen <= MAX_GEN) {
		cout << "\nGeneration :" << gen << endl;
		recombination ();
		densityCalc ();
		fitnessAssign ();
		environmentSelection ();

	}
	printPop();
	printArc();

}

void Spea2::initialization () {
	
#ifdef DEBUG
	printf ("Function %s\n", __PRETTY_FUNCTION__ );
#endif

	for (int i=0; i < POPSIZE; i++) {
		population[i] = new GenericIndividual;
		population[i]->index = i;
	}
	for (int i=POPSIZE; i < POPSIZE+ARCSIZE; i++) {
		population[i] = new GenericIndividual(true);
	}

	#ifdef DEBUG
		printPop();
		cout << line <<endl;
	#endif
}

//procedimento de fitness
void Spea2::fitnessAssign () {
		
#ifdef DEBUG
	printf ("Function %s\n", __PRETTY_FUNCTION__ );
#endif
	
	std::vector<int> strenght = vector<int>(all_pop,0);
	for (int i=0; i < POPSIZE; i++) {
		population[i]->fitness = 0.0; //vamos ver se roda
		for (int j=0; j < POPSIZE; j++) {
			if (i==j) continue;

			if (MultiObjective::dominate (population[i]->obj,population[j]->obj)) {
				strenght[i]++;
			}

		}
	}
	
	//calcula do raw-fitness R
	for (int i=0; i < POPSIZE; i++) {
		for (int j=0; j < POPSIZE; j++) {
			if (i==j) continue;
			if (MultiObjective::dominate (population[i]->obj,population[j]->obj)) {
				population[j]->fitness +=  strenght[i];
			}
		}
	}

	for (int i=0; i < POPSIZE; i++) {
		population[i]->fitness += getDensity (i);
	}	

	#ifdef DEBUG
		printPop();
		if (gen > 1) printArc();
		cout << line <<endl;
	#endif

}

void Spea2::densityCalc () {
	
#ifdef DEBUG
	printf ("Function %s\n", __PRETTY_FUNCTION__ );
#endif
	
	//limpando distance matrix
	for (int i = 0; i < POPSIZE; i++) {
		
		population[i]->index = i; //modifiquei aqui //TODO
		
		for (int j = 0; j < POPSIZE; j++) {
			distance[i * POPSIZE + j].index = 0;
			distance[i * POPSIZE + j].eDist = 0.0;

		}
	}
	
	for (int i = 0; i < POPSIZE; i++) {
		
		for (int j = i; j < POPSIZE; j++) {
			
			if (i==j) continue;
			
			distance[i * POPSIZE + j].index = j;
			distance[i * POPSIZE + j].eDist = MultiObjective::distanceCalc (population[i]->obj, population[j]->obj);
			distance[j * POPSIZE + i].index = i;
			distance[j * POPSIZE + i].eDist = distance[i * POPSIZE + j].eDist;

		}
	}	

	

	#ifdef DEBUG

	for (int i=0; i < POPSIZE;++i) {
		for (int j=0; j < POPSIZE;++j) {
			cout << distance[i * POPSIZE + j].eDist << " ";
		}
		cout << endl;
	}
	#endif

	//sorting the list of k-th nearest neighbor
	for (int i = 0; i < POPSIZE; i++) {
		sort (distance.begin() + (POPSIZE*i), distance.begin () + (POPSIZE*(i+1)), compare);
	}

	#ifdef DEBUG
	cout << endl;
	for (int i=0; i < POPSIZE;++i) {
		for (int j=0; j < POPSIZE;++j) {
			cout << distance[i * POPSIZE + j].eDist << " ";
		}
		cout << endl;
	}

	for (int i=0; i < POPSIZE;++i) {
		cout << "Edit: " << getDensity(i) << endl;
	}

	#endif

}

double Spea2::getDensity (int i) {
	//modifiquei, troquei all_pop por POPSIZE
	return (double)(1/(distance[i * POPSIZE + kth].eDist + 2));
}

void Spea2::environmentSelection () {
	
#ifdef DEBUG
	printf ("Function %s\n", __PRETTY_FUNCTION__ );
#endif

	int count = all_pop - 1;
	int in_archive = 0;
	for (int i = (POPSIZE - 1); i >= 0; --i) {
		if (population[i]->fitness <  1.0) {
			
			changePos (count,i);
			count--;
			in_archive++;
			
		}
	}
	
	if (in_archive == ARCSIZE) return;
	truncation2 (in_archive);

	#ifdef DEBUG
		printPop();
		if (gen > 1) printArc();
		cout << line <<endl;
	#endif
}

void Spea2::truncation2(int arc_size) {

#ifdef DEBUG
	printf ("Function %s\n", __PRETTY_FUNCTION__ );
#endif

	if (arc_size < ARCSIZE) {

		sort(population,population+(all_pop-ARCSIZE),compareByFitness);
		for (int i=0; i < (all_pop-ARCSIZE); i++) {
			if (population[i]->fitness > 1.0) {

				changePos (all_pop-arc_size-1,i);

				arc_size++;
				if (arc_size == ARCSIZE) return;
			}
		}
	} else {

		int beginArch = all_pop - arc_size;
		while (arc_size > ARCSIZE) {

			int closest = beginArch;
			double min_dist = numeric_limits<double>::max();

			for (int i=beginArch; i < all_pop; i++) {

				for (int j=i; j < all_pop; j++) {

					if (i==j) continue;

					double dist = MultiObjective::distanceCalc(
									population[i]->obj,population[j]->obj);

					if (dist < min_dist) {
						min_dist = dist;
						closest = i;
					}

				} //end for j
			}// end for i

			removal(beginArch,closest);
			beginArch++;
			--arc_size;
		}

	}

}

//remove index by replace it by all individuals before it
void Spea2::removal (int beginArch , int index) {
	
	for (int i = index; i > beginArch; --i) {		
		changePos (i,i-1);
	}
}

//change pos of idx_i and idx_j
//the first parameter indicates de new position
//the second paramater indicates the old positions
//it algo does the following
//put change idx_i to idx_j position
void Spea2::changePos (int idx_i, int idx_j) {

	if (idx_i == idx_j) return;
	
	GenericIndividual * pointer = new GenericIndividual(false);
	pointer->assign (population[idx_i]);
	int index = population[idx_j]->index;
	
	if (population[idx_i]->assign (population[idx_j])) {
		
		population[idx_i]->index = index; //update the index of ind in archive				
		
		if (population[idx_j]->assign(pointer)) {
			population[idx_j]->index = pointer->index;
		}
	}
	
	delete pointer;
	
}

//here you can put your way to perform recombination
void Spea2::recombination () {
	
#ifdef DEBUG
	printf ("Function %s\n", __PRETTY_FUNCTION__ );
#endif
	
	for (int i=0; i < (all_pop - ARCSIZE); i++) {
		
		GenericIndividual * ind = new GenericIndividual(false);
		int _p1 = binaryTournament();
		int _p2 = binaryTournament();
		while (_p1 == _p2 ) _p2 = binaryTournament();

		GenericIndividual * p1 = population[_p1];
		GenericIndividual * p2 = population[_p2];
		

		// AQUI EU APLICO CROSSOOVER, DEFINA O SEU.
		//rca::crossover3 ( p1->mpp_ind, p2->mpp_ind, &(ind->mpp_ind) );


		ind->index = i;
		ind->obj[0] = ind->mpp_ind.getObjective(0);
		ind->obj[1] = ind->mpp_ind.getObjective(1);
		population[i]->assign (ind);

		delete ind;
	}

	for (int i=(all_pop - ARCSIZE); i< all_pop; ++i) {
		population[i]->fitness = 0.0;
	}
}

int Spea2::binaryTournament  () {

	int ind1 = rand () % ARCSIZE + (all_pop - ARCSIZE);
	int ind2 = rand () % ARCSIZE + (all_pop - ARCSIZE);
	while (ind1 == ind2) ind2 = rand () % ARCSIZE + (all_pop - ARCSIZE);

	return (population[ind1]->fitness < population[ind2]->fitness ? ind1 : ind2);

}

void Spea2::repair () { 
	// TODO
}


void Spea2::printPop () {
	
	printf("Current Population\n");
	for (int i=0; i < all_pop - ARCSIZE; i++) {
		printf ("Index: %d  fitness: %f \t",
				population[i]->index,
				population[i]->fitness);
		double obj1 = population[i]->obj[0];
		double obj2 = population[i]->obj[1];
		printf ("%f %f\n",obj1, obj2);
	}
}

void Spea2::printAsPisa () {
	for (int i=all_pop - ARCSIZE; i < all_pop; i++) {	
		double obj1 = population[i]->obj[0];
		double obj2 = population[i]->obj[1];
		printf ("%f %f\n",obj1, obj2);
	}
	printf ("\n");
}

void Spea2::printArc () {
	printf("Current Archive\n");
	for (int i=all_pop - ARCSIZE; i < all_pop; i++) {
		printf ("Index: %d  fitness: %f \t",
				population[i]->index,
				population[i]->fitness);
		double obj1 = population[i]->obj[0];
		double obj2 = population[i]->obj[1];
		printf ("%f %f\n",obj1, obj2);
	}
}

#endif
