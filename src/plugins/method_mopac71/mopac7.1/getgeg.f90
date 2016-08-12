      subroutine getgeg(iread, labels, geo, lopt, na, nb, nc) 
!-----------------------------------------------
!   M o d u l e s 
!-----------------------------------------------
      USE vast_kind_param, ONLY:  double 
      use molkst_C, only : natoms, nvar, ndep, ltxt
      use parameters_C, only : ams
      USE symmetry_C, ONLY: locpar, idepfn, locdep 
      use permanent_arrays, only : atmass, loc, xparam, simbol, txtatm
      use chanel_C, only : iw
!***********************************************************************
!DECK MOPAC
!...Translated by Pacific-Sierra Research 77to90  4.4G  10:47:16  03/09/06  
!...Switches: -rl INDDO=2 INDIF=2 
!-----------------------------------------------
!   I n t e r f a c e   B l o c k s
!-----------------------------------------------
      use reada_I 
      use getval_I 
      use mopend_I 
      implicit none
!-----------------------------------------------
!   G l o b a l   P a r a m e t e r s
!-----------------------------------------------
!-----------------------------------------------
!   D u m m y   A r g u m e n t s
!-----------------------------------------------
      integer , intent(in) :: iread 
      integer , intent(out) :: labels(*) 
      integer , intent(out) :: lopt(3,*) 
      integer , intent(out) :: na(*) 
      integer , intent(out) :: nb(*) 
      integer , intent(out) :: nc(*) 
      real(double)  :: geo(3,*)  

!-----------------------------------------------
!   L o c a l   P a r a m e t e r s
!-----------------------------------------------
!-----------------------------------------------
!   L o c a l   V a r i a b l e s
!-----------------------------------------------
      integer , dimension(20) :: istart 
      integer , dimension(3,100) :: lgeo 
      integer :: i, nerr, numat, maxtxt, k, iline, nvalue, j, ivar, kerr, merr&
        , l, n, lerr 
      real(double) :: sum, degree 
      logical :: leadsp 
      character :: line*80 
      character , dimension(3,100) :: tgeo*12 
      character :: string*80 
      character, dimension(107) :: elemnt*2 

      save elemnt 
!-----------------------------------------------
      data (elemnt(i),i=1,107)/ ' H', 'HE', 'LI', 'BE', ' B', ' C', ' N', ' O'&
        , ' F', 'NE', 'NA', 'MG', 'AL', 'SI', ' P', ' S', 'CL', 'AR', 'K ', &
        'CA', 'SC', 'TI', ' V', 'CR', 'MN', 'FE', 'CO', 'NI', 'CU', 'ZN', 'GA'&
        , 'GE', 'AS', 'SE', 'BR', 'KR', 'RB', 'SR', ' Y', 'ZR', 'NB', 'MO', &
        'TC', 'RU', 'RH', 'PD', 'AG', 'CD', 'IN', 'SN', 'SB', 'TE', ' I', 'XE'&
        , 'CS', 'BA', 'LA', 'CE', 'PR', 'ND', 'PM', 'SM', 'EU', 'GD', 'TB', &
        'DY', 'HO', 'ER', 'TM', 'YB', 'LU', 'HF', 'TA', ' W', 'RE', 'OS', 'IR'&
        , 'PT', 'AU', 'HG', 'TL', 'PB', 'BI', 'PO', 'AT', 'RN', 'FR', 'RA', &
        'AC', 'TH', 'PA', 'U', 'NP', 'PU', 'AM', 'CM', 'BK', 'CF', 'XX', 'FM', &
        'MD', 'CB', '++', ' +', '--', ' -', 'TV'/  
      nerr = 0 
      numat = 0 
      na(1) = 0 
      nb(1) = 0 
      nc(1) = 0 
      nb(2) = 0 
      nc(2) = 0 
      nc(3) = 0 
      maxtxt = 0 
      do natoms = 1, 900 
        read (iread, '(A)', end=70, err=70) line 
        if (line == ' ') exit  
!
!   SEE IF TEXT IS ASSOCIATED WITH THIS ELEMENT
!
        i = index(line,'(') 
        if (i /= 0) then 
