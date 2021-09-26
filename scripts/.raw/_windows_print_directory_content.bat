::
:: file:			findfl.bat
:: created on:		2021 Mar 20
:: created by:		
:: purpose:			...
::

@echo off
setlocal EnableDelayedExpansion

dir /s /b | findstr /i %1

exit /b 0

endlocal
