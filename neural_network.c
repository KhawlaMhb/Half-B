//#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
//#include <termios.h>


// ******** PARAMETERS ********* //
#define numInputs  64       // Number of pixels per letter (without bias)
#define numHidden 128        // Double number of pixels (Why?)
#define numOutput 26      // Number of different letters (exemples)

#define numExamples 128 // ??? VICTOR ??? 


const int numEpochs = 500;
const double LR_IH = 0.7;
const double LR_HO = 0.07;

 int patNum = 0;
 double errThisPat[numOutput];
 double outPred[numOutput]; 
 double RMSerror = 0.0;

 // the weights
 double weightsIH[numInputs+1][numHidden];
 double weightsHO[numHidden][numOutput];

 // the data
double InputsVal[numOutput][numInputs+1];      // 26 Letters of 64 Pixels each
double hiddenVal[numHidden];                   // Output of the Hidden neurons
double OutputVal[numOutput];                   // 26 Output Results
double delta[numOutput];

double desiredOutput[numOutput][numOutput];

int limit_stuck = 2000;
int limit_nbstuck = 5;

// ***********  FUNCTIONS  ********** //

int currentExemple;

double between01()
{
  double r= rand() / ((double) RAND_MAX);
  return r;
}

double square(double x)
{
  return x * x;
}

double myabs(double x)
{
  if(x < 0) return -x;
  else return x;
}

double logistic(double x)
{
  return 1/(1 + exp(-x));
}

double special(double x)
{
  return x * (1-x);
}

void initInputs()
{
  //Init biais neuron
  for(int i = 0; i < numHidden; i++) InputsVal[i][numInputs] = 1;

  //FOR THE XOR

  //first example, (1, -1) -> 1
  InputsVal[0][0] = 1;
  InputsVal[0][1] = -1;
  desiredOutput[0][0] = 1;
  //desiredOutput[0][1] = 1;

  //second example, (-1, 1) -> 1
  InputsVal[1][0] = -1;
  InputsVal[1][1] = 1;
  desiredOutput[1][0] = 1;
  //desiredOutput[1][1] = -1;

  //third example, (1, 1) -> -1
  InputsVal[2][0] = 1;
  InputsVal[2][1] = 1;
  desiredOutput[2][0] = -1;
  //desiredOutput[2][1] = -1;

  //fourth example, (-1, -1) -> -1
  InputsVal[3][0] = -1;
  InputsVal[3][1] = -1;
  desiredOutput[3][0] = -1;
  //desiredOutput[3][1] = 1;



  /*
    for(int i = 0; i < nbExamples; i++)
    {
    for(int j=0; j < nbInputs; j++)
    {
    if(rand() % 2 == 0)
    neuronValueInputs[i][j] = 1;
    else
    neuronValueInputs[i][j] = -1;
    }
    for(int j=0; j < nbOutput; j++)
    {
    if(rand() % 2 == 0)
    desiredOutput[i][j] = 1;
    else
    desiredOutput[i][j] = -1;
    }
    }
  */
}

// set weights to random numbers
void initWeights(void)
{
  for(int i = 0; i < numInputs+1; i++)
    for(int j = 0; j < numHidden; j++)
      weightsIH[i][j] = (between01() - 0.5) / 2; //change here if not -1 / 1

  for(int i = 0; i < numHidden; i++)
    for(int j = 0;j<numOutput;j++)
      {
	weightsHO[i][j] = (between01() - 0.5) / 2;
      }
}

void fowardPropagation()
{
  //From input layer to hidden layer
  for(int i =0; i < numHidden; i++)
    {
      hiddenVal[i] = 0.0;
      for(int j=0; j < numInputs + 1; j++)
	hiddenVal[i] += InputsVal[currentExemple][j] * weightsIH[j][i];

      hiddenVal[i] = tanh(hiddenVal[i]);
    }

  //From hidden layer to output layer
  for(int i = 0; i < numOutput; i++)
    {
      OutputVal[i] = 0.0;
      for(int j = 0; j < numHidden; j++)
	OutputVal[i] += hiddenVal[j] * weightsHO[j][i];

      delta[i] = OutputVal[i] - desiredOutput[currentExemple][i];
    }
}

void backPropagation()
{
  //Hidden to Output transitions weights
  for(int i = 0; i < numOutput; i++)
    {
      for(int j = 0; j < numHidden; j++)
	{
	  double theChange = LR_HO * delta[i] * hiddenVal[j];
	  weightsHO[j][i] -= theChange;

	  if(weightsHO[j][i] < -5)
	    weightsHO[j][i] = -5;
	  else if(weightsHO[j][i] > 5)
	    weightsHO[j][i] = 5;
	}
    }

  //input to hidden weights
  for(int i = 0; i < numHidden; i++)
    {
      for(int j = 0; j < numInputs + 1; j++)
	{
	  double x = 1 - (square(hiddenVal[i]));
	  double calcul = 0;
	  for(int k = 0; k < numOutput; k++)
	    calcul += weightsHO[i][k] * delta[k] * LR_IH;
	  x *= calcul;
	  x *= InputsVal[currentExemple][j];
	  weightsIH[j][i] -= x;
	}
    }
}

//void initData();
//void calcNet();
//void WeightChangesHO();
//void WeightChangesIH();
//void calcOverallError();
void displayResults();
//double getRand();


FILE * fp;                      // Write all outputs in this file for debugging

