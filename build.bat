@echo off
IF NOT EXIST "build\\" (
  mkdir build
)

pushd build
pwd
cl -Zi /FC /nologo ../main.cpp
popd build
