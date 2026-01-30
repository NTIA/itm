#!/bin/sh

mkdir ../dist

em++ -O3 \
  ../src/itm_em_wrapper.cpp \
  ../src/itm_p2p.cpp \
  ../src/itm_area.cpp \
  ../src/ValidateInputs.cpp \
  ../src/ComputeDeltaH.cpp \
  ../src/DiffractionLoss.cpp \
  ../src/FindHorizons.cpp \
  ../src/FreeSpaceLoss.cpp \
  ../src/FresnelIntegral.cpp \
  ../src/H0Function.cpp \
  ../src/InitializeArea.cpp \
  ../src/InitializePointToPoint.cpp \
  ../src/InverseComplementaryCumulativeDistributionFunction.cpp \
  ../src/KnifeEdgeDiffraction.cpp \
  ../src/LinearLeastSquaresFit.cpp \
  ../src/LineOfSightLoss.cpp \
  ../src/LongleyRice.cpp \
  ../src/QuickPfl.cpp \
  ../src/SigmaHFunction.cpp \
  ../src/SmoothEarthDiffraction.cpp \
  ../src/TerrainRoughness.cpp \
  ../src/TroposcatterLoss.cpp \
  ../src/Variability.cpp \
  -std=c++17 -lembind \
  -sMODULARIZE=1 -sEXPORT_NAME=ITM \
  -sENVIRONMENT=web,worker,node \
  -sALLOW_MEMORY_GROWTH=1 \
  -o ../dist/itm.js
