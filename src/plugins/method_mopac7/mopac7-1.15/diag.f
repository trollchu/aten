      SUBROUTINE DIAG(FAO,VECTOR,NOCC,EIG,MDIM,N)
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      INCLUDE 'SIZES'
      DIMENSION FAO(*),VECTOR(MDIM,*),EIG(*),WS(MAXORB)
C***********************************************************************
C
C   "FAST" DIAGONALISATION PROCEDURE.
C
C    ON INPUT FAO CONTAINS THE LOWER HALF TRIANGLE OF THE MATRIX TO BE
C                         DIAGONALISED, PACKED.
C             VECTOR  CONTAINS THE OLD EIGENVECTORS ON INPUT, THE NEW
C             VECTORS ON EXITING.
C             NOCC = NUMBER OF OCCUPIED MOLECULAR ORBITALS.
C             EIG  = EIGENVALUES FROM AN EXACT DIAGONALISATION
C             MDIM = DECLARED SIZE OF MATRIX "C".
C             N = NUMBER OF ATOMIC ORBITALS IN BASIS SET
C
C  DIAG IS A PSEUDO-DIAGONALISATION PROCEDURE, IN THAT THE VECTORS THAT
C       ARE GENERATED BY IT ARE MORE NEARLY ABLE TO BLOCK-DIAGONALISE
C       THE FOCK MATRIX OVER MOLECULAR ORBITALS THAN THE STARTING
C       VECTORS. IT MUST BE CONSIDERED PSEUDO FOR SEVERAL REASONS:
C       (A) IT DOES NOT GENERATE EIGENVECTORS - THE SECULAR DETERMINANT
C           IS NOT DIAGONALISED, ONLY THE OCCUPIED-VIRTUAL INTERSECTION.
C       (B) MANY SMALL ELEMENTS IN THE SEC.DET. ARE IGNORED AS BEING TOO
C           SMALL COMPARED WITH THE LARGEST ELEMENT.
C       (C) WHEN ELEMENTS ARE ELIMINATED BY ROTATION, THE REST OF THE
C           SEC. DET. IS ASSUMED NOT TO CHANGE, I.E. ELEMENTS CREATED
C           ARE IGNORED.
C       (D) THE ROTATION REQUIRED TO ELIMINATE THOSE ELEMENTS CONSIDERED
C           SIGNIFICANT IS APPROXIMATED TO USING THE EIGENVALUES OF THE
C           EXACT DIAGONALISATION THROUGHOUT THE REST OF THE ITERATIVE
C           PROCEDURE.
C
C  (NOTE:- IN AN ITERATIVE PROCEDURE ALL THE APPROXIMATIONS PRESENT IN
C          DIAG BECOME VALID AT SELF-CONSISTENCY, SELF-CONSISTENCY IS
C          NOT SLOWED DOWN BY USE OF THESE APPROXIMATIONS)
C
C    REFERENCE:
C             "FAST SEMIEMPIRICAL CALCULATIONS",
C             STEWART. J.J.P., CSASZAR, P., PULAY, P., J. COMP. CHEM.,
C             3, 227, (1982)
C
C***********************************************************************
      COMMON /SCRACH/ FMO(MORB2), XDUMY(MAXPAR**2-MORB2)
C             FMO  IS A WORK-SPACE OF SIZE (N-NOCC)*NOCC, IT WILL HOLD
C                  THE FOCK MOLECULAR ORBITAL INTERACTION MATRIX.
C
C  FIRST, CONSTRUCT THAT PART OF A SECULAR DETERMINANT OVER MOLECULAR
C  ORBITALS WHICH CONNECTS THE OCCUPIED AND VIRTUAL SETS.
C
C***********************************************************************
C
      LOGICAL FIRST
      DATA FIRST /.TRUE./
      IF(FIRST)THEN
         FIRST=.FALSE.
C
C   EPS IS THE SMALLEST NUMBER WHICH, WHEN ADDED TO 1.D0, IS NOT
C   EQUAL TO 1.D0
         CALL EPSETA(EPS,ETA)
C
C   INCREASE EPS TO ALLOW FOR A LOT OF ROUND-OFF
C
         BIGEPS=10.D0*SQRT(EPS)
      ENDIF
      TINY=0.D0
      LUMO=NOCC+1
      IJ=0
C#      CALL TIMER('SQUARING')
      DO 60 I=LUMO,N
         KK=0
         DO 30 J=1,N
            SUM=0.D0
            DO 10 K=1,J
               KK=KK+1
   10       SUM=SUM+FAO(KK)*VECTOR(K,I)
            IF(J.EQ.N) GOTO 30
            J1=J+1
            K2=KK
            DO 20 K=J1,N
               K2=K2+K-1
   20       SUM=SUM+FAO(K2)*VECTOR(K,I)
   30    WS(J)=SUM
         DO 50 J=1,NOCC
            IJ=IJ+1
            SUM=0.D0
            DO 40 K=1,N
   40       SUM=SUM+WS(K)*VECTOR(K,J)
            IF(TINY.LT.ABS(SUM)) TINY=ABS(SUM)
   50    FMO(IJ)=SUM
   60 CONTINUE
      TINY=0.05D0*TINY
C***********************************************************************
C
C   NOW DO A CRUDE 2 BY 2 ROTATION TO "ELIMINATE" SIGNIFICANT ELEMENTS
C
C***********************************************************************
C#      CALL TIMER('ROTATING')
      IJ=0
      DO 90 I=LUMO,N
         DO 80 J=1,NOCC
            IJ=IJ+1
            IF(ABS(FMO(IJ)).LT.TINY) GOTO 80
C
C      BEGIN 2 X 2 ROTATIONS
C
            A=EIG(J)
            B=EIG(I)
            C=FMO(IJ)
            D=A-B
C
C    USE BIGEPS TO DETERMINE WHETHER TO DO A 2 BY 2 ROTATION
C
            IF(ABS(C/D).LT.BIGEPS) GOTO 80
C
C  AT THIS POINT WE KNOW THAT
            E=SIGN(SQRT(4.D0*C*C+D*D),D)
            ALPHA=SQRT(0.5D0*(1.D0+D/E))
            BETA=-SIGN(SQRT(1.D0-ALPHA*ALPHA),C)
C
C      ROTATION OF PSEUDO-EIGENVECTORS
C
            DO 70 M=1,N
               A=VECTOR(M,J)
               B=VECTOR(M,I)
               VECTOR(M,J)=ALPHA*A+BETA*B
               VECTOR(M,I)=ALPHA*B-BETA*A
   70       CONTINUE
   80    CONTINUE
   90 CONTINUE
C#      CALL TIMER('RETURNING')
      RETURN
      END
