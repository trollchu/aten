      MODULE charg_I   
      INTERFACE
!...Generated by Pacific-Sierra Research 77to90  4.4G  12:41:19  03/10/06  
      REAL(KIND(0.0D0)) FUNCTION charg (R, L1, L2, M, DA, DB, ADD) 
      USE vast_kind_param,ONLY: DOUBLE 
      real(DOUBLE), INTENT(IN) :: R 
      integer, INTENT(IN) :: L1 
      integer, INTENT(IN) :: L2 
      integer, INTENT(IN) :: M 
      real(DOUBLE), INTENT(IN) :: DA 
      real(DOUBLE), INTENT(IN) :: DB 
      real(DOUBLE), INTENT(IN) :: ADD 
      END FUNCTION  
      END INTERFACE 
      END MODULE 
