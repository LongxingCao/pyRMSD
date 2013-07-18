/*
 * RMSDCalculatorFactory.cpp
 *
 *  Created on: 06/03/2013
 *      Author: victor
 */

#include "RMSDCalculatorFactory.h"
#include "../RMSDCalculationData.h"
#include <iostream>
#include <cstdlib>

#include "../KernelFunctions.h"
#include "../RMSDCalculator.h"
#include "../KABSCH/KABSCHSerialKernel.h"
#include "../KABSCH/KABSCHOmpKernel.h"
#include "../QTRFIT/QTRFITSerialKernel.h"
#include "../QTRFIT/QTRFITOmpKernel.h"
#include "../QCP/QCPSerialKernel.h"
#include "../QCP/QCPOmpKernel.h"
#include "../QCP/QCPSerialFloatKernel.h"

#ifdef USE_CUDA
	#include "../QCP/QCPCUDAKernel.h"
	#include "../QCP/QCPCUDAMemKernel.h"
#endif

using namespace std;

RMSDCalculatorFactory::RMSDCalculatorFactory() {}

RMSDCalculatorFactory::~RMSDCalculatorFactory() {}

RMSDCalculator* RMSDCalculatorFactory::createCalculator(
		RMSDCalculatorType type,
		int numberOfConformations,
		int atomsPerFittingConformation,
		double* allFittingCoordinates,
		int atomsPerCalculationConformation,
		double* allCalculationCoordinates,
		int number_of_threads,
		int threads_per_block,
		int blocks_per_grid) {

	KernelFunctions* kernelFunctions;


	switch (type) {
		case KABSCH_SERIAL_CALCULATOR:
					kernelFunctions = new KABSCHSerialKernel;
					break;

		case KABSCH_OMP_CALCULATOR:
					kernelFunctions = new KABSCHOmpKernel(number_of_threads);
					break;

		case QTRFIT_SERIAL_CALCULATOR:
					kernelFunctions = new QTRFITSerialKernel;
					break;

		case QTRFIT_OMP_CALCULATOR:
					kernelFunctions = new QTRFITOmpKernel(number_of_threads);
					break;


		case QCP_SERIAL_CALCULATOR:
					kernelFunctions = new QCPSerialKernel;
					break;

		case QCP_SERIAL_FLOAT_CALCULATOR:
					kernelFunctions = new QCPSerialFloatKernel;
					break;

		case QCP_OMP_CALCULATOR:
					kernelFunctions = new QCPOmpKernel(number_of_threads);
					break;

#ifdef USE_CUDA
		case KABSCH_CUDA_CALCULATOR:
					kernelFunctions = NULL;
					break;

		case QTRFIT_CUDA_CALCULATOR:
					kernelFunctions = NULL;
					break;

		case QCP_CUDA_CALCULATOR:
					kernelFunctions = new QCPCUDAKernel(
											allFittingCoordinates,
											atomsPerFittingConformation,
											atomsPerFittingConformation*3,
											numberOfConformations,
											threads_per_block,
											blocks_per_grid);
					break;
		case QCP_CUDA_MEM_CALCULATOR:
					kernelFunctions = new QCPCUDAMemKernel(
											allFittingCoordinates,
											atomsPerFittingConformation,
											atomsPerFittingConformation*3,
											numberOfConformations,
											threads_per_block,
											blocks_per_grid);
					break;
#endif

		default:
			cout<<"[ERROR] Not kernel type implementation for type: "<<calculatorTypeToString(type)<<endl;
			exit(-1);
	}


	// Package input data
	RMSDCalculationData* rmsdData = new RMSDCalculationData(	numberOfConformations,
															atomsPerFittingConformation,
															allFittingCoordinates,
															atomsPerCalculationConformation,
															allCalculationCoordinates);

	RMSDCalculator* calculator = new RMSDCalculator(rmsdData, kernelFunctions);

	return calculator;
}
