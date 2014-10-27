//#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>


#define numInputs  3
#define numPatterns  4

#define numHidden 4
const int numEpochs = 500;
const double LR_IH = 0.7;
const double LR_HO = 0.07;

void initWeights();
void initData();
void calcNet();
void WeightChangesHO();
void WeightChangesIH();
void calcOverallError();
void displayResults();
double getRand();

int patNum = 0;
double errThisPat = 0.0;
double outPred = 0.0;
double RMSerror = 0.0;

// the outputs of the hidden neurons
double hiddenVal[numHidden];

// the weights
double weightsIH[numInputs][numHidden];
double weightsHO[numHidden];

// the data
int trainInputs[numPatterns][numInputs];
int trainOutput[numPatterns];

// calculates the network output
void calcNet(void)
{
  //calculate the outputs of the hidden neurons
  int i = 0;
  int j = 0;

  for(i = 0;i<numHidden;i++)
    {
      hiddenVal[i] = 0.0;

      for(j = 0;j<numInputs;j++)
        {
	  hiddenVal[i]=hiddenVal[i]+(trainInputs[patNum][j] * weightsIH[j][i]);
        }

      hiddenVal[i] = tanh(hiddenVal[i]);
    }

  //calculate the output of the network
  outPred = 0.0;

  for(i = 0;i<numHidden;i++)
    {
      outPred = outPred + hiddenVal[i] * weightsHO[i];
    }
  //calculate the error
  errThisPat = outPred - trainOutput[patNum];

}

void WeightChangesHO(void)
{
  for(int k = 0;k<numHidden;k++)
    {
      double weightChange = LR_HO * errThisPat * hiddenVal[k];
      weightsHO[k] = weightsHO[k] - weightChange;

      //regularisation on the output weights
      if (weightsHO[k] < -5)
	{
	  weightsHO[k] = -5;
	}
      else if (weightsHO[k] > 5)
	{
	  weightsHO[k] = 5;
	}
    }
}

// adjust the weights input-hidden
void WeightChangesIH(void)
{

  for(int i = 0;i<numHidden;i++)
    {
      for(int k = 0;k<numInputs;k++)
	{
	  double x = 1 - (hiddenVal[i] * hiddenVal[i]);
	  x = x * weightsHO[i] * errThisPat * LR_IH;
	  x = x * trainInputs[patNum][k];
	  double weightChange = x;
	  weightsIH[k][i] = weightsIH[k][i] - weightChange;
	}
    }
}

double getRand(void)
{
  return ((double)rand())/(double)RAND_MAX;
}

// set weights to random numbers 
void initWeights(void)
{

  for(int j = 0;j<numHidden;j++)
    {
      weightsHO[j] = (getRand() - 0.5)/2;
      for(int i = 0;i<numInputs;i++)
	{
	  weightsIH[i][j] = (getRand() - 0.5)/5;
	  printf("Weight = %f\n", weightsIH[i][j]);
	}
    }

}

void initData(void)
{
  printf("initialising data\n");

  // it has been rescaled to the range [-1][1]
  trainInputs[0][0]  = 1;
  trainInputs[0][1]  = -1;
  trainInputs[0][2]  = 1;
  trainOutput[0] = 1;

  trainInputs[1][0]  = -1;
  trainInputs[1][1]  = 1;
  trainInputs[1][2]  = 1;
  trainOutput[1] = 1;

  trainInputs[2][0]  = 1;
  trainInputs[2][1]  = 1;
  trainInputs[2][2]  = 1;
  trainOutput[2] = -1;

  trainInputs[3][0]  = -1;
  trainInputs[3][1]  = -1;
  trainInputs[3][2]  = 1;
  trainOutput[3] = -1;

}

// display results
void displayResults(void)
{
  for(int i = 0;i<numPatterns;i++)
    {
      patNum = i;
      calcNet();
      printf("pattern number = %d current = %d neural model = %f\n",
	     patNum+1,trainOutput[patNum],outPred);
    }
}

// calculate the overall error
void calcOverallError(void)
{
  RMSerror = 0.0;
  for(int i = 0;i<numPatterns;i++)
    {
      patNum = i;
      calcNet();
      RMSerror = RMSerror + (errThisPat * errThisPat);
    }
  RMSerror = RMSerror/numPatterns;
  RMSerror = sqrt(RMSerror);
}

int main(void)
{
  // seed random number function
  srand ( time(NULL) );

  initWeights();

  initData();

  for(int j = 0;j <= numEpochs;j++)
    {
      for(int i = 0;i<numPatterns;i++)
        {
          patNum = rand()%numPatterns;

          calcNet();

          WeightChangesHO();
          WeightChangesIH();
        }

      //after each epoch
      calcOverallError();

      printf("iteration = %d Error rate = %f\n",j,RMSerror);
    }

  displayResults();

  system("PAUSE");
  return 0;
}
