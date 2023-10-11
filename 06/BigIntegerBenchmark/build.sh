#!/bin/bash

echo "Building the project..."
dotnet build

if [ $? -eq 0 ]; then
  echo "Build completed successfully."
  echo "Running benchmarks..."
  
  dotnet run -c Release --filter *Benchmark
else
  echo "Build failed. Benchmarks will not be executed."
fi