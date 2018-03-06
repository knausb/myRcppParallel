
/**
 * @title Parallel vector of strings with RcppParallel
 * @author Brian J. Knaus
 * @license GPL (>= 2)
 * @tags parallel
 * @summary Demonstrates using a vector of strings with RcppParallel.
 *
 * The data structures used by R and Rcpp are not thread safe because garbage 
 * collection may occur.
 * The [RcppParallel](https://rcppcore.github.com/RcppParallel) package 
 * includes RVector and RMatrix as thread safe dat structures.
 * However, they can only contain numeric data.
 * If you have non-numeric data (e.g., string, char) you will need to use a
 * different data structure.
 * Most of the C++ data structures should be thread safe.
 * Here we demonstrate how to use a vector of strings with RcppParallel.
 * 
 * 
 */


/**
 * ### Implementation using Rcpp
 * 
 *
 */


#include <Rcpp.h>


std::string getElement( std::string mystring, char split, int element ) {
  std::string rstring;
  int elementN = 0;
  unsigned int i=0;
  int start = 0;
  
  for(i=1; i<mystring.size(); i++){
    if(mystring[i] == split){
      // We matched our splitting character.
      if(elementN == element){
        // We've arrived at our desired element.
        rstring = mystring.substr(start, i - start);
        return rstring;
      } else {
        // We need to iterate to the next element.
        start = i + 1;
        i = i + 1;
        elementN = elementN + 1;
      }
    }
  }
  
  // The end of the string has no delimiter.
  // We test if it is the correct element and parse if so.
  if(elementN == element){
    rstring = mystring.substr(start, i - start);
    return rstring;
  } else {
    rstring = "-999";
    return rstring;
  }
}

// [[Rcpp::export]]
Rcpp::StringVector delimitString( Rcpp::StringVector myvector, char myDelim, int element ) {
  Rcpp::StringVector rvector( myvector.size() );
  std::string myString;
  
  // R is one based, C++ is zero based.
  element = element - 1;
  
  unsigned int i=0;
  for(i=0; i<myvector.size(); i++){
    myString = myvector[i];
    rvector[i] = getElement(myString, myDelim, element);
  }
  
  return rvector;
}


/**
 * ### Implementation using RcppParallel
 * 
 *
 */

#include <RcppParallel.h>
using namespace RcppParallel;
// [[Rcpp::depends(RcppParallel)]]


struct getElement : public Worker {
   // input vector to read from
   const std::vector< std::string > myVector;
   
   // output vector to write to
   std::vector< std::string > &retVector;
   std::string myString;

   // initialize input and output vectors
   getElement(const std::vector< std::string > myVector, std::vector< std::string > &retVector)
      : myVector(myVector), retVector(retVector) {}
   
   // function call operator that work for the specified range (begin/end)
   void operator()(std::size_t begin, std::size_t end) {
//     Rcpp::Rcout << "Value: " << begin << "\n";
     for (std::size_t i = begin; i < end; i++) {
//       Rcpp::checkUserInterrupt();
//       retVector[i] = "blah";
//       retVector[i] = myVector[i];
       myString = myVector[i];
       retVector[i] = myString;
//       retVector[i] = i;
//       Rcpp::Rcout << "Value: " << myString << "\n";
//       Rcpp::Rcout << "Value: " << retVector[i] << "\n";
     }
   }
};



// [[Rcpp::export]]
Rcpp::StringVector rcpp_parallel_delimitString(Rcpp::StringVector myVector) {
  
  // Because Rcpp data structures are not thread safe
  // we'll use std::vector for input and output.
  std::vector< std::string > tmpVector1(myVector.size());
  std::vector< std::string > tmpVector2(myVector.size());

  // Convert Rcpp::StringVector to std::vector.
  unsigned int i;
  std::string tmpString;

  for(i=0; i<myVector.size(); i++){
//    Rcpp::checkUserInterrupt();
    tmpString = myVector[i];
    tmpVector1[i] = tmpString;
  }
  
  // Create the worker
  struct getElement getElement(tmpVector1, tmpVector2);

  // Call it with parallelFor
  parallelFor(0, tmpVector1.size() - 1, getElement);

  // allocate the string we will return 
  Rcpp::StringVector retVector(tmpVector2.size());
  
  // Copy to Rcpp container to send back to R.
  retVector = tmpVector2;
  
  return retVector;
}




/**
 * ### Fabricate example data
 * 
 *
 */



/*** R
#n <- 1e7
#n <- 1e6
#
n <- 1e1
myGT <- paste(sample(0:1, size = n, replace = TRUE),
              sample(0:1, size = n, replace = TRUE),
              sep = "/")
myGT <- paste(myGT, LETTERS, 
              paste(sample(1:200, size = n, replace = TRUE), 
                    sample(1:200, size = n, replace = TRUE), sep=","),
              sep = ":")
myGT <- paste(myGT, sample(1:200, size = n, replace = TRUE), sep = ":")
head(myGT)
*/


/**
 * ### Benchmark
 * 
 *
 */



/*** R
library(microbenchmark)
nTimes <- 20
microbenchmark(gt1 <- delimitString(myGT, ":", 3), times = nTimes)
head(gt1)
*/


/*** R
#
microbenchmark(gt2 <- rcpp_parallel_delimitString(myGT), times = nTimes)
#
head(gt2)
*/



