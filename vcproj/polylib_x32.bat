rem 64bit <=‚U‚Sƒrƒbƒg‚ÍA³‹K•\Œ»‚ÌPOSIXƒ‰ƒCƒuƒ‰ƒŠ‚ª‚R‚QBIT”Å‚ÌDLL‚µ‚©‚È‚¢‚Ì‚Å‚â‚ç‚È‚¢B
rem set MPIDIR=C:\Program Files\MPICH2

rem 32bit
set MPIDIR=C:\Program Files (x86)\MPICH2
set PATH=%MPIDIR%\bin;%PATH%

set TP_ROOT=Z:\projects\fxgen\github\Polylib_kawanabe\ext\TextParser
set RG_ROOT=Z:\projects\fxgen\github\Polylib_kawanabe\ext\regex-2.7-bin

polylib.sln

rem mpich2 ‚Ìƒf[ƒ‚ƒ“Às
"%MPIDIR%\bin\smpd.exe" -d 0
pause;