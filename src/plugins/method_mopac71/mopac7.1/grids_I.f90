      MODULE grids_I   
      INTERFACE
!...Generated by Pacific-Sierra Research 77to90  4.4G  10:47:33  03/09/06  
      SUBROUTINE grids (CO, POTPT, NMEP) 
      USE vast_kind_param,ONLY: DOUBLE 
      real(DOUBLE), DIMENSION(3,*), INTENT(IN) :: CO 
      real(DOUBLE), DIMENSION(3,*), INTENT(OUT) :: POTPT 
      integer, INTENT(INOUT) :: NMEP 
      END SUBROUTINE  
      END INTERFACE 
      END MODULE 
