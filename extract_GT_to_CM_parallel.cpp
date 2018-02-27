#include <Rcpp.h>
// [[Rcpp::depends(RcppParallel)]]
#include <RcppParallel.h>
using namespace RcppParallel;


struct GT_to_CM_parallel : public Worker {
  
  // input matrix to read from
  const RMatrix<SEXP> mat;
  
  // output matrix to write to
  RMatrix<SEXP> rmat;
  
  // initialize from Rcpp input and output matrixes (the RMatrix class
  // can be automatically converted to from the Rcpp matrix type)
  GT_to_CM_parallel(const Rcpp::StringMatrix mat, Rcpp::CharacterMatrix rmat) : mat(mat), rmat(rmat) {}
  
  
     // function call operator that work for the specified range (begin/end)
   void operator()(std::size_t begin, std::size_t end) {
      for (std::size_t i = begin; i < end; i++) {
         for (std::size_t j = 0; j < i; j++) {
           
         }
      }
   }
  
};




//' @export
// [[Rcpp::export(name=".extract_GT_to_CM_parallel")]]
Rcpp::StringMatrix extract_GT_to_CM_parallel( Rcpp::StringMatrix fix,
                                         Rcpp::StringMatrix gt,
                                         std::string element="DP",
                                         int alleles = 0,
                                         int extract = 1,
                                         int convertNA = 1) {
  
  Rcpp::CharacterMatrix rmat(gt.nrow(), gt.ncol() - 1);
  
   // create the worker
   GT_to_CM_parallel GT_to_CM_parallel(gt, rmat);
     
   // call it with parallelFor
   parallelFor(0, gt.nrow(), GT_to_CM_parallel);
  
  return rmat;
}


