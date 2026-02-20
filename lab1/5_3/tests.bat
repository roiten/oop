@echo off
setlocal enabledelayedexpansion

set EXE=rle.exe

%EXE% pack empty.bin output_empty.rle
fc /b output_empty.rle expected_pack_empty.bin
if %errorlevel% neq 0 echo Test 1 Pack Failed & goto :end
%EXE% unpack output_empty.rle output_empty_unpacked.bin
fc /b output_empty_unpacked.bin empty.bin
if %errorlevel% neq 0 echo Test 1 Unpack Failed & goto :end

%EXE% pack 255.bin output_255.rle
fc /b output_255.rle expected_pack_255.bin
if %errorlevel% neq 0 echo Test 2 Pack Failed & goto :end
%EXE% unpack output_255.rle output_255_unpacked.bin
fc /b output_255_unpacked.bin 255.bin
if %errorlevel% neq 0 echo Test 2 Unpack Failed & goto :end

%EXE% pack 256.bin output_256.rle
fc /b output_256.rle expected_pack_256.bin
if %errorlevel% neq 0 echo Test 3 Pack Failed & goto :end
%EXE% unpack output_256.rle output_256_unpacked.bin
fc /b output_256_unpacked.bin 256.bin
if %errorlevel% neq 0 echo Test 3 Unpack Failed & goto :end

%EXE% pack 257.bin output_257.rle
fc /b output_257.rle expected_pack_257.bin
if %errorlevel% neq 0 echo Test 4 Pack Failed & goto :end
%EXE% unpack output_257.rle output_257_unpacked.bin
fc /b output_257_unpacked.bin 257.bin
if %errorlevel% neq 0 echo Test 4 Unpack Failed & goto :end

%EXE% pack 255char.bin output_255char.rle
fc /b output_255char.rle expected_pack_255char.bin
if %errorlevel% neq 0 echo Test 5 Pack Failed & goto :end
%EXE% unpack output_255char.rle output_255char_unpacked.bin
fc /b output_255char_unpacked.bin 255char.bin
if %errorlevel% neq 0 echo Test 5 Unpack Failed & goto :end

echo All tests passed!
:end