// ****** NOT USED ***** //
/*
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
	  hiddenVal[i]=hiddenVal[i]+(neuronValueInputs[patNum][j] * weightsIH[j][i]);
        }

      hiddenVal[i] = tanh(hiddenVal[i]);
    }

  //calculate the output of the network
  //outPred = 0.0;

  //TODO: outPred doit etre un tableau
  // rajouter une boucle
  // changer outPred en fonction des resultats voulus pour chaque lettre
  for(int k = 0; k<numPatterns ; k++)
    {
      outPred[k] = 0.0;
  for(i = 0;i<numHidden;i++)
    {
      outPred[k] = outPred[k] + hiddenVal[i] * weightsHO[i];
    }
  //calculate the error
  errThisPat[k] = outPred[k] - trainOutput[patNum][k];
    }

}

void WeightChangesHO(void)
{
  for(int k = 0;k<numHidden;k++)
    {
      double weightChange = 0.0;
      for( int i = 0; i<numPatterns; i++)
	{
	  weightChange += LR_HO * errThisPat[i]*hiddenVal[k];
	}
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
	  // Find what to put in errThisPat[?] 25 won't work
	  x = x * weightsHO[i] * errThisPat[25] * LR_IH;
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
*/

// **** NOT USED **** //

void AddLetter(float arr[8][8],int LetterNumber)
{
    for(int x = 0; x<8; x++)
        for(int y = 0; y<8; y++)
        {
            InputsVal[LetterNumber][x + y * 8]  = arr[x][y];
        }

    for(int i = 0; i<numOutput; i++)
      if (i==LetterNumber)
	desiredOutput[LetterNumber][i] = 1;
      else
	desiredOutput[LetterNumber][i] = -1;

    // Neurone de biais
    //InputsVal[LetterNumber][numInputs +1] = 1; 
 }

// ***** NOT USED ***** //
/*
void initTrainOutput(void)
{
  for(int i = 0; i< numHidden ; i++)
    {
      if (i == 0) trainOutput[0][0] = 1;
      else trainOutput[0][i] = -1;
    }
}

void initData(void)
{
  printf("initialising data\n");

  // Read Alphabet Image
  // Get all the letters and initialise trainInputs


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

  }*/

// display results
void displayResults(void)
{
  fowardPropagation();
  for(int i = 0;i<numInputs;i++) 
    printf("Entry %i : %g ",i,InputsVal[currentExemple][i]);
  
  printf("\n");

  for(int j =0; j<numOutput; j++)
    printf("Desired output %i : %g ",j,desiredOutput[currentExemple][j]);

  printf("\n");

  for(int k = 0; k < numOutput; k++)
    printf("Neural output %i : %g ",k,OutputVal[k]);

  printf("\n -------------- \n");
  
/*
    patNum = i;
    calcNet();
    printf("pattern number = %d expected = %d obtained = %f\n",
    patNum+1,trainOutput[patNum][j],outPred[j]);
    fprintf(fp,"pattern number = %d current = %d neural model = %f\n",
    patNum+1,trainOutput[patNum][j],outPred[j]);
    
  */    
}

// **** NOT USED **** //
/*
// calculate the overall error
void calcOverallError(void)
{
  RMSerror = 0.0;
  for(int i = 0;i<numPatterns;i++)
    {
      patNum = i;
      fprintf(fp,"Now calculating pattern %d\n",patNum);
      calcNet();
      RMSerror = RMSerror + (errThisPat * errThisPat);
    }
  RMSerror = RMSerror/numPatterns;
  RMSerror = sqrt(RMSerror);
}

*/

int mainNW()
{
  //init random stuff
  srand(time(NULL));

  //initInputs(); we shall use addletter
  initWeights();
  int numIterations = 0;
  int numcorrect = 0;
  int numstuck = 0;
  do
    {
      for(int i = 0; i < numExamples; i++)
	{
	  currentExemple = i;//use maybe random for perfs
	  fowardPropagation();
	  backPropagation();
	}

      //check error
      numcorrect = 0;
      for(int i =0; i < numExamples; i++)
	{
	  currentExemple = i;
	  fowardPropagation();
	  for(int j = 0; j < numOutput; j++)
	    {
	      double calcul=myabs(myabs(desiredOutput[i][j])-myabs(OutputVal[j]));
	      if(calcul < 0.03)
		numcorrect++;
	    }
	}

      numIterations++;

      //check if stuck
      if(numIterations > limit_stuck)
	{
	  numIterations = 0;
	  initWeights();
	  numstuck++;
	}
      if(numstuck > limit_nbstuck)
	break;

    }while(numcorrect != numExamples * numOutput);

  if(numstuck <= limit_nbstuck)
    {
      for(int i=0; i < numExamples; i++)
	{
	  currentExemple = i;
	  printf("Executed with %i iterations \n\n", numIterations);
	  displayResults();
	}
      if(numstuck > 0)
	printf("\n---------\n The neural network has been stuck %i times\n", numstuck);
    }
  else
    {
      printf("The neural network has been stucked too many times \n");
    }

  return 0;

  /*
  for(int j = 0;j <= numEpochs;j++)
    {
      for(int i = 0;i<numPatterns;i++)
        {
	  // patNum = rand()%numPatterns;
	  patNum = 0;
          calcNet();

          WeightChangesHO();
          WeightChangesIH();
        }

      //after each epoch
      calcOverallError();

      printf("iteration = %d Error rate = %f\n",j,RMSerror);
      fprintf(fp,"iteration = %d Error rate = %f\n",j,RMSerror);
    }

    displayResults();*/

  //system("PAUSE");

  fclose(fp);
  return 0;
}
