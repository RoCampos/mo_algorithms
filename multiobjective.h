#ifndef _MULTI_OBJECTIVE_IMPL_H_
#define _MULTI_OBJECTIVE_IMPL_H_

#include <cmath>
#include <cassert>
#include <fstream>
#include <iostream>

#include <sstream>

#include "problem_info.h"

namespace MultiObjective {

	/**
	 * Está função verifica se o vetor2 é dominado pelo vetor1
	 * A verificação é feita para todos os objetivos.
	 *
	 * Todos os objetivos são considerados como minimização.
	 * Portanto, caso algum objetivo do vetor1 tenha valor maior
	 * que o mesmo objetivo do vetor2 então é considerado que ele
	 * não domina.
	 *
	 * Se em todos os objetivos a propriedade acima não é encontrada
	 * então o vetor2 não é dominado pelo vetor1.
	 *
	 * @date 11/10/2012
	 * @author Romerito Campos
	 */
	int dominate (double *vetor1, double *vetor2);

	/**
	 * Está função calcula a distância euclidiana entre dois
	 * pontos no espaço n-dimensinal.
	 *
	 * @date 11/10/2012
	 * @author Romerito Campos
	 */
	double distanceCalc (double *, double *);

	/* Recebe um arquivo de texto filtra os não dominados*/
	void filter (std::string file_name);

	enum {DOMINATED = 1, NONDOMINTED = 0};
	
}
#endif 

namespace MultiObjective {
	
	int dominate (double * vetor1, double * vetor2) {
		
		
		int equal = 0;
		for (int i=0; i < Info::OBJECTIVES; i++) {
			
			if (rca::compareDouble (vetor1[i],vetor2[i])) {
				equal++;
			}
			
			//se algum vetor1[i] é maior que vetor2[i] então vetor1 não domina vetor2
			if (vetor1[i]*Info::objconf[i] > vetor2[i]*Info::objconf[i]) {
				return NONDOMINTED;
			}
		}
		
		//se nenhum vetor[i] é maior que vetor2[i] então vetor1 domina vetor2
		return DOMINATED;
	}
	
	double distanceCalc (double * vector1, double * vector2) {
		double sum = 0.0;
		for (int i = 0; i < Info::OBJECTIVES; i++) {
			sum += pow(vector1[i] - vector2[i], 2.0);
		}		
		
		return sqrt (sum);
	}

	void filter(std::string file_name) {

		std::ifstream file (file_name.c_str(), std::ifstream::in);

		int size = 0;
		char l;
		string ss;
		double value = 0.0;

		double **individuals;

		while ( !file.get(l).eof() ) {

			if (l != ' ' && l != '\n') {
				ss.push_back(l);
			}

			if (l == ' ') {
				ss.clear();
			}

			if ( l == '\n') {
				ss.clear();
				size++;
			}
		}

		individuals = new double*[size];
		for (int i=0; i < size; ++i) {
			individuals[i] = new double[Info::OBJECTIVES];
		}

		file.close();

		file.open(file_name.c_str(), std::ifstream::in);

		size = 0;
		int obj = 0;
		while ( !file.get(l).eof() ) {

			if (l != ' ' && l != '\n') {
				ss.push_back(l);
			}

			if (l == ' ') {

				value = atof(ss.c_str());
				individuals[size][obj] = value;
				obj++;
				ss.clear();
			}

			if ( l == '\n') {
				value = atof(ss.c_str());
				individuals[size][obj] = value;
				ss.clear();

				size++;
				obj = 0;
			}
		}


		for (int i=0; i < size; ++i) {

			int count = 0;
			int j = 0;
			for (; j < size; ++j) {

				if (j==i) continue;

				if (dominate(individuals[j],individuals[i])) {
					count++;
				}
			}

			if (count == 0) {
				cout << individuals[i][0] << " " << individuals[i][1] << endl;
			}

		}

		//freeing memory
		for (int i=0; i < size; ++i) {
			delete individuals[i];
		}
		delete [] individuals;
	}

}
