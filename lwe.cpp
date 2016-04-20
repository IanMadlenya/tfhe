#include <cstdlib>
#include "lwe.h"
#include "lweparams.h"
#include "lwekey.h"
#include "lwesamples.h"
#include <random>

using namespace std;

default_random_engine generator;


// Gaussian sample centered in message, with standard deviation sigma
void gaussian32(Torus32 x, Torus32 message, double sigma){
    double mu = message >> 31;
    normal_distribution<double> distribution(mu,sigma);
    x = (Torus32) distribution(generator) << 31;
}


// Used to approximate the phase to the nearest message possible in the message space
// The constant Msize will indicate on which message space we are working
void approxPhase(double result, Torus32 phase, int Msize){
    //double phi = phase >> 31;
}






/**
 * This function generates a random LWE key for the given parameters.
 * The LWE key for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweKeyGen(LWEKey* result) {
  int n = result->params->n;

  for (int i=0; i<n; i++) {
    result->key[i]=rand()%2;
    // à completer je crois (entropy of the key)
  }
}



/**
 * This function encrypts message by using key, with average noise alpha
 * The LWE sample for the result must be allocated and initialized
 * (this means that the parameters are already in the result)
 */
EXPORT void lweSymEncrypt(LWESample* result, Torus32 message, double alpha, const LWEKey* key){
    int n = key->params->n;

    gaussian32(result->b, message, alpha); 
    for (int i = 0; i < n; ++i)
    {
        result->a[i] = (Torus32) (rand()%2) << 31;
        result->b += result->a[i]*key->key[i];
    }
}



/**
 * This function computes the phase of sample by using key : phi = b - a.s
 */
EXPORT Torus32 lwePhase(const LWESample* sample, const LWEKey* key){
    int n = key->params->n;
    Torus32 phi = sample->b;

    for (int i = 0; i < n; ++i) phi -= sample->a[i]*key->key[i];
    return phi;
}



/**
 * This function computes the decryption of sample by using key
 * The constant Msize indicates the message space and is used to approximate the phase
 */
EXPORT double lweSymDecrypt(const LWESample* sample, const LWEKey* key, const int Msize){
    Torus32 phi;
    double mu = 0;

    phi = lwePhase(sample, key);
    approxPhase(mu, phi, Msize);
    return mu; 
}









EXPORT void lweLinearCombination(LWESample* result, const int* combi, const LWESample* samples, const LWEParams* params);

EXPORT void lweKeySwitch(LWESample* result, const LWEKeySwitchKey* ks, const LWESample* sample);

// Ring
EXPORT void ringLweKeyGen(LWEKey* result);
EXPORT void ringLweSymEncrypt(LWESample* result, double message, const LWEKey* key);
EXPORT double ringLweSymDecrypt(const LWESample* sample, const LWEKey* key);

EXPORT void ringLwePolyCombination(LWESample* result, const int* combi, const LWESample* samples, const LWEParams* params);

// GSW
EXPORT void ringGswKeyGen(LWEKey* result);
EXPORT void ringGswSymEncrypt(LWESample* result, double message, const LWEKey* key);
EXPORT double ringGswSymDecrypt(const LWESample* sample, const LWEKey* key);

EXPORT void ringGswPolyCombination(LWESample* result, const int* combi, const LWESample* samples, const LWEParams* params);

//extractions Ring LWE -> LWE
EXPORT void keyExtract(LWEKey* result, const RingLWEKey*); //sans doute un param supplémentaire
EXPORT void sampleExtract(LWESample* result, const RingLWESample* x);

//extraction RingGSW -> GSW
EXPORT void gswKeyExtract(RingLWEKey* result, const RingGSWKey* key); //sans doute un param supplémentaire
EXPORT void gswSampleExtract(RingLWESample* result, const RingGSWSample* x);

//bootstrapping
EXPORT void bootstrap(LWESample* result, const LWEBootstrappingKey* bk, double mu1, double mu0, const LWESample* x);
