//#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
//#include <termios.h>


// ******** PARAMETERS ********* //
#define numInputs  64       // Number of pixels per letter (without bias)
#define numHidden 70        // Double number of pixels (Why?)
#define numOutput 26      // Number of different letters (exemples)

#define numExamples 25 // ??? VICTOR ??? 


const int numEpochs = 500;
const double LR_IH = 0.07;
const double LR_HO = 0.007;

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

//Save the weights
void Serialization(void)
{
  // Open the file to write
    FILE *f;

    //double weightsIH[numInputs+1][numHidden];
    f = fopen("WeightsIH.txt","w");

    //Saves the weights from Inputs to Hidden
    for (int y = 0; y < numInputs; y++)
    {
        for(int x = 0;x < numOutput; x++) 
	  {
	    fprintf(f,"%f ",weightsIH[y][x]);
	  }
        fprintf(f,"\n");
    }
    fclose(f);

    //double weightsHO[numHidden][numOutput];

    f = fopen("WeightsHO.txt","w");
    for (int y = 0; y < numInputs; y++)
      {
        for(int x = 0;x < numHidden; x++) 
	  {
	    fprintf(f,"%f ",weightsIH[y][x]);
	  }
        fprintf(f,"\n");
      }
    fclose(f);
}

void Deserialization(void)
{
  FILE *f;
  float v; 
  //double weightsIH[numInputs+1][numHidden];
  f = fopen("WeightsIH.txt","r");

  //Saves the weights from Inputs to Hidden
  for (int y = 0; y < numInputs; y++)
    {
      for(int x = 0;x < numOutput; x++) 
	{
	  fscanf(f,"%f",&v);
	  weightsIH[y][x] = v;
	}
    }
  fclose(f);

  //double weightsHO[numHidden][numOutput];
  // read file
  f = fopen("WeightsHO.txt","r");

  //weights from Inputs to Hidden equals to the float in the file
  for (int y = 0; y < numInputs; y++)
    {
      for(int x = 0;x < numOutput; x++) 
	{
	  fscanf(f,"%f",&v);
	  weightsHO[y][x] = v;
	}
    }
    fclose(f);
}

//Get random between 0 and 1
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

  //File that will contain the matrices of the letters with 1 and -1
  FILE *f;
  f = fopen("Letters.txt","r");

  //For each letter
  for(int i =0; i < numExamples; i++)
    {
      //Read the letter
      char letter;
      float valPixel;
      fscanf(f,"%c",&letter);

      while (letter == ' ') fscanf(f,"%s",&letter);
      for (int y = 0;y<8;y++)
	{
	  for(int x = 0;x<8;x++) 
	    {
	      fscanf(f,"%f ",&valPixel);	  
	      InputsVal[letter-65][x + y *8] = valPixel;
	    }
	  for (int x = 0; x < numExamples+1; x++)
	    {
	      if(x == letter - 65) desiredOutput[letter-65][x] = 1;
	      else desiredOutput[letter-65][x] = -1;
	    }
	}
    }

  //Tp Check if the document was correctly read
  for(int y = 0; y<8;y++)
    {
      for(int x = 0; x < 8 ; x++)
	printf("%f ",InputsVal[0][x+y *8]);
      printf("\n");
    }

  // Verify if the desired output has been well impletmented
  for(int x = 0; x<numExamples+1; x++)
    {
      printf("desired output: %f\n",desiredOutput[0][x]);
    }

  // *** End of function *** //
  fclose(f);
  return;

  // *** FOR THE XOR *** //

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
}

FILE * fp;                      // Write all outputs in this file for debugging

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

int mainNW_Read()
{
  //Initialize the Inputs values, desired output values
  initInputs();

  Deserialization();

  return 0;
}

int mainNW_Learn()
{
  //init random stuff
  srand(time(NULL));

  //Initialize the Inputs values, desired output values
  initInputs();

  initWeights(); 
  int numIterations = 0;
  int numcorrect = 0;
  int numstuck = 0;

  // Faire appel à AddLetter pour initialiser InputsVal !!!!!
  //float arr[8][8];
  //AddLetter(arr, LetterNumber);  

  do
    {
      // printf("Iteration: %d\n",numIterations);
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
}