!
!  YES, ELEMENT IS LABELLED.
!
          k = index(line,')') 
          txtatm(natoms) = line(i:k) 
          maxtxt = max(maxtxt,k - i + 1) 
          string = line(1:i-1)//line(k+1:) 
          line = string 
        else 
          txtatm(natoms) = ' ' 
        endif 
!***********************************************************************
        do i = 1, 80 
          iline = ichar(line(i:i)) 
          if (iline<ichar('a') .or. iline>ichar('z')) cycle  
          line(i:i) = char(iline + ichar('A') - ichar('a')) 
        end do 
!***********************************************************************
        nvalue = 0 
        leadsp = .TRUE. 
        do i = 1, 80 
          if (leadsp .and. line(i:i)/=' ') then 
            nvalue = nvalue + 1 
            istart(nvalue) = i 
          endif 
          leadsp = line(i:i) == ' ' 
        end do 
        do j = 1, 107 
          if (index(' '//line(istart(1):istart(1)+2),elemnt(j)//' ') /= 0) &
            go to 40 
        end do 
        if (index(' '//line(istart(1):istart(1)+2),' X') /= 0) then 
          j = 99 
          go to 40 
        endif 
        write (iw, '(2A)') ' ELEMENT NOT RECOGNIZED: ', line(istart(1):istart(1&
          )+2) 
        nerr = nerr + 1 
   40   continue 
        labels(natoms) = j 
        if (j /= 99) then 
          numat = numat + 1 
          atmass(numat) = reada(line(1:max(istart(2)-1,1)),istart(1)) 
          if (atmass(numat) > 1.D-15) then 
            write (iw, &
              '('' FOR ATOM'',I4,''  ISOTOPIC MASS:''             ,F15.5)') &
              natoms, atmass(numat) 
          else 
            atmass(numat) = ams(j) 
          endif 
!#         WRITE(IW,*)NATOMS,NUMAT,ATMASS(NUMAT)
        endif 
        tgeo(1,natoms) = ' ' 
        tgeo(2,natoms) = ' ' 
        tgeo(3,natoms) = ' ' 
        if (natoms == 1) cycle  
        na(natoms) = nint(reada(line,istart(2))) 
        call getval (line(istart(3):), geo(1,natoms), tgeo(1,natoms)) 
        if (natoms == 2) cycle  
        nb(natoms) = nint(reada(line,istart(4))) 
        call getval (line(istart(5):), geo(2,natoms), tgeo(2,natoms)) 
        if (natoms == 3) cycle  
        nc(natoms) = nint(reada(line,istart(6))) 
        call getval (line(istart(7):), geo(3,natoms), tgeo(3,natoms)) 
      end do 
   70 continue 
      natoms = natoms - 1 
      lgeo(:,:natoms) = -1 
      ivar = -1 
      nvar = 0 
      ndep = 0 
      kerr = 0 
   90 continue 
      read (iread, '(A)', end=180, err=180) line 
      if (line == ' ') then 
        if (ivar == (-1)) then 
          merr = 0 
          do i = 1, natoms 
            merr = merr + count(geo(:,i)<(-998)) 
          end do 
!
!  IF ALL SYMBOLS ARE DEFINED, THEN DO NOT READ 'FIXED' SYMBOLS
!
          if (merr == 0) go to 180 
          ivar = nvar 
          go to 90 
        else 
          go to 180 
        endif 
      endif 
!***********************************************************************
      do i = 1, 80 
        iline = ichar(line(i:i)) 
        if (iline<ichar('a') .or. iline>ichar('z')) cycle  
        line(i:i) = char(iline + ichar('A') - ichar('a')) 
      end do 
!***********************************************************************
      do i = 1, 80 
        if (line(i:i) == ' ') cycle  
        exit  
      end do 
      do l = i, i + 12 
        if (line(l:l) /= ' ') cycle  
        exit  
      end do 
      sum = reada(line,l) 
      n = 0 
      lerr = 0 
      do j = 1, natoms 
        do k = 1, 3 
          if (.not.(tgeo(k,j)==line(i:l) .or. tgeo(k,j)(2:)==line(i:l) .and. &
            tgeo(k,j)(1:1)=='-')) cycle  
          if (lgeo(k,j) /= (-1)) lerr = 1 
          lgeo(k,j) = lgeo(k,j) + 1 
          n = n + 1 
          geo(k,j) = sum 
          if (n == 1) then 
            nvar = nvar + 1 
            loc(1,nvar) = j 
            loc(2,nvar) = k 
            xparam(nvar) = sum 
!fck -384  !  Deliberately use only 10 characters
!--TGAY 08/2016 - simbol() is an array of character*4 entities, so only use first four chars of tgeo()
            simbol(nvar) = tgeo(k,j)(:4) 
!fck +384
            if (simbol(nvar)(1:1) == '-') then 
              write (iw, '(A)') &
      ' NEGATIVE SYMBOLICS MUST BE PRECEEDED BY  THE POSITIVE EQUIVALENT' 
              write (iw, '(A)') ' FAULTY SYMBOLIC:  '//simbol(nvar) 
              call mopend (&
      'NEGATIVE SYMBOLICS MUST BE PRECEEDED BY THE POSITIVE EQUIVALENT') 
              return  
            endif 
          endif 
          if (n <= 1) cycle  
          ndep = ndep + 1 
          locpar(ndep) = loc(1,nvar) 
          idepfn(ndep) = loc(2,nvar) 
          if (tgeo(k,j)(1:1) == '-') then 
            idepfn(ndep) = 14 
            if (loc(2,nvar) /= 3) then 
              kerr = kerr + 1 
              write (iw, '(2A)') ' ONLY DIHEDRAL SYMBOLICS ', &
                ' CAN BE PRECEEDED BY A "-" SIGN' 
            endif 
          endif 
          locdep(ndep) = j 
        end do 
      end do 
      kerr = kerr + lerr 
      if (lerr == 1) then 
        write (iw, '(2A)') &
          ' THE FOLLOWING SYMBOL HAS BEEN DEFINED MORE THAN ONCE:', line(i:l) 
        nerr = nerr + 1 
      endif 
      if (n == 0) then 
        write (iw, '(2A)') ' THE FOLLOWING SYMBOLIC WAS NOT USED:', line(i:l) 
        nerr = nerr + 1 
      endif 
      go to 90 
  180 continue 
      merr = 0 
      do i = 1, natoms 
        merr = merr + count(geo(:,i)<(-998)) 
      end do 
      if (merr /= 0) write (iw, '(I4,A)') merr, &
        ' GEOMETRY VARIABLES WERE NOT DEFINED' 
      if (merr + kerr + nerr /= 0) then 
        write (iw, '(A,I3,A)') ' THE GEOMETRY DATA-SET CONTAINED', merr + kerr&
           + nerr, ' ERRORS' 
        call mopend ('THE GEOMETRY DATA-SET CONTAINED ERRORS') 
        return  
      endif 
!
!  SORT PARAMETERS TO BE OPTIMIZED INTO INCREASING ORDER OF ATOMS
!
      if (ivar /= (-1)) nvar = ivar 
      do i = 1, nvar 
        j = 100000 
        do l = i, nvar 
          if (j <= loc(1,l)*4 + loc(2,l)) cycle  
          k = l 
          j = loc(1,l)*4 + loc(2,l) 
        end do 
        string(1:10) = simbol(i) 
        simbol(i) = simbol(k)
!fck -384  !  Deliberately use only 10 characters
        simbol(k) = string(:10) 
!fck +384
        sum = xparam(i) 
        xparam(i) = xparam(k) 
        xparam(k) = sum 
        do j = 1, 2 
          l = loc(j,i) 
          loc(j,i) = loc(j,k) 
          loc(j,k) = l 
        end do 
      end do 
!
! Convert to radians
!
      degree = 1.7453292519943D-02 
      where (loc(2,:nvar) /= 1)  
        xparam(:nvar) = xparam(:nvar)*degree 
      end where 
      geo(2:3,:natoms) = geo(2:3,:natoms)*degree 
      ltxt = char(maxtxt) 
!
!  Put optimization flage into LOPT in case this will be followed by
!  an OLDGEO calculation
!
      lopt(:,:natoms) = 0 
      do i = 1, nvar 
        lopt(loc(2,i),loc(1,i)) = 1 
      end do 
      return  
      end subroutine getgeg 
