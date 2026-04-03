
#include <iostream>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_sf_trig.h> 
const static int N = 1000;
const double PI=3.141592653589793;

gsl_vector* algorytm_thomasa(gsl_vector *a_3diag, gsl_vector *b_3diag, gsl_vector *c_3diag, gsl_vector *ve_d) {   
    gsl_vector *ve_L = gsl_vector_alloc(N+1);     
    gsl_vector *ve_U = gsl_vector_alloc(N);   
    gsl_vector *ve_wyniki = gsl_vector_alloc(N+1);  

    double b = gsl_vector_get(b_3diag, 0);  
    double c = gsl_vector_get(c_3diag, 0);  
    double d = gsl_vector_get(ve_d, 0);    

    gsl_vector_set(ve_U, 0, c / b);  
    gsl_vector_set(ve_L, 0, d / b);  

    for (int i = 1; i < N+1; i++) {
        double a = gsl_vector_get(a_3diag, i - 1);  
        b = gsl_vector_get(b_3diag, i);         
        d = gsl_vector_get(ve_d, i);              

        double mianownik = b - a * gsl_vector_get(ve_U, i - 1);
        
        if (i < N) {
            double c = gsl_vector_get(c_3diag, i);
            gsl_vector_set(ve_U, i, c / mianownik); 
        }

        gsl_vector_set(ve_L, i, (d - a * gsl_vector_get(ve_L, i - 1)) / mianownik);
    }

    // Przejście wstecz
    gsl_vector_set(ve_wyniki, N, gsl_vector_get(ve_L, N));
    for (int i = N - 1; i >= 0; i--) {
        double xi = gsl_vector_get(ve_L, i) - gsl_vector_get(ve_U, i) * gsl_vector_get(ve_wyniki, i + 1);
        gsl_vector_set(ve_wyniki, i, xi);
    }
    gsl_vector_free(ve_L);
    gsl_vector_free(ve_U);
   return ve_wyniki;  
}


gsl_vector* shm_algorithm(gsl_vector *b_3diag,  gsl_vector * c_3diag,   gsl_vector * a_3diag,
         gsl_vector *ve_d)
{
    double h = 2.0/(N+1);
    gsl_vector * u = gsl_vector_alloc(N+1);
    gsl_vector * v = gsl_vector_alloc(N+1);
    gsl_vector *ve_wyniki = gsl_vector_alloc(N+1);
    
   gsl_vector *z=algorytm_thomasa(a_3diag, b_3diag,c_3diag, ve_d);
    
    gsl_vector_set(u, 0, 1);
    gsl_vector_set(u, N, 1/(h*h));
    for (int i = 1; i < N; i++)
        gsl_vector_set(u, i, 0);
        
     gsl_vector *q=algorytm_thomasa(a_3diag, b_3diag,c_3diag, u);
     
    gsl_vector_set(v, 0, 1);
    gsl_vector_set(v, N, 1/(h*h));
    for (int i = 1; i < N; i++)
        gsl_vector_set(v, i, 0);
        
      double mianownik=0;
      double licznik=0;
      for(int i=0;i<N+1;i++){
       licznik=gsl_vector_get(v, i)*gsl_vector_get(z, i)+licznik;
      	mianownik=gsl_vector_get(v, i)*gsl_vector_get(q, i)+mianownik;
      }
      double poprawka=licznik/(mianownik+1);
        
     for (int i=0; i < N+1; i++)
        gsl_vector_set(ve_wyniki, i, gsl_vector_get(z, i) - poprawka * gsl_vector_get(q,i));

    gsl_vector_free(u);
    gsl_vector_free(v);
    gsl_vector_free(z);
    gsl_vector_free(q);
    return ve_wyniki; 
}

int main()
{
    double h = 2.0/(N+1);
    gsl_vector * a_3diag = gsl_vector_alloc(N);
    gsl_vector * b_3diag = gsl_vector_alloc(N+1);
    gsl_vector * c_3diag = gsl_vector_alloc(N);
    gsl_vector * ve_d = gsl_vector_alloc(N+1);
    
    gsl_vector_set(b_3diag,0, (-2.0 / (h * h)));
    gsl_vector_set(b_3diag, N, (-2.0 / (h * h)) + 1-1 / (h * h*h*h));
    for (int i = 1; i < N; ++i) {
        gsl_vector_set(b_3diag, i, (-2.0 / (h * h)) + 1);
    }

    for (int i = 0; i < N; ++i) {
        gsl_vector_set(c_3diag, i, 1 / (h * h));
    }

    for (int i = 0; i < N; ++i) {
        gsl_vector_set(a_3diag, i, 1 / (h * h));
    }
    
      for (int i = 0; i < N+1; i++)
    gsl_vector_set(ve_d, i, gsl_sf_cos(2 *PI * i * h));

    gsl_vector *solution=shm_algorithm(b_3diag, c_3diag, a_3diag, ve_d);

    for (int i = 0; i < N+1; i++)
        std::cout << h*i << " " << gsl_vector_get(solution, i) << std::endl;
    
    gsl_vector_free(a_3diag);
    gsl_vector_free(b_3diag);
    gsl_vector_free(c_3diag);
    gsl_vector_free(ve_d);
    gsl_vector_free(solution);
    return 0;
}

